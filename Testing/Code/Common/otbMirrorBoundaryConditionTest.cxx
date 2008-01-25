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
#include "otbMirrorBoundaryCondition.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkMacro.h"

int otbMirrorBoundaryConditionTest(int argc, char * argv[])
{
  typedef otb::VectorImage<double,2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  typedef itk::ConstNeighborhoodIterator<ImageType> NeighborhoodIteratorType; 
  typedef NeighborhoodIteratorType::RadiusType   RadiusType;
  typedef otb::MirrorBoundaryCondition<ImageType> ConditionType;
  typedef NeighborhoodIteratorType::OffsetType OffsetType;
  typedef ImageType::PixelType PixelType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  
  RadiusType rad;
  rad.Fill(atoi(argv[2]));

  ConditionType nbc;

  NeighborhoodIteratorType it(rad,reader->GetOutput(),reader->GetOutput()->GetLargestPossibleRegion());
  it.OverrideBoundaryCondition( &nbc );
  it.GoToBegin();

  OffsetType off1,off2;

  itk::OStringStream oss;
  PixelType in,out;

  for(unsigned int i = 0;i<rad[0];++i)
    {
      for(unsigned int j = 0;j<rad[1];++j)
	{
	  off1[0] = -static_cast<int>(i);
	  off1[1] = -static_cast<int>(j);
	  off2[0] = i;
	  off2[1] = j;

	  out=it.GetPixel(off1);
	  in = it.GetPixel(off2);
	  
	  for(unsigned int band = 0;band<reader->GetOutput()->GetNumberOfComponentsPerPixel();++band)
	    {
	      oss.str("");
	      oss<<"Band "<<band<<" : OutOfBound offset: "<<off1<<" <-> value: "<<out[band]<<" != inside image offset: "<<off2<<" <-> value: "<<in[band];
	      otbControlConditionTestMacro(out[band]!=in[band],oss.str().c_str());
	      
	    }
	}
    }

  return EXIT_SUCCESS;
}
