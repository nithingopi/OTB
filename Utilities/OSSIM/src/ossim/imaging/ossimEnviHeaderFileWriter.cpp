//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Class definition for ossimEnviHeaderFileWriter.  Meta data class for
// writing an ENVI (The Environment for Visualizing Images) header file.
//
//----------------------------------------------------------------------------
// $Id: ossimEnviHeaderFileWriter.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimEnviHeaderFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageSourceInterface.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimMapProjectionInfo.h>


RTTI_DEF1(ossimEnviHeaderFileWriter,
          "ossimEnviHeaderFileWriter",
          ossimMetadataFileWriter)

ossimEnviHeaderFileWriter::ossimEnviHeaderFileWriter()
   :
   ossimMetadataFileWriter(),
   theHdr()
{
}

ossimEnviHeaderFileWriter::~ossimEnviHeaderFileWriter()
{
}

bool ossimEnviHeaderFileWriter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   bool result = false;
   
   if (ossimMetadataFileWriter::loadState(kwl, prefix))
   {
      result = theHdr.loadState(kwl, prefix);
   }
   
   return result;
}

bool writeFile()
{
   bool result = true;

   return result;
}

bool ossimEnviHeaderFileWriter::writeFile()
{
   theHdr.setLines(theAreaOfInterest.height());
   theHdr.setSamples(theAreaOfInterest.width());
   theHdr.setBands(theInputConnection->getNumberOfOutputBands());
   
   // Get the geometry from the input.
   ossimKeywordlist kwl;
   theInputConnection->getImageGeometry(kwl);

   // Create the projection.
   ossimMapProjection* mapProj =  NULL;
   ossimRefPtr<ossimProjection> proj =
      ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   if (proj.valid())
   {
      mapProj = PTR_CAST(ossimMapProjection, proj.get());

      if (mapProj)
      {
         // Create the projection info.
         ossimRefPtr<ossimMapProjectionInfo> projectionInfo
            = new ossimMapProjectionInfo(mapProj, theAreaOfInterest);
         
         // Set the tie points in the keyword list.
         projectionInfo->getGeom(kwl);
         
         // Pass it on to envi header to set the map info string from geometry.
         theHdr.setMapInfo(kwl);
      }
   }

   return theHdr.writeFile(theFilename);
}

void ossimEnviHeaderFileWriter::getMetadatatypeList(
   std::vector<ossimString>& metadatatypeList) const
{
   metadatatypeList.push_back(ossimString("envi_header")); 
}

bool ossimEnviHeaderFileWriter::hasMetadataType(
   const ossimString& metadataType)const
{
   return (metadataType == "envi_header");
}
