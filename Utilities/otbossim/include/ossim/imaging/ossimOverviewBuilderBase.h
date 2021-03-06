//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  Base class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimOverviewBuilderBase_HEADER
#define ossimOverviewBuilderBase_HEADER

#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>

#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

class ossimImageFileWriter;
class ossimImageHandler;
class ossimFilename;


class OSSIM_DLL ossimOverviewBuilderBase
   :
      public ossimSource,
      public ossimProcessInterface,
      public ossimConnectableObjectListener
{
public:

   /** default constructor */
   ossimOverviewBuilderBase();

   /**
    * @brief Sets an optional file writer to the builder.
    * Currently used by the virtual overview builder.
    * 
    * @param outputWriter The file writer of the builder.
    * 
    * @return True on successful initialization, false on error.
    */
   virtual bool setOutputWriter(ossimImageFileWriter* outputWriter);

   /**
    * @brief Sets the input to the builder. (pure virtual)
    * 
    * @param imageSource The input to the builder.
    * 
    * @return True on successful initialization, false on error.
    */
   virtual bool setInputSource(ossimImageHandler* imageSource) = 0;

   /**
    * @brief Sets the output file name. (pure virtual)
    * @prama file This will be the output file name like foo.ovr
    */
   virtual void setOutputFile(const ossimFilename& file)=0;

   /**
    * @brief Gets the output file name. (pure virtual)
    *
    * @return The output file name or ossimFilename::NIL if it was not set
    * yet and the image handle has not been initialized.
    *
    * @note This will return ossimFilename::NIL unless one of was called,
    * setInputSource or setOutputFile.
    */
   virtual ossimFilename getOutputFile() const=0;

   /**
    * @brief Sets the overview output type. (pure virtual)
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    * @return true if type is handled, false if not.

    * @note Currently handled types are 
    */
   virtual bool setOverviewType(const ossimString& type)=0;

   /**
    * @brief Gets the overview type. (pure virtual)
    * @return The overview output type as a string. */
   virtual ossimString getOverviewType() const=0;

   /**
    * @brief Method to check if builder can handle type.
    * @return true if type is handled by builder, false if not.
    */
   virtual bool hasOverviewType(const ossimString& type) const;

   /**
    * @brief Method to populate class supported types. (pure virtual)
    * Example:
    * ossimTiffOverviewNearest
    * ossimTiffOverviewBox
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const=0;

   /**
    * @brief Get the overview stop dimension.
    * @return The overview stop dimension.
    */
   virtual ossim_uint32 getOverviewStopDimension() const;

   /**
    * @brief Sets the overview stop dimension.
    *
    * This controls how many layers will be built. If set to 64 then the
    * builder will stop when height and width for current level are less
    * than or equal to 64.  Note a default can be set in the ossim preferences
    * file, setting the keyword "overview_stop_dimension".
    * 
    * @param dim The overview stop dimension
    */
   virtual void setOverviewStopDimension(ossim_uint32 dim);

   /**
    * @brief Gets the histogram accumulation mode.
    * @return mode NONE, NORMAL or FAST.
    */
   ossimHistogramMode getHistogramMode() const;

   /**
    * @brief Sets the histogram accumulation mode.
    * @param mode NONE, NORMAL or FAST.
    */
   void setHistogramMode(ossimHistogramMode mode);
 
   /**
    * @brief Builds the overviews. (pure virtual)
    *
    * @return true on success, false on error.
    */
   virtual bool execute()=0;

    /**
    * @brief Sets the histogram accumulation mode.
    * @param mode NONE, NORMAL or FAST.
    */
   void setMaskBuildFlag(bool maskBuildFlag) { m_maskBuildFlag = maskBuildFlag; }

protected:
   /** virtual destructor */
   virtual ~ossimOverviewBuilderBase();

   /**
    *  @brief Gets the required number of res levels.
    *
    *  Convenience method to get the required number of reduced resolution
    *  data sets to get to the smallest dimension of the output tile size.
    *  Note that this include r0.
    *
    *  @param ih Pointer to the image handler.
    *
    *  @return number of res levels.
    */
   ossim_uint32 getRequiredResLevels(const ossimImageHandler* ih) const;
  
   /**
    * @brief Gets the default stop dimension.
    *
    * Looks for overview_stop_dimension, then will use minimum default tile
    * size if that is not found.
    * 
    * @returns Returns the default stop dimension.
    */
   ossim_uint32 getDefaultStopDimension() const;
   
   ossim_uint32 m_overviewStopDimension;
   ossimHistogramMode m_histoMode;   

   bool m_maskBuildFlag;

   /** for rtti stuff */
   TYPE_DATA
};

#endif /* End of "#ifndef ossimOverviewBuilderBase_HEADER" */
