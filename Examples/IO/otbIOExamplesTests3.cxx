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

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests

#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
#if defined(ITK_USE_REVIEW)
  REGISTER_TEST(LidarToImageExampleTest);
#endif
}

#if defined(ITK_USE_REVIEW)
#undef main
#define main LidarToImageExampleTest
#include "LidarToImageExample.cxx"
#endif
