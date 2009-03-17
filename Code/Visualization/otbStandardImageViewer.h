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
#ifndef __otbStandardImageViewer_h
#define __otbStandardImageViewer_h

#include "otbImage.h"
#include "itkRGBPixel.h"
#include "otbImageLayer.h"
#include "otbImageLayerRenderingModel.h"
#include "otbImageView.h"
#include "otbImageWidgetController.h"
#include "otbImageLayerGenerator.h"
#include "otbWidgetResizingActionHandler.h"
#include "otbChangeScaledExtractRegionActionHandler.h"
#include "otbChangeExtractRegionActionHandler.h"
#include "otbChangeScaleActionHandler.h"
#include "otbCurves2DWidget.h"
#include "otbHistogramCurve.h"
#include "otbPixelDescriptionModel.h"
#include "otbPixelDescriptionActionHandler.h"
#include "otbPixelDescriptionView.h"

#include <Fl/Fl_Tile.H>
#include <Fl/Fl_Group.H>

namespace otb
{
/** \class StandardImageViewer
*   \brief This class implements a standard visualization tool to be
*   plugged at the end of a pipeline.
*  
*/

template <class TImage>  
class StandardImageViewer
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef StandardImageViewer               Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(StandardImageViewer,Object);

  /** Input image type */
  typedef TImage                                    ImageType;
  typedef typename ImageType::Pointer               ImagePointerType;

  /** Output image type */
  typedef itk::RGBPixel<unsigned char>              RGBPixelType;
  typedef Image<RGBPixelType,2>                     OutputImageType;
  
  /** Image layer type */
  typedef ImageLayer<ImageType>                     ImageLayerType;
  typedef typename ImageLayerType::Pointer          ImageLayerPointerType;
  typedef typename ImageLayerType::HistogramType    HistogramType;
  
  /** Image layer generator type */
  typedef ImageLayerGenerator<ImageLayerType>       ImageLayerGeneratorType;
  typedef typename ImageLayerGeneratorType::Pointer ImageLayerGeneratorPointerType;

  /** Rendering model type */
  typedef ImageLayerRenderingModel<OutputImageType> RenderingModelType;
  typedef typename RenderingModelType::Pointer      RenderingModelPointerType;
  
  /** View type */
  typedef ImageView<RenderingModelType>             ViewType;
  typedef typename ViewType::Pointer                ViewPointerType;

  /** Widget controller */
  typedef ImageWidgetController                     WidgetControllerType;
  typedef typename WidgetControllerType::Pointer    WidgetControllerPointerType;
  
  /** Curves 2D widget */
  typedef Curves2DWidget                            CurvesWidgetType;
  typedef typename CurvesWidgetType::Pointer        CurvesWidgetPointerType;
  typedef HistogramCurve<HistogramType>             HistogramCurveType;
  typedef typename HistogramCurveType::Pointer      HistogramCurvePointerType;

  /** Standard action handlers */
  typedef otb::WidgetResizingActionHandler
  <RenderingModelType,ViewType>                     ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
  <RenderingModelType,ViewType>                     ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
  <RenderingModelType,ViewType>                     ChangeRegionHandlerType;
  typedef otb::ChangeScaleActionHandler
  <RenderingModelType,ViewType>                     ChangeScaleHandlerType;

  /** Pixel description */
  typedef PixelDescriptionModel<OutputImageType>    PixelDescriptionModelType;
  typedef typename PixelDescriptionModelType::Pointer PixelDescriptionModelPointerType;
  typedef PixelDescriptionActionHandler
    < PixelDescriptionModelType, ViewType>          PixelDescriptionActionHandlerType;
  typedef otb::PixelDescriptionView
    < PixelDescriptionModelType >                   PixelDescriptionViewType;
  typedef typename PixelDescriptionViewType::Pointer PixelDescriptionViewPointerType;

  /** Set/Get the image to render */
  itkSetObjectMacro(Image,ImageType);
  itkGetObjectMacro(Image,ImageType);

  /** Update and show the widget (you should call Fl::run() to hold to
   * display */
  void Update();

protected:
  /** Constructor */
  StandardImageViewer();
  /** Destructor */
  ~StandardImageViewer();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const {};
 
private:
  StandardImageViewer(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Pointer to the image */
  ImagePointerType            m_Image;
  
  /** The image layer */
  ImageLayerPointerType       m_ImageLayer;

  /** The rendering model */
  RenderingModelPointerType   m_RenderingModel;

  /** The pixel description model */
  PixelDescriptionModelPointerType m_PixelDescriptionModel;
  
  /** The view */
  ViewPointerType             m_View;

  /** The pixel description view */
  PixelDescriptionViewPointerType m_PixelDescriptionView;

  /** Curve widget */
  CurvesWidgetPointerType     m_CurveWidget;

  /** The widget controller */
  WidgetControllerPointerType m_Controller;

  /** The window */
  Fl_Window * m_Window;

  Fl_Group *  m_FullGroup;

   Fl_Group *  m_SideGroup;

  Fl_Tile  *  m_Tile;

  /** Intial sizes */
  int m_Width;
  
  int m_Height;

  int m_SideBarWidth;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStandardImageViewer.txx"
#endif

#endif
