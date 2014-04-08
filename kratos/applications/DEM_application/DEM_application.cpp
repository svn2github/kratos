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
#include "geometries/quadrilateral_3d_4.h"
#include "geometries/triangle_3d_3.h"

namespace Kratos
{


  KRATOS_CREATE_VARIABLE(WeakPointerVector< Element >, CONTINUUM_INI_NEIGHBOUR_ELEMENTS)
  KRATOS_CREATE_VARIABLE(WeakPointerVector< Element >, NODE_TO_NEIGH_ELEMENT_POINTER)
  
  // OPTIONS AND FLAGS
  
  KRATOS_CREATE_VARIABLE(int, FORCE_INTEGRATION_GROUP)
  KRATOS_CREATE_VARIABLE(int, TOP)
  KRATOS_CREATE_VARIABLE(int, BOTTOM)
  KRATOS_CREATE_VARIABLE(int, BOUNDING_BOX_OPTION)
  KRATOS_CREATE_VARIABLE(int, ROTATION_OPTION)
  KRATOS_CREATE_VARIABLE(int, CRITICAL_TIME_OPTION)
  KRATOS_CREATE_VARIABLE(int, VIRTUAL_MASS_OPTION)
  KRATOS_CREATE_VARIABLE(int, ACTIVATE_SEARCH)
  KRATOS_CREATE_VARIABLE(vector<int>, ACTIVATE_SEARCH_VECTOR)
  KRATOS_CREATE_VARIABLE(int, LIMIT_SURFACE_OPTION)
  KRATOS_CREATE_VARIABLE(int, LIMIT_CYLINDER_OPTION)
  KRATOS_CREATE_VARIABLE(int, CLEAN_INDENT_OPTION)
  KRATOS_CREATE_VARIABLE(int, TRIHEDRON_OPTION)
  KRATOS_CREATE_VARIABLE(int, FORCE_CALCULATION_TYPE)
  KRATOS_CREATE_VARIABLE(int, DAMP_TYPE)
  KRATOS_CREATE_VARIABLE(int, ROLLING_FRICTION_OPTION)
  KRATOS_CREATE_VARIABLE(int, NEIGH_INITIALIZED)
  KRATOS_CREATE_VARIABLE(int, GROUP_ID)
  KRATOS_CREATE_VARIABLE(int, TRIAXIAL_TEST_OPTION)
  KRATOS_CREATE_VARIABLE(int, FIX_VELOCITIES_FLAG)
  KRATOS_CREATE_VARIABLE(int, STRESS_STRAIN_OPTION)
  KRATOS_CREATE_VARIABLE(double, DUMMY_DEBUG_DOUBLE)
  KRATOS_CREATE_VARIABLE(int, DUMMY_DEBUG_INT)
  KRATOS_CREATE_VARIABLE(int, PARTICLE_ID)

  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_VARIABLE(bool, AREA_CALCULATED_FLAG)
  KRATOS_CREATE_VARIABLE(Vector, DEM_AREA_VECTOR)
  KRATOS_CREATE_VARIABLE(bool, DELTA_OPTION)
  KRATOS_CREATE_VARIABLE(int, DEMPACK_OPTION)
  KRATOS_CREATE_VARIABLE(int, CASE_OPTION)
  KRATOS_CREATE_VARIABLE(int, SKIN_SPHERE)
  KRATOS_CREATE_VARIABLE(int, CONTACT_MESH_OPTION)
  KRATOS_CREATE_VARIABLE(int, FAILURE_CRITERION_OPTION)
  KRATOS_CREATE_VARIABLE(int, CONCRETE_TEST_OPTION)
  KRATOS_CREATE_VARIABLE(int, COHESIVE_GROUP)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_TENSION)
  KRATOS_CREATE_VARIABLE(double, PARTICLE_COHESION)
  KRATOS_CREATE_VARIABLE(int, plot_OPTIONS)
  KRATOS_CREATE_VARIABLE(double, DEMPACK_DAMPING)
  KRATOS_CREATE_VARIABLE(double, DEMPACK_GLOBAL_DAMPING)
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
  KRATOS_CREATE_VARIABLE(int, BUOYANCY_FORCE_TYPE)
  KRATOS_CREATE_VARIABLE(int, DRAG_FORCE_TYPE)
  KRATOS_CREATE_VARIABLE(int, VIRTUAL_MASS_FORCE_TYPE)
  KRATOS_CREATE_VARIABLE(int, LIFT_FORCE_TYPE)
  
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
  KRATOS_CREATE_VARIABLE(int, FLUID_MODEL_TYPE)
  // *************** Swimming only END ***************

  // TIME RELATED

  KRATOS_CREATE_VARIABLE(double, TIME_INCREASING_RATIO)
  KRATOS_CREATE_VARIABLE(double, FINAL_SIMULATION_TIME)


  // MATERIAL PARAMETERS

  KRATOS_CREATE_VARIABLE(double, NODAL_MASS_COEFF)
  KRATOS_CREATE_VARIABLE(double, DEM_MAGIC_FACTOR)
  KRATOS_CREATE_VARIABLE(double, DEM_MAGIC_FACTOR_POISSON)
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
  KRATOS_CREATE_VARIABLE(double, SLOPE_FRACTION_N3)
  KRATOS_CREATE_VARIABLE(double, SLOPE_LIMIT_COEFF_C1)
  KRATOS_CREATE_VARIABLE(double, SLOPE_LIMIT_COEFF_C2)
  KRATOS_CREATE_VARIABLE(double, SLOPE_LIMIT_COEFF_C3)
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
  KRATOS_CREATE_VARIABLE(double, UNIDIMENSIONAL_DAMAGE)
  KRATOS_CREATE_VARIABLE(double, CONTACT_ORIENTATION)
  KRATOS_CREATE_VARIABLE(double, CONTACT_SIGMA_MIN)
  KRATOS_CREATE_VARIABLE(double, CONTACT_TAU_ZERO)
  KRATOS_CREATE_VARIABLE(double, CONTACT_INTERNAL_FRICC)
  KRATOS_CREATE_VARIABLE(double, LOW_POISSON_FORCE)
  KRATOS_CREATE_VARIABLE(double, HIGH_POISSON_FORCE)
  // *************** Continuum only END *************

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, INIT_DRAG_FORCE)
  KRATOS_CREATE_VARIABLE(double, DRAG_LAW_SLOPE)
  // *************** Swimming only END ***************

  // GEOMETRIC PARAMETERS

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_VARIABLE(double, PARTICLE_SPHERICITY)
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

  //INLET PARAMETERS
  KRATOS_CREATE_VARIABLE(double, INLET_START_TIME)
  KRATOS_CREATE_VARIABLE(double, INLET_STOP_TIME)
  KRATOS_CREATE_VARIABLE(double, INLET_NUMBER_OF_PARTICLES)

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

  // *************** Swimming only BEGIN *************
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LIFT_FORCE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VIRTUAL_MASS_FORCE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(HYDRODYNAMIC_FORCE)
  // *************** Swimming only END ***************

  // ENERGY

   KRATOS_CREATE_VARIABLE(double, KINETIC_ENERGY)
   KRATOS_CREATE_VARIABLE(double, ELASTIC_ENERGY_OF_CONTACTS)


  // *************** Continuum only BEGIN *************
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INITIAL_ROTA_MOMENT)
  KRATOS_CREATE_VARIABLE(Vector, PARTICLE_BLOCK_CONTACT_FORCE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_APPLIED_FORCE)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE)
  KRATOS_CREATE_VARIABLE(VectorArray3Double, PARTICLE_CONTACT_FORCES )

  // CONCRETE TEST

  KRATOS_CREATE_VARIABLE(double, FIXED_VEL_TOP)
  KRATOS_CREATE_VARIABLE(double, FIXED_VEL_BOT)
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
  
  KRATOS_CREATE_VARIABLE(WeakPointerVector<Condition >,     NEIGHBOUR_RIGID_FACES)

  KRATOS_CREATE_VARIABLE(Vector,    NEIGHBOUR_RIGID_FACES_PRAM)
  
  KRATOS_CREATE_VARIABLE(WeakPointerVector<Element >, NEIGHBOUR_PARTICLE_OF_RIGID_FACE)
  
  KRATOS_CREATE_VARIABLE(Vector,   NEIGHBOUR_RIGID_FACES_ELASTIC_CONTACT_FORCE)
  KRATOS_CREATE_VARIABLE(Vector,   NEIGHBOUR_RIGID_FACES_TOTAL_CONTACT_FORCE)


  // DUMMIES INT AND DOUBLE VARIABLES

  KRATOS_CREATE_VARIABLE(int, DUMMY_SWITCH)
  KRATOS_CREATE_VARIABLE(double, DUMMY_FORCES)
  KRATOS_CREATE_VARIABLE(double, DUMMY_LOCAL_AXES)

  // EXPORTS
  
  KRATOS_CREATE_VARIABLE(double, EXPORT_SKIN_SPHERE)
  KRATOS_CREATE_VARIABLE(double, EXPORT_PARTICLE_FAILURE_ID)
  KRATOS_CREATE_VARIABLE(double, EXPORT_ID)
  KRATOS_CREATE_VARIABLE(int, PRINT_EXPORT_ID)
  KRATOS_CREATE_VARIABLE(int, PRINT_SKIN_SPHERE)
  KRATOS_CREATE_VARIABLE(int, PRINT_GROUP_ID)
  
  //For DEM_FEM element
  KRATOS_CREATE_VARIABLE(double, LOCAL_DAMP_RATIO)

  // DUMMY VARIABLES FOR CALCULATE
  KRATOS_CREATE_VARIABLE(double, CALCULATE_COMPUTE_NEW_NEIGHBOURS_HISTORICAL_DATA)
  KRATOS_CREATE_VARIABLE(double, CALCULATE_COMPUTE_NEW_RIGID_FACE_NEIGHBOURS_HISTORICAL_DATA)
  KRATOS_CREATE_VARIABLE(double, CALCULATE_SET_INITIAL_DEM_CONTACTS)
  KRATOS_CREATE_VARIABLE(double, CALCULATE_SET_INITIAL_FEM_CONTACTS)
  
  

    ///Cfeng,131013,RigidFace Movement
  
  KRATOS_CREATE_VARIABLE(double, WALL_FRICTION)
  
  KRATOS_CREATE_VARIABLE(double, RIGID_FACE_ROTA_SPEED)
  KRATOS_CREATE_VARIABLE(double, RIGID_FACE_AXIAL_SPEED)
  KRATOS_CREATE_VARIABLE(int,    RIGID_FACE_PROP_ID)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_ORIGIN_COORD)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_AXIAL_DIR)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_GLOBAL_VELOCITY)
  KRATOS_CREATE_VARIABLE(double, RIGID_FACE_BEGIN_TIME)
  KRATOS_CREATE_VARIABLE(double, RIGID_FACE_END_TIME)
  KRATOS_CREATE_VARIABLE(int, RIGID_FACE_FLAG)
  KRATOS_CREATE_VARIABLE(Vector, RIGID_FACE_COMPUTE_MOVEMENT)

  //OPTIMIZATION
  KRATOS_CREATE_VARIABLE(int,    VELOCITY_X_DOF_POS)
  KRATOS_CREATE_VARIABLE(int,    VELOCITY_Y_DOF_POS)
  KRATOS_CREATE_VARIABLE(int,    VELOCITY_Z_DOF_POS)
  KRATOS_CREATE_VARIABLE(int,    ANGULAR_VELOCITY_X_DOF_POS)
  KRATOS_CREATE_VARIABLE(int,    ANGULAR_VELOCITY_Y_DOF_POS)
  KRATOS_CREATE_VARIABLE(int,    ANGULAR_VELOCITY_Z_DOF_POS)
  KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(OLD_COORDINATES)
 
  //FLAGS
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,HAS_ROTATION, 0);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,HAS_ROTATION_SPRING, 1);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,HAS_ROLLING_FRICTION, 2);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,HAS_INITIALIZED_VARIABLES, 3);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,HAS_CRITICAL_TIME, 4);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,FIXED_VEL_X, 5);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,FIXED_VEL_Y, 6);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,FIXED_VEL_Z, 7);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,FIXED_ANG_VEL_X, 8);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,FIXED_ANG_VEL_Y, 9);
  KRATOS_CREATE_LOCAL_FLAG(DEMFlags,FIXED_ANG_VEL_Z, 10);

 
  

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
  mParticleContactElement( 0, Element::GeometryType::Pointer( new Line3D2<Node<3> >( Element::GeometryType::PointsArrayType( 2, Node<3>() ) ) ) ),
  mRigidFace3D3N( 0, Element::GeometryType::Pointer( new Triangle3D3 <Node<3> >( Element::GeometryType::PointsArrayType( 3, Node<3>() ) ) ) ),
  mRigidFace3D4N( 0, Element::GeometryType::Pointer( new Quadrilateral3D4 <Node<3> >( Element::GeometryType::PointsArrayType( 4, Node<3>() ) ) ) ),
  mRigidEdge3D2N( 0, Element::GeometryType::Pointer( new Line3D2 <Node<3> >( Element::GeometryType::PointsArrayType( 2, Node<3>() ) ) ) )
  {}
    

  void KratosDEMApplication::Register()
  {

    // Calling base class register to register Kratos components

    KratosApplication::Register();
    std::cout << "Initializing KratosDEMApplication... " << std::endl;

    
    KRATOS_REGISTER_VARIABLE (CONTINUUM_INI_NEIGHBOUR_ELEMENTS )
    KRATOS_REGISTER_VARIABLE ( NODE_TO_NEIGH_ELEMENT_POINTER )
  
        
    // OPTIONS AND FLAGS
    
    KRATOS_REGISTER_VARIABLE( TOP)
    KRATOS_REGISTER_VARIABLE( BOTTOM)
    KRATOS_REGISTER_VARIABLE( FORCE_INTEGRATION_GROUP)
    KRATOS_REGISTER_VARIABLE(BOUNDING_BOX_OPTION)
    KRATOS_REGISTER_VARIABLE(ROTATION_OPTION)
    KRATOS_REGISTER_VARIABLE(CRITICAL_TIME_OPTION)
    KRATOS_REGISTER_VARIABLE(VIRTUAL_MASS_OPTION)
    KRATOS_REGISTER_VARIABLE(ACTIVATE_SEARCH)
    KRATOS_REGISTER_VARIABLE(ACTIVATE_SEARCH_VECTOR)
    KRATOS_REGISTER_VARIABLE(LIMIT_SURFACE_OPTION)
    KRATOS_REGISTER_VARIABLE(LIMIT_CYLINDER_OPTION)
    KRATOS_REGISTER_VARIABLE(CLEAN_INDENT_OPTION)
    KRATOS_REGISTER_VARIABLE(TRIHEDRON_OPTION)
    KRATOS_REGISTER_VARIABLE(FORCE_CALCULATION_TYPE)
    KRATOS_REGISTER_VARIABLE(DAMP_TYPE)
    KRATOS_REGISTER_VARIABLE(NEIGH_INITIALIZED)
    KRATOS_REGISTER_VARIABLE(GROUP_ID)
    KRATOS_REGISTER_VARIABLE(TRIAXIAL_TEST_OPTION)
    KRATOS_REGISTER_VARIABLE(FIX_VELOCITIES_FLAG)
    KRATOS_REGISTER_VARIABLE(STRESS_STRAIN_OPTION)
    KRATOS_REGISTER_VARIABLE(DUMMY_DEBUG_DOUBLE)
    KRATOS_REGISTER_VARIABLE(DUMMY_DEBUG_INT)
    KRATOS_REGISTER_VARIABLE(PARTICLE_ID)
    
    
    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_VARIABLE(AREA_CALCULATED_FLAG)
    KRATOS_REGISTER_VARIABLE(DEM_AREA_VECTOR)
    KRATOS_REGISTER_VARIABLE(DELTA_OPTION)
    KRATOS_REGISTER_VARIABLE(CASE_OPTION)
    KRATOS_REGISTER_VARIABLE(DEMPACK_OPTION)
    KRATOS_REGISTER_VARIABLE(SKIN_SPHERE)
    KRATOS_REGISTER_VARIABLE(CONTACT_MESH_OPTION)
    KRATOS_REGISTER_VARIABLE(FAILURE_CRITERION_OPTION)
    KRATOS_REGISTER_VARIABLE(CONCRETE_TEST_OPTION)
    KRATOS_REGISTER_VARIABLE(COHESIVE_GROUP)
    KRATOS_REGISTER_VARIABLE( PARTICLE_TENSION )
    KRATOS_REGISTER_VARIABLE( PARTICLE_COHESION )
    
    KRATOS_REGISTER_VARIABLE(plot_OPTIONS)
    KRATOS_REGISTER_VARIABLE(DEMPACK_DAMPING)
    KRATOS_REGISTER_VARIABLE(DEMPACK_GLOBAL_DAMPING)
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
    KRATOS_REGISTER_VARIABLE(BUOYANCY_FORCE_TYPE)
    KRATOS_REGISTER_VARIABLE(DRAG_FORCE_TYPE)
    KRATOS_REGISTER_VARIABLE(VIRTUAL_MASS_FORCE_TYPE)
    KRATOS_REGISTER_VARIABLE(LIFT_FORCE_TYPE)
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
    KRATOS_REGISTER_VARIABLE(FLUID_MODEL_TYPE)
    // *************** Swimming only END ***************

    // TIME RELATED

    KRATOS_REGISTER_VARIABLE(TIME_INCREASING_RATIO)
    KRATOS_REGISTER_VARIABLE(FINAL_SIMULATION_TIME)

    // MATERIAL PARAMETERS

    KRATOS_REGISTER_VARIABLE(NODAL_MASS_COEFF)
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
    KRATOS_REGISTER_VARIABLE(SLOPE_FRACTION_N3)
    KRATOS_REGISTER_VARIABLE(SLOPE_LIMIT_COEFF_C1)
    KRATOS_REGISTER_VARIABLE(SLOPE_LIMIT_COEFF_C2)
    KRATOS_REGISTER_VARIABLE(SLOPE_LIMIT_COEFF_C3)
    KRATOS_REGISTER_VARIABLE(YOUNG_MODULUS_PLASTIC)
    KRATOS_REGISTER_VARIABLE(PLASTIC_YIELD_STRESS)
    KRATOS_REGISTER_VARIABLE(DAMAGE_FACTOR)
    KRATOS_REGISTER_VARIABLE(DONZE_G1)
    KRATOS_REGISTER_VARIABLE(DONZE_G2)
    KRATOS_REGISTER_VARIABLE(DONZE_G3)
    KRATOS_REGISTER_VARIABLE(DONZE_MAX_DEF)

    KRATOS_REGISTER_VARIABLE(CONTACT_FAILURE)
    KRATOS_REGISTER_VARIABLE(CONTACT_ORIENTATION)
    KRATOS_REGISTER_VARIABLE(CONTACT_SIGMA)
    KRATOS_REGISTER_VARIABLE(CONTACT_TAU)
    KRATOS_REGISTER_VARIABLE(FAILURE_CRITERION_STATE)
    KRATOS_REGISTER_VARIABLE(UNIDIMENSIONAL_DAMAGE)
    KRATOS_REGISTER_VARIABLE(CONTACT_SIGMA_MIN)
    KRATOS_REGISTER_VARIABLE(CONTACT_TAU_ZERO)
    KRATOS_REGISTER_VARIABLE(CONTACT_INTERNAL_FRICC)
    KRATOS_REGISTER_VARIABLE(LOW_POISSON_FORCE)
    KRATOS_REGISTER_VARIABLE(HIGH_POISSON_FORCE)
    // *************** Continuum only END *************

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_VARIABLE(INIT_DRAG_FORCE)
    KRATOS_REGISTER_VARIABLE(DRAG_LAW_SLOPE)
    // *************** Swimming only END ***************

    // GEOMETRIC PARAMETERS

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_VARIABLE(PARTICLE_SPHERICITY)
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
            
    // INLET PARAMETERS
    
    KRATOS_REGISTER_VARIABLE(INLET_START_TIME)    
    KRATOS_REGISTER_VARIABLE(INLET_STOP_TIME)
    KRATOS_REGISTER_VARIABLE(INLET_NUMBER_OF_PARTICLES)
    

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

    // *************** Swimming only BEGIN *************
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LIFT_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VIRTUAL_MASS_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(HYDRODYNAMIC_FORCE)
    // *************** Swimming only END ***************

    // ENERGY

    KRATOS_REGISTER_VARIABLE(KINETIC_ENERGY)
    KRATOS_REGISTER_VARIABLE(ELASTIC_ENERGY_OF_CONTACTS)


    // *************** Continuum only BEGIN *************
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INITIAL_ROTA_MOMENT)
    KRATOS_REGISTER_VARIABLE(PARTICLE_BLOCK_CONTACT_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_APPLIED_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_CONTACT_FORCE)
    KRATOS_REGISTER_VARIABLE(PARTICLE_CONTACT_FORCES )

    // CONCRETE TEST

    KRATOS_REGISTER_VARIABLE(FIXED_VEL_TOP)
    KRATOS_REGISTER_VARIABLE(FIXED_VEL_BOT)
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
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_RIGID_FACES)
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_RIGID_FACES_PRAM)
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_PARTICLE_OF_RIGID_FACE)	
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_RIGID_FACES_ELASTIC_CONTACT_FORCE)
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_RIGID_FACES_TOTAL_CONTACT_FORCE)
    

    // DUMMIES INT AND DOUBLE VARIABLES

    KRATOS_REGISTER_VARIABLE(DUMMY_SWITCH)
    KRATOS_REGISTER_VARIABLE(DUMMY_FORCES)
    KRATOS_REGISTER_VARIABLE(DUMMY_LOCAL_AXES)
    
    //EXPORTS
    KRATOS_REGISTER_VARIABLE(EXPORT_SKIN_SPHERE)
    KRATOS_REGISTER_VARIABLE(EXPORT_ID)
    KRATOS_REGISTER_VARIABLE(PRINT_EXPORT_ID)
    KRATOS_REGISTER_VARIABLE(PRINT_SKIN_SPHERE)
    KRATOS_REGISTER_VARIABLE(PRINT_GROUP_ID)
    KRATOS_REGISTER_VARIABLE(LOCAL_DAMP_RATIO)
    // DUMMY VARIABLES FOR CALCULATE
    KRATOS_REGISTER_VARIABLE(CALCULATE_COMPUTE_NEW_NEIGHBOURS_HISTORICAL_DATA)
    KRATOS_REGISTER_VARIABLE(CALCULATE_COMPUTE_NEW_RIGID_FACE_NEIGHBOURS_HISTORICAL_DATA)
    KRATOS_REGISTER_VARIABLE(CALCULATE_SET_INITIAL_DEM_CONTACTS)
    KRATOS_REGISTER_VARIABLE(CALCULATE_SET_INITIAL_FEM_CONTACTS)
    
    

	///Cfeng,131013,RigidFace Movement
    
    KRATOS_REGISTER_VARIABLE(WALL_FRICTION)
    
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_ROTA_SPEED)
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_AXIAL_SPEED)
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_PROP_ID)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_ORIGIN_COORD)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_AXIAL_DIR)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(RIGID_FACE_ROTA_GLOBAL_VELOCITY)
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_BEGIN_TIME)
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_END_TIME)
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_FLAG)
    KRATOS_REGISTER_VARIABLE(RIGID_FACE_COMPUTE_MOVEMENT)
       
    //OPTIMIZATION
    KRATOS_REGISTER_VARIABLE(VELOCITY_X_DOF_POS)        
    KRATOS_REGISTER_VARIABLE(VELOCITY_Y_DOF_POS)        
    KRATOS_REGISTER_VARIABLE(VELOCITY_Z_DOF_POS)
    KRATOS_REGISTER_VARIABLE(ANGULAR_VELOCITY_X_DOF_POS)        
    KRATOS_REGISTER_VARIABLE(ANGULAR_VELOCITY_Y_DOF_POS)        
    KRATOS_REGISTER_VARIABLE(ANGULAR_VELOCITY_Z_DOF_POS)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(OLD_COORDINATES)    
            
    // ELEMENTS
    KRATOS_REGISTER_ELEMENT("CylinderParticle2D", mCylinderParticle2D)
    KRATOS_REGISTER_ELEMENT("CylinderContinuumParticle2D", mCylinderContinuumParticle2D)
    KRATOS_REGISTER_ELEMENT("SphericParticle3D", mSphericParticle3D)
    KRATOS_REGISTER_ELEMENT("SphericContinuumParticle3D", mSphericContinuumParticle3D)
    KRATOS_REGISTER_ELEMENT("SphericSwimmingParticle3D", mSphericSwimmingParticle3D)
    KRATOS_REGISTER_ELEMENT("DEM_FEM_Particle3D", mDEM_FEM_Particle3D)
    KRATOS_REGISTER_ELEMENT("ParticleContactElement", mParticleContactElement)
		
	
    KRATOS_REGISTER_CONDITION( "RigidFace3D", mRigidFace3D3N )
    KRATOS_REGISTER_CONDITION( "RigidFace3D3N", mRigidFace3D3N )
    KRATOS_REGISTER_CONDITION( "RigidFace3D4N", mRigidFace3D4N )
    KRATOS_REGISTER_CONDITION( "RigidEdge3D", mRigidEdge3D2N )
    KRATOS_REGISTER_CONDITION( "RigidEdge3D2N", mRigidEdge3D2N )


    // SERIALIZER
    Serializer::Register( "VariablesList", mVariablesList );

  }

}  // namespace Kratos.


