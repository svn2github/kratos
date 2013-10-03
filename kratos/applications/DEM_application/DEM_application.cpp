//
//   Project Name:        Kratos       
//   Last Modified by:    $Author:  $
//   Date:                $Date:  $
//   Revision:            $Revision: 1.3 $
//

// System includes

// External includes 

// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "DEM_application.h"
#include "geometries/point_3d.h"
#include "geometries/line_3d_2.h"

namespace Kratos
{

  // OPTIONS AND FLAGS

  KRATOS_CREATE_VARIABLE(int, BOUNDING_BOX_OPTION)
  KRATOS_CREATE_VARIABLE(int, ROTATION_OPTION)
  KRATOS_CREATE_VARIABLE(int, CRITICAL_TIME_OPTION)
  KRATOS_CREATE_VARIABLE(int, VIRTUAL_MASS_OPTION)
  KRATOS_CREATE_VARIABLE(int, UNIFORM_MATERIAL_OPTION)
  KRATOS_CREATE_VARIABLE(int, ACTIVATE_SEARCH)
  KRATOS_CREATE_VARIABLE(int, LIMIT_SURFACE_OPTION)
  KRATOS_CREATE_VARIABLE(int, LIMIT_CYLINDER_OPTION)
  KRATOS_CREATE_VARIABLE(int, CLEAN_INDENT_OPTION)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_VARIABLES_OPTION)
  KRATOS_CREATE_VARIABLE(int, TRIHEDRON_OPTION)
  KRATOS_CREATE_VARIABLE(int, FORCE_CALCULATION_TYPE)
  KRATOS_CREATE_VARIABLE(int, DAMP_TYPE)
  KRATOS_CREATE_VARIABLE(int, ROTA_DAMP_TYPE)
  KRATOS_CREATE_VARIABLE(int, NEIGH_INITIALIZED)
  KRATOS_CREATE_VARIABLE(int, GROUP_ID)
  KRATOS_CREATE_VARIABLE(int, TRIAXIAL_TEST_OPTION)
  KRATOS_CREATE_VARIABLE(int, SWITCH_PRESSURE)
  KRATOS_CREATE_VARIABLE(int, FIX_VELOCITIES_FLAG)
  KRATOS_CREATE_VARIABLE(int, STRESS_STRAIN_OPTION)
  KRATOS_CREATE_VARIABLE(double, DUMMY_DEBUG_DOUBLE)
  KRATOS_CREATE_VARIABLE(int, DUMMY_DEBUG_INT)

  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_VARIABLE(Vector, DEM_AREA_VECTOR)
  KRATOS_CREATE_VARIABLE(bool, DELTA_OPTION)
  KRATOS_CREATE_VARIABLE(int, DEMPACK_OPTION)
  KRATOS_CREATE_VARIABLE(int, CASE_OPTION)
  KRATOS_CREATE_VARIABLE(int, SKIN_SPHERE)
  KRATOS_CREATE_VARIABLE(int, ROTATION_SPRING_OPTION)
  KRATOS_CREATE_VARIABLE(int, CONTACT_MESH_OPTION)
  KRATOS_CREATE_VARIABLE(int, FAILURE_CRITERION_OPTION)
  KRATOS_CREATE_VARIABLE(int, CONCRETE_TEST_OPTION)
  KRATOS_CREATE_VARIABLE(int, PARTICLE_CONTINUUM)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_TENSION)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_COHESION)
  KRATOS_CREATE_VARIABLE(int, plot_OPTIONS)
  KRATOS_CREATE_VARIABLE(double, DEMPACK_DAMPING)
  KRATOS_CREATE_VARIABLE(double, SURFACE_FRICTION_1)
  KRATOS_CREATE_VARIABLE(double, SURFACE_FRICTION_2)
  KRATOS_CREATE_VARIABLE(double, SURFACE_FRICTION_3)
  KRATOS_CREATE_VARIABLE(double, SURFACE_FRICTION_4)
  KRATOS_CREATE_VARIABLE(double, SURFACE_FRICTION_5)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_FRICTION_1)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_FRICTION_2)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_FRICTION_3)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_FRICTION_4)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_FRICTION_5)
  KRATOS_CREATE_VARIABLE(int, IF_BOUNDARY_ELEMENT)
  KRATOS_CREATE_VARIABLE(Vector, IF_BOUNDARY_FACE)
  KRATOS_CREATE_VARIABLE(vector<int>, PARTICLE_CONTACT_FAILURE_ID)
    
 //KRATOS_CREATE_VARIABLE(int, PARTICLE_FAILURE_ID)
  KRATOS_CREATE_VARIABLE(double, PREDEFINED_SKIN)
  // *************** Continuum only END ***************

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_VARIABLE(int, NON_NEWTONIAN_OPTION)
  KRATOS_CREATE_VARIABLE(int, MANUALLY_IMPOSED_DRAG_LAW_OPTION)
  KRATOS_CREATE_VARIABLE(int, DRAG_MODIFIER_TYPE)
  // *************** Swimming only END ***************

  // COUNTING

  KRATOS_CREATE_VARIABLE(int, NUM_OF_NEIGH)
  KRATOS_CREATE_VARIABLE(int, TOTAL_CONTACTS)
  KRATOS_CREATE_VARIABLE(int, NUM_PARTICLES_INITIALIZED)
  KRATOS_CREATE_VARIABLE(std::size_t, POTENTIAL_NEIGHBOURS)

  // TOLERANCES

  KRATOS_CREATE_VARIABLE(double, DISTANCE_TOLERANCE)

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, POWER_LAW_TOLERANCE)
  // *************** Swimming only END ***************

  // TIME RELATED

  KRATOS_CREATE_VARIABLE(double, TIME_INCREASING_RATIO)
  KRATOS_CREATE_VARIABLE(double, FINAL_SIMULATION_TIME)

  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, INITIAL_PRESSURE_TIME)
  // *************** Continuum only END ***************

  // MATERIAL PARAMETERS

  KRATOS_CREATE_VARIABLE(double, NODAL_MASS_COEFF)
  KRATOS_CREATE_VARIABLE(double, DEM_MAGIC_FACTOR)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_MOMENT_OF_INERTIA)
  KRATOS_CREATE_VARIABLE(double, ROLLING_FRICTION)
  KRATOS_CREATE_VARIABLE(double, GENERAL_DENSITY)
  KRATOS_CREATE_VARIABLE(double, GENERAL_YOUNG_MODULUS)
  KRATOS_CREATE_VARIABLE(double, GENERAL_POISSON_RATIO)
  KRATOS_CREATE_VARIABLE(double, GENERAL_COHESION)
  KRATOS_CREATE_VARIABLE(double, GENERAL_ROLLING_FRICTION)
  KRATOS_CREATE_VARIABLE(double, GENERAL_STATIC_FRICTION)
  KRATOS_CREATE_VARIABLE(double, GENERAL_DYNAMIC_FRICTION)
  KRATOS_CREATE_VARIABLE(double, GENERAL_RESTITUTION_COEFF)
  KRATOS_CREATE_VARIABLE(double, HISTORICAL_MIN_K)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_KN)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_KT)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_INERTIA)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_DENSITY)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_FRICTION)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_STATIC_FRICTION_COEF)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_DYNAMIC_FRICTION_COEF)
  KRATOS_CREATE_VARIABLE(double, RESTITUTION_COEFF)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_ROTATION_DAMP_RATIO)
  KRATOS_CREATE_VARIABLE(double, SQRT_OF_MASS)
  KRATOS_CREATE_VARIABLE(double, LN_OF_RESTITUTION_COEFF)

  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, GLOBAL_KR)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_RN)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_RT)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_RR)
  KRATOS_CREATE_VARIABLE(double, GLOBAL_FRI_ANG)

  KRATOS_CREATE_VARIABLE(double, NON_ELASTIC_STAGE)
  KRATOS_CREATE_VARIABLE(double, SLOPE_FRACTION_N1)
  KRATOS_CREATE_VARIABLE(double, SLOPE_FRACTION_N2)
  KRATOS_CREATE_VARIABLE(double, SLOPE_LIMIT_COEFF_C1)
  KRATOS_CREATE_VARIABLE(double, SLOPE_LIMIT_COEFF_C2)
  KRATOS_CREATE_VARIABLE(double, YOUNG_MODULUS_PLASTIC)
  KRATOS_CREATE_VARIABLE(double, PLASTIC_YIELD_STRESS)
  KRATOS_CREATE_VARIABLE(double, DAMAGE_FACTOR)
  KRATOS_CREATE_VARIABLE(double, DONZE_G1)
  KRATOS_CREATE_VARIABLE(double, DONZE_G2)
  KRATOS_CREATE_VARIABLE(double, DONZE_G3)
  KRATOS_CREATE_VARIABLE(double, DONZE_MAX_DEF)

  KRATOS_CREATE_VARIABLE(double, CONTACT_FAILURE)
  KRATOS_CREATE_VARIABLE(double, CONTACT_SIGMA)
  KRATOS_CREATE_VARIABLE(double, CONTACT_TAU)
  KRATOS_CREATE_VARIABLE(double, FAILURE_CRITERION_STATE)
  KRATOS_CREATE_VARIABLE(double, CONTACT_SIGMA_MAX)
  KRATOS_CREATE_VARIABLE(double, CONTACT_SIGMA_MIN)
  KRATOS_CREATE_VARIABLE(double, CONTACT_TAU_ZERO)
  KRATOS_CREATE_VARIABLE(double, CONTACT_INTERNAL_FRICC)
  KRATOS_CREATE_VARIABLE(double, LOW_POISSON_FORCE)
  KRATOS_CREATE_VARIABLE(double, HIGH_POISSON_FORCE)
  // *************** Continuum only END *************

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, GEL_STRENGTH)
  KRATOS_CREATE_VARIABLE(double, POWER_LAW_N)
  KRATOS_CREATE_VARIABLE(double, POWER_LAW_K)
  KRATOS_CREATE_VARIABLE(double, INIT_DRAG_FORCE)
  KRATOS_CREATE_VARIABLE(double, DRAG_LAW_SLOPE)
  // *************** Swimming only END ***************

  // GEOMETRIC PARAMETERS

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, SPHERICITY)
  // *************** Swimming only END ***************

  KRATOS_CREATE_VARIABLE(double, MAX_INDENTATION)

  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, LOCAL_CONTACT_AREA_HIGH)
  KRATOS_CREATE_VARIABLE(double, LOCAL_CONTACT_AREA_LOW)
  KRATOS_CREATE_VARIABLE(double, MEAN_CONTACT_AREA)
  KRATOS_CREATE_VARIABLE(double, REPRESENTATIVE_VOLUME)
  KRATOS_CREATE_VARIABLE(Vector, PARTICLE_INITIAL_DELTA)
  KRATOS_CREATE_VARIABLE(Vector, PARTICLE_CONTACT_DELTA)
  KRATOS_CREATE_VARIABLE(VectorArray3Double, ARROW_POINT)
  // *************** Continuum only END ***************

  // KINEMATICS

  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_ROTATION_ANGLE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(EULER_ANGLES)
  KRATOS_CREATE_VARIABLE(double, ORIENTATION_REAL)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ORIENTATION_IMAG)
  KRATOS_CREATE_VARIABLE(Vector, INITIAL_AXES_TRACKING)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_1)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_2)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_3)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_4)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_5)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_1)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_2)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_3)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_4)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_5)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_1)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_2)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_3)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_4)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_5)     
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_1)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_2)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_3)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_4)  
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_5)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_RADIUS_1)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_RADIUS_2)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_RADIUS_3)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_RADIUS_4)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_RADIUS_5)    
  KRATOS_CREATE_VARIABLE(double, CYLINDER_VELOCITY_1)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_VELOCITY_2)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_VELOCITY_3)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_VELOCITY_4)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_VELOCITY_5)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_1)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_2)  
  KRATOS_CREATE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_3)   
  KRATOS_CREATE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_4)
  KRATOS_CREATE_VARIABLE(double, CYLINDER_ANGULAR_VELOCITY_5)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DELTA_DISPLACEMENT)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DELTA_ROTA_DISPLACEMENT)

  // FORCE AND MOMENTUM

  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(TOTAL_FORCES)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_1)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_2) 
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_3)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_4)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_5)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_1)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_2)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_3)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_4)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_5)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_MOMENT)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MAX_ROTA_MOMENT)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ELASTIC_FORCES)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_MOMENTUM)

  // ENERGY

   KRATOS_CREATE_VARIABLE(double, KINETIC_ENERGY)
   KRATOS_CREATE_VARIABLE(double, ELASTIC_ENERGY_OF_CONTACTS)


  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_ROTA_MOMENT)
  KRATOS_CREATE_VARIABLE(Vector, PARTICLE_BLOCK_CONTACT_FORCE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_APPLIED_FORCE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE_LOW)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE_HIGH)
  KRATOS_CREATE_VARIABLE(VectorArray3Double, PARTICLE_CONTACT_FORCES )

  // CONCRETE TEST

  KRATOS_CREATE_VARIABLE(double, FIXED_VEL_TOP)
  KRATOS_CREATE_VARIABLE(double, FIXED_VEL_BOT)
  KRATOS_CREATE_VARIABLE(int, STEP_FIX_VELOCITIES)
  KRATOS_CREATE_VARIABLE(double, RADIAL_DISPLACEMENT)
  KRATOS_CREATE_VARIABLE(double, AREA_VERTICAL_TAPA)
  KRATOS_CREATE_VARIABLE(double, AREA_VERTICAL_CENTRE)

  // TENSION

  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_XX)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_XY)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_XZ)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_YX)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_YY)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_YZ)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_ZX)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_ZY)
  KRATOS_CREATE_VARIABLE(double, DEM_STRESS_ZZ)
  // *************** Continuum only END *************

  // Possible future blocks (no FEM) interaction

  KRATOS_CREATE_VARIABLE(Vector, PARTICLE_BLOCK_CONTACT_FAILURE_ID)
  KRATOS_CREATE_VARIABLE(Vector, PARTICLE_BLOCK_IF_INITIAL_CONTACT)
  KRATOS_CREATE_VARIABLE(WeakPointerVector<Element >, NEIGHBOUR_PARTICLE_BLOCK_ELEMENTS)

  // DUMMIES INT AND DOUBLE VARIABLES

  KRATOS_CREATE_VARIABLE(int, DUMMY_SWITCH)
  KRATOS_CREATE_VARIABLE(double, DUMMY_FORCES)
  KRATOS_CREATE_VARIABLE(double, DUMMY_LOCAL_AXES)

  // EXPORTS
  
  KRATOS_CREATE_VARIABLE(double, EXPORT_SKIN_SPHERE)
  KRATOS_CREATE_VARIABLE(double, EXPORT_PARTICLE_FAILURE_ID)
  KRATOS_CREATE_VARIABLE(double, EXPORT_GROUP_ID)
  KRATOS_CREATE_VARIABLE(double, EXPORT_ID)
  KRATOS_CREATE_VARIABLE(int, PRINT_EXPORT_ID)
  KRATOS_CREATE_VARIABLE(int, PRINT_SKIN_SPHERE)
  KRATOS_CREATE_VARIABLE(int, PRINT_GROUP_ID)
  KRATOS_CREATE_VARIABLE(int, PRINT_RADIAL_DISPLACEMENT)
  


  /************************************************************************************************************************************************************/
  /************************************************************************************************************************************************************/

  //ELEMENTS
    
  KratosDEMApplication::KratosDEMApplication():
  mCylinderParticle2D( 0, Element::GeometryType::Pointer( new Point3D<Node<3> >( Element::GeometryType::PointsArrayType( 1, Node<3>() ) ) ) ),
  mCylinderContinuumParticle2D( 0, Element::GeometryType::Pointer( new Point3D<Node<3> >( Element::GeometryType::PointsArrayType( 1, Node<3>() ) ) ) ),
  mSphericParticle3D( 0, Element::GeometryType::Pointer( new Point3D<Node<3> >( Element::GeometryType::PointsArrayType( 1, Node<3>() ) ) ) ),
  mSphericContinuumParticle3D( 0, Element::GeometryType::Pointer( new Point3D<Node<3> >( Element::GeometryType::PointsArrayType( 1, Node<3>() ) ) ) ),
  mSphericSwimmingParticle3D( 0, Element::GeometryType::Pointer( new Point3D<Node<3> >( Element::GeometryType::PointsArrayType( 1, Node<3>() ) ) ) ),
  mDEM_FEM_Particle3D( 0, Element::GeometryType::Pointer( new Point3D<Node<3> >( Element::GeometryType::PointsArrayType( 1, Node<3>() ) ) ) ),
  mParticleContactElement( 0, Element::GeometryType::Pointer( new Line3D2<Node<3> >( Element::GeometryType::PointsArrayType( 2, Node<3>() ) ) ) )
  {}
        
  void KratosDEMApplication::Register()
  {

    // Calling base class register to register Kratos components

    KratosApplication::Register();
    std::cout << "Initializing KratosDEMApplication... " << std::endl;

    // OPTIONS AND FLAGS

    KRATOS_REGISTER_VARIABLE(BOUNDING_BOX_OPTION)
    KRATOS_REGISTER_VARIABLE(ROTATION_OPTION)
    KRATOS_REGISTER_VARIABLE(CRITICAL_TIME_OPTION)
    KRATOS_REGISTER_VARIABLE(VIRTUAL_MASS_OPTION)
    KRATOS_REGISTER_VARIABLE(UNIFORM_MATERIAL_OPTION)
    KRATOS_REGISTER_VARIABLE(ACTIVATE_SEARCH)
    KRATOS_REGISTER_VARIABLE(LIMIT_SURFACE_OPTION)
    KRATOS_REGISTER_VARIABLE(LIMIT_CYLINDER_OPTION)
    KRATOS_REGISTER_VARIABLE(CLEAN_INDENT_OPTION)
    KRATOS_REGISTER_VARIABLE(GLOBAL_VARIABLES_OPTION)
    KRATOS_REGISTER_VARIABLE(TRIHEDRON_OPTION)
    KRATOS_REGISTER_VARIABLE(FORCE_CALCULATION_TYPE)
    KRATOS_REGISTER_VARIABLE(DAMP_TYPE)
    KRATOS_REGISTER_VARIABLE(ROTA_DAMP_TYPE)
    KRATOS_REGISTER_VARIABLE(NEIGH_INITIALIZED)
    KRATOS_REGISTER_VARIABLE(GROUP_ID)
    KRATOS_REGISTER_VARIABLE(TRIAXIAL_TEST_OPTION)
    KRATOS_REGISTER_VARIABLE( SWITCH_PRESSURE)  
    KRATOS_REGISTER_VARIABLE(FIX_VELOCITIES_FLAG)
    KRATOS_REGISTER_VARIABLE(STRESS_STRAIN_OPTION)
    KRATOS_REGISTER_VARIABLE(DUMMY_DEBUG_DOUBLE)
    KRATOS_REGISTER_VARIABLE(DUMMY_DEBUG_INT)
    
    
    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_VARIABLE(DEM_AREA_VECTOR)
    KRATOS_REGISTER_VARIABLE(DELTA_OPTION)
    KRATOS_REGISTER_VARIABLE(CASE_OPTION)
    KRATOS_REGISTER_VARIABLE(DEMPACK_OPTION)
    KRATOS_REGISTER_VARIABLE(SKIN_SPHERE)
    KRATOS_REGISTER_VARIABLE(ROTATION_SPRING_OPTION)
    KRATOS_REGISTER_VARIABLE(CONTACT_MESH_OPTION)
    KRATOS_REGISTER_VARIABLE(FAILURE_CRITERION_OPTION)
    KRATOS_REGISTER_VARIABLE(CONCRETE_TEST_OPTION)
    KRATOS_REGISTER_VARIABLE(PARTICLE_CONTINUUM)
    KRATOS_REGISTER_VARIABLE( PARTICLE_TENSION )
    KRATOS_REGISTER_VARIABLE( PARTICLE_COHESION )
    
    KRATOS_REGISTER_VARIABLE(plot_OPTIONS)
    KRATOS_REGISTER_VARIABLE(DEMPACK_DAMPING)
    KRATOS_REGISTER_VARIABLE(SURFACE_FRICTION_1)
    KRATOS_REGISTER_VARIABLE(SURFACE_FRICTION_2)
    KRATOS_REGISTER_VARIABLE(SURFACE_FRICTION_3)
    KRATOS_REGISTER_VARIABLE(SURFACE_FRICTION_4)
    KRATOS_REGISTER_VARIABLE(SURFACE_FRICTION_5)    
    KRATOS_REGISTER_VARIABLE(CYLINDER_FRICTION_1)
    KRATOS_REGISTER_VARIABLE(CYLINDER_FRICTION_2)    
    KRATOS_REGISTER_VARIABLE(CYLINDER_FRICTION_3)
    KRATOS_REGISTER_VARIABLE(CYLINDER_FRICTION_4)        
    KRATOS_REGISTER_VARIABLE(CYLINDER_FRICTION_5)    
    KRATOS_REGISTER_VARIABLE(IF_BOUNDARY_ELEMENT)
    KRATOS_REGISTER_VARIABLE(IF_BOUNDARY_FACE)
    KRATOS_REGISTER_VARIABLE(PARTICLE_CONTACT_FAILURE_ID)
    //KRATOS_REGISTER_VARIABLE(PARTICLE_FAILURE_ID)
    KRATOS_REGISTER_VARIABLE(EXPORT_PARTICLE_FAILURE_ID)
    KRATOS_REGISTER_VARIABLE(PREDEFINED_SKIN)
    // *************** Continuum only END ***************

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_VARIABLE(NON_NEWTONIAN_OPTION)
    KRATOS_REGISTER_VARIABLE(MANUALLY_IMPOSED_DRAG_LAW_OPTION)
    KRATOS_REGISTER_VARIABLE(DRAG_MODIFIER_TYPE)
    // *************** Swimming only END ***************

    // COUNTING

    KRATOS_REGISTER_VARIABLE(NUM_OF_NEIGH)
    KRATOS_REGISTER_VARIABLE(TOTAL_CONTACTS)
    KRATOS_REGISTER_VARIABLE(NUM_PARTICLES_INITIALIZED)
    KRATOS_REGISTER_VARIABLE(POTENTIAL_NEIGHBOURS)

    // TOLERANCES

    KRATOS_REGISTER_VARIABLE(DISTANCE_TOLERANCE)

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_VARIABLE(POWER_LAW_TOLERANCE)
    // *************** Swimming only END ***************

    // TIME RELATED

    KRATOS_REGISTER_VARIABLE(TIME_INCREASING_RATIO)
    KRATOS_REGISTER_VARIABLE(FINAL_SIMULATION_TIME)

    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_VARIABLE(INITIAL_PRESSURE_TIME)
    // *************** Continuum only END ***************

    // MATERIAL PARAMETERS

    KRATOS_REGISTER_VARIABLE(NODAL_MASS_COEFF)
    KRATOS_REGISTER_VARIABLE(DEM_MAGIC_FACTOR)
    KRATOS_REGISTER_VARIABLE(PARTICLE_MOMENT_OF_INERTIA)
    KRATOS_REGISTER_VARIABLE(ROLLING_FRICTION)
    KRATOS_REGISTER_VARIABLE(GENERAL_DENSITY)
    KRATOS_REGISTER_VARIABLE(GENERAL_YOUNG_MODULUS)
    KRATOS_REGISTER_VARIABLE(GENERAL_POISSON_RATIO)
    KRATOS_REGISTER_VARIABLE(GENERAL_COHESION)
    KRATOS_REGISTER_VARIABLE(GENERAL_ROLLING_FRICTION)
    KRATOS_REGISTER_VARIABLE(GENERAL_STATIC_FRICTION)
    KRATOS_REGISTER_VARIABLE(GENERAL_DYNAMIC_FRICTION)
    KRATOS_REGISTER_VARIABLE(GENERAL_RESTITUTION_COEFF)
    KRATOS_REGISTER_VARIABLE(HISTORICAL_MIN_K)
    KRATOS_REGISTER_VARIABLE(GLOBAL_KN)
    KRATOS_REGISTER_VARIABLE(GLOBAL_KT)
    KRATOS_REGISTER_VARIABLE(PARTICLE_INERTIA)
    KRATOS_REGISTER_VARIABLE(PARTICLE_DENSITY)
    KRATOS_REGISTER_VARIABLE(PARTICLE_FRICTION)
    KRATOS_REGISTER_VARIABLE(PARTICLE_STATIC_FRICTION_COEF)
    KRATOS_REGISTER_VARIABLE(PARTICLE_DYNAMIC_FRICTION_COEF)
    KRATOS_REGISTER_VARIABLE(RESTITUTION_COEFF)
    KRATOS_REGISTER_VARIABLE(PARTICLE_ROTATION_DAMP_RATIO)
    KRATOS_REGISTER_VARIABLE(SQRT_OF_MASS)
    KRATOS_REGISTER_VARIABLE(LN_OF_RESTITUTION_COEFF)

    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_VARIABLE(GLOBAL_KR)
    KRATOS_REGISTER_VARIABLE(GLOBAL_RN)
    KRATOS_REGISTER_VARIABLE(GLOBAL_RT)
    KRATOS_REGISTER_VARIABLE(GLOBAL_RR)
    KRATOS_REGISTER_VARIABLE(GLOBAL_FRI_ANG)

    KRATOS_REGISTER_VARIABLE(NON_ELASTIC_STAGE)
    KRATOS_REGISTER_VARIABLE(SLOPE_FRACTION_N1)
    KRATOS_REGISTER_VARIABLE(SLOPE_FRACTION_N2)
    KRATOS_REGISTER_VARIABLE(SLOPE_LIMIT_COEFF_C1)
    KRATOS_REGISTER_VARIABLE(SLOPE_LIMIT_COEFF_C2)
    KRATOS_REGISTER_VARIABLE(YOUNG_MODULUS_PLASTIC)
    KRATOS_REGISTER_VARIABLE(PLASTIC_YIELD_STRESS)
    KRATOS_REGISTER_VARIABLE(DAMAGE_FACTOR)
    KRATOS_REGISTER_VARIABLE(DONZE_G1)
    KRATOS_REGISTER_VARIABLE(DONZE_G2)
    KRATOS_REGISTER_VARIABLE(DONZE_G3)
    KRATOS_REGISTER_VARIABLE(DONZE_MAX_DEF)

    KRATOS_REGISTER_VARIABLE(CONTACT_FAILURE)
    KRATOS_REGISTER_VARIABLE(CONTACT_SIGMA)
    KRATOS_REGISTER_VARIABLE(CONTACT_TAU)
    KRATOS_REGISTER_VARIABLE(FAILURE_CRITERION_STATE)
    KRATOS_REGISTER_VARIABLE(CONTACT_SIGMA_MAX)
    KRATOS_REGISTER_VARIABLE(CONTACT_SIGMA_MIN)
    KRATOS_REGISTER_VARIABLE(CONTACT_TAU_ZERO)
    KRATOS_REGISTER_VARIABLE(CONTACT_INTERNAL_FRICC)
    KRATOS_REGISTER_VARIABLE(LOW_POISSON_FORCE)
    KRATOS_REGISTER_VARIABLE(HIGH_POISSON_FORCE)
    // *************** Continuum only END *************

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_VARIABLE(GEL_STRENGTH)
    KRATOS_REGISTER_VARIABLE(POWER_LAW_N)
    KRATOS_REGISTER_VARIABLE(POWER_LAW_K)
    KRATOS_REGISTER_VARIABLE(INIT_DRAG_FORCE)
    KRATOS_REGISTER_VARIABLE(DRAG_LAW_SLOPE)
    // *************** Swimming only END ***************

    // GEOMETRIC PARAMETERS

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_VARIABLE(SPHERICITY)
    // *************** Swimming only END ***************

    KRATOS_REGISTER_VARIABLE(MAX_INDENTATION)

    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_VARIABLE(LOCAL_CONTACT_AREA_HIGH)
    KRATOS_REGISTER_VARIABLE(LOCAL_CONTACT_AREA_LOW)
    KRATOS_REGISTER_VARIABLE(MEAN_CONTACT_AREA)
    KRATOS_REGISTER_VARIABLE(REPRESENTATIVE_VOLUME)
    KRATOS_REGISTER_VARIABLE(PARTICLE_INITIAL_DELTA)
    KRATOS_REGISTER_VARIABLE(PARTICLE_CONTACT_DELTA)

    KRATOS_REGISTER_VARIABLE(ARROW_POINT)
    // *************** Continuum only END ***************

    // KINEMATICS

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_ROTATION_ANGLE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(EULER_ANGLES)
    KRATOS_REGISTER_VARIABLE(ORIENTATION_REAL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ORIENTATION_IMAG)
    KRATOS_REGISTER_VARIABLE(INITIAL_AXES_TRACKING)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_2)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_3)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_4)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_NORMAL_DIR_5)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_2)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_3)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_4)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SURFACE_POINT_COOR_5)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_2)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_3)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_4)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CYLINDER_AXIS_DIR_5)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_2)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_3)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_4)    
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_BASE_CYLINDER_CENTRE_5)    
    KRATOS_REGISTER_VARIABLE(CYLINDER_RADIUS_1)
    KRATOS_REGISTER_VARIABLE(CYLINDER_RADIUS_2)    
    KRATOS_REGISTER_VARIABLE(CYLINDER_RADIUS_3)    
    KRATOS_REGISTER_VARIABLE(CYLINDER_RADIUS_4)
    KRATOS_REGISTER_VARIABLE(CYLINDER_RADIUS_5)
    KRATOS_REGISTER_VARIABLE(CYLINDER_VELOCITY_1)
    KRATOS_REGISTER_VARIABLE(CYLINDER_VELOCITY_2)   
    KRATOS_REGISTER_VARIABLE(CYLINDER_VELOCITY_3)    
    KRATOS_REGISTER_VARIABLE(CYLINDER_VELOCITY_4)
    KRATOS_REGISTER_VARIABLE(CYLINDER_VELOCITY_5)  
    KRATOS_REGISTER_VARIABLE(CYLINDER_ANGULAR_VELOCITY_1)
    KRATOS_REGISTER_VARIABLE(CYLINDER_ANGULAR_VELOCITY_2)
    KRATOS_REGISTER_VARIABLE(CYLINDER_ANGULAR_VELOCITY_3)
    KRATOS_REGISTER_VARIABLE(CYLINDER_ANGULAR_VELOCITY_4)
    KRATOS_REGISTER_VARIABLE(CYLINDER_ANGULAR_VELOCITY_5)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DELTA_DISPLACEMENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DELTA_ROTA_DISPLACEMENT)

    // FORCE AND MOMENTUM

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(TOTAL_FORCES)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_2)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_3)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_4)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_SURFACE_CONTACT_FORCES_5)     
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_2)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_3)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_4)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_CYLINDER_CONTACT_FORCES_5)        
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARTICLE_MOMENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MAX_ROTA_MOMENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ELASTIC_FORCES)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_MOMENTUM)

    // ENERGY

    KRATOS_REGISTER_VARIABLE(KINETIC_ENERGY)
    KRATOS_REGISTER_VARIABLE(ELASTIC_ENERGY_OF_CONTACTS)


    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_ROTA_MOMENT)
    KRATOS_REGISTER_VARIABLE(PARTICLE_BLOCK_CONTACT_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_APPLIED_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE_LOW)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE_HIGH)
    KRATOS_REGISTER_VARIABLE(PARTICLE_CONTACT_FORCES )

    // CONCRETE TEST

    KRATOS_REGISTER_VARIABLE(FIXED_VEL_TOP)
    KRATOS_REGISTER_VARIABLE(FIXED_VEL_BOT)
    KRATOS_REGISTER_VARIABLE(STEP_FIX_VELOCITIES)
    KRATOS_REGISTER_VARIABLE(RADIAL_DISPLACEMENT)
    KRATOS_REGISTER_VARIABLE(AREA_VERTICAL_TAPA)
    KRATOS_REGISTER_VARIABLE(AREA_VERTICAL_CENTRE)

    // TENSION

    KRATOS_REGISTER_VARIABLE(DEM_STRESS_XX)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_XY)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_XZ)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_YX)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_YY)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_YZ)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_ZX)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_ZY)
    KRATOS_REGISTER_VARIABLE(DEM_STRESS_ZZ)
    // *************** Continuum only END *************

    // Possible future blocks (no FEM) interaction

    KRATOS_REGISTER_VARIABLE(PARTICLE_BLOCK_CONTACT_FAILURE_ID)
    KRATOS_REGISTER_VARIABLE(PARTICLE_BLOCK_IF_INITIAL_CONTACT)
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_PARTICLE_BLOCK_ELEMENTS)

    // DUMMIES INT AND DOUBLE VARIABLES

    KRATOS_REGISTER_VARIABLE(DUMMY_SWITCH)
    KRATOS_REGISTER_VARIABLE(DUMMY_FORCES)
    KRATOS_REGISTER_VARIABLE(DUMMY_LOCAL_AXES)
    
    //EXPORTS
    KRATOS_REGISTER_VARIABLE(EXPORT_SKIN_SPHERE)
    KRATOS_REGISTER_VARIABLE(EXPORT_GROUP_ID)
    KRATOS_REGISTER_VARIABLE(EXPORT_ID)
    KRATOS_REGISTER_VARIABLE(PRINT_EXPORT_ID)
    KRATOS_REGISTER_VARIABLE(PRINT_SKIN_SPHERE)
    KRATOS_REGISTER_VARIABLE(PRINT_GROUP_ID)
    KRATOS_REGISTER_VARIABLE(PRINT_RADIAL_DISPLACEMENT)
    


    

    // ELEMENTS
    KRATOS_REGISTER_ELEMENT("CylinderParticle2D", mCylinderParticle2D)
    KRATOS_REGISTER_ELEMENT("CylinderContinuumParticle2D", mCylinderContinuumParticle2D)
    KRATOS_REGISTER_ELEMENT("SphericParticle3D", mSphericParticle3D)
    KRATOS_REGISTER_ELEMENT("SphericContinuumParticle3D", mSphericContinuumParticle3D)
    KRATOS_REGISTER_ELEMENT("SphericSwimmingParticle3D", mSphericSwimmingParticle3D)
    KRATOS_REGISTER_ELEMENT("DEM_FEM_Particle3D", mDEM_FEM_Particle3D)
    KRATOS_REGISTER_ELEMENT("ParticleContactElement", mParticleContactElement)

    // SERIALIZER
    Serializer::Register( "VariablesList", mVariablesList );

  }

}  // namespace Kratos.


