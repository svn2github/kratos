//
//   Project Name:        $Project:     TopOpt_Application $
//   Last modified by:    $Author:      Gonzalez, Baumgärtner $
//   Date:                $Date:        October 2015 $
//   Revision:            $Revision:    0.0 $
//

#if !defined(KRATOS_TOPOPT_APPLICATION_H_INCLUDED )
#define  KRATOS_TOPOPT_APPLICATION_H_INCLUDED



// System includes
#include <string>
#include <iostream> 


// External includes 


// Project includes
#include "includes/define.h"
#include "includes/serializer.h"
#include "includes/constitutive_law.h"
#include "includes/variables.h"
#include "includes/ublas_interface.h"
#include "includes/kratos_application.h"

#include "containers/flags.h"

//conditions
#include "custom_conditions/point_load_3D_condition.hpp"
#include "custom_conditions/line_load_3D_condition.hpp"
#include "custom_conditions/surface_load_3D_condition.hpp"

// Elements
#include "custom_elements/topopt_small_displacement_element.h"

//constitutive laws
#include "custom_constitutive/hyperelastic_3D_law.hpp"
#include "custom_constitutive/linear_elastic_3D_law.hpp"




namespace Kratos
{

	///@name Kratos Globals
	///@{ 

//for explicit schemes
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MIDDLE_VELOCITY )

//solution
KRATOS_DEFINE_VARIABLE(bool, COMPUTE_DYNAMIC_TANGENT )
KRATOS_DEFINE_VARIABLE(int, WRITE_ID )
KRATOS_DEFINE_VARIABLE(double, PREVIOUS_DELTA_TIME )
KRATOS_DEFINE_VARIABLE(double, NEWMARK_BETA )
KRATOS_DEFINE_VARIABLE(double, NEWMARK_GAMMA )
KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_ALPHA )
KRATOS_DEFINE_VARIABLE(double, RAYLEIGH_BETA )

//geometrical
//KRATOS_DEFINE_VARIABLE( double, AREA )
//KRATOS_DEFINE_VARIABLE( double, IX )
//KRATOS_DEFINE_VARIABLE( double, IY )
//KRATOS_DEFINE_VARIABLE( double, IZ )
//KRATOS_DEFINE_VARIABLE( double, CROSS_AREA )
//KRATOS_DEFINE_VARIABLE( double, MEAN_RADIUS )
//KRATOS_DEFINE_VARIABLE( int,    SECTION_SIDES )
KRATOS_DEFINE_VARIABLE( Matrix , GEOMETRIC_STIFFNESS )

//constitutive law
KRATOS_DEFINE_VARIABLE(std::string, CONSTITUTIVE_LAW_NAME )
KRATOS_DEFINE_VARIABLE(ConstitutiveLaw::Pointer, CONSTITUTIVE_LAW_POINTER )
KRATOS_DEFINE_VARIABLE(Matrix, CONSTITUTIVE_MATRIX )
KRATOS_DEFINE_VARIABLE(Matrix, DEFORMATION_GRADIENT )
KRATOS_DEFINE_VARIABLE(double, DETERMINANT_F )
KRATOS_DEFINE_VARIABLE(bool,   IMPLEX  )

//cross section
//KRATOS_DEFINE_VARIABLE( ShellCrossSection::Pointer, SHELL_CROSS_SECTION )
//KRATOS_DEFINE_VARIABLE( int,          SHELL_CROSS_SECTION_OUTPUT_PLY_ID )
//KRATOS_DEFINE_VARIABLE( double,	SHELL_CROSS_SECTION_OUTPUT_PLY_LOCATION )

//condition nodal load variables
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( POINT_LOAD )
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LINE_LOAD )
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( SURFACE_LOAD )

KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_POINT_LOAD )
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_LINE_LOAD )
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_SURFACE_LOAD )

KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( POINT_TORQUE )
//KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LOCAL_POINT_TORQUE )

//shell generalized variables
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_STRAIN )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_STRAIN_GLOBAL )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_CURVATURE )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_CURVATURE_GLOBAL )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_FORCE )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_FORCE_GLOBAL )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_MOMENT )
//KRATOS_DEFINE_VARIABLE( Matrix, SHELL_MOMENT_GLOBAL )

//material orientation
//KRATOS_DEFINE_VARIABLE( Vector3, MATERIAL_ORIENTATION_DX )
//KRATOS_DEFINE_VARIABLE( Vector3, MATERIAL_ORIENTATION_DY )
//KRATOS_DEFINE_VARIABLE( Vector3, MATERIAL_ORIENTATION_DZ )

//othotropic/anisotropic constants
KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_X )
KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_Y )
KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_Z )
KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS_XY )
KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS_YZ )
KRATOS_DEFINE_VARIABLE( double, SHEAR_MODULUS_XZ )
KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO_XY )
KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO_YZ )
KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO_XZ )

//material : hyperelastic_plastic
//KRATOS_DEFINE_VARIABLE(double, NORM_ISOCHORIC_STRESS )
//KRATOS_DEFINE_VARIABLE(double, PLASTIC_STRAIN )
//KRATOS_DEFINE_VARIABLE(double, DELTA_PLASTIC_STRAIN )
//KRATOS_DEFINE_VARIABLE(double, ISOTROPIC_HARDENING_MODULUS  )
//KRATOS_DEFINE_VARIABLE(double, KINEMATIC_HARDENING_MODULUS  )
//KRATOS_DEFINE_VARIABLE(double, HARDENING_EXPONENT )
//KRATOS_DEFINE_VARIABLE(double, REFERENCE_HARDENING_MODULUS )
//KRATOS_DEFINE_VARIABLE(double, INFINITY_HARDENING_MODULUS )

//thermal
KRATOS_DEFINE_VARIABLE(double, THERMAL_EXPANSION_COEFFICIENT )
KRATOS_DEFINE_VARIABLE(double, REFERENCE_TEMPERATURE )
//KRATOS_DEFINE_VARIABLE(double, PLASTIC_DISSIPATION )
//KRATOS_DEFINE_VARIABLE(double, DELTA_PLASTIC_DISSIPATION )

//element
KRATOS_DEFINE_VARIABLE(Vector, RESIDUAL_VECTOR )
KRATOS_DEFINE_VARIABLE(Vector, EXTERNAL_FORCES_VECTOR )
KRATOS_DEFINE_VARIABLE(Vector, INTERNAL_FORCES_VECTOR )
KRATOS_DEFINE_VARIABLE(Vector, CONTACT_FORCES_VECTOR )

KRATOS_DEFINE_VARIABLE(Vector, CAUCHY_STRESS_VECTOR )
KRATOS_DEFINE_VARIABLE(Vector, PK2_STRESS_VECTOR )

KRATOS_DEFINE_VARIABLE(Matrix, ALMANSI_STRAIN_TENSOR )
KRATOS_DEFINE_VARIABLE(Vector, GREEN_LAGRANGE_STRAIN_VECTOR )
KRATOS_DEFINE_VARIABLE(Vector, ALMANSI_STRAIN_VECTOR )

KRATOS_DEFINE_VARIABLE(Matrix, MATERIAL_STIFFNESS_MATRIX )
KRATOS_DEFINE_VARIABLE(Matrix, GEOMETRIC_STIFFNESS_MATRIX )

KRATOS_DEFINE_VARIABLE(double, VON_MISES_STRESS )

//nodal dofs
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( IMPOSED_DISPLACEMENT )
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( IMPOSED_ROTATION )
KRATOS_DEFINE_VARIABLE(double, PRESSURE_REACTION )

//SIMP variables
KRATOS_DEFINE_VARIABLE( double, E_MIN )
KRATOS_DEFINE_VARIABLE( double, PENAL )
KRATOS_DEFINE_VARIABLE( double, X_PHYS )
KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS_OLD )
KRATOS_DEFINE_VARIABLE( double, DCDX )
KRATOS_DEFINE_VARIABLE( double, OBJ_FCN )
KRATOS_DEFINE_VARIABLE( double, X_PHYS_OLD )
KRATOS_DEFINE_VARIABLE( double, UKU )
KRATOS_DEFINE_VARIABLE( double, SEARCHRADIUS )
KRATOS_DEFINE_VARIABLE( double, Q )
KRATOS_DEFINE_VARIABLE( double, ELEM_CENTER_X )
KRATOS_DEFINE_VARIABLE( double, ELEM_CENTER_Y )
KRATOS_DEFINE_VARIABLE( double, ELEM_CENTER_Z )
KRATOS_DEFINE_VARIABLE( int, SOLID_VOID )




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
	class KratosTopOptApplication : public KratosApplication
	{
	public:
		///@name Type Definitions
		///@{
		

		/// Pointer definition of KratosTopOptApplication
		KRATOS_CLASS_POINTER_DEFINITION(KratosTopOptApplication);

		///@}
		///@name Life Cycle 
		///@{ 

		/// Default constructor.
		KratosTopOptApplication();

		/// Destructor.
		virtual ~KratosTopOptApplication(){}


		///@}
		///@name Operators 
		///@{


		///@}
		///@name Operations
		///@{

		virtual void Register();



		///@}
		///@name Access
		///@{ 


		///@}
		///@name Inquiry
		///@{


		///@}      
		///@name Input and output
		///@{

		/// Turn back information as a string.
		virtual std::string Info() const
		{
			return "KratosTopOptApplication";
		}

		/// Print information about this object.
		virtual void PrintInfo(std::ostream& rOStream) const
		{
			rOStream << Info();
			PrintData(rOStream);
		}

		///// Print object's data.
      virtual void PrintData(std::ostream& rOStream) const
      {
      	KRATOS_WATCH("in my application");
      	KRATOS_WATCH(KratosComponents<VariableData>::GetComponents().size() );
		rOStream << "Variables:" << std::endl;
		KratosComponents<VariableData>().PrintData(rOStream);
		rOStream << std::endl;
		rOStream << "Elements:" << std::endl;
		KratosComponents<Element>().PrintData(rOStream);
		rOStream << std::endl;
		rOStream << "Conditions:" << std::endl;
		KratosComponents<Condition>().PrintData(rOStream);
      }


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

//        const TopOptSmallDisplacementElement mTopOptSmallDisplacementElement3D8N;

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



		//       static const ApplicationCondition  msApplicationCondition; 

		///@} 
		///@name Member Variables 
		///@{ 

        //element
        const TopOptSmallDisplacementElement mTopOptSmallDisplacementElement3D8N;

        //conditions
        const PointLoad3DCondition      mPointLoad3DCondition;
        const LineLoad3DCondition       mLineLoadCondition3D2N;

        const SurfaceLoad3DCondition    mSurfaceLoadCondition3D3N;
        const SurfaceLoad3DCondition    mSurfaceLoadCondition3D4N;
        const SurfaceLoad3DCondition    mSurfaceLoadCondition3D6N;
        const SurfaceLoad3DCondition    mSurfaceLoadCondition3D8N;
        const SurfaceLoad3DCondition    mSurfaceLoadCondition3D9N;

        //constitutive
        const HyperElastic3DLaw         mHyperElastic3DLaw;
        const LinearElastic3DLaw        mLinearElastic3DLaw;



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
		///@name Private Inquiry 
		///@{ 


		///@}    
		///@name Un accessible methods 
		///@{ 

		/// Assignment operator.
		KratosTopOptApplication& operator=(KratosTopOptApplication const& rOther);

		/// Copy constructor.
		KratosTopOptApplication(KratosTopOptApplication const& rOther);


		///@}    

	}; // Class KratosTopOptApplication 

	///@} 


	///@name Type Definitions       
	///@{ 


	///@} 
	///@name Input and output 
	///@{ 

	///@} 


}  // namespace Kratos.

#endif // KRATOS_TOPOPT_APPLICATION_H_INCLUDED  defined 


