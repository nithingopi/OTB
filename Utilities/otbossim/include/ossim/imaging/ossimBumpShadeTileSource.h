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
// $Id: ossimBumpShadeTileSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimBumpShadeTileSource_HEADER
#define ossimBumpShadeTileSource_HEADER
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>

class ossimImageData;

class ossimImageSourceConnection;

/**
 * This uses a derivative of Blinn's bump function to compute a 3-D
 * looking image.  The elevation data can be from Grey Scale image or from
 * a Dted file.  This class can take one or two inputs.  The First input is an
 * elevation input that is used to compute a perturbed Normal which is
 * used in a simple lambertian shading equation.  The second input is some
 * color source (Grey or RGB) that defines the diffuse color used within
 * the illumination equation for each pixel. If the diffuse color input is not
 * supplied then an all white color is used for the color.  There are several
 * adjustable parameters that can be set:
 * 1) the Incident light vector described by the Azimuth and Elevation Angle,
 * 2) the pixel scale used along the x and y direction, and
 * 3) the partial derivative matrix used for the partial derivative
 * computation along the x and y direction.
 *
 * <pre>
 * 
 * Some examples of good values:
 *
 * If we are using dted 90 meters as input for shading then:
 * 1) set pixel scale to 180.  Since the partials do 2*dx and 2*dy then
 *    the scale is 2*90meters = 180.
 * 2) set azimuth angle to 45 and set elevation angle to 45 degrees.  Change
 *    them to see the effects.
 * 3) use the following matrix -1 0 1
 *                             -1 0 1
 *                             -1 0 1
 *                                &
 *                              1  1  1
 *                              0  0  0
 *                             -1 -1 -1
 * 
 *    for the partials along x and y directions respectively.  These matrices
 *
 *    (f(x+dx, y) - f(x-dx, y))/2*dx and
 *    (f(x, y+dy) - f(x, y-dy))/2*dy
 *
 *    for an apprximate derivative about some point x, y
 *
 * If we are using pixel intesity (1 - 255) then set the scales to around
 * 100.
 *
 *  Note: varying the scales will vary the smoothness/roughness of the surface
 *        Smaller the scale the rougher the surface will look.
 *
 * The output of this source is always a 3-band 8-bit image.
 *
 * Keywords:
 *   type:               ossimBumpShadeTileSource
 *   azimuth_angle:      45.000000000000000
 *   elevation_angle:    45.000000000000000
 *   scale_per_pixel_x:  180.000000000000000  // this is the 2*dx defined above
 *   scale_per_pixel_y:  180.000000000000000  // this is the 2*dy defined above
 *
 *   the next set of keywords are optional and the default is as defined above.
 *   these define the matrix.
 *
 *
 * A typical chain would look something like this:
 * 
 * OrthoMosaicOfElevation-->Normals-->Renderer-->|
 *                                               |--> BumpShade-->Output 
 * ColorSource----------------------->Renderer-->|
 *
 * NOTE:  "Normals" is an ossimImageToPlaneNormalFilter class
 * 
 * </pre>
 * 
 */
class OSSIMDLLEXPORT ossimBumpShadeTileSource : public ossimImageCombiner
{
public:
   ossimBumpShadeTileSource();

   /**
    * The bump map  input source is used to bump or shade the input color
    * source.  The input color source currently must be a 1 or 3 band
    * image.  Note that the output of this source is always
    * 3 bands (r, g, b).
    */
   ossimBumpShadeTileSource(ossimImageSource* colorSource,
                            ossimImageSource* bumpMapSource);
      
   ossimString getShortName()const;
   ossimString getLongName()const;

   ossimRefPtr<ossimImageData> getTile(const  ossimIrect& tileRect,
                                       ossim_uint32 resLevel=0);

   /**
    * Since the input bump souce is not used for bounding rect computation
    * this call is passed on to theColorInputSource.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result) const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;
   virtual ossim_uint32 getNumberOfDecimationLevels()const;
   
   /**
    * This should always return 3
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

   /**
    * The output scalar type should always be 8-bit
    */
   virtual ossimScalarType getOutputScalarType() const;
   
   /**
    * the null pixel is 0.0.
    * \return null pixel value is always 0
    *
    */
   virtual double getNullPixelValue()const;

   /**
    * the min pixel is 1.0.
    * \return min pixel value is always 1
    *
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;

   /**
    * the max pixel is 255.
    * \return max pixel value is always 255
    *
    */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   /**
    * Returns the current tile width.  This is passed to first input.
    * \return Returns the tile width.
    */
   virtual ossim_uint32 getTileWidth()const;

   /**
    * Returns the current tile height.  This is passed to first input.
    * \return Returns the tile height..
    */
   virtual ossim_uint32 getTileHeight()const;

   /**
    * \return Returns the rotation of the incident vector..
    */
   virtual double getAzimuthAngle()const;

   /**
    * \return Returns the elevation of the incident vector.
    */
   virtual double getElevationAngle()const;

   /**
    *
    */
   virtual void setAzimuthAngle(double angle);

   /**
    *
    */
   virtual void setElevationAngle(double angle);

   /**
    *
    */
   virtual void initialize();

   /**
    *
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    *
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    *
    */
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;
      
   /**
    *
    */
   virtual void connectInputEvent(ossimConnectionEvent& event);

   /**
    *
    */
   virtual void disconnectInputEvent(ossimConnectionEvent& event);

   /**
    *
    */
   virtual void propertyEvent(ossimPropertyEvent& event);

   virtual void refreshEvent(ossimRefreshEvent& event);

   /**
    * Will re-compute the light direction given the elevation angle and
    * azimuth angle.
    */
   void computeLightDirection();

   /* ------------------- PROPERTY INTERFACE -------------------- */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /* ------------------ PROPERTY INTERFACE END ------------------- */
   
protected:
   virtual ~ossimBumpShadeTileSource();
   void allocate();
   /**
    * The result of the illumination equation is stored in
    * here.  This is populated on each call to getTile.
    */
   ossimRefPtr<ossimImageData> theTile;

   /**
    * Used for the light vector computation.
    */
   double theLightSourceElevationAngle;

   /**
    * Used for the light vector computation.
    */
   double theLightSourceAzimuthAngle;

   /**
    * This is computed from the elevation and
    * azimuth angles of the light source.
    */
   NEWMAT::ColumnVector theLightDirection;

   void computeColor(ossim_uint8& r,
                     ossim_uint8& g,
                     ossim_uint8& b,
                     ossim_float64 normalX,
                     ossim_float64 normalY,
                     ossim_float64 normalZ,
                     ossim_uint8 dr,
                     ossim_uint8 dg,
                     ossim_uint8 db)const;
   
TYPE_DATA
};

#endif
