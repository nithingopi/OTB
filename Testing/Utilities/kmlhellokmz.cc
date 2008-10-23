/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    Some parts of this code are derived from kml library examples. See KMLCopyright.txt
    for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// An example of the KMZ API, showing DataIsKmz() and ReadKmlFromKmz().
// Prints the default KML file in a KMZ archive, if found, or the first KML
// file.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"

using std::cout;
using std::endl;
using kmlengine::KmzFile;

int kmlhellokmz(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " kmzfile" << endl;
    return EXIT_FAILURE;
  }

  boost::scoped_ptr<KmzFile> kmz_file(KmzFile::OpenFromFile(argv[1]));
  if (!kmz_file) {
    cout << "error: " << argv[1] << " is not a valid kmz file" << endl;
    return EXIT_FAILURE;
  }

  std::string kml;
  if (!kmz_file->ReadKml(&kml)) {
    cout << "error: no data read from " << argv[1] << endl;
    return EXIT_FAILURE;
  }

  std::cout << kml << endl;
  return EXIT_SUCCESS;
}
