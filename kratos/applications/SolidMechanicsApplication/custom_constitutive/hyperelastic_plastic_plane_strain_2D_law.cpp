//
//   Project Name:        KratosSolidMechanicsApplication $
//   Last modified by:    $Author:            JMCarbonell $
//   Date:                $Date:                July 2013 $
//   Revision:            $Revision:                  0.0 $
//
//

// System includes
#include <iostream>

// External includes
#include<cmath>

// Project includes
#include "includes/properties.h"
#include "custom_constitutive/hyperelastic_plastic_plane_strain_2D_law.hpp"

#include "solid_mechanics_application.h"

namespace Kratos
{

//******************************CONSTRUCTOR*******************************************
//************************************************************************************

HyperElasticPlasticPlaneStrain2DLaw::HyperElasticPlasticPlaneStrain2DLaw()
    : HyperElasticPlastic3DLaw()
{

}


//******************************CONSTRUCTOR*******************************************
//************************************************************************************

HyperElasticPlasticPlaneStrain2DLaw::HyperElasticPlasticPlaneStrain2DLaw(FlowRulePointer pFlowRule, YieldCriterionPointer pYieldCriterion, HardeningLawPointer pHardeningLaw)
    : HyperElasticPlastic3DLaw()
{
}

//******************************COPY CONSTRUCTOR**************************************
//************************************************************************************

HyperElasticPlasticPlaneStrain2DLaw::HyperElasticPlasticPlaneStrain2DLaw(const HyperElasticPlasticPlaneStrain2DLaw& rOther)
    : HyperElasticPlastic3DLaw(rOther)
{

}

//********************************CLONE***********************************************
//************************************************************************************

ConstitutiveLaw::Pointer HyperElasticPlasticPlaneStrain2DLaw::Clone() const
{
    HyperElasticPlasticPlaneStrain2DLaw::Pointer p_clone(new HyperElasticPlasticPlaneStrain2DLaw(*this));
    return p_clone;
}

//*******************************DESTRUCTOR*******************************************
//************************************************************************************

HyperElasticPlasticPlaneStrain2DLaw::~HyperElasticPlasticPlaneStrain2DLaw()
{
}



//************* COMPUTING  METHODS
//************************************************************************************
//************************************************************************************

//***********************COMPUTE TOTAL STRAIN*****************************************
//************************************************************************************

void HyperElasticPlasticPlaneStrain2DLaw::CalculateGreenLagrangeStrain( const Matrix & rRightCauchyGreen,
        Vector& rStrainVector )
{

    //E= 0.5*(FT*F-1)
    rStrainVector[0] = 0.5 * ( rRightCauchyGreen( 0, 0 ) - 1.00 );
    rStrainVector[1] = 0.5 * ( rRightCauchyGreen( 1, 1 ) - 1.00 );
    rStrainVector[2] = rRightCauchyGreen( 0, 1 );

}


//***********************COMPUTE TOTAL STRAIN*****************************************
//************************************************************************************

void HyperElasticPlasticPlaneStrain2DLaw::CalculateAlmansiStrain( const Matrix & rLeftCauchyGreen,
        Vector& rStrainVector )
{

    // e= 0.5*(1-invbT*invb)
    Matrix InverseLeftCauchyGreen ( rLeftCauchyGreen.size1() , rLeftCauchyGreen.size2() );
    double det_b=0;
    MathUtils<double>::InvertMatrix( rLeftCauchyGreen, InverseLeftCauchyGreen, det_b);

    rStrainVector.clear();
    rStrainVector[0] = 0.5 * ( 1.0 - InverseLeftCauchyGreen( 0, 0 ) );
    rStrainVector[1] = 0.5 * ( 1.0 - InverseLeftCauchyGreen( 1, 1 ) );
    rStrainVector[2] = -InverseLeftCauchyGreen( 0, 1 );


}


//***********************COMPUTE ISOCHORIC CONSTITUTIVE MATRIX************************
//************************************************************************************

void HyperElasticPlasticPlaneStrain2DLaw::CalculateIsochoricConstitutiveMatrix (const MaterialResponseVariables & rElasticVariables,
        const Vector & rIsoStressVector,
        Matrix& rConstitutiveMatrix)
{

    rConstitutiveMatrix.clear();

    Matrix IsoStressMatrix = MathUtils<double>::StressVectorToTensor( rIsoStressVector );

    static const unsigned int msIndexVoigt2D [3][2] = { {0, 0}, {1, 1}, {0, 1} };

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
            rConstitutiveMatrix( i, j ) = IsochoricConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, IsoStressMatrix,
                                          msIndexVoigt2D[i][0], msIndexVoigt2D[i][1], msIndexVoigt2D[j][0], msIndexVoigt2D[j][1]);
        }

    }


}

//***********************COMPUTE VOLUMETRIC CONSTITUTIVE MATRIX***********************
//************************************************************************************


void HyperElasticPlasticPlaneStrain2DLaw::CalculateVolumetricConstitutiveMatrix ( const MaterialResponseVariables & rElasticVariables,
								  Matrix& rConstitutiveMatrix)
{

    rConstitutiveMatrix.clear();

    Vector Factors = ZeroVector(3);
    Factors = CalculateDomainPressureFactors( rElasticVariables, Factors );

    static const unsigned int msIndexVoigt2D [3][2] = { {0, 0}, {1, 1}, {0, 1} };

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
            rConstitutiveMatrix( i, j ) = VolumetricConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, Factors,
                                          msIndexVoigt2D[i][0], msIndexVoigt2D[i][1], msIndexVoigt2D[j][0], msIndexVoigt2D[j][1]);
        }

    }


}


//***********************COMPUTE PLASTIC CONSTITUTIVE MATRIX**************************
//************************************************************************************

void HyperElasticPlasticPlaneStrain2DLaw::CalculatePlasticConstitutiveMatrix (const MaterialResponseVariables & rElasticVariables,
									      FlowRule::RadialReturnVariables & rReturnMappingVariables,
									      Matrix& rConstitutiveMatrix)
{

    rConstitutiveMatrix.clear();

    Matrix IsoStressMatrix = MathUtils<double>::StressVectorToTensor( rReturnMappingVariables.TrialIsoStressVector );

    //std::cout<< " TrialStressMatrix 2D "<<IsoStressMatrix<<std::endl;

    FlowRule::PlasticFactors ScalingFactors;
    mpFlowRule->CalculateScalingFactors( rReturnMappingVariables, ScalingFactors );

    static const unsigned int msIndexVoigt2D [3][2] = { {0, 0}, {1, 1}, {0, 1} };

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
		rConstitutiveMatrix( i, j ) = PlasticConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, IsoStressMatrix, ScalingFactors,
                                          msIndexVoigt2D[i][0], msIndexVoigt2D[i][1], msIndexVoigt2D[j][0], msIndexVoigt2D[j][1]);
        }

    }


}



//***********************COMPUTE ISOCHORIC CONSTITUTIVE MATRIX PULL-BACK**************
//************************************************************************************

void HyperElasticPlasticPlaneStrain2DLaw::CalculateIsochoricConstitutiveMatrix (const MaterialResponseVariables & rElasticVariables,
								     const Matrix & rInverseDeformationGradientF,
								     const Vector & rIsoStressVector,
								     Matrix& rConstitutiveMatrix)
{

    rConstitutiveMatrix.clear();

    Matrix IsoStressMatrix = MathUtils<double>::StressVectorToTensor( rIsoStressVector );

    static const unsigned int msIndexVoigt2D [3][2] = { {0, 0}, {1, 1}, {0, 1} };

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
	  rConstitutiveMatrix( i, j ) = IsochoricConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, rInverseDeformationGradientF, IsoStressMatrix,
                                          msIndexVoigt2D[i][0], msIndexVoigt2D[i][1], msIndexVoigt2D[j][0], msIndexVoigt2D[j][1]);
        }

    }


}

//***********************COMPUTE VOLUMETRIC CONSTITUTIVE MATRIX PULL-BACK*************
//************************************************************************************


void HyperElasticPlasticPlaneStrain2DLaw::CalculateVolumetricConstitutiveMatrix ( const MaterialResponseVariables & rElasticVariables,
								       const Matrix & rInverseDeformationGradientF,
								       Matrix& rConstitutiveMatrix)
{

    rConstitutiveMatrix.clear();

    Vector Factors = ZeroVector(3);
    Factors = CalculateDomainPressureFactors( rElasticVariables, Factors );

    static const unsigned int msIndexVoigt2D [3][2] = { {0, 0}, {1, 1}, {0, 1} };

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
	  rConstitutiveMatrix( i, j ) = VolumetricConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, rInverseDeformationGradientF, Factors,
                                          msIndexVoigt2D[i][0], msIndexVoigt2D[i][1], msIndexVoigt2D[j][0], msIndexVoigt2D[j][1]);
        }

    }


}


//***********************COMPUTE PLASTIC CONSTITUTIVE MATRIX PULL-BACK****************
//************************************************************************************

void HyperElasticPlasticPlaneStrain2DLaw::CalculatePlasticConstitutiveMatrix (const MaterialResponseVariables & rElasticVariables,
								   const Matrix & rInverseDeformationGradientF,
								   FlowRule::RadialReturnVariables & rReturnMappingVariables,
								   Matrix& rConstitutiveMatrix)
{

    rConstitutiveMatrix.clear();

    Matrix IsoStressMatrix = MathUtils<double>::StressVectorToTensor( rReturnMappingVariables.TrialIsoStressVector );

    FlowRule::PlasticFactors ScalingFactors;
    mpFlowRule->CalculateScalingFactors( rReturnMappingVariables, ScalingFactors );

    static const unsigned int msIndexVoigt2D [3][2] = { {0, 0}, {1, 1}, {0, 1} };

    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
	  rConstitutiveMatrix( i, j ) = PlasticConstitutiveComponent(rConstitutiveMatrix( i, j ), rElasticVariables, rInverseDeformationGradientF, IsoStressMatrix, ScalingFactors,
									   msIndexVoigt2D[i][0], msIndexVoigt2D[i][1], msIndexVoigt2D[j][0], msIndexVoigt2D[j][1]);
        }

    }


}


} // Namespace Kratos
