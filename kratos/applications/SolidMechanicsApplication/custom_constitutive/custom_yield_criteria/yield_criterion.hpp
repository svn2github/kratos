//
//   Project Name:        KratosSolidMechanicsApplication $
//   Last modified by:    $Author:            JMCarbonell $
//   Date:                $Date:                July 2013 $
//   Revision:            $Revision:                  0.0 $
//
//

#if !defined(KRATOS_YIELD_CRITERION_H_INCLUDED )
#define  KRATOS_YIELD_CRITERION_H_INCLUDED



// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/serializer.h"
#include "includes/properties.h"
#include "custom_constitutive/custom_hardening_laws/hardening_law.hpp"

namespace Kratos
{
///@addtogroup ApplicationNameApplication
///@{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Short class definition.
/** Detail class definition.
*/
class YieldCriterion
{
    public:


	struct Parameters
	{
		const double* mpStressNorm;

                //Set Parameters
		void SetStressNorm  (const double& rStressNorm)  { mpStressNorm = &rStressNorm; };

		//Get Parameters
		const double& GetStressNorm  () const { return *mpStressNorm;  };


		HardeningLaw::Parameters HardeningParameters;

		//Set Hardening Parameters
		void SetRateFactor  (double rRateFactor)         { HardeningParameters.SetRateFactor(rRateFactor);   };
		void SetDeltaGamma  (const double& rDeltaGamma)  { HardeningParameters.SetDeltaGamma(rDeltaGamma);   };
		void SetLameMu_bar  (const double& rLameMu_bar)  { HardeningParameters.SetLameMu_bar(rLameMu_bar);   };
		void SetDeltaTime   (const double& rDeltaTime)   { HardeningParameters.SetDeltaTime(rDeltaTime);     };
		void SetTemperature (const double& rTemperature) { HardeningParameters.SetTemperature(rTemperature); };
		
		void SetEquivalentPlasticStrain       (const double& rEquivalentPlasticStrain)       {  HardeningParameters.SetEquivalentPlasticStrain(rEquivalentPlasticStrain);       };
		void SetEquivalentPlasticStrainOld    (const double& rEquivalentPlasticStrainOld)    {  HardeningParameters.SetEquivalentPlasticStrainOld(rEquivalentPlasticStrainOld); };
		
		//Get Hardening Parameters
		const double&  GetRateFactor () const { return HardeningParameters.GetRateFactor();   };
		const double& GetDeltaGamma  () const { return HardeningParameters.GetDeltaGamma();   };
		const double& GetLameMu_bar  () const { return HardeningParameters.GetLameMu_bar();   };
		const double& GetDeltaTime   () const { return HardeningParameters.GetDeltaTime();    };
		const double& GetTemperature () const { return HardeningParameters.GetTemperature();  };
		
		const double& GetEquivalentPlasticStrain       () const { return HardeningParameters.GetEquivalentPlasticStrain();    };
		const double& GetEquivalentPlasticStrainOld    () const { return HardeningParameters.GetEquivalentPlasticStrainOld(); };
		
		const HardeningLaw::Parameters& GetHardeningParameters  () const { return HardeningParameters; };

	};

        ///@name Type Definitions
        ///@{
        typedef HardeningLaw::Pointer        HardeningLawPointer;

        /// Pointer definition of YieldCriterion
        KRATOS_CLASS_POINTER_DEFINITION(YieldCriterion);

        ///@}
        ///@name Life Cycle
        ///@{

        /// Default constructor.
        YieldCriterion()
	{
	  //KRATOS_ERROR(std::logic_error, "calling the default constructor in YieldCriterion ... illegal operation!!","");
	};

        /// Copy constructor.
        YieldCriterion(YieldCriterion const& rOther)
	:mpHardeningLaw(rOther.mpHardeningLaw)
	{

	};

        /// Assignment operator.
        YieldCriterion& operator=(YieldCriterion const& rOther)
	{
		mpHardeningLaw = rOther.mpHardeningLaw;
		return *this;
	}


        /// Destructor.
        virtual ~YieldCriterion() {};


        ///@}
        ///@name Operators
        ///@{


        ///@}
        ///@name Operations
        ///@{
	void InitializeMaterial (HardeningLawPointer pHardeningLaw)
	{
		mpHardeningLaw = pHardeningLaw;
	}


        virtual double& CalculateYieldCondition(double & rStateFunction, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rStateFunction;
	};


        virtual double& CalculateStateFunction(double & rStateFunction, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rStateFunction;
	};

        virtual double& CalculateDeltaStateFunction(double & rDeltaStateFunction, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rDeltaStateFunction;
	};


        virtual double& CalculatePlasticDissipation(double & rPlasticDissipation, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rPlasticDissipation;
	};


        virtual double& CalculateDeltaPlasticDissipation(double & rDeltaPlasticDissipation, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rDeltaPlasticDissipation;
	};


        virtual double& CalculateImplexPlasticDissipation(double & rPlasticDissipation, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rPlasticDissipation;
	};


        virtual double& CalculateImplexDeltaPlasticDissipation(double & rDeltaPlasticDissipation, const Parameters& rVariables)
	{
		KRATOS_ERROR(std::logic_error, "calling the base class function in YieldCriterion ... illegal operation!!","");

		return rDeltaPlasticDissipation;
	};

        ///@}
        ///@name Access
        ///@{
        

        ///@}
        ///@name Inquiry
        ///@{


        ///@}
        ///@name Input and output
        ///@{

        // /// Turn back information as a string.
        // virtual std::string Info() const;

        // /// Print information about this object.
        // virtual void PrintInfo(std::ostream& rOStream) const;

        // /// Print object's data.
        // virtual void PrintData(std::ostream& rOStream) const;


        ///@}
        ///@name Friends
        ///@{


        ///@}

    protected:
        ///@name Protected static Member Variables
        ///@{


        ///@}
        ///@name Protected member Variables
        ///@{
	
	HardeningLawPointer mpHardeningLaw;
	
        ///@}
        ///@name Protected Operators
        ///@{


        ///@}
        ///@name Protected Operations
        ///@{


        ///@}
        ///@name Protected  Access
        ///@{


        ///@}
        ///@name Protected Inquiry
        ///@{


        ///@}
        ///@name Protected LifeCycle
        ///@{


        ///@}

    private:
        ///@name Static Member Variables
        ///@{


        ///@}
        ///@name Member Variables
        ///@{
	

        ///@}
        ///@name Private Operators
        ///@{


        ///@}
        ///@name Private Operations
        ///@{


        ///@}
        ///@name Private  Access
        ///@{

	
	///@}
	///@name Serialization
	///@{
	friend class Serializer;

	// A private default constructor necessary for serialization

	virtual void save(Serializer& rSerializer) const
	{
	};

	virtual void load(Serializer& rSerializer)
	{
	};

        ///@}
        ///@name Private Inquiry
        ///@{


        ///@}
        ///@name Un accessible methods
        ///@{

        ///@}

    }; // Class YieldCriterion

    ///@}

    ///@name Type Definitions
    ///@{


    ///@}
    ///@name Input and output
    ///@{


    // /// input stream function
    // inline std::istream& operator >> (std::istream& rIStream,
    //                                   YieldCriterion& rThis);

    // /// output stream function
    // inline std::ostream& operator << (std::ostream& rOStream,
    //                                   const YieldCriterion& rThis)
    // {
    //     rThis.PrintInfo(rOStream);
    //     rOStream << std::endl;
    //     rThis.PrintData(rOStream);

    //     return rOStream;
    // }
    ///@}

    ///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_YIELD_CRITERION_H_INCLUDED  defined 


