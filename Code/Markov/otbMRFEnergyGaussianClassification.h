
#ifndef _otbMRFEnergyGaussianClassification_h
#define _otbMRFEnergyGaussianClassification_h

#include "otbMRFEnergy.h"

#define M_SQUARE(x) ((x)*(x))

namespace otb
{
  /**
   * \class MRFEnergyGaussianClassification
   * \brief This is the implementation of the Gaussian model for Markov classification.
  *
  * This is the implementation of the Gaussian Energy model for Markov classification, to be used for
  * the fidelity term for classification. Energy is:
  * \f[   U(x_s / y_s) = \frac{(y_s+\mu_{x_s})^2}{2\sigma^2_{x_s}}+\log{\sqrt{2\pi}\sigma_{x_s}} \f]
  * with
  * - \f$ x_s \f$ the label on site s
  * - \f$ y_s \f$ the value on the reference image
  * - \f$ \mu_{x_s} \f$ and \f$ \sigma^2_{x_s} \f$ the mean and variance of label \f$ x_s \f$
   */
  
     template< class TInput1, class TInput2>    
                class ITK_EXPORT MRFEnergyGaussianClassification:public MRFEnergy< TInput1, TInput2>
    {
      public:
        typedef MRFEnergyGaussianClassification Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self>  Pointer;
        typedef itk::SmartPointer<const Self>  ConstPointer;
        
        typedef TInput1 InputImageType;
        typedef TInput2 LabelledImageType;
        typedef typename InputImageType::PixelType InputImagePixelType;
        typedef typename LabelledImageType::PixelType LabelledImagePixelType;
        
        typedef itk::Array< double > ParametersType;
    
        itkNewMacro(Self);
        
        itkTypeMacro(MRFEnergy,Object);
  
        void SetNumberOfParameters(unsigned int nParameters){ 
          this->m_NumberOfParameters=nParameters;
          this->m_Parameters.SetSize(this->m_NumberOfParameters);
          this->Modified();
        }
          
        double GetSingleValue(const InputImagePixelType & value1,  const LabelledImagePixelType & value2)   
        {
            if (value2 >= this->m_NumberOfParameters/2) {
              itkExceptionMacro(<<"Number of parameters does not correspond to number of classes" );
            } 
            double result=
                    M_SQUARE(value1-this->m_Parameters[2*value2])/(2*M_SQUARE(this->m_Parameters[2*value2+1]))
                    + vcl_log(vcl_sqrt(2*M_PI)*this->m_Parameters[2*value2+1]);
                

            return static_cast<double>( result );
        }
     

    
      protected:
      // The constructor and destructor.
        MRFEnergyGaussianClassification() {
            this->m_NumberOfParameters = 4;
            this->m_Parameters.SetSize(this->m_NumberOfParameters);
            this->m_Parameters[0]=50.0; //Class 0 mean
            this->m_Parameters[1]=10.0; //Class 0 stdev
            this->m_Parameters[2]=140.0;//Class 1 mean
            this->m_Parameters[3]=10.0; //Class 1 stdev
        };
        virtual ~MRFEnergyGaussianClassification() {};

    };
}

#endif
