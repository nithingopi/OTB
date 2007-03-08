//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGrect.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimGrect_HEADER
#define ossimGrect_HEADER
#include <vector>
#include <iostream>

#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimCommon.h>

class OSSIMDLLEXPORT ossimGrect
{
public:
   friend std::ostream& operator<<(std::ostream& os, const ossimGrect& rect);

   /**
    * Will default to 0,0,0,0.
    */
    ossimGrect()
      :
         theUlCorner(0.0, 0.0, 0.0),
         theUrCorner(0.0, 0.0, 0.0),
         theLrCorner(0.0, 0.0, 0.0),
         theLlCorner(0.0, 0.0, 0.0)
      {}

   /**
    * Copies the passed in rectangle to this
    * object.
    */
   ossimGrect(const ossimGrect& rect)
      :
         theUlCorner(rect.ul()),
         theUrCorner(rect.ur()),
         theLrCorner(rect.lr()),
         theLlCorner(rect.ll())
      {}

   /**
    * WIll take two ground points and fill the
    * bounding rect appropriately.
    */
   ossimGrect(const ossimGpt& ul,
              const ossimGpt& lr)
      : 
         theUlCorner(ul),
         theUrCorner(ul.latd(), lr.lond(),0,ul.datum()),
         theLrCorner(lr),
         theLlCorner(lr.latd(), ul.lond(), 0, ul.datum())
      {
      }

   /**
    * Takes the upper left and lower right ground
    * points
    */
   ossimGrect(const double ulLat,
              const double ulLon,
              const double lrLat,
              const double lrLon,
              const ossimDatum* aDatum=ossimDatumFactory::instance()->wgs84())
      : 
         theUlCorner(ulLat, ulLon,0, aDatum),
         theUrCorner(ulLat, lrLon, 0, aDatum),
         theLrCorner(lrLat, lrLon, 0, aDatum),
         theLlCorner(lrLat, ulLon, 0, aDatum)
      {}
   ossimGrect(const ossimGpt& point,
              double latSpacingInDegrees,
              double lonSpacingInDegrees)
      :
         theUlCorner(point),
         theUrCorner(point),
         theLrCorner(point),
         theLlCorner(point)
      {
         std::vector<ossimGrect> v;
         computeEvenTiles(v, latSpacingInDegrees, lonSpacingInDegrees);
         if(v.size())
            *this = v[0];
      }
   ossimGrect(std::vector<ossimGpt>& points);
   ossimGrect(const ossimGpt& p1,
              const ossimGpt& p2,
              const ossimGpt& p3,
              const ossimGpt& p4);
              

   const ossimGrect& operator=(const ossimGrect& rect)
      {
         theUlCorner = rect.ul();
         theUrCorner = rect.ur();
         theLrCorner = rect.lr();
         theLlCorner = rect.ll();
         return *this;
      }

   ossimGpt midPoint()const;
   
   /**
    * Returns the height of a rectangle.
    */
   ossim_float64 height() const;

   /**
    * Returns the width of a rectangle.
    */
   ossim_float64 width()  const;
   
   const ossimGpt& ul()const;
   const ossimGpt& ur()const;
   const ossimGpt& ll()const;
   const ossimGpt& lr()const;
   
   ossimGpt& ul();
   ossimGpt& ur();
   ossimGpt& ll();
   ossimGpt& lr();
	
   void makeNan();
   
   bool isLonLatNan()const;
	
   bool hasNans()const;
   
   bool isNan()const;
   
   /*!
    * Returns true if "this" rectangle is contained completely within the
    * input rectangle "rect".
    */
   bool completely_within(const ossimGrect& rect) const;
	
   /*!
    * Returns true if any portion of an input rectangle "rect" intersects
    * "this" rectangle.  
    */
   bool intersects(const ossimGrect& rect) const;

   ossimGrect clipToRect(const ossimGrect& rect)const;

   ossimGrect combine(const ossimGrect& rect)const;

   /**
    * METHOD: pointWithin(ossimGpt)
    *
    * @param gpt Point to test for withinness.
    * 
    * @return true if argument is inside of horizontal rectangle
    *
    * @note Height is not considered and there is no datum shift applied if
    * gpt is of a different datum than this datum.
    */
   bool pointWithin(const ossimGpt& gpt) const; //inline below

   ossimGrect stretchToEvenBoundary(double latSpacingInDegrees,
                                    double lonSpacingInDegrees)const;
   
   void computeEvenTiles(std::vector<ossimGrect>& result,
                         double latSpacingInDegrees,
                         double lonSpacingInDegrees,
                         bool clipToGeographicBounds = true)const;
private:
   ossimGpt theUlCorner;
   ossimGpt theUrCorner;
   ossimGpt theLrCorner;
   ossimGpt theLlCorner;
  
};

//==================== BEGIN INLINE DEFINITIONS ===============================

//*****************************************************************************
//  INLINE METHOD: ossimGrect::midPoint()
//*****************************************************************************
inline ossimGpt ossimGrect::midPoint()const
{
   return ossimGpt((ul().latd() + ur().latd() + ll().latd() + lr().latd())*.25,
                   (ul().lond() + ur().lond() + ll().lond() + lr().lond())*.25,
                   (ul().height()+ur().height()+ll().height()+
                    lr().height())*.25,
                   ul().datum() );
}

//*****************************************************************************
//  INLINE METHOD: ossimGrect::clipToRect()
//*****************************************************************************
inline ossimGrect ossimGrect::clipToRect(const ossimGrect& rect)const
{
    double     ulx, uly, lrx, lry;

    ulx = ossimMax(rect.ul().lond(),ul().lond());
    uly = ossimMin(rect.ul().latd(),ul().latd());
    lrx = ossimMin(rect.lr().lond(),lr().lond());
    lry = ossimMax(rect.lr().latd(),lr().latd());

    if( lrx < ulx || lry > uly )
    {
       return ossimGrect(ossimGpt(0,0,0),ossimGpt(0,0,0));
    }
    else
    {
       return ossimGrect(ossimGpt(uly, ulx, 0, rect.ul().datum()),
                         ossimGpt(lry, lrx, 0, rect.ul().datum()));
    }
}

//*******************************************************************
// Inline Method: ossimDrect::combine(const ossimDrect& rect)
//*******************************************************************
inline ossimGrect ossimGrect::combine(const ossimGrect& rect)const
{
   ossimGpt ulCombine;
   ossimGpt lrCombine;
	
	ulCombine.lon = ((ul().lon <= rect.ul().lon)?ul().lon:rect.ul().lon);
   ulCombine.lat = ((ul().lat >= rect.ul().lat)?ul().lat:rect.ul().lat);
   lrCombine.lon = ((lr().lon >= rect.lr().lon)?lr().lon:rect.lr().lon);
   lrCombine.lat = ((lr().lat <= rect.lr().lat)?lr().lat:rect.lr().lat);
	
   return ossimGrect(ulCombine, lrCombine);
}

//*****************************************************************************
//  INLINE METHOD: ossimGrect::pointWithin()
//*****************************************************************************
inline bool ossimGrect::pointWithin(const ossimGpt& gpt) const
{
   return ((gpt.lat <= theUlCorner.lat) && (gpt.lat >= theLrCorner.lat) &&
           (gpt.lon >= theUlCorner.lon) && (gpt.lon <= theLrCorner.lon));
}

inline ossim_float64 ossimGrect::height() const
{
   return fabs(theLlCorner.latd() - theUlCorner.latd()) + 1.0;
}

inline ossim_float64 ossimGrect::width() const
{
   return fabs(theLrCorner.lond() - theLlCorner.lond()) + 1.0;
}

inline const ossimGpt& ossimGrect::ul() const
{
   return theUlCorner;
}

inline const ossimGpt& ossimGrect::ur() const
{
   return theUrCorner;
}

inline const ossimGpt& ossimGrect::ll() const
{
   return theLlCorner;
}

inline const ossimGpt& ossimGrect::lr() const
{
   return theLrCorner;
}
   
inline ossimGpt& ossimGrect::ul()
{
   return theUlCorner;
}

inline ossimGpt& ossimGrect::ur()
{
   return theUrCorner;
}

inline ossimGpt& ossimGrect::ll()
{
   return theLlCorner;
}

inline ossimGpt& ossimGrect::lr()
{
   return theLrCorner;
}
	
inline void ossimGrect::makeNan()
{
   theUlCorner.makeNan();
   theLlCorner.makeNan();
   theLrCorner.makeNan();
   theUrCorner.makeNan();
}

inline bool ossimGrect::isLonLatNan() const
{
   return ( ossimIsNan(theUlCorner.lat) ||
            ossimIsNan(theUlCorner.lon) ||
            ossimIsNan(theUrCorner.lat) ||
            ossimIsNan(theUrCorner.lon) ||
            ossimIsNan(theLrCorner.lat) ||
            ossimIsNan(theLrCorner.lon) ||
            ossimIsNan(theLlCorner.lat) ||
            ossimIsNan(theLlCorner.lon) );
}

inline bool ossimGrect::hasNans() const
{
   return ( theUlCorner.hasNans() ||
            theLlCorner.hasNans() ||
            theLrCorner.hasNans() ||
            theUrCorner.hasNans() );
}

inline bool ossimGrect::isNan()const
{
   return ( theUlCorner.hasNans() &&
            theLlCorner.hasNans() &&
            theLrCorner.hasNans() &&
            theUrCorner.hasNans() );
}

#endif /* End of "#ifndef ossimGrect_HEADER" */
