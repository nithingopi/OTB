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
#ifndef __otbImageViewerScrollWidget_h
#define __otbImageViewerScrollWidget_h

#include "vcl_deprecated_header.h"

#include "otbFixedSizeFullImageWidget.h"
#include "otbImageWidgetRectangleForm.h"
#include "otbImageWidgetBoxForm.h"
#include "otbImageWidgetPointForm.h"
#include <FL/Fl.H>

namespace otb
{
template <class TPixel, class TLabel> class ImageViewerBase;

/** \class ImageViewerScrollWidget
 * \brief <b>DEPRECATED</b>: Custom scroll image widget.
 *
 * This class derives from otb::FixedSizeFullImageWidget, and implements behaviours specific to the image viewer.
 *
 *  \deprecated use the new Visualization framework instead.
 * \sa StandardImageViewer
 *
 * \sa ImageViewer, FixedSizeFullImageWidget
 *
 */
template <class TPixel, class TLabel>
class ITK_EXPORT ImageViewerScrollWidget
  : public FixedSizeFullImageWidget<TPixel>
{
public:
  /** Standard class typedefs */
  typedef ImageViewerScrollWidget          Self;
  typedef FixedSizeFullImageWidget<TPixel> Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewerScrollWidget, FixedSizeFullImageWidget);

  typedef TPixel                         PixelType;
  typedef TLabel                         LabelType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType  SizeType;

  typedef ImageViewerBase<PixelType, LabelType> ParentType;
  typedef ParentType*                           ParentPointerType;

  typedef otb::ImageWidgetBoxForm BoxType;
  typedef BoxType::ColorType      ColorType;

  itkSetMacro(Parent, ParentPointerType);
  itkGetMacro(Parent, ParentPointerType);

  /** Handle method */
  virtual int handle(int event)
  {
    switch (event)
      {
      case FL_PUSH:
        {
        int       x = Fl::event_x();
        int       y = Fl::event_y();
        IndexType clickedIndex;
        clickedIndex[0] = x;
        clickedIndex[1] = y;
        clickedIndex = this->WindowToImageCoordinates(clickedIndex);
        clickedIndex[0] = clickedIndex[0] * m_Parent->GetShrinkFactor();
        clickedIndex[1] = clickedIndex[1] * m_Parent->GetShrinkFactor();
        //std::cout<<"Scroll widget: "<<m_Parent->GetLabel()<<" before parent update"<<std::endl;
        m_Parent->ChangeFullViewedRegion(clickedIndex);
        m_Parent->ChangeZoomViewedRegion(clickedIndex);
        m_Parent->Update();
        //std::cout<<"Scroll widget: "<<m_Parent->GetLabel()<<" after parent update"<<std::endl;
        return 1;
        }
      case FL_ENTER:
        {
        m_MouseIn = true;
        return 1;
        }
      case FL_LEAVE:
        {
        m_MouseIn = false;
        m_Parent->ClearPixLocVal();
        return 1;
        }
      case FL_MOVE:
        {
        m_MouseIn = true;
        if (m_MouseMoveCount % m_ValueUpdateFrequency == 0)
          {
          m_MousePos[0] = Fl::event_x();
          m_MousePos[1] = Fl::event_y();
          IndexType newIndex = this->WindowToImageCoordinates(m_MousePos);
          IndexType realIndex;
          realIndex[0] = newIndex[0] * m_Parent->GetShrinkFactor();
          realIndex[1] = newIndex[1] * m_Parent->GetShrinkFactor();
          m_Parent->ReportPixel(realIndex);
          m_MouseMoveCount = 0;
          }
        m_MouseMoveCount++;
        return 1;
        }
      case FL_HIDE:
        {
        return 0;
        }
      }
    return 0;
  }

protected:
  /**
   * Constructor.
   */
  ImageViewerScrollWidget()
    {
    m_MouseIn = false;
    m_MousePos.Fill(0);
    m_MouseMoveCount = 0;
    m_ValueUpdateFrequency = 5;
    };
  /**
   * Destructor.
   */
  virtual ~ImageViewerScrollWidget()
    {
    m_Parent = NULL;
    }

private:
  ParentPointerType m_Parent;
  IndexType         m_MousePos;
  bool              m_MouseIn;
  unsigned int      m_MouseMoveCount;
  unsigned int      m_ValueUpdateFrequency;

};
} // end namespace otb
#endif
