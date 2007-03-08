//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationEllipseObject.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <sstream>

#include <ossim/imaging/ossimGeoAnnotationEllipseObject.h>
#include <ossim/imaging/ossimAnnotationEllipseObject.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimUnitConversionTool.h>

RTTI_DEF1(ossimGeoAnnotationEllipseObject,
          "ossimGeoAnnotationEllipseObject",
          ossimGeoAnnotationObject);
   
ossimGeoAnnotationEllipseObject::ossimGeoAnnotationEllipseObject(
   const ossimGpt& center,
   const ossimDpt& widthHeight,
   bool enableFill,
   ossim_uint8 r,
   ossim_uint8 g,
   ossim_uint8 b,
   ossim_uint8 thickness)
   :ossimGeoAnnotationObject(r, g, b, thickness),
    theProjectedEllipse(NULL),
    theCenter(center),
    theWidthHeight(widthHeight),
    theEllipseWidthHeightUnitType(OSSIM_PIXEL) // default to image space
{
   theProjectedEllipse = new ossimAnnotationEllipseObject(ossimDpt(0,0),
							  ossimDpt(0,0),
							  enableFill,
							  r,
							  g,
							  b,
							  thickness);
}

ossimGeoAnnotationEllipseObject::ossimGeoAnnotationEllipseObject(
   const ossimGeoAnnotationEllipseObject& rhs)
   :ossimGeoAnnotationObject(rhs),
    theProjectedEllipse(rhs.theProjectedEllipse?(ossimAnnotationEllipseObject*)rhs.theProjectedEllipse->dup():(ossimAnnotationEllipseObject*)NULL),
    theCenter(rhs.theCenter),
    theWidthHeight(rhs.theWidthHeight),
    theEllipseWidthHeightUnitType(rhs.theEllipseWidthHeightUnitType)
{
}

ossimGeoAnnotationEllipseObject::~ossimGeoAnnotationEllipseObject()
{
   if(theProjectedEllipse)
   {
      delete theProjectedEllipse;
      theProjectedEllipse = NULL;
   }
}

ossimObject* ossimGeoAnnotationEllipseObject::dup()const
{
   return new ossimGeoAnnotationEllipseObject(*this);
}

void ossimGeoAnnotationEllipseObject::applyScale(double x,
                                                 double y)
{
   if(theProjectedEllipse) theProjectedEllipse->applyScale(x, y);
   theCenter.lond(theCenter.lond()*x);
   theCenter.latd(theCenter.latd()*y);
   theWidthHeight .x *= x;
   theWidthHeight .y *= y;
}

std::ostream& ossimGeoAnnotationEllipseObject::print(std::ostream& out)const
{
   out << "center ground:                  " <<  theCenter << endl
       << "width height image space flag = " << theEllipseWidthHeightUnitType
       << endl
       << *theProjectedEllipse;
   return out;
}

void ossimGeoAnnotationEllipseObject::draw(ossimRgbImage& anImage)const
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->draw(anImage);
   }
}

bool ossimGeoAnnotationEllipseObject::intersects(const ossimDrect& rect)const
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->intersects(rect);
   }

   return false;
}

ossimAnnotationObject* ossimGeoAnnotationEllipseObject::getNewClippedObject(const ossimDrect& rect)const
{
   if(intersects(rect))
   {
      if(theProjectedEllipse)
      {
         return theProjectedEllipse->getNewClippedObject(rect);
      }
   }
   
   return (ossimAnnotationObject*)NULL;
}

void ossimGeoAnnotationEllipseObject::getBoundingRect(ossimDrect& rect)const
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->getBoundingRect(rect);
   }
   else
   {
      rect.makeNan();
   }
}

void ossimGeoAnnotationEllipseObject::computeBoundingRect()
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->computeBoundingRect();
   }
}

void ossimGeoAnnotationEllipseObject::transform(ossimProjection* projection)
{
   if(!projection)
   {
      return;
   }

   // Ellipse center, height and width in image space.
   ossimDpt projectedCenter;
   ossimDpt projectedWidthHeight;

   // first get the center projected
   projection->worldToLineSample(theCenter, projectedCenter);

   getWidthHeightInPixels(projectedWidthHeight, projection);

   theProjectedEllipse->setCenterWidthHeight(projectedCenter,
                                             projectedWidthHeight);      
}

bool ossimGeoAnnotationEllipseObject::saveState(ossimKeywordlist& kwl,
                                                const char* prefix) const
{
   kwl.add(prefix, "center", theCenter.toString().c_str());
   kwl.add(prefix, "ellipse_width", theWidthHeight.x);
   kwl.add(prefix, "ellipse_height", theWidthHeight.y);
   kwl.add(prefix, "fill", (ossim_int32)theProjectedEllipse->getFill());
   kwl.add(prefix, "units",
           ossimUnitTypeLut::instance()->getEntryString(theEllipseWidthHeightUnitType).c_str());
   
   return ossimGeoAnnotationObject::saveState(kwl, prefix);
}

bool ossimGeoAnnotationEllipseObject::loadState(const ossimKeywordlist& kwl,
                                                const char* prefix)
{
   if (!theProjectedEllipse)
   {
      return false;
   }
   
   //---
   // Base class state must be called first to pick up colors...
   //---
   bool status =  ossimGeoAnnotationObject::loadState(kwl, prefix);
   if (status == true)
   {
      theProjectedEllipse->setColor(theRed, theGreen, theBlue);
      theProjectedEllipse->setThickness(theThickness);
   }

   const char* center         = kwl.find(prefix, "center");
   const char* ellipse_width  = kwl.find(prefix, "ellipse_width");
   const char* ellipse_height = kwl.find(prefix, "ellipse_height");
   const char* fill           = kwl.find(prefix, "fill");
   const char* units          = kwl.find(prefix, "units");
   
   if(units)
   {
      theEllipseWidthHeightUnitType =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->getEntryNumber(units));
      if (theEllipseWidthHeightUnitType == OSSIM_UNIT_UNKNOWN)
      {
         theEllipseWidthHeightUnitType = OSSIM_PIXEL;
      }
   }

   if (center)
   {
      std::istringstream is(center);
      is >> theCenter;
   }
   if(ellipse_width)
   {
      theWidthHeight.x = ossimString(ellipse_width).toDouble();
   }
   if(ellipse_height)
   {
      theWidthHeight.y = ossimString(ellipse_height).toDouble();
   }
   if(fill)
   {
      if(ossimString(fill).toLong())
      {
         theProjectedEllipse->setFill(true);
      }
      else
      {
         theProjectedEllipse->setFill(false);
      }
   }

   return status;
}

void ossimGeoAnnotationEllipseObject::setEllipseWidthHeightUnitType(
   ossimUnitType type)
{
   theEllipseWidthHeightUnitType = type;
}

void ossimGeoAnnotationEllipseObject::setWidthHeight(
   const ossimProjection* projection, const ossimDpt& pt)
{
   theWidthHeight = pt;

   if (projection)
   {
      // Ellipse center, height and width in image space.
      ossimDpt projectedCenter;

      // first get the center projected
      projection->worldToLineSample(theCenter, projectedCenter);
      
      theProjectedEllipse->setCenterWidthHeight(projectedCenter, pt);
   }
}

void ossimGeoAnnotationEllipseObject::getWidthHeightInPixels(
   ossimDpt& widthHeight, const ossimProjection* projection) const
{
   switch (theEllipseWidthHeightUnitType)
   {
      case OSSIM_PIXEL:
      {
         widthHeight = theWidthHeight;

         break;
      }
      
      case OSSIM_DEGREES:
      case OSSIM_MINUTES:
      case OSSIM_SECONDS:
      {
         ossimGpt origin = projection->origin();
         ossimDpt gsd = projection->getMetersPerPixel();
         
         ossimUnitConversionTool uct(origin,
                                     theWidthHeight.x,
                                     theEllipseWidthHeightUnitType);

         double x = uct.getValue(OSSIM_METERS);

         uct.setValue(theWidthHeight.y, theEllipseWidthHeightUnitType);
         
         double y = uct.getValue(OSSIM_METERS);

         widthHeight.x = ceil(x/gsd.x);
         widthHeight.y = ceil(y/gsd.y);
         
         break;
      }

      case OSSIM_METERS:
      {
         ossimDpt gsd = projection->getMetersPerPixel();
         widthHeight.x = static_cast<ossim_int32>(theWidthHeight.x/gsd.x+0.5);
         widthHeight.y = static_cast<ossim_int32>(theWidthHeight.y/gsd.y+0.5);

         break;
      }

      default:
      {
         ossimDpt gsd = projection->getMetersPerPixel();
         
         ossimUnitConversionTool uct(theWidthHeight.x,
                                     theEllipseWidthHeightUnitType);

         double x = uct.getValue(OSSIM_METERS);
         
         uct.setValue(theWidthHeight.y, theEllipseWidthHeightUnitType);
         
         double y = uct.getValue(OSSIM_METERS);
         
         widthHeight.x = ceil(x/gsd.x);
         widthHeight.y = ceil(y/gsd.y);
         
         break;
      }
   }
}
void ossimGeoAnnotationEllipseObject::setColor(ossim_uint8 r,
                                               ossim_uint8 g,
                                               ossim_uint8 b)
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->setColor(r, g, b);
   }
}

void ossimGeoAnnotationEllipseObject::setThickness(ossim_uint8 thickness)
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->setThickness(thickness);
   }
}

void ossimGeoAnnotationEllipseObject::setFillFlag(bool flag)
{
   if(theProjectedEllipse)
   {
      theProjectedEllipse->setFill(flag);
   }
}
