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
//   Last modified by:    $Author: rrossi $
//   Date:                $Date: 2009-01-22 17:13:57 $
//   Revision:            $Revision: 1.5 $
//
//


// System includes

// External includes
#include <boost/python.hpp>


// Project includes
#include "includes/define.h"
#include "processes/process.h"
#include "python/add_processes_to_python.h"
#include "processes/find_nodal_h_process.h"
#include "processes/find_nodal_neighbours_process.h"
#include "processes/find_conditions_neighbours_process.h"
#include "processes/find_elements_neighbours_process.h"
#include "processes/calculate_nodal_area_process.h"
#include "processes/node_erase_process.h"
#include "processes/element_erase_process.h"
#include "processes/condition_erase_process.h"
#include "processes/eliminate_isolated_nodes_process.h"
#include "processes/calculate_signed_distance_to_3d_skin_process.h"
#include "processes/calculate_signed_distance_to_3d_condition_skin_process.h" 
#include "processes/translation_operation.h"
#include "processes/rotation_operation.h"
#include "includes/node.h"


namespace Kratos
{

namespace Python
{
void  AddProcessesToPython()
{
    using namespace boost::python;
    class_<Process>("Process")
            .def("Execute",&Process::Execute)
            .def("ExecuteBeforeSolutionLoop",&Process::ExecuteBeforeSolutionLoop)
            .def("ExecuteInitializeSolutionStep",&Process::ExecuteInitializeSolutionStep)
            .def("ExecuteFinalizeSolutionStep",&Process::ExecuteFinalizeSolutionStep)
            .def("ExecuteBeforeOutputStep",&Process::ExecuteBeforeOutputStep)
            .def("ExecuteAfterOutputStep",&Process::ExecuteAfterOutputStep)
            .def("ExecuteFinalize",&Process::ExecuteFinalize)
            .def(self_ns::str(self))
    ;

    class_<FindNodalHProcess, bases<Process> >("FindNodalHProcess",init<ModelPart&>())
    .def("Execute",&FindNodalHProcess::Execute)
    ;
    class_<FindNodalNeighboursProcess, bases<Process> >("FindNodalNeighboursProcess",
            init<ModelPart&, int, int>())
    .def("ClearNeighbours",&FindNodalNeighboursProcess::ClearNeighbours)
    ;

    class_<FindConditionsNeighboursProcess, bases<Process> >("FindConditionsNeighboursProcess",
            init<ModelPart&, int, int>())
    .def("ClearNeighbours",&FindConditionsNeighboursProcess::ClearNeighbours)
    ;

    class_<FindElementalNeighboursProcess, bases<Process> >("FindElementalNeighboursProcess",
            init<ModelPart&, int, int>())
    .def("ClearNeighbours",&FindElementalNeighboursProcess::ClearNeighbours)
    ;

    class_<CalculateNodalAreaProcess, bases<Process> >("CalculateNodalAreaProcess",
            init<ModelPart&, unsigned int>())
    ;

    class_<NodeEraseProcess, bases<Process> >("NodeEraseProcess",
            init<ModelPart&>())
    ;
    
    class_<ElementEraseProcess, bases<Process> >("ElementEraseProcess",
            init<ModelPart&>())
    ;
    
    class_<ConditionEraseProcess, bases<Process> >("ConditionEraseProcess",
            init<ModelPart&>())
    ;

    class_<EliminateIsolatedNodesProcess, bases<Process> >("EliminateIsolatedNodesProcess",
            init<ModelPart&>())
    ;

    class_<CalculateSignedDistanceTo3DSkinProcess, bases<Process>, boost::noncopyable >("CalculateSignedDistanceTo3DSkinProcess",
            init<ModelPart&, ModelPart&>())
    .def("GenerateSkinModelPart",&CalculateSignedDistanceTo3DSkinProcess::GenerateSkinModelPart)
    .def("MappingPressureToStructure",&CalculateSignedDistanceTo3DSkinProcess::MappingPressureToStructure)
    ;

   class_<CalculateSignedDistanceTo3DConditionSkinProcess, bases<Process> >("CalculateSignedDistanceTo3DConditionSkinProcess",
            init<ModelPart&, ModelPart&>())
    ;
    
    class_<TranslationOperation, bases<Process> >("TranslationOperation",
            init<ModelPart&, boost::numeric::ublas::vector<int> ,boost::numeric::ublas::vector<int> ,unsigned int>())
    ;

    class_<RotationOperation, bases<Process> >("RotationOperation",
            init<ModelPart&, boost::numeric::ublas::vector<int> ,boost::numeric::ublas::vector<int> ,unsigned int>())
    ;

    //typedef PointerVectorSet<Node<3>, IndexedObject> NodesContainerType;
    //typedef PointerVectorSet<Dof<double>, IndexedObject> DofsContainerType;

    //class_<AddDofsNodalProcess<Variable<double> >, bases<Process> >("AddDoubleDofsNodalProcess")
    // .def(init<Variable<double>, NodesContainerType&, DofsContainerType&>())
    // ;
    //class_<AddDofsNodalProcess<VariableComponent<Kratos::VectorComponentAdaptor<Kratos::array_1d<double, 3> > > >, bases<Process> >("AddArrayComponentDofsNodalProcess")
    // ;
}

}  // namespace Python.

} // Namespace Kratos

