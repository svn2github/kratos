/*
==============================================================================
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain

Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNER.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/

//
//   Project Name:        Kratos
//   Last Modified by:    $Author: JMCarbonell $
//   Date:                $Date:          2014 $
//   Revision:            $Revision:      1.20 $
//   Note:  IT HAS TO BE CLEANED AND SIMPLIFIED
//


#if !defined(KRATOS_VARIABLES_H_INCLUDED )
#define  KRATOS_VARIABLES_H_INCLUDED



// System includes
#include <string>
#include <iostream>

// External includes


// Project includes
#include "includes/define.h"
#include "containers/variable.h"
#include "containers/variable_component.h"
#include "containers/vector_component_adaptor.h"
#include "includes/kratos_components.h"
#include "includes/ublas_interface.h"
#include "containers/array_1d.h"
#include "containers/weak_pointer_vector.h"
#include "containers/periodic_variables_container.h"
#include "includes/kratos_export_dll.h"

#undef KRATOS_DEFINE_VARIABLE
#undef KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS
#define KRATOS_DEFINE_VARIABLE KRATOS_DEFINE_VARIABLE_DLL
#define KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS_DLL

namespace Kratos
{

    //Define Variables by type:

    //bools

    //for Structural application:
    KRATOS_DEFINE_VARIABLE( bool, IS_INACTIVE )
    KRATOS_DEFINE_VARIABLE( bool, USE_DISTRIBUTED_PROPERTIES )

    //for Level Set application:
    KRATOS_DEFINE_VARIABLE( bool, IS_DUPLICATED )
    KRATOS_DEFINE_VARIABLE( bool, SPLIT_ELEMENT )
    KRATOS_DEFINE_VARIABLE( bool, SPLIT_NODAL )

    //ints

    //for General kratos application: 
    KRATOS_DEFINE_VARIABLE( int, TIME_STEPS )

    //for Structural application
    KRATOS_DEFINE_VARIABLE( int, CONTACT_RAMP )
    KRATOS_DEFINE_VARIABLE( int, CONTACT_SLAVE_INTEGRATION_POINT_INDEX )
    KRATOS_DEFINE_VARIABLE( int, CONTACT_DOUBLE_CHECK )
    KRATOS_DEFINE_VARIABLE( int, IS_CONTACT_MASTER )
    KRATOS_DEFINE_VARIABLE( int, IS_CONTACT_SLAVE )
    KRATOS_DEFINE_VARIABLE( int, FIRST_TIME_STEP )
    KRATOS_DEFINE_VARIABLE( int, QUASI_STATIC_ANALYSIS )


    KRATOS_DEFINE_VARIABLE( int, OSS_SWITCH )
    KRATOS_DEFINE_VARIABLE( int, NL_ITERATION_NUMBER )
    KRATOS_DEFINE_VARIABLE( int, FRACTIONAL_STEP )
    KRATOS_DEFINE_VARIABLE( int, WRINKLING_APPROACH )
    KRATOS_DEFINE_VARIABLE( int, CALCULATE_INSITU_STRESS )
    KRATOS_DEFINE_VARIABLE( int, PERIODIC_PAIR_INDEX )
    KRATOS_DEFINE_VARIABLE( int, STATIONARY )
    KRATOS_DEFINE_VARIABLE( int, ACTIVATION_LEVEL )

    //for ALE application:
    KRATOS_DEFINE_VARIABLE( int, PARTITION_MASK )

    //for PFEM fluids application:
    KRATOS_DEFINE_VARIABLE( int, SCALE )
    KRATOS_DEFINE_VARIABLE( int, IS_JACK_LINK )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_PRESSURE )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_VELOCITY_X )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_VELOCITY_Y )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_VELOCITY_Z )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_ANGULAR_VELOCITY_X )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_ANGULAR_VELOCITY_Y )
    KRATOS_DEFINE_VARIABLE( int, IMPOSED_ANGULAR_VELOCITY_Z )

    //for Level Set application:
    KRATOS_DEFINE_VARIABLE( int, REFINEMENT_LEVEL )

    //for DEM application:
    KRATOS_DEFINE_VARIABLE( double, GEL_STRENGTH)
    KRATOS_DEFINE_VARIABLE( double, POWER_LAW_N)
    KRATOS_DEFINE_VARIABLE( double, POWER_LAW_K)
    KRATOS_DEFINE_VARIABLE( int, FIXED_MESH_OPTION)  
    KRATOS_DEFINE_VARIABLE( int, PARTICLE_MATERIAL )

    //doubles

    //for General kratos application: 
    KRATOS_DEFINE_VARIABLE( double, TIME )
    KRATOS_DEFINE_VARIABLE( double, START_TIME )
    KRATOS_DEFINE_VARIABLE( double, END_TIME )
    KRATOS_DEFINE_VARIABLE( double, DELTA_TIME )

    KRATOS_DEFINE_VARIABLE( double, TEMPERATURE )
    KRATOS_DEFINE_VARIABLE( double, PRESSURE )

    KRATOS_DEFINE_VARIABLE( double, NEGATIVE_FACE_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, POSITIVE_FACE_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, FACE_HEAT_FLUX )

    KRATOS_DEFINE_VARIABLE( double, DENSITY )
    KRATOS_DEFINE_VARIABLE( double, YOUNG_MODULUS )
    KRATOS_DEFINE_VARIABLE( double, POISSON_RATIO )
    KRATOS_DEFINE_VARIABLE( double, THICKNESS )
    KRATOS_DEFINE_VARIABLE( double, YIELD_STRESS )

    KRATOS_DEFINE_VARIABLE( double, NODAL_H )

    //for Structural application
    KRATOS_DEFINE_VARIABLE( double, FRICTION_COEFFICIENT )
    KRATOS_DEFINE_VARIABLE( double, LAMBDA )
    KRATOS_DEFINE_VARIABLE( double, MIU )
    KRATOS_DEFINE_VARIABLE( double, NORMAL_CONTACT_STRESS )
    KRATOS_DEFINE_VARIABLE( double, TANGENTIAL_CONTACT_STRESS )
    KRATOS_DEFINE_VARIABLE( double, CONTACT_STICK )

    KRATOS_DEFINE_VARIABLE( double, DYNAMIC_TAU )
    KRATOS_DEFINE_VARIABLE( double, PARTITION_INDEX )
    KRATOS_DEFINE_VARIABLE( double, ERASE_FLAG )
    KRATOS_DEFINE_VARIABLE( double, TEMPERATURE_OLD_IT )
    KRATOS_DEFINE_VARIABLE( double, VISCOSITY )
    KRATOS_DEFINE_VARIABLE( double, KINEMATIC_VISCOSITY)
    KRATOS_DEFINE_VARIABLE( double, DYNAMIC_VISCOSITY)
    KRATOS_DEFINE_VARIABLE( double, ERROR_RATIO )
    KRATOS_DEFINE_VARIABLE( double, MU )
    KRATOS_DEFINE_VARIABLE( double, TAU )
    KRATOS_DEFINE_VARIABLE( double, EFFECTIVE_VISCOSITY )
    KRATOS_DEFINE_VARIABLE( double, EQ_STRAIN_RATE )
    KRATOS_DEFINE_VARIABLE( double, RHS_WATER )
    KRATOS_DEFINE_VARIABLE( double, RHS_AIR )
    KRATOS_DEFINE_VARIABLE( double, WEIGHT_FATHER_NODES )
    KRATOS_DEFINE_VARIABLE( double, INITIAL_PENALTY )
    KRATOS_DEFINE_VARIABLE( double, DP_EPSILON )
    KRATOS_DEFINE_VARIABLE( double, DP_ALPHA1 )
    KRATOS_DEFINE_VARIABLE( double, DP_K )
    KRATOS_DEFINE_VARIABLE( double, INTERNAL_FRICTION_ANGLE )
    KRATOS_DEFINE_VARIABLE( double, PERMEABILITY_28_DAYS )
    KRATOS_DEFINE_VARIABLE( double, PERMEABILITY_1_DAY )
    KRATOS_DEFINE_VARIABLE( double, PERMEABILITY_TRANSITION )
    KRATOS_DEFINE_VARIABLE( double, K0 )
    KRATOS_DEFINE_VARIABLE( double, NODAL_VOLUME )

    KRATOS_DEFINE_VARIABLE( double, PRESSURE_DT )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, REACTION_WATER_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_DT )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_ACCELERATION )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_NULL )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_NULL_DT )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_NULL_ACCELERATION )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_EINS )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_EINS_DT )
    KRATOS_DEFINE_VARIABLE( double, WATER_PRESSURE_EINS_ACCELERATION )

    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, REACTION_AIR_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_DT )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_ACCELERATION )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_NULL )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_NULL_DT )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_NULL_ACCELERATION )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_EINS )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_EINS_DT )
    KRATOS_DEFINE_VARIABLE( double, AIR_PRESSURE_EINS_ACCELERATION )
    KRATOS_DEFINE_VARIABLE( double, SUCTION )
    KRATOS_DEFINE_VARIABLE( double, FLAG_VARIABLE )
    KRATOS_DEFINE_VARIABLE( double, DISTANCE )

    KRATOS_DEFINE_VARIABLE( double, LAGRANGE_AIR_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, LAGRANGE_WATER_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, LAGRANGE_TEMPERATURE )
    KRATOS_DEFINE_VARIABLE( double, IS_INLET )

    //for ALE application
    KRATOS_DEFINE_VARIABLE( double, IS_INTERFACE )
    KRATOS_DEFINE_VARIABLE( double, IS_VISITED )
    KRATOS_DEFINE_VARIABLE( double, IS_EROSIONABLE )
    KRATOS_DEFINE_VARIABLE( double, AUX_MESH_VAR )

    //for Electric application
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_DT )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_DT2 )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_NULL )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_EINS )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_NULL_DT )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_EINS_DT )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_NULL_DT2 )
    KRATOS_DEFINE_VARIABLE( double, ELECTRIC_POTENTIAL_EINS_DT2 )
    KRATOS_DEFINE_VARIABLE( double, LAGRANGE_ELECTRIC_POTENTIAL )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_DT )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_DT2 )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_NULL )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_EINS )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_NULL_DT )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_EINS_DT )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_NULL_DT2 )
    KRATOS_DEFINE_VARIABLE( double, CONCENTRATION_EINS_DT2 )
    KRATOS_DEFINE_VARIABLE( double, LAGRANGE_CONCENTRATION )

    //for PFEM fluids application: 
    KRATOS_DEFINE_VARIABLE( double, NODAL_AREA )
    KRATOS_DEFINE_VARIABLE( double, IS_STRUCTURE )
    KRATOS_DEFINE_VARIABLE( double, IS_POROUS )
    KRATOS_DEFINE_VARIABLE( double, IS_WATER )
    KRATOS_DEFINE_VARIABLE( double, IS_FLUID )
    KRATOS_DEFINE_VARIABLE( double, IS_BOUNDARY )
    KRATOS_DEFINE_VARIABLE( double, IS_FREE_SURFACE )
    KRATOS_DEFINE_VARIABLE( double, IS_LAGRANGIAN_INLET )
    KRATOS_DEFINE_VARIABLE( double, IS_WATER_ELEMENT )

    KRATOS_DEFINE_VARIABLE( double, BULK_MODULUS )
    KRATOS_DEFINE_VARIABLE( double, SATURATION )
    KRATOS_DEFINE_VARIABLE( double, DENSITY_WATER )
    KRATOS_DEFINE_VARIABLE( double, VISCOSITY_WATER )
    KRATOS_DEFINE_VARIABLE( double, DENSITY_AIR )
    KRATOS_DEFINE_VARIABLE( double, VISCOSITY_AIR )
    KRATOS_DEFINE_VARIABLE( double, POROSITY )
    KRATOS_DEFINE_VARIABLE( double, DIAMETER )
    KRATOS_DEFINE_VARIABLE( double, LIN_DARCY_COEF )
    KRATOS_DEFINE_VARIABLE( double, NONLIN_DARCY_COEF )

    KRATOS_DEFINE_VARIABLE( double, AIR_ENTRY_VALUE )
    KRATOS_DEFINE_VARIABLE( double, FIRST_SATURATION_PARAM )
    KRATOS_DEFINE_VARIABLE( double, SECOND_SATURATION_PARAM )
    KRATOS_DEFINE_VARIABLE( double, PERMEABILITY_WATER )
    KRATOS_DEFINE_VARIABLE( double, PERMEABILITY_AIR )
    KRATOS_DEFINE_VARIABLE( double, BULK_AIR )
    KRATOS_DEFINE_VARIABLE( double, C_SMAGORINSKY )
    KRATOS_DEFINE_VARIABLE( double, MOLECULAR_VISCOSITY )
    KRATOS_DEFINE_VARIABLE( double, TURBULENT_VISCOSITY )

    KRATOS_DEFINE_VARIABLE( double, TEMP_CONV_PROJ )
    KRATOS_DEFINE_VARIABLE( double, CONVECTION_COEFFICIENT )

    KRATOS_DEFINE_VARIABLE( double, SOUND_VELOCITY )
    KRATOS_DEFINE_VARIABLE( double, AIR_SOUND_VELOCITY )
    KRATOS_DEFINE_VARIABLE( double, WATER_SOUND_VELOCITY )
    KRATOS_DEFINE_VARIABLE( double, NODAL_MASS )
    KRATOS_DEFINE_VARIABLE( double, AUX_INDEX )
    KRATOS_DEFINE_VARIABLE( double, EXTERNAL_PRESSURE )
    KRATOS_DEFINE_VARIABLE( double, PRESSURE_OLD_IT )
    KRATOS_DEFINE_VARIABLE( double, IMPOSED_VELOCITY_X_VALUE )
    KRATOS_DEFINE_VARIABLE( double, IMPOSED_VELOCITY_Y_VALUE )
    KRATOS_DEFINE_VARIABLE( double, IMPOSED_VELOCITY_Z_VALUE )
    KRATOS_DEFINE_VARIABLE( double, IMPOSED_ANGULAR_VELOCITY_X_VALUE )
    KRATOS_DEFINE_VARIABLE( double, IMPOSED_ANGULAR_VELOCITY_Y_VALUE )
    KRATOS_DEFINE_VARIABLE( double, IMPOSED_ANGULAR_VELOCITY_Z_VALUE )        
    KRATOS_DEFINE_VARIABLE( double, VELOCITY_PERIOD )
    KRATOS_DEFINE_VARIABLE( double, ANGULAR_VELOCITY_PERIOD )        
    KRATOS_DEFINE_VARIABLE( double, DIVPROJ )
    KRATOS_DEFINE_VARIABLE( double, THAWONE )
    KRATOS_DEFINE_VARIABLE( double, THAWTWO )

    //for Other applications:
    KRATOS_DEFINE_VARIABLE( double, ARRHENIUS )
    KRATOS_DEFINE_VARIABLE( double, ARRHENIUSAUX )
    KRATOS_DEFINE_VARIABLE( double, ARRHENIUSAUX_ )
    KRATOS_DEFINE_VARIABLE( double, PRESSUREAUX )
    KRATOS_DEFINE_VARIABLE( double, NODAL_MAUX )
    KRATOS_DEFINE_VARIABLE( double, NODAL_PAUX )
    KRATOS_DEFINE_VARIABLE( double, HEAT_FLUX )
    KRATOS_DEFINE_VARIABLE( double, TC )
    KRATOS_DEFINE_VARIABLE( double, IS_BURN )
    KRATOS_DEFINE_VARIABLE( double, CONDUCTIVITY )
    KRATOS_DEFINE_VARIABLE( double, SPECIFIC_HEAT )
    KRATOS_DEFINE_VARIABLE( double, IS_DRIPPING )
    KRATOS_DEFINE_VARIABLE( double, IS_PERMANENT )
    KRATOS_DEFINE_VARIABLE( double, MATERIAL_VARIABLE )
    KRATOS_DEFINE_VARIABLE( double, IS_WALL )
    KRATOS_DEFINE_VARIABLE( double, FUEL )
    KRATOS_DEFINE_VARIABLE( double, YO )
    KRATOS_DEFINE_VARIABLE( double, YF )
    KRATOS_DEFINE_VARIABLE( double, YI )
    KRATOS_DEFINE_VARIABLE( double, M )
    KRATOS_DEFINE_VARIABLE( double, Y1 )
    KRATOS_DEFINE_VARIABLE( double, Y2 )
    KRATOS_DEFINE_VARIABLE( double, YP )
    KRATOS_DEFINE_VARIABLE( double, Ypr )
    KRATOS_DEFINE_VARIABLE( double, Yox )
    KRATOS_DEFINE_VARIABLE( double, Yfuel )
    KRATOS_DEFINE_VARIABLE( double, Hfuel )
    KRATOS_DEFINE_VARIABLE( double, Hpr )
    KRATOS_DEFINE_VARIABLE( double, Hpr1 )
    KRATOS_DEFINE_VARIABLE( double, Hox )

    KRATOS_DEFINE_VARIABLE( double, ABSORPTION_COEFFICIENT )
    KRATOS_DEFINE_VARIABLE( double, STEFAN_BOLTZMANN_CONSTANT )

    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_1 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_2 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_3 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_4 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_5 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_6 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_7 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_8 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_9 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_10 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_11 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_12 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_13 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_14 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_15 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_16 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_17 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_18 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_19 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_20 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_21 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_22 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_23 )
    KRATOS_DEFINE_VARIABLE( double, RADIATIVE_INTENSITY_24 )
    KRATOS_DEFINE_VARIABLE( double, EMISSIVITY )
    KRATOS_DEFINE_VARIABLE( double, ENTHALPY )
    KRATOS_DEFINE_VARIABLE( double, MIXTURE_FRACTION )
    KRATOS_DEFINE_VARIABLE( double, rhoD )
    KRATOS_DEFINE_VARIABLE( double, xi )
    KRATOS_DEFINE_VARIABLE( double, a )
    KRATOS_DEFINE_VARIABLE( double, b )

    KRATOS_DEFINE_VARIABLE( double, YCH4 )
    KRATOS_DEFINE_VARIABLE( double, YO2 )
    KRATOS_DEFINE_VARIABLE( double, YCO2 )
    KRATOS_DEFINE_VARIABLE( double, YH2O )
    KRATOS_DEFINE_VARIABLE( double, YN2 )

    KRATOS_DEFINE_VARIABLE( double, IS_SLIP )
    KRATOS_DEFINE_VARIABLE( double, WET_VOLUME )
    KRATOS_DEFINE_VARIABLE( double, CUTTED_AREA )
    KRATOS_DEFINE_VARIABLE( double, NET_INPUT_MATERIAL )

    KRATOS_DEFINE_VARIABLE( double, INCIDENT_RADIATION_FUNCTION )

    KRATOS_DEFINE_VARIABLE( double, SWITCH_TEMPERATURE )
    KRATOS_DEFINE_VARIABLE( double, NODAL_SWITCH )
      
    //for Xfem application:
    KRATOS_DEFINE_VARIABLE( double, CRACK_OPENING )
    KRATOS_DEFINE_VARIABLE( double, CRACK_TRANSLATION )

    //for Level Set application:
    KRATOS_DEFINE_VARIABLE( double, IS_DIVIDED )
    KRATOS_DEFINE_VARIABLE( double, MIN_DT )
    KRATOS_DEFINE_VARIABLE( double, MAX_DT )
    KRATOS_DEFINE_VARIABLE( double, VEL_ART_VISC )
    KRATOS_DEFINE_VARIABLE( double, PR_ART_VISC )
    KRATOS_DEFINE_VARIABLE( double, INTERNAL_ENERGY )

    //for DEM Application:
    KRATOS_DEFINE_VARIABLE( double, PARTICLE_MASS )
    KRATOS_DEFINE_VARIABLE( double, RADIUS )
    KRATOS_DEFINE_VARIABLE( double, SEARCH_TOLERANCE )
    KRATOS_DEFINE_VARIABLE( double, AMPLIFIED_CONTINUUM_SEARCH_RADIUS_EXTENSION )
    KRATOS_DEFINE_VARIABLE( double, DEM_DELTA_TIME )

    // Swimming DEM Application BEGINNING
    KRATOS_DEFINE_VARIABLE( int, NON_NEWTONIAN_OPTION )
    KRATOS_DEFINE_VARIABLE( int, MANUALLY_IMPOSED_DRAG_LAW_OPTION )
    KRATOS_DEFINE_VARIABLE( int, DRAG_MODIFIER_TYPE )
    KRATOS_DEFINE_VARIABLE( int, BUOYANCY_FORCE_TYPE )
    KRATOS_DEFINE_VARIABLE( int, DRAG_FORCE_TYPE )
    KRATOS_DEFINE_VARIABLE( int, VIRTUAL_MASS_FORCE_TYPE )
    KRATOS_DEFINE_VARIABLE( int, LIFT_FORCE_TYPE )
    KRATOS_DEFINE_VARIABLE( int, FLUID_MODEL_TYPE )
    KRATOS_DEFINE_VARIABLE( double, POWER_LAW_TOLERANCE )
    KRATOS_DEFINE_VARIABLE( double, PARTICLE_SPHERICITY )
    KRATOS_DEFINE_VARIABLE( double, INIT_DRAG_FORCE )
    KRATOS_DEFINE_VARIABLE( double, DRAG_LAW_SLOPE )
    KRATOS_DEFINE_VARIABLE( double, SOLID_FRACTION )
    KRATOS_DEFINE_VARIABLE( double, SOLID_FRACTION_RATE )
    KRATOS_DEFINE_VARIABLE( double, FLUID_FRACTION )
    KRATOS_DEFINE_VARIABLE( double, FLUID_FRACTION_RATE )
    KRATOS_DEFINE_VARIABLE( double, PHASE_FRACTION )
    KRATOS_DEFINE_VARIABLE( double, PHASE_FRACTION_RATE )
    KRATOS_DEFINE_VARIABLE( double, SOLID_FRACTION_PROJECTED )
    KRATOS_DEFINE_VARIABLE( double, FLUID_FRACTION_PROJECTED )
    KRATOS_DEFINE_VARIABLE( double, FLUID_DENSITY_PROJECTED )
    KRATOS_DEFINE_VARIABLE( double, FLUID_VISCOSITY_PROJECTED )
    KRATOS_DEFINE_VARIABLE( double, REYNOLDS_NUMBER )
    KRATOS_DEFINE_VARIABLE( double, SHEAR_RATE_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( HYDRODYNAMIC_FORCE )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FLUID_VEL_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FLUID_ACCEL_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FLUID_VORTICITY_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( HYDRODYNAMIC_REACTION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DRAG_REACTION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LIFT_FORCE )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( VIRTUAL_MASS_FORCE )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( BUOYANCY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( PRESSURE_GRADIENT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( PRESSURE_GRAD_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( SOLID_FRACTION_GRADIENT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( SOLID_FRACTION_GRADIENT_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FLUID_FRACTION_GRADIENT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FLUID_FRACTION_GRADIENT_PROJECTED )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( PHASE_FRACTION_GRADIENT )
    // Swimming DEM Application END

    //for Vulcan application
    KRATOS_DEFINE_VARIABLE( double, LATENT_HEAT )
    KRATOS_DEFINE_VARIABLE( double, SOLID_TEMPERATURE )
    KRATOS_DEFINE_VARIABLE( double, FLUID_TEMPERATURE ) 
    KRATOS_DEFINE_VARIABLE( double, AMBIENT_TEMPERATURE )
    KRATOS_DEFINE_VARIABLE( double, Y_WALL)
    KRATOS_DEFINE_VARIABLE( double, COUNTER )
    KRATOS_DEFINE_VARIABLE( double, DISTANCE_CORRECTION )
    KRATOS_DEFINE_VARIABLE( double, COMPUTED_DISTANCE )
    KRATOS_DEFINE_VARIABLE( double, MATERIAL )

    //vectors

    //for General kratos application: 
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ROTATION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( TORQUE )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ANGULAR_VELOCITY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ANGULAR_ACCELERATION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( REACTION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( VELOCITY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ACCELERATION )

    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( VOLUME_ACCELERATION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FORCE )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MOMENT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FACE_LOAD )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( NORMAL )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( BODY_FORCE )

    //for Structural application: 
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MOMENTUM )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_OLD )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_DT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_NULL )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_NULL_DT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ACCELERATION_NULL )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_EINS )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_EINS_DT )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ACCELERATION_EINS )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( LAGRANGE_DISPLACEMENT )

    KRATOS_DEFINE_VARIABLE( Vector, ELEMENTAL_DISTANCES )
    KRATOS_DEFINE_VARIABLE( Vector, MATERIAL_PARAMETERS )
    KRATOS_DEFINE_VARIABLE( Vector, INTERNAL_VARIABLES )
    KRATOS_DEFINE_VARIABLE( Vector, INSITU_STRESS )

    KRATOS_DEFINE_VARIABLE( Vector, PENALTY )
    KRATOS_DEFINE_VARIABLE( Vector, PENALTY_T )
    KRATOS_DEFINE_VARIABLE( Vector, LAMBDAS )
    KRATOS_DEFINE_VARIABLE( Vector, GAPS )
    KRATOS_DEFINE_VARIABLE( Vector, DELTA_LAMBDAS )
    KRATOS_DEFINE_VARIABLE( Vector, STICK )
    KRATOS_DEFINE_VARIABLE( Vector, NORMAL_STRESS )
    KRATOS_DEFINE_VARIABLE( Vector, TANGENTIAL_STRESS )

    KRATOS_DEFINE_VARIABLE( vector<int>, NEIGHBOURS_INDICES )

    //for PFEM fluids application:
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( GRAVITY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( NORMAL_TO_WALL )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( STRUCTURE_VELOCITY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DRAG_FORCE )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( EMBEDDED_VELOCITY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FRACT_VEL )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( PRESS_PROJ )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( CONV_PROJ )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ROTATION_CENTER )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( ADVPROJ )


    //for Other applications:
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( SEEPAGE_DRAG )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MASS )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( RHS )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( FORCE_CM )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MOMENTUM_CM )

    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( xi_c )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DIRECTION )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( Y )

    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY1 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY2 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY3 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY4 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY5 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY6 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY7 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY8 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY9 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY10 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY11 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY12 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY13 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY14 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY15 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY16 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY17 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY18 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY19 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY20 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY21 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY22 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY23 )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( MESH_VELOCITY24 )


    KRATOS_DEFINE_VARIABLE( Vector, BDF_COEFFICIENTS )


    //for DEM application:
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( DAMP_FORCES )
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS( AUX_VEL )

    KRATOS_DEFINE_VARIABLE( Vector, NEIGHBOURS_IDS_DOUBLE )
    KRATOS_DEFINE_VARIABLE( Vector, PARTICLE_ROTATE_SPRING_FAILURE_TYPE )

    KRATOS_DEFINE_VARIABLE( vector<int>, OLD_NEIGHBOURS_IDS )
    KRATOS_DEFINE_VARIABLE( vector<int>, INI_NEIGHBOURS_IDS )
    KRATOS_DEFINE_VARIABLE( vector<int>, CONTINUUM_INI_NEIGHBOURS_IDS )
    KRATOS_DEFINE_VARIABLE( vector<int>, NEIGHBOURS_IDS )
    KRATOS_DEFINE_VARIABLE( vector<int>, PARTICLE_INITIAL_FAILURE_ID )
    KRATOS_DEFINE_VARIABLE( vector<int>, CONTINUUM_PARTICLE_INITIAL_FAILURE_ID )       


    //for Vulcan application
    KRATOS_DEFINE_VARIABLE( Vector, ENRICHED_PRESSURES )


    //matrices

    //for General kratos application: 
    KRATOS_DEFINE_VARIABLE( Matrix, GREEN_LAGRANGE_STRAIN_TENSOR )
    KRATOS_DEFINE_VARIABLE( Matrix, PK2_STRESS_TENSOR )
    KRATOS_DEFINE_VARIABLE( Matrix, CAUCHY_STRESS_TENSOR )

    //for Structural application:
    KRATOS_DEFINE_VARIABLE( Matrix, LAMBDAS_T )
    KRATOS_DEFINE_VARIABLE( Matrix, DELTA_LAMBDAS_T )
    KRATOS_DEFINE_VARIABLE( Matrix, CONTACT_LINK_M )
    KRATOS_DEFINE_VARIABLE( Matrix, INERTIA )
    KRATOS_DEFINE_VARIABLE( Matrix, LOCAL_INERTIA )
    KRATOS_DEFINE_VARIABLE( Matrix, AUXILIARY_MATRIX_1 )
    KRATOS_DEFINE_VARIABLE( Matrix, ELASTIC_LEFT_CAUCHY_GREEN_OLD )

    //others

    //for General kratos application: 
    //KRATOS_DEFINE_VARIABLE( ConstitutiveLaw::Pointer, CONSTITUTIVE_LAW )


    //for Structural application:
    //KRATOS_DEFINE_VARIABLE( WeakPointerVector< GeometricalObject >, NEIGHBOUR_EMBEDDED_FACES )
    //KRATOS_DEFINE_VARIABLE( ConvectionDiffusionSettings::Pointer,  CONVECTION_DIFFUSION_SETTINGS )
    //KRATOS_DEFINE_VARIABLE( RadiationSettings::Pointer,  RADIATION_SETTINGS )


    //for DEM application:  
    typedef vector<array_1d<double,3> > VectorArray3Double;
    KRATOS_DEFINE_VARIABLE( VectorArray3Double, PARTICLE_ROTATE_SPRING_MOMENT )
    KRATOS_DEFINE_VARIABLE( PeriodicVariablesContainer, PERIODIC_VARIABLES )

    // for Vulcan application
    KRATOS_DEFINE_VARIABLE(double,  LAST_AIR)
    KRATOS_DEFINE_VARIABLE(double,  PRESSURES)
    KRATOS_DEFINE_VARIABLE(double,  TEMPERATURES)
    KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(VELOCITIES)

}  // namespace Kratos.

// Resotre the default defines
#undef KRATOS_DEFINE_VARIABLE
#undef KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS
#define KRATOS_DEFINE_VARIABLE KRATOS_DEFINE_VARIABLE_NO_DLL
#define KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS_NO_DLL

#endif // KRATOS_VARIABLES_H_INCLUDED  defined 
