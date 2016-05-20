// Kratos Multi-Physics
// 
// Copyright (c) 2016, Pooyan Dadvand, Riccardo Rossi, CIMNE (International Center for Numerical Methods in Engineering)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// 	-	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 	-	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
// 		in the documentation and/or other materials provided with the distribution.
// 	-	All advertising materials mentioning features or use of this software must display the following acknowledgement: 
// 			This product includes Kratos Multi-Physics technology.
// 	-	Neither the name of the CIMNE nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
// 	
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED ANDON ANY 
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
// THE USE OF THISSOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


// System includes

// External includes
#include <boost/python.hpp>


// Project includes
#include "includes/define.h"
#include "includes/ublas_interface.h"
#include "containers/data_value_container.h"
//#include "containers/hash_data_value_container.h"
// #include "containers/variables_list_data_value_container.h"
// #include "containers/fix_data_value_container.h"
// #include "containers/vector_component_adaptor.h"
#include "containers/flags.h"
//#include "containers/all_variables_data_value_container.h"
// #include "includes/kratos_flags.h"
#include "includes/dem_variables.h"
// #include "includes/constitutive_law.h"
#include "python/add_dem_variables_to_python.h"
// #include "python/vector_python_interface.h"
// #include "python/vector_scalar_operator_python.h"
// #include "python/vector_vector_operator_python.h"
// #include "python/bounded_vector_python_interface.h"

// #include "includes/convection_diffusion_settings.h"
// #include "includes/radiation_settings.h"
#include "utilities/timer.h"



#ifdef KRATOS_REGISTER_IN_PYTHON_FLAG_IMPLEMENTATION
#undef KRATOS_REGISTER_IN_PYTHON_FLAG_IMPLEMENTATION
#endif
#define KRATOS_REGISTER_IN_PYTHON_FLAG_IMPLEMENTATION(flag) \
 scope().attr(#flag) = boost::ref(flag)      \

#ifdef KRATOS_REGISTER_IN_PYTHON_FLAG
#undef KRATOS_REGISTER_IN_PYTHON_FLAG
#endif
#define KRATOS_REGISTER_IN_PYTHON_FLAG(flag) \
    KRATOS_REGISTER_IN_PYTHON_FLAG_IMPLEMENTATION(flag);   \
    KRATOS_REGISTER_IN_PYTHON_FLAG_IMPLEMENTATION(NOT_##flag)

#ifdef KRATOS_REGISTER_IN_PYTHON_VARIABLE
#undef KRATOS_REGISTER_IN_PYTHON_VARIABLE
#endif
#define KRATOS_REGISTER_IN_PYTHON_VARIABLE(variable) \
    scope().attr(#variable) = boost::ref(variable);


#ifdef KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS
#undef KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS
#endif
#define KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(name) \
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(name) \
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(name##_X) \
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(name##_Y) \
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(name##_Z)

namespace Kratos
{
//KRATOS_CREATE_FLAG(STRUCTURE,   63);

namespace Python
{
using namespace boost::python;

void  AddDEMVariablesToPython()
{
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( TOTAL_FORCES )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DAMP_FORCES )   
    //variables in the general Discrete_element class & MPI & gidio

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_MASS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( RADIUS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SEARCH_TOLERANCE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_MATERIAL )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_DENSITY_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_VISCOSITY_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SOLID_FRACTION_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( REYNOLDS_NUMBER )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DRAG_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_RATE_PROJECTED )
    //variables for the neighbour search
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( AMPLIFIED_CONTINUUM_SEARCH_RADIUS_EXTENSION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( OLD_NEIGHBOURS_IDS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( INI_NEIGHBOURS_IDS)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONTINUUM_INI_NEIGHBOURS_IDS)        
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( NEIGHBOURS_IDS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( NEIGHBOURS_IDS_DOUBLE )
            
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( ELEMENT_TYPE )     

    // Swimming DEM Application BEGINNING
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( COUPLING_TYPE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( NON_NEWTONIAN_OPTION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( MANUALLY_IMPOSED_DRAG_LAW_OPTION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DRAG_MODIFIER_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( BUOYANCY_FORCE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DRAG_FORCE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( VIRTUAL_MASS_FORCE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( BASSET_FORCE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( LIFT_FORCE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( MAGNUS_FORCE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( HYDRO_TORQUE_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_MODEL_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DRAG_POROSITY_CORRECTION_TYPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( POWER_LAW_TOLERANCE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PARTICLE_SPHERICITY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( INIT_DRAG_FORCE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DRAG_LAW_SLOPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SOLID_FRACTION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SOLID_FRACTION_RATE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_FRACTION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_FRACTION_RATE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PHASE_FRACTION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PHASE_FRACTION_RATE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SOLID_FRACTION_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_FRACTION_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_DENSITY_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_VISCOSITY_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( REYNOLDS_NUMBER )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DRAG_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_RATE_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ACCELERATION )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( HYDRODYNAMIC_FORCE )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( HYDRODYNAMIC_MOMENT )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_VEL_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_VEL_PROJECTED_RATE )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_VEL_LAPL_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_VEL_LAPL_RATE_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_FLUID_ACCEL_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_ACCEL_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_VORTICITY_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( HYDRODYNAMIC_REACTION )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MEAN_HYDRODYNAMIC_REACTION )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DRAG_REACTION )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( LIFT_FORCE )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( VIRTUAL_MASS_FORCE )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( BASSET_FORCE )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( BUOYANCY )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( PRESSURE_GRADIENT )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( PRESSURE_GRAD_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( SOLID_FRACTION_GRADIENT )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( SOLID_FRACTION_GRADIENT_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_FRACTION_GRADIENT )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FLUID_FRACTION_GRADIENT_PROJECTED )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( PHASE_FRACTION_GRADIENT )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( AUX_VEL )
    // Swimming DEM Application END

    // for DEM and DEM-FEM_Application

    KRATOS_REGISTER_IN_PYTHON_VARIABLE(PARTICLE_ROTATE_SPRING_FAILURE_TYPE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(DEM_DELTA_TIME)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE(FIXED_MESH_OPTION)
            
    // for DEM Sintering

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( ATOMIC_VOLUME )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SURFACE_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DIHEDRAL_ANGLE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SINTERING_START_TEMPERATURE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( RELAXATION_TIME )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( LARGE_VISCOSITY_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( THERMAL_EXPANSION_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PRE_EXP_DIFFUSION_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( GB_WIDTH )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( ENTHAPLY_ACTIVATION )

}
}  // namespace Python.
} // Namespace Kratos

#undef KRATOS_REGISTER_IN_PYTHON_FLAG
#undef KRATOS_REGISTER_IN_PYTHON_VARIABLE
#undef KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS
