/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGridForwardWarpImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-10-27 18:12:48 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkGridForwardWarpImageFilter_txx
#define __itkGridForwardWarpImageFilter_txx

#include "itkGridForwardWarpImageFilter.h"

#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkLineIterator.h"

namespace itk
{

/**
 * Default constructor.
 */
template <class TDeformationField,class TOutputImage>
GridForwardWarpImageFilter<TDeformationField,TOutputImage>
::GridForwardWarpImageFilter()
{
  // Setup default values
  m_BackgroundValue = NumericTraits<PixelType>::Zero;
  m_ForegroundValue = NumericTraits<PixelType>::One;
  m_GridPixSpacing = 5;
}

/**
 * Standard PrintSelf method.
 */
template <class TDeformationField,class TOutputImage>
void
GridForwardWarpImageFilter<TDeformationField,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{

  Superclass::PrintSelf(os, indent);

  os << indent << "BackgroundValue: "
     << static_cast<typename NumericTraits<PixelType>::PrintType>(m_BackgroundValue)
     << std::endl;
os << indent << "ForegroundValue: "
     << static_cast<typename NumericTraits<PixelType>::PrintType>(m_ForegroundValue)
     << std::endl;
  
}


/**
 * Compute the output for the region specified by outputRegionForThread.
 */
template <class TDeformationField,class TOutputImage>
void
GridForwardWarpImageFilter<TDeformationField,TOutputImage>
::GenerateData()
{
  OutputImagePointer outputPtr = this->GetOutput();
  DeformationFieldConstPointer fieldPtr = this->GetInput();

  SpacingType spacing = fieldPtr->GetSpacing();

  outputPtr->SetRegions( fieldPtr->GetRequestedRegion() );
  outputPtr->SetOrigin( fieldPtr->GetOrigin() );
  outputPtr->SetSpacing( spacing );
  outputPtr->Allocate();
  outputPtr->FillBuffer(m_BackgroundValue);

  IndexType FirstIndex = fieldPtr->GetRequestedRegion().GetIndex();
  IndexType LastIndex = fieldPtr->GetRequestedRegion().GetIndex() + 
     fieldPtr->GetRequestedRegion().GetSize();

  // iterator for the output image
  typedef ImageRegionIteratorWithIndex<OutputImageType> OutputImageIteratorWithIndex;
  OutputImageIteratorWithIndex iter(outputPtr, outputPtr->GetRequestedRegion());

  // iterator for the deformation field
  typedef ImageRegionConstIterator<DeformationFieldType> DeformationFieldIterator;
  DeformationFieldIterator fieldIt(fieldPtr, outputPtr->GetRequestedRegion());

  // Bresenham line iterator
  typedef LineIterator<OutputImageType> LineIteratorType;

  typedef typename IndexType::IndexValueType IndexValueType;

  IndexType index, refIndex, targetIndex;
  ContinuousIndex<float, ImageDimension> contindex;
  DisplacementType displacement;
  bool inside, targetIn;

  for (iter.GoToBegin(), fieldIt.GoToBegin(); !iter.IsAtEnd(); ++iter, ++fieldIt)
    {
    index = iter.GetIndex();

    unsigned int numGridIntersect = 0;
    for( unsigned int dim = 0; dim < ImageDimension; dim++ )
      {
      numGridIntersect += ( ( index[dim] % m_GridPixSpacing ) == 0 );
      }
    
    if (numGridIntersect == ImageDimension)
      {
      // we are on a grid point => transform it

      // get the required displacement
      displacement = fieldIt.Get();
      
      // compute the mapped point
      inside = true;
      for(unsigned int j = 0; j < ImageDimension; j++ )
        {
        contindex[j] = index[j] + displacement[j]/spacing[j];
        if (contindex[j]<FirstIndex[j] || contindex[j]>(LastIndex[j]-1)) 
          {
          inside = false;
          break;
          }
        refIndex[j] = Math::Round<IndexValueType>(contindex[j]);
        }

      if( inside )
        {
        // We know the current grid point is inside
        // we will check if the grid points that are above are also inside
        // In such a case we draw a Bresenham line
        for( unsigned int dim = 0; dim < ImageDimension; dim++ )
          {
          targetIndex = index;
          targetIndex[dim] += m_GridPixSpacing;
          if ( targetIndex[dim]<LastIndex[dim] )
            {
            // get the required displacement
            displacement = fieldPtr->GetPixel( targetIndex );

            // compute the mapped point
            targetIn = true;
            for( unsigned int j = 0; j < ImageDimension; j++ )
              {
              contindex[j] = targetIndex[j] + displacement[j]/spacing[j];
              if( contindex[j]<FirstIndex[j] || contindex[j]>(LastIndex[j]-1) )
                {
                targetIn = false;
                break;
                }
              targetIndex[j] = Math::Round<IndexValueType>(contindex[j]);
              }

            if( targetIn ) 
              {
              for ( LineIteratorType lineIter(outputPtr, refIndex, targetIndex);
                    !lineIter.IsAtEnd(); ++lineIter )
                {
                lineIter.Set(m_ForegroundValue);
                }
              }
            }
          }
        }
      }
    }

  //ProgressReporter progress(this, 0, numiter+1, numiter+1);
}


} // end namespace itk

#endif
