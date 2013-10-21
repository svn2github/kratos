/*
==============================================================================
KratosTestApplication 
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi
pooyan@cimne.upc.edu 
rrossi@cimne.upc.edu
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain


Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

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
//   Last modified by:    $Author: G.Casas$
//   Date:                $Date: 2011-6-27 16:07:33 $
//   Revision:            $Revision: 1.2 $
//
//

// System includes 

// External includes 
#include <boost/python.hpp>

// Project includes

#include "includes/model_part.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_utilities/create_and_destroy.h"
#include "custom_utilities/calculate_global_physical_properties.h"
#include "custom_utilities/pre_utilities.h"
#include "custom_utilities/post_utilities.h"



namespace Kratos{

namespace Python{

typedef ModelPart::NodesContainerType::iterator PointIterator;
typedef std::vector<array_1d<double, 3 > > ComponentVectorType;
typedef std::vector<array_1d<double, 3 > >::iterator ComponentIteratorType;


void  AddCustomUtilitiesToPython(){
    using namespace boost::python;
    
      class_<ParticleCreatorDestructor, boost::noncopyable >
        ("ParticleCreatorDestructor", init<>())
        .def("NodeCreator", &ParticleCreatorDestructor::NodeCreator)
        .def("ElementCreator", &ParticleCreatorDestructor::ElementCreator)
        .def("PrintingTest", &ParticleCreatorDestructor::PrintingTest)
        .def("CalculateSurroundingBoundingBox", &ParticleCreatorDestructor::CalculateSurroundingBoundingBox)
        .def("MarkParticlesForErasingGivenBoundingBox", &ParticleCreatorDestructor::MarkParticlesForErasingGivenBoundingBox)
        .def("MarkParticlesForErasingGivenScalarVariableValue", &ParticleCreatorDestructor::MarkParticlesForErasingGivenScalarVariableValue)
        .def("MarkParticlesForErasingGivenVectorVariableModulus", &ParticleCreatorDestructor::MarkParticlesForErasingGivenVectorVariableModulus)
        .def("GetHighNode", &ParticleCreatorDestructor::GetHighNode)
        .def("GetLowNode", &ParticleCreatorDestructor::GetLowNode)
        .def("SetHighNode", &ParticleCreatorDestructor::SetHighNode)
        .def("SetLowNode", &ParticleCreatorDestructor::SetLowNode)
        ;
      
      class_<DEM_Inlet, boost::noncopyable >
        ("DEM_Inlet", init<ModelPart&>())
        .def("CreateElementsFromInletMesh", &DEM_Inlet::CreateElementsFromInletMesh)        
        .def("InitializeDEM_Inlet", &DEM_Inlet::InitializeDEM_Inlet) 
        ;

     class_<SphericElementGlobalPhysicsCalculator, boost::noncopyable >
        ("SphericElementGlobalPhysicsCalculator", init<ModelPart&>())
        .def("CalculateTotalVolume", &SphericElementGlobalPhysicsCalculator::CalculateTotalVolume)
        .def("CalculateTotalMass", &SphericElementGlobalPhysicsCalculator::CalculateTotalMass)
        .def("CalculateD50", &SphericElementGlobalPhysicsCalculator::CalculateD50)
        .def("CalculateCenterOfMass", &SphericElementGlobalPhysicsCalculator::CalculateCenterOfMass)
        .def("GetInitialCenterOfMass", &SphericElementGlobalPhysicsCalculator::GetInitialCenterOfMass)
        .def("CalculateKineticEnergy", &SphericElementGlobalPhysicsCalculator::CalculateKineticEnergy)
        .def("CalculateElasticEnergy", &SphericElementGlobalPhysicsCalculator::CalculateElasticEnergy)
        .def("CalculateGravitationalPotentialEnergy", &SphericElementGlobalPhysicsCalculator::CalculateGravitationalPotentialEnergy)
        .def("CalculateTotalMomentum", &SphericElementGlobalPhysicsCalculator::CalculateTotalMomentum)
        .def("CalulateTotalAngularMomentum", &SphericElementGlobalPhysicsCalculator::CalulateTotalAngularMomentum)
        ;

      class_<PreUtilities, boost::noncopyable >
        ("PreUtilities", init<ModelPart&>())
        .def("MeasureTopHeigh", &PreUtilities::MeasureTopHeight)        
        .def("MeasureBotHeigh", &PreUtilities::MeasureBotHeight) 
        ;

     class_<PostUtilities, boost::noncopyable >
        ("PostUtilities", init<>())
        .def("VelocityTrap", &PostUtilities::VelocityTrap)
        .def("QuasiStaticAdimensionalNumber", &PostUtilities::QuasiStaticAdimensionalNumber)     
        ;

  
    }
}  // namespace Python.

} // Namespace Kratos
