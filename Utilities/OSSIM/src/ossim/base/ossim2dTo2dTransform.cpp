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
// $Id: ossim2dTo2dTransform.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossim2dTo2dTransform.h>
#include <sstream>

RTTI_DEF1(ossim2dTo2dTransform, "ossim2dTo2dTransform", ossimObject);

#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotifyContext.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossim2dTo2dTransform:exec");
static ossimTrace traceDebug ("ossim2dTo2dTransform:debug");

static const double DEFAULT_THRESHOLD      = 1000.0*DBL_EPSILON;
static const int    DEFAULT_MAX_ITERATIONS = 10;

//*****************************************************************************
//  CONSTRUCTOR: 
//*****************************************************************************
ossim2dTo2dTransform::ossim2dTo2dTransform()
   :
      theConvergenceThreshold (DEFAULT_THRESHOLD),
      theMaxIterations        (DEFAULT_MAX_ITERATIONS),
      theDxDy(1.0, 1.0)
{
}
 
//*****************************************************************************
//  METHOD: 
//*****************************************************************************
void ossim2dTo2dTransform::inverse(const ossimDpt& input,
                                   ossimDpt&       output) const
{
   static const char MODULE[] = "ossim2dTo2dTransform::inverse";
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << ", entering...\n";
   
   //***
   // Begin with guess. Forward transform is defined as trasforming left to
   // right. We are therefore solving for left:
   //***
   ossimDpt left = getOrigin();
   ossimDpt left_dx;
   ossimDpt left_dy;
   ossimDpt right;
   ossimDpt right_dx;
   ossimDpt right_dy;
   ossimDpt dr_dx;
   ossimDpt dr_dy;
   ossimDpt r_diff;
   ossimDpt l_diff;
   double inverse_norm;
   int iters=0;
   //***
   // Begin iterations:
   //***
   do
   {
      //***
      // establish perturbed image points about the guessed point:
      //***
      left_dx.x = left.x + theDxDy.x;
      left_dx.y = left.y;
      left_dy.x = left.x;
      left_dy.y = left.y + theDxDy.y;
      
      //***
      // Compute numerical partials at current guessed point:
      //***
      forward(left,    right);
      forward(left_dx, right_dx);
      forward(left_dy, right_dy);

      dr_dx.x = (right_dx.x - right.x)/theDxDy.x; //e
      dr_dx.y = (right_dx.y - right.y)/theDxDy.y; //g
      dr_dy.x = (right_dy.x - right.x)/theDxDy.x; //f
      dr_dy.y = (right_dy.y - right.y)/theDxDy.y; //h

      //***
      // Test for convergence:
      //***
      r_diff = input - right;
      
      //***
      // Compute linearized estimate of image point given gp delta:
      //***
      inverse_norm = dr_dy.x*dr_dx.y - dr_dx.x*dr_dy.y; // fg-eh

      if (inverse_norm != 0)
      {
         l_diff.x = (-dr_dy.y*r_diff.x + dr_dy.x*r_diff.y)/inverse_norm;
         l_diff.y = ( dr_dx.y*r_diff.x - dr_dx.x*r_diff.y)/inverse_norm;

         left += l_diff;
      }
      else
      {
         l_diff.x = 0;
         l_diff.y = 0;
      }

      iters++;
      
   } while (((fabs(l_diff.x) > theConvergenceThreshold) ||
             (fabs(l_diff.y) > theConvergenceThreshold)) &&
            (iters < theMaxIterations));

   //***
   // Note that this error mesage appears only if max count was reached while
   // iterating. A linear (no iteration) solution would finish with iters =
   // MAX_NUM_ITERATIONS + 1:
   //***
    if (iters == theMaxIterations)
    {
       ossimNotify(ossimNotifyLevel_WARN) << "WARNING: " << MODULE << ", exceeded max number of iterations computing inverse "
                                          << "transform for point: " << input << "\n";
    }

   output = left;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: " << MODULE << ", returning...\n";
   return;
   
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
bool ossim2dTo2dTransform::saveState(ossimKeywordlist& kwl,
                                     const char* prefix) const
{
   kwl.add(prefix,
           ossimKeywordNames::CONVERGENCE_THRESHOLD_KW,
           theConvergenceThreshold,
           true);
   kwl.add(prefix,
           ossimKeywordNames::MAX_ITERATIONS_KW,
           theMaxIterations,
           true);
   kwl.add(prefix,
           "dxdy",
           ossimString::toString(theDxDy.x) + " " +
           ossimString::toString(theDxDy.y),
           true);
           
   return ossimObject::saveState(kwl, prefix);
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
bool ossim2dTo2dTransform::loadState(const ossimKeywordlist& kwl,
               const char* prefix)
{
  bool result = true;

   const char* buf;

   buf= kwl.find(prefix, ossimKeywordNames::CONVERGENCE_THRESHOLD_KW);

   if (buf)
   {
      theConvergenceThreshold = atof(buf);
   }
   else
   {
      theConvergenceThreshold = .00000000000002;
   }
   
   buf= kwl.find(prefix, ossimKeywordNames::MAX_ITERATIONS_KW);
   if(buf)
   {
      theMaxIterations = atoi(buf);
   }
   else
   {
      theMaxIterations = 10;
   }
   const char* dxdy = kwl.find(prefix, "dxdy");
   if(dxdy)
   {
      std::istringstream in(dxdy);
      in>>theDxDy.x >> theDxDy.y;
   }
   if(result)
   {
      ossimObject::loadState(kwl, prefix);
   }
   
   return result;
}
   
void ossim2dTo2dTransform::forward(ossimDpt&  modify_this) const 
{
   ossimDpt output;
   forward(modify_this, output);
   modify_this = output;
}

void ossim2dTo2dTransform::inverse(ossimDpt&  modify_this) const 
{
   ossimDpt output;
   inverse(modify_this, output);
   modify_this = output;
}

ossimDpt ossim2dTo2dTransform::getOrigin()const
{
   return ossimDpt(0,0);
}

void ossim2dTo2dTransform::setConvergenceThreshold(const double& new_threshold)
{
   theConvergenceThreshold = new_threshold;
}

void ossim2dTo2dTransform::setMaxIterations(int new_max_iters)
{
   theMaxIterations = new_max_iters;
}

void ossim2dTo2dTransform::setDxDy(const ossimDpt& dxdy)
{
   theDxDy.x = dxdy.x;
   theDxDy.y = dxdy.y;
}

void ossim2dTo2dTransform::operator=(const ossim2dTo2dTransform& /* rhs */ )
{
}
