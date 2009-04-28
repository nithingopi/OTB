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

#include "otbVectorDataKeywordlist.h"
#include <iomanip>

namespace otb
{


VectorDataKeywordlist
  ::VectorDataKeywordlist()
{
  //Nothing to do here
}

VectorDataKeywordlist
  ::~VectorDataKeywordlist()
{
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
  {
    if (m_FieldList[i].first->GetType() == OFTString)
    {
      CPLFree( m_FieldList[i].second.String );
    }
    delete(m_FieldList[i].first);
  }
}

void VectorDataKeywordlist::
    AddField(OGRFieldDefn* fieldDefn, OGRField* field)
{
  FieldType newField;
  newField.first = fieldDefn;
  newField.second = *field;
  //TODO: evaluate performance impact of fieldDefn copy
  // the object itself could be handle at the VectorData level
  // keeping only pointer here. (but it does not seem
  // necessary so far...)
  m_FieldList.push_back(CopyOgrField(newField));
};

void
VectorDataKeywordlist::
    operator=(const Self& p)
{
  for (unsigned int i = 0; i < p.m_FieldList.size(); ++i)
  {
    m_FieldList.push_back(CopyOgrField(p.m_FieldList[i]));
  }
}

void
VectorDataKeywordlist::
    Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void
VectorDataKeywordlist::
    PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << " VectorData Keyword list: ";
  os << indent << " - Size: " << m_FieldList.size() << std::endl;
  for (unsigned int i = 0; i < m_FieldList.size(); ++i)
  {
    os << indent << "    " << PrintField(m_FieldList[i]);
  }
}

std::string
VectorDataKeywordlist::
    PrintField(FieldType field) const
{
  std::stringstream output;
  output << std::setprecision(15);
  output << field.first->GetNameRef() << " (";
  output << field.first->GetFieldTypeName(field.first->GetType()) << "): ";
  switch(field.first->GetType())
  {
    case OFTInteger:
    {
      output << field.second.Integer;
      break;
    }
    case OFTIntegerList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTReal:
    {
      output << field.second.Real;
      break;
    }
    case OFTRealList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTString:
    {
      if (field.second.String != NULL)
      {
        output << field.second.String;
      }
      break;
    }
    case OFTStringList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTWideString:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTWideStringList:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTBinary:
    {
      output << "Type not handled for printing";
      break;
    }
    case OFTDate:
    {
      output << field.second.Date.Year << field.second.Date.Month << field.second.Date.Day;
      break;
    }
    case OFTTime:
    {
      output << field.second.Date.Hour << field.second.Date.Minute << field.second.Date.Second;
      break;
    }
    case OFTDateTime:
    {
      output << field.second.Date.Year << field.second.Date.Month << field.second.Date.Day << "-"
          << field.second.Date.Hour << field.second.Date.Minute << field.second.Date.Second;
      break;
    }
  }
  output << std::endl;
  return output.str();
}


VectorDataKeywordlist::FieldType
VectorDataKeywordlist::
    CopyOgrField(FieldType field)
{
  FieldType outField;
  outField.first = new OGRFieldDefn(field.first);
  switch(field.first->GetType())
  {
    case OFTInteger:
    {
      outField.second.Integer = field.second.Integer;
      break;
    }
    case OFTIntegerList:
    {
      std::cerr  << "OGR type not handled" << std::endl;
      break;
    }
    case OFTReal:
    {
      outField.second.Real = field.second.Real;
      break;
    }
    case OFTRealList:
    {
      std::cerr << "OGR type not handled" << std::endl;
      break;
    }
    case OFTString:
    {
      if (field.second.String != NULL)
      {
        CPLFree( outField.second.String );
        outField.second.String = CPLStrdup( field.second.String );
      }
      break;
    }
    case OFTStringList:
    {
      std::cerr << "OGR type not handled" << std::endl;
      break;
    }
    case OFTWideString:
    {
      std::cerr << "OGR type not handled" << std::endl;
      break;
    }
    case OFTWideStringList:
    {
      std::cerr << "OGR type not handled" << std::endl;
      break;
    }
    case OFTBinary:
    {
      std::cerr << "OGR type not handled" << std::endl;
      break;
    }
    case OFTDate:
    {
      outField.second.Date.Year = field.second.Date.Year;
      outField.second.Date.Month = field.second.Date.Month;
      outField.second.Date.Day = field.second.Date.Day;
      break;
    }
    case OFTTime:
    {
      outField.second.Date.Hour = field.second.Date.Hour;
      outField.second.Date.Minute = field.second.Date.Minute;
      outField.second.Date.Second = field.second.Date.Second;
      break;
    }
    case OFTDateTime:
    {
      outField.second.Date.Year = field.second.Date.Year;
      outField.second.Date.Month = field.second.Date.Month;
      outField.second.Date.Day = field.second.Date.Day;
      outField.second.Date.Hour = field.second.Date.Hour;
      outField.second.Date.Minute = field.second.Date.Minute;
      outField.second.Date.Second = field.second.Date.Second;
      break;
    }
  }
  return outField;
}


std::ostream &
    operator<<(std::ostream &os, const VectorDataKeywordlist &kwl)
{
  kwl.Print(os);
  return os;
}

}