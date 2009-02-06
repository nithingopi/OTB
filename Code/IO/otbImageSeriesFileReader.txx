/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:	  $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

	 This software is distributed WITHOUT ANY WARRANTY; without even 
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	 PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbImageSeriesFileReader__txx
#define __otbImageSeriesFileReader__txx

#include "otbImageSeriesFileReader.h"

namespace otb {

template < class TImage >
ImageSeriesFileReader< TImage > 
::ImageSeriesFileReader ()
{
  m_OutputList = OutputImageListType::New();
  m_ListReader = ListReaderType::New();
}

template < class TImage >
typename ImageSeriesFileReader<TImage>::OutputImageListType *
ImageSeriesFileReader< TImage > 
::GetOutput ()
{
  if ( this->GetNumberOfOutputs() < 1 )
    return 0;

  return static_cast<OutputImageListType*>( this->m_OutputList );
}

template < class TImage >
typename ImageSeriesFileReader<TImage>::OutputImageType *
ImageSeriesFileReader< TImage > 
::GetOutput ( unsigned int idx )
{
  if ( this->GetNumberOfOutputs() < 1 )
    return 0;

  if ( idx >= m_ListReader->Size() )
    return 0;

  return static_cast<OutputImageType*>( this->m_ListReader->GetNthElement( idx )->GetOutput() );
}

template < class TImage >
void
ImageSeriesFileReader< TImage > 
::GenerateData( )
{
  this->TestFileExistanceAndReadability();

  std::ifstream inputFile ( m_FileName.c_str(), std::ios_base::in );
  std::string aLine;

	inputFile >> aLine;
	if ( aLine != "ENVI" ) 
	{
    inputFile.close();
    itk::OStringStream msg;
    msg << "The file " << m_FileName << " is not a \"ENVI META FILE\" format\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
    throw e;
    return;
  }

  inputFile >> aLine;
	if ( aLine != "META" ) 
	{
    inputFile.close();
    itk::OStringStream msg;
    msg << "The file " << m_FileName << " is not a \"ENVI META FILE\" format\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
    throw e;
    return;
  }
  
  inputFile >> aLine;
	if ( aLine != "FILE" ) 
	{
    inputFile.close();
    itk::OStringStream msg;
    msg << "The file " << m_FileName << " is not a \"ENVI META FILE\" format\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
    throw e;
    return;
  }

  while ( 1 )
	{
    /* 
     * Reading the filename
     */
    do {
			inputFile >> aLine;
			//std::cerr << "-> '" << aLine << "'\n";
		} while ( aLine != "File" && inputFile.good() );

		if ( inputFile.good() )
    {
      do {
        inputFile >> aLine;
        //std::cerr << "--> '" << aLine << "'\n";
      } while ( aLine != ":" && inputFile.good() );

      if ( !inputFile.good() )
      {
        inputFile.close();
        itk::OStringStream msg;
        msg << "Unable to read image files in the  \"ENVI META FILE\" file \n";
        msg << "FileName: " << m_FileName << "\n";
        ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
        throw e;
        return;
      }
    }
    else
      return; // normal exit 
    
    inputFile >> aLine;

    std::cerr << "-> " << aLine << "\n";

    ReaderPointerType aReader = ReaderType::New();
    aReader->SetFileName( aLine );

    /*
     * Reading the Band number
     */
    do {
			inputFile >> aLine;
			//std::cerr << "-> '" << aLine << "'\n";
		} while ( aLine != "Bands:" && inputFile.good() );

    if ( !inputFile.good() )
    {
      inputFile.close();
      itk::OStringStream msg;
      msg << "Unable to read the number of bands in the images in the  \"ENVI META FILE\" file \n";
      msg << "FileName: " << m_FileName << "\n";
      ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
      throw e;
      return;
    }

    int numBand = 0;
    inputFile >> numBand;
    //std::cerr << "-> '" << numBand << "'\n";

#if 0
    // FIXME
    // At this step, we do not know how to handle band selection from ENVI META FILE...
    //
    if( strcmp( this->GetOutput()->GetNameOfClass(), "VectorImage" ) == 0 )
    {

    }
#endif

    if ( numBand != 1 && strcmp( this->GetOutput()->GetNameOfClass(), "Image" ) == 0 )
    {
      inputFile.close();
      itk::OStringStream msg;
      msg << "Unable to handle multicomponent file from Image<> class\n";
      msg << "\"ENVI META FILE\" FileName: " << m_FileName << "\n";
      msg << "Image FileName             : " << aReader->GetFileName() << "\n";
      ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
      throw e;
      return;
    }

    /*
     * Reading the Region selection
     */
    do {
			inputFile >> aLine;
			//std::cerr << "-> '" << aLine << "'\n";
		} while ( aLine != "Dims" && inputFile.good() );

		if ( inputFile.good() )
    {
      do {
        inputFile >> aLine;
        //std::cerr << "--> '" << aLine << "'\n";
      } while ( aLine != ":" && inputFile.good() );

      if ( !inputFile.good() )
      {
        inputFile.close();
        itk::OStringStream msg;
        msg << "Unable to read image region in the  \"ENVI META FILE\" file \n";
        msg << "FileName : " << m_FileName << "\n";
        msg << "ImageName: " << aReader->GetFileName() << "\n";
        ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
        throw e;
        return;
      }
    }

    int beg_line,end_line,beg_col,end_col;
    char sep1,sep2,sep3;
    inputFile >> beg_col >> sep1 >> beg_line >> sep2 >> end_col >> sep3 >> end_line;

    if ( !inputFile.good() )
    {
      inputFile.close();
      itk::OStringStream msg;
      msg << "Unable to read image region selection in the  \"ENVI META FILE\" file \n";
      msg << "FileName : " << m_FileName << "\n";
      msg << "ImageName: " << aReader->GetFileName() << "\n";
      ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
      throw e;
      return;
    }

    IndexType index;
    index[0] = beg_line - 1; 
    index[1] = beg_col - 1;

    SizeType size;
    size[0] = end_line - beg_line;
    size[1] = end_col - beg_col;

    RegionType region;
    region.SetSize( size );
    region.SetIndex( index );

    /*
     * FIXME: it seems that teh region selection is not working as it stands...
     * May be necessary to use MultiChannelExtractROI...
     */

    aReader->UpdateOutputInformation(); 
    aReader->GetOutput()->SetRequestedRegion( region );
    aReader->GetOutput()->PropagateRequestedRegion();
    aReader->GetOutput()->UpdateOutputData(); 

    m_OutputList->PushBack( aReader->GetOutput() );  
    m_ListReader->PushBack( aReader );
  }
}

template < class TImage >
void
ImageSeriesFileReader< TImage > 
::PrintSelf ( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "File to be read : " << m_FileName << "\n";

  if ( m_ListReader->Size() > 0 )
  {
    os << indent << "Image File(s) to be read\n";
    for ( typename ListReaderType::Iterator file = m_ListReader->Begin();
      file != m_ListReader->End(); ++file )
    {
      os << indent << "  " << file.Get()->GetFileName() << "\n";
    }
  }
}

template < class TImage >
void
ImageSeriesFileReader< TImage > 
::TestFileExistanceAndReadability ()
{
  // Test if the file exists.
  if( !itksys::SystemTools::FileExists( m_FileName.c_str() ) )
  {
    ImageSeriesFileReaderException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << m_FileName
        << std::endl;
    e.SetDescription( msg.str().c_str() );
    throw e;
    return;
  }

  // Test if the file can be open for reading access.
  std::ifstream readTester;
  readTester.open( m_FileName.c_str() );
  if( readTester.fail() )
  {
    readTester.close();
    itk::OStringStream msg;
    msg << "The file couldn't be opened for reading. "
        << std::endl << "Filename: " << m_FileName
        << std::endl;
    ImageSeriesFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
    throw e;
    return;
  }
  readTester.close();
}

} // end of namespace otb

#endif


