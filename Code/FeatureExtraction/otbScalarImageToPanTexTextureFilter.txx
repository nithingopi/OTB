/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbScalarImageToPanTexTextureFilter_txx
#define __otbScalarImageToPanTexTextureFilter_txx

#include "otbScalarImageToPanTexTextureFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage,class TOutputImage>
ScalarImageToPanTexTextureFilter<TInputImage,TOutputImage>
::ScalarImageToPanTexTextureFilter() : m_Radius(),
                                  m_NumberOfBinsPerAxis(),
                                  m_InputImageMinimum(0),
                                  m_InputImageMaximum(256)
{
  // There are 1 output corresponding to the Pan Tex texture indice
  this->SetNumberOfOutputs(1);
  
  //Fill the offset list for contrast computation
  OffsetType off;
  off[0] = 0;
  off[1] = 1;
  m_OffsetList.push_back(off);   //(0,1)
  off[1] = 2;
  m_OffsetList.push_back(off);   //(0,2)
  off[0] = 1;
  off[1] = -2;
  m_OffsetList.push_back(off);   //(1,-2)
  off[1] = -1;
  m_OffsetList.push_back(off);   //(1,-1)
  off[1] = 0;
  m_OffsetList.push_back(off);   //(1,0)
  off[1] = 1;
  m_OffsetList.push_back(off);   //(1,1)
  off[1] = 2;
  m_OffsetList.push_back(off);   //(1,2)
  off[0] = 2;
  off[1] = -1;
  m_OffsetList.push_back(off);   //(2,-1)
  off[1] = 0;
  m_OffsetList.push_back(off);   //(2,0)
  off[1] = 1;
  m_OffsetList.push_back(off);   //(2,1)
}

template <class TInputImage,class TOutputImage>
ScalarImageToPanTexTextureFilter<TInputImage,TOutputImage>
::~ScalarImageToPanTexTextureFilter()
{}
 

template <class TInputImage,class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion()
{
  // First, call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve the input and output pointers
  InputImagePointerType inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();

  if(!inputPtr || !outputPtr)
    {
    return;
    }

  // Retrieve the output requested region
  // We use only the first output since requested regions for all outputs are enforced to be equal
  // by the default GenerateOutputRequestedRegiont() implementation
  OutputRegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

  typename OutputRegionType::IndexType outputIndex = outputRequestedRegion.GetIndex();
  typename OutputRegionType::SizeType outputSize   = outputRequestedRegion.GetSize();
  typename InputRegionType::IndexType inputIndex;
  typename InputRegionType::SizeType  inputSize;

  // First, apply offset
  for(unsigned int dim = 0; dim<InputImageType::ImageDimension;++dim)
    {
    inputIndex[dim]=std::min(outputIndex[dim],outputIndex[dim]+2);
    inputSize[dim] =std::max(outputIndex[dim]+outputSize[dim],outputIndex[dim]+outputSize[dim]+2)-inputIndex[dim];
    }

  // Build the input requested region
  InputRegionType inputRequestedRegion;
  inputRequestedRegion.SetIndex(inputIndex);
  inputRequestedRegion.SetSize(inputSize);

  // Apply the radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // Try to apply the requested region to the input image
  if(inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    }
  else
    {
    // Build an exception
    itk::InvalidRequestedRegionError e(__FILE__,__LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
}

template <class TInputImage,class TOutputImage>
void
ScalarImageToPanTexTextureFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputRegionType & outputRegionForThread, int threadId)
{
  // Retrieve the input and output pointers
  InputImagePointerType  inputPtr             =      const_cast<InputImageType *>(this->GetInput());
  OutputImagePointerType outputPtr = this->GetOutput();
  
  itk::ImageRegionIteratorWithIndex<OutputImageType> outputIt(outputPtr,outputRegionForThread);
  
  // Go to begin
  outputIt.GoToBegin();

  // Set-up progress reporting
  itk::ProgressReporter progress(this,threadId,outputRegionForThread.GetNumberOfPixels());

  // Iterate on outputs to compute textures
  while(!outputIt.IsAtEnd())
    {
    double     out = itk::NumericTraits<double>::max();
    const unsigned int offsetListSize = m_OffsetList.size();
    
    
      
    for (unsigned int i = 0; i < offsetListSize; ++i)
      {
      // Find the input region on which texture will be computed
      InputRegionType currentRegion;
      typename InputRegionType::IndexType currentIndex = outputIt.GetIndex()-m_Radius;
      typename InputRegionType::SizeType  currentSize;

      for(unsigned int dim = 0; dim<InputImageType::ImageDimension;++dim)
        {
        // Compute current size before applying offset
        currentSize[dim] = 2*m_Radius[dim]+1;

        // Apply offset
        currentIndex[dim] = std::min(currentIndex[dim],currentIndex[dim]+m_OffsetList[i][dim]);
        currentSize[dim] = std::max(currentIndex[dim]+currentSize[dim],currentIndex[dim]+currentSize[dim]+m_OffsetList[i][dim])-currentIndex[dim];
        }

      // Fill current region
      currentRegion.SetIndex(currentIndex);
      currentRegion.SetSize(currentSize);
      
      // Build the co-occurence matrix generator
      CoocurrenceMatrixGeneratorPointerType coOccurenceMatrixGenerator = CoocurrenceMatrixGeneratorType::New();
      coOccurenceMatrixGenerator->SetInput(inputPtr);
      coOccurenceMatrixGenerator->SetOffset(m_OffsetList[i]);
      coOccurenceMatrixGenerator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis);
      coOccurenceMatrixGenerator->SetPixelValueMinMax(m_InputImageMinimum,m_InputImageMaximum);
      
      // Compute the co-occurence matrix
      coOccurenceMatrixGenerator->SetRegion(currentRegion);
      coOccurenceMatrixGenerator->Compute();
      
      //HistogramType::Pointer histo(coOccurenceMatrixGenerator->GetOutput());
      
      FrequencyType totalFrequency = coOccurenceMatrixGenerator->GetOutput()->GetTotalFrequency();
      if ( (totalFrequency - itk::NumericTraits<MeasurementType>::One) > 0.0001 )
        {
        // Doesn't look normalized:
        HistogramIterator hit;
        FrequencyType totalFrequency = 
          coOccurenceMatrixGenerator->GetOutput()->GetTotalFrequency();
              
        for (hit = coOccurenceMatrixGenerator->GetOutput()->Begin(); hit != coOccurenceMatrixGenerator->GetOutput()->End(); ++hit)
          {
          hit.SetFrequency(hit.GetFrequency() / totalFrequency);
          }
        }
      double inertia = 0;
      for (HistogramIterator hit = coOccurenceMatrixGenerator->GetOutput()->Begin();
       hit != coOccurenceMatrixGenerator->GetOutput()->End(); ++hit)
      {
      MeasurementType frequency = hit.GetFrequency();
      if (frequency == 0)
        {
        continue; // no use doing these calculations if we're just multiplying by zero.
        }
      typename InputRegionType::IndexType index = coOccurenceMatrixGenerator->GetOutput()->GetIndex(hit.GetInstanceIdentifier());
      inertia += (index[0] - index[1]) * (index[0] - index[1]) * frequency;
      }
      
      if (inertia < out) out = inertia;
      }
      
      outputIt.Set(out);
    }
}

} // End namespace otb

#endif
