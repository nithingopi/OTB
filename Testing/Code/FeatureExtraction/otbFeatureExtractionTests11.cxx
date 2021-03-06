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

// this file defines the otbCommonTest for the test driver
// and all it expects is that you have a function called RegisterTests


#include "otbTestMain.h"

void RegisterTests()
{
  REGISTER_TEST(otbCloudEstimatorFilterNew);
  REGISTER_TEST(otbCloudEstimatorDefaultFilter);
  REGISTER_TEST(otbCloudEstimatorFilter);
  REGISTER_TEST(otbCloudDetectionFilterNew);
  REGISTER_TEST(otbCloudDetectionFilter);
  REGISTER_TEST(otbSimplifyManyPathListFilter);
  REGISTER_TEST(otbLineSegmentDetectorNew);
  REGISTER_TEST(otbLineSegmentDetector);
  REGISTER_TEST(otbLineSegmentDetector_8b_16b_compare);
  REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilterNew);
  REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilter);
  REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilterByStepsOutputAscii);
  //REGISTER_TEST(otbLineSpatialObjectListToRightAnglePointSetFilterOutputImage);
  REGISTER_TEST(otbVectorDataToRightAngleVectorDataFilterNew);
  REGISTER_TEST(otbVectorDataToRightAngleVectorDataFilter);
}
