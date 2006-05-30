/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSpatialObjectProperty.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 17:59:18 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __SpatialObjectProperty_h
#define __SpatialObjectProperty_h

#include <string>

#include "itkIndent.h"
#include "itkRGBAPixel.h"
#include "itkLightObject.h"

namespace itk{

/**
 * This class contains the objects properties such as colors, opacity, etc...
 * it's templated over the representation to use for each color component. 
 */
 
template< class TComponentType = float >
class ITK_EXPORT SpatialObjectProperty 
  : public LightObject 
{
public:

  typedef SpatialObjectProperty< TComponentType > Self;
  typedef LightObject Superclass;
  typedef RGBAPixel< TComponentType > PixelType;
  typedef std::string StringType;

  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  itkNewMacro( Self );
  itkTypeMacro( SpatialObjectProperty, LightObject );

  const PixelType & GetColor( void ) const;
  void SetColor(const PixelType & color );
  void SetColor( TComponentType r, TComponentType g ,TComponentType b);

  void SetRed( TComponentType r );
  TComponentType GetRed( void ) const;

  void SetGreen( TComponentType g );
  TComponentType GetGreen( void ) const;

  void SetBlue( TComponentType b );
  TComponentType GetBlue( void ) const;

  void SetAlpha( TComponentType a);
  TComponentType GetAlpha( void ) const;

  SpatialObjectProperty();
  virtual ~SpatialObjectProperty();

  void SetName( const char * name );
  StringType GetName( void ) const;

  unsigned long GetMTime(void){return m_MTime;}

protected:

  void PrintSelf(std::ostream &os, Indent indent) const;
  void Modified(void){m_MTime++;}

private:    

  PixelType m_Color;
  StringType m_Name;
  unsigned long m_MTime;

};

}



#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSpatialObjectProperty.txx"
#endif

#endif // __SpatialObjectProperty_h
