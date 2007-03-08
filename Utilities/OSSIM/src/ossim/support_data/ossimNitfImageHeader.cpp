//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: See top level LICENSE.txt for more details.
//
// Author: Garrett Potts
//
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeader.cpp 10173 2007-01-03 18:21:26Z gpotts $

#include <cmath> /* for fmod */
#include <iomanip>

#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimContainerProperty.h>

RTTI_DEF2(ossimNitfImageHeader,
          "ossimNitfImageHeader",
          ossimObject,
          ossimPropertyInterface)
static const char* TAGS_KW = "tags";
ossimNitfImageHeader::ossimNitfImageHeader()
{
}

ossimNitfImageHeader::~ossimNitfImageHeader()
{
}

bool ossimNitfImageHeader::getTagInformation(ossimNitfTagInformation& tagInfo,
                                             const ossimString& tagName)const
{
   if(theTagList.size())
   {
      for(ossim_uint32 idx = 0; idx < theTagList.size(); ++idx)
      {
         if(theTagList[idx].getTagName() == tagName)
         {
            tagInfo = theTagList[idx];
            return true;
         }
      }
   }
   
   return false;
}

bool ossimNitfImageHeader::getTagInformation(ossimNitfTagInformation& tag,
                                             ossim_uint32 idx)const
{
   bool result = false;
   
   if(idx < theTagList.size())
   {
      tag = theTagList[idx];
      result = true;
   }
   
   return result;
}

ossimRefPtr<ossimNitfRegisteredTag> ossimNitfImageHeader::getTagData(
   const ossimString& tagName)
{
   if(theTagList.size())
   {
      for(ossim_uint32 idx = 0; idx < theTagList.size(); ++idx)
      {
         if(theTagList[idx].getTagName() == tagName)
         {
            return theTagList[idx].getTagData();
         }
      }
   }

   return ossimRefPtr<ossimNitfRegisteredTag>();
}
   
const ossimRefPtr<ossimNitfRegisteredTag> ossimNitfImageHeader::getTagData(
   const ossimString& tagName) const
{
   if(theTagList.size())
   {
      for(ossim_uint32 idx = 0; idx < theTagList.size(); ++idx)
      {
         if(theTagList[idx].getTagName() == tagName)
         {
            return theTagList[idx].getTagData();
         }
      }
   }
   
   return false;
}

ossim_uint32 ossimNitfImageHeader::getNumberOfTags()const
{
   return theTagList.size();
}

void ossimNitfImageHeader::addTag(ossimNitfTagInformation tag)
{
   removeTag(tag.getTagName());
   theTagList.push_back(tag);
}

void ossimNitfImageHeader::removeTag(const ossimString& tagName)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theTagList.size(); ++idx)
   {
      if(theTagList[idx].getTagName() == tagName)
      {
         theTagList.erase(theTagList.begin() + idx);
         return;
      }
   }
}

bool ossimNitfImageHeader::isSameAs(const ossimNitfImageHeader* hdr) const
{
   if (!hdr) return false;
   
   return ( (isCompressed() == hdr->isCompressed()) &&
            (getNumberOfRows() == hdr->getNumberOfRows()) &&
            (getNumberOfBands() == hdr->getNumberOfBands()) &&
            (getNumberOfCols() == hdr->getNumberOfCols()) &&
            (getNumberOfBlocksPerRow() == hdr->getNumberOfBlocksPerRow()) &&
            (getNumberOfBlocksPerCol() == hdr->getNumberOfBlocksPerCol()) &&
            (getNumberOfPixelsPerBlockHoriz() ==
             hdr->getNumberOfPixelsPerBlockHoriz()) &&
            (getNumberOfPixelsPerBlockVert() ==
             hdr->getNumberOfPixelsPerBlockVert()) &&
            (getBitsPerPixelPerBand() == hdr->getBitsPerPixelPerBand()) &&
            (getImageRect() == hdr->getImageRect()) &&
            (getIMode() == hdr->getIMode()) &&
            (getCoordinateSystem() == hdr->getCoordinateSystem()) &&
            (getGeographicLocation() == hdr->getGeographicLocation()) );
}


void ossimNitfImageHeader::setProperty(ossimRefPtr<ossimProperty> property)
{
}

ossimRefPtr<ossimProperty> ossimNitfImageHeader::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;

   if(name == TAGS_KW)
   {
      ossim_uint32 idxMax = getNumberOfTags();
      if(idxMax > 0)
      {
         ossimContainerProperty* containerProperty = new ossimContainerProperty;
         containerProperty->setName(name);
         ossim_uint32 idx = 0;

         result = containerProperty;
         for(idx = 0; idx < idxMax; ++idx)
         {
            ossimNitfTagInformation tagInfo;
            getTagInformation(tagInfo, idx);
            
            const ossimRefPtr<ossimNitfRegisteredTag> tagData = tagInfo.getTagData();
            if(tagData.valid())
            {
               ossimContainerProperty* containerPropertyTag = new ossimContainerProperty;
               containerPropertyTag->setName(tagInfo.getTagName());
               std::vector<ossimRefPtr<ossimProperty> > propertyList;
               tagData->getPropertyList(propertyList);
               
               containerPropertyTag->addChildren(propertyList);
               containerProperty->addChild(containerPropertyTag);
            }
            else
            {
               containerProperty->addStringProperty(tagInfo.getTagName(), "", true);
            }
         }
      }
   }
   
   return result;
}

void ossimNitfImageHeader::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(TAGS_KW);
  
}

ossim_uint32 ossimNitfImageHeader::getTotalTagLength()const
{
   ossim_uint32 tagLength = 0;
   
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      tagLength += theTagList[i].getTotalTagLength();
   }

   return tagLength;
}

void ossimNitfImageHeader::printTags(std::ostream& out)const
{
   out << setiosflags(std::ios::left)
       << "ossimNitfImageHeader::printTags";
   
   for(ossim_uint32 i = 0; i < theTagList.size(); ++i)
   {
      out << std::setw(24) << "\nTag name:"   << theTagList[i].getTagName()
          << std::setw(24) << "\nTag Length:" << theTagList[i].getTagLength()
          << std::endl;

      ossimRefPtr<ossimNitfRegisteredTag> tag = theTagList[i].getTagData();
      if (tag.valid())
      {
         tag->print(out);
      }
   }
}

bool ossimNitfImageHeader::hasLut()const
{
   return true;
}

ossimRefPtr<ossimNBandLutDataObject> ossimNitfImageHeader::createLut(ossim_uint32 bandIdx)const
{
   ossimRefPtr<ossimNBandLutDataObject> result;
   
   if(bandIdx < (ossim_uint32)getNumberOfBands())
   {
      const ossimRefPtr<ossimNitfImageBand> band = getBandInformation(bandIdx);
      if(band.valid())
      {
         ossim_uint32 bands = band->getNumberOfLuts();
         if(bands > 0)
         {
            if(band->getLut(0).valid())
            {
               ossim_uint32 entries = band->getLut(0)->getNumberOfEntries();
               
               result = new ossimNBandLutDataObject();
               result->create(entries, band->getNumberOfLuts());
               ossim_uint32 bIdx;
               ossim_uint32 entryIdx;

               for(bIdx = 0; bIdx < bands; ++bIdx)
               {
                  const ossimRefPtr<ossimNitfImageLut> lut = band->getLut(bIdx);
                  if(lut.valid())
                  {
                     for(entryIdx = 0; entryIdx < entries; ++entryIdx)
                     {
                        (*result)[entryIdx][bIdx] = (ossimNBandLutDataObject::LUT_ENTRY_TYPE)(lut->getValue(entryIdx));
                     }
                  }
               }
            }
         }
      }
   }

   return result;
}


void ossimNitfImageHeader::checkForGeographicTiePointTruncation(
   const ossimDpt& tie) const
{
   // One arc second in decimal degrees.
   const ossim_float64 ARC_SECOND = 1.0/3600.0;

   // Very small number.
   const ossim_float64 FUDGE_FACTOR = 0.000000001;

   // Remainder portion of latitude divided by an arc second.
   ossim_float64 y = std::fmod(tie.y, ARC_SECOND);

   // Remainder portion of longitue divided by an arc second.
   ossim_float64 x = std::fmod(tie.x, ARC_SECOND);

   if ( (std::fabs(y) > FUDGE_FACTOR) || (std::fabs(x) > FUDGE_FACTOR) )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfImageHeader WARNING:\n"
         << "Tie point will be truncated in image header:  "
         << tie
         << std::endl;
   }
}
