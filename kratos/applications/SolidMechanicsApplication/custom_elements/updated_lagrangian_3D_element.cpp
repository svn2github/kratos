//   
//   Project Name:        KratosSolidMechanicsApplication $      
//   Last modified by:    $Author:            JMCarbonell $ 
//   Date:                $Date:                July 2013 $
//   Revision:            $Revision:                  0.0 $
//
//

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "custom_elements/updated_lagrangian_3D_element.hpp"
#include "utilities/math_utils.h"
#include "includes/constitutive_law.h"
#include "solid_mechanics_application.h"


namespace Kratos
{
 

  //******************************CONSTRUCTOR*******************************************
  //************************************************************************************

  UpdatedLagrangian3DElement::UpdatedLagrangian3DElement( IndexType NewId, GeometryType::Pointer pGeometry )
    : SpatialLagrangian3DElement( NewId, pGeometry )
  {
    //DO NOT ADD DOFS HERE!!!
  }


  //******************************CONSTRUCTOR*******************************************
  //************************************************************************************

  UpdatedLagrangian3DElement::UpdatedLagrangian3DElement( IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties )
    : SpatialLagrangian3DElement( NewId, pGeometry, pProperties )
  {
    //DO NOT ADD DOFS HERE!!!
  }


  //******************************COPY CONSTRUCTOR**************************************
  //************************************************************************************

  UpdatedLagrangian3DElement::UpdatedLagrangian3DElement( UpdatedLagrangian3DElement const& rOther)
    :SpatialLagrangian3DElement(rOther)
  {
  }


  //*******************************ASSIGMENT OPERATOR***********************************
  //************************************************************************************

  UpdatedLagrangian3DElement&  UpdatedLagrangian3DElement::operator=(UpdatedLagrangian3DElement const& rOther)
  {
    SpatialLagrangian3DElement::operator=(rOther);

    return *this;
  }


  //*********************************OPERATIONS*****************************************
  //************************************************************************************

  Element::Pointer UpdatedLagrangian3DElement::Create( IndexType NewId, NodesArrayType const& rThisNodes, PropertiesType::Pointer pProperties ) const
  {
    return Element::Pointer( new UpdatedLagrangian3DElement( NewId, GetGeometry().Create( rThisNodes ), pProperties ) );
  }


  //*******************************DESTRUCTOR*******************************************
  //************************************************************************************

  UpdatedLagrangian3DElement::~UpdatedLagrangian3DElement()
  {
  }


  //************************************************************************************
  //************************************************************************************

  void UpdatedLagrangian3DElement::SetStandardParameters(Standard& rVariables,
							 ConstitutiveLaw::Parameters& rValues,
							 const int & rPointNumber)
  {
    LargeDisplacement3DElement::SetStandardParameters(rVariables,rValues,rPointNumber);

    //Set extra options for the contitutive law
    Flags &ConstitutiveLawOptions=rValues.GetOptions();
    ConstitutiveLawOptions.Set(ConstitutiveLaw::LAST_KNOWN_CONFIGURATION);

  }

  //*********************************COMPUTE KINEMATICS*********************************
  //************************************************************************************


  void UpdatedLagrangian3DElement::CalculateKinematics(Standard& rVariables,
						     const double& rPointNumber)

  {
    KRATOS_TRY
      
    const GeometryType::ShapeFunctionsGradientsType& DN_De = GetGeometry().ShapeFunctionsLocalGradients( mThisIntegrationMethod );

    unsigned int dimension = GetGeometry().WorkingSpaceDimension();

    //Parent to reference configuration
    rVariables.StressMeasure = ConstitutiveLaw::StressMeasure_PK2;

    //Parent to reference configuration
    Matrix J ( dimension , dimension);
    J = GetGeometry().Jacobian( J, rPointNumber , mThisIntegrationMethod );

    //Calculating the inverse of the jacobian and the parameters needed
    Matrix InvJ;
    MathUtils<double>::InvertMatrix( J, InvJ, rVariables.detJ);

    //Compute cartesian derivatives
    noalias( rVariables.DN_DX ) = prod( DN_De[rPointNumber] , InvJ );

    //Calculate Delta Position
    Matrix& DeltaPosition =  CalculateDeltaPosition(DeltaPosition);

    //Current Deformation Gradient F
    this->CalculateDeformationGradient (rVariables.DN_DX, rVariables.F, DeltaPosition);

    //Determinant of the Deformation Gradient F0
    rVariables.detF0 = mDeterminantF0[rPointNumber];
    rVariables.F0    = mDeformationGradientF0[rPointNumber];

    //Compute the deformation matrix B
    this->CalculateDeformationMatrix(rVariables.B, rVariables.F, rVariables.DN_DX);


    KRATOS_CATCH( "" )
      }


  //************************************************************************************
  //************************************************************************************

  void UpdatedLagrangian3DElement::CalculateDeformationMatrix(Matrix& rB,
							      Matrix& rF,
							      Matrix& rDN_DX)
  {
    KRATOS_TRY
    const unsigned int number_of_nodes = GetGeometry().PointsNumber();
 
    for ( unsigned int i = 0; i < number_of_nodes; i++ )
      {
	unsigned int index = 3 * i;

	rB( 0, index + 0 ) = rF( 0, 0 ) * rDN_DX( i, 0 );
	rB( 0, index + 1 ) = rF( 1, 0 ) * rDN_DX( i, 0 );
	rB( 0, index + 2 ) = rF( 2, 0 ) * rDN_DX( i, 0 );
	rB( 1, index + 0 ) = rF( 0, 1 ) * rDN_DX( i, 1 );
	rB( 1, index + 1 ) = rF( 1, 1 ) * rDN_DX( i, 1 );
	rB( 1, index + 2 ) = rF( 2, 1 ) * rDN_DX( i, 1 );
	rB( 2, index + 0 ) = rF( 0, 2 ) * rDN_DX( i, 2 );
	rB( 2, index + 1 ) = rF( 1, 2 ) * rDN_DX( i, 2 );
	rB( 2, index + 2 ) = rF( 2, 2 ) * rDN_DX( i, 2 );
	rB( 3, index + 0 ) = rF( 0, 0 ) * rDN_DX( i, 1 ) + rF( 0, 1 ) * rDN_DX( i, 0 );
	rB( 3, index + 1 ) = rF( 1, 0 ) * rDN_DX( i, 1 ) + rF( 1, 1 ) * rDN_DX( i, 0 );
	rB( 3, index + 2 ) = rF( 2, 0 ) * rDN_DX( i, 1 ) + rF( 2, 1 ) * rDN_DX( i, 0 );
	rB( 4, index + 0 ) = rF( 0, 1 ) * rDN_DX( i, 2 ) + rF( 0, 2 ) * rDN_DX( i, 1 );
	rB( 4, index + 1 ) = rF( 1, 1 ) * rDN_DX( i, 2 ) + rF( 1, 2 ) * rDN_DX( i, 1 );
	rB( 4, index + 2 ) = rF( 2, 1 ) * rDN_DX( i, 2 ) + rF( 2, 2 ) * rDN_DX( i, 1 );
	rB( 5, index + 0 ) = rF( 0, 2 ) * rDN_DX( i, 0 ) + rF( 0, 0 ) * rDN_DX( i, 2 );
	rB( 5, index + 1 ) = rF( 1, 2 ) * rDN_DX( i, 0 ) + rF( 1, 0 ) * rDN_DX( i, 2 );
	rB( 5, index + 2 ) = rF( 2, 2 ) * rDN_DX( i, 0 ) + rF( 2, 0 ) * rDN_DX( i, 2 );

      }

    KRATOS_CATCH( "" )
      }


  //************************************CALCULATE TOTAL MASS****************************
  //************************************************************************************

  double& UpdatedLagrangian3DElement::CalculateTotalMass( double& rTotalMass )
  {
    KRATOS_TRY

    rTotalMass = GetGeometry().DomainSize() * GetProperties()[DENSITY];

    return rTotalMass;

    KRATOS_CATCH( "" )
  }



  //************************************************************************************
  //************************************************************************************



  void UpdatedLagrangian3DElement::save( Serializer& rSerializer ) const
  {
    KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, SpatialLagrangian3DElement );

   }

  void UpdatedLagrangian3DElement::load( Serializer& rSerializer )
  {
    KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, SpatialLagrangian3DElement );

  }


} // Namespace Kratos


