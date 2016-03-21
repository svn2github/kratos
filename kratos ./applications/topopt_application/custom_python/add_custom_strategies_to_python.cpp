/*
==============================================================================
KratosTestApplication 
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi, Janosch Stascheit, Felix Nagel 
pooyan@cimne.upc.edu 
rrossi@cimne.upc.edu
janosch.stascheit@rub.de
nagel@sd.rub.de
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain
- Ruhr-University Bochum, Institute for Structural Mechanics, Germany


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
//   Last modified by:    $Author:  $
//   Date:                $Date:  $
//   Revision:            $Revision: 1.2 $
//
//


// System includes 


// External includes 
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/timer.hpp> 


// Project includes
#include "includes/define.h"
#include "containers/flags.h"

#include "custom_python/add_custom_strategies_to_python.h"

#include "spaces/ublas_space.h"

//strategies
#include "solving_strategies/strategies/solving_strategy.h"
#include "custom_strategies/residual_based_newton_raphson_strategy.hpp"
#include "custom_strategies/adjoint_structure_strategy.h"

//builders and solvers
#include "custom_strategies/residual_based_builder_and_solver.hpp"

//convergence criteria
#include "solving_strategies/convergencecriterias/convergence_criteria.h"

//schemes
#include "custom_strategies/custom_schemes/residual_based_static_scheme.hpp"

//linear solvers
#include "linear_solvers/linear_solver.h"

// Adjoint strategies
#include "custom_strategies/adjoint_structure_strategy.h"



namespace Kratos
{

	namespace Python
	{		
		using namespace boost::python;

		void  AddCustomStrategiesToPython()
		{
			typedef UblasSpace<double, CompressedMatrix, Vector> SparseSpaceType;
			typedef UblasSpace<double, Matrix, Vector> LocalSpaceType;

            // base types
            typedef LinearSolver<SparseSpaceType, LocalSpaceType > LinearSolverType;
            typedef SolvingStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType > BaseSolvingStrategyType;
            typedef BuilderAndSolver< SparseSpaceType, LocalSpaceType, LinearSolverType > BuilderAndSolverType;
            typedef Scheme< SparseSpaceType, LocalSpaceType > BaseSchemeType;
            typedef ConvergenceCriteria< SparseSpaceType, LocalSpaceType > ConvergenceCriteriaType;

            //custom strategy types
            typedef ResidualBasedNewtonRaphsonStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType > ResidualBasedNewtonRaphsonStrategyType;

            //custom builder_and_solver types
            typedef ResidualBasedBuilderAndSolver< SparseSpaceType, LocalSpaceType, LinearSolverType > ResidualBasedBuilderAndSolverType;

            //custom scheme types
            typedef ResidualBasedStaticScheme< SparseSpaceType, LocalSpaceType > ResidualBasedStaticSchemeType;

            //********************************************************************
            //*************************BUILDER AND SOLVER*************************
            //********************************************************************

            // Residual Based Builder and Solver
            class_< ResidualBasedBuilderAndSolverType, bases<BuilderAndSolverType>, boost::noncopyable >
                    (
                "ResidualBasedBuilderAndSolver", init< LinearSolverType::Pointer > ()
                    );

            //********************************************************************
            //*************************SHCHEME CLASSES****************************
            //********************************************************************

            // Static Scheme Type
            class_< ResidualBasedStaticSchemeType,
              bases< BaseSchemeType >, boost::noncopyable >
                    (
                   "ResidualBasedStaticScheme", init< >() )

                    .def("Initialize", &ResidualBasedStaticScheme<SparseSpaceType, LocalSpaceType>::Initialize)
                    ;

            //********************************************************************
            //*************************STRATEGY CLASSES***************************
            //********************************************************************

            // Residual Based Newton-Raphson Strategy
            class_< ResidualBasedNewtonRaphsonStrategyType,
                bases< BaseSolvingStrategyType >, boost::noncopyable >
              (
               "ResidualBasedNewtonRaphsonStrategy",
               init < ModelPart&, BaseSchemeType::Pointer, LinearSolverType::Pointer, ConvergenceCriteriaType::Pointer, int, bool, bool, bool >())

              .def(init < ModelPart&, BaseSchemeType::Pointer, LinearSolverType::Pointer, ConvergenceCriteriaType::Pointer, BuilderAndSolverType::Pointer, int, bool, bool, bool >())
              .def("SetMaxIterationNumber", &ResidualBasedNewtonRaphsonStrategyType::SetMaxIterationNumber)
              .def("GetMaxIterationNumber", &ResidualBasedNewtonRaphsonStrategyType::GetMaxIterationNumber)
              .def("SetInitializePerformedFlag", &ResidualBasedNewtonRaphsonStrategyType::SetInitializePerformedFlag)
              .def("GetInitializePerformedFlag", &ResidualBasedNewtonRaphsonStrategyType::GetInitializePerformedFlag)
              .def("SetKeepSystemConstantDuringIterations", &ResidualBasedNewtonRaphsonStrategyType::SetKeepSystemConstantDuringIterations)
              .def("GetKeepSystemConstantDuringIterations", &ResidualBasedNewtonRaphsonStrategyType::GetKeepSystemConstantDuringIterations)
              ;

            //********************************************************************
            //*************************ADJOINT STRATEGY CLASSES*******************
            //********************************************************************
            class_< AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >,
                    bases< BaseSolvingStrategyType >,  boost::noncopyable >
                    ("AdjointStructureStrategy",
                     init<ModelPart&, LinearSolverType::Pointer, int>() )
                    .def("ComputeSensitivities",&AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >::ComputeSensitivities)
                    .def("FilterSensitivities",&AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >::FilterSensitivities)
                    .def("PrepareFilter",&AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >::PrepareFilter)
                    .def("UpdateDensities",&AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >::UpdateDensities)
                    ;

//            class_< AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >,
//                    bases< BaseSolvingStrategyType >,  boost::noncopyable >
//                    ("AdjointStructureStrategy",
//                     init<ModelPart&, double, double >() )
//                     .def("UpdateDensities",&AdjointStructureStrategy< SparseSpaceType, LocalSpaceType, LinearSolverType >::UpdateDensities)
//                    ;


		}

	}  // namespace Python.

} // Namespace Kratos

