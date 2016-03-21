domain_size = 3

#Problem Data
#################################################

ProblemType = "Mechanical"
NumberofThreads = 1
Solution_method = "Newton-Raphson"
SolverType = "StaticSolver"
time_step = 1
nsteps    = 1
end_time    = 1

#Solver Data
#################################################

class SolverSettings:
    solver_type = "mechanical_TopOpt_solver"
    domain_size = 3
    echo_level  = 0 

    max_delta_time  = time_step
    time_integration_method = "Implicit"
    explicit_integration_scheme = "CentralDifferences"
    time_step_prediction_level  = 0

    RotationDofs = False
    PressureDofs = False
    ReformDofSetAtEachStep = False
    LineSearch = False
    Implex = False
    ComputeReactions = False
    ComputeContactForces = False
    scheme_type = "StaticSolver"
    convergence_criterion = "Residual_criteria"
    displacement_relative_tolerance = 1.0E-4
    displacement_absolute_tolerance = 1.0E-9
    residual_relative_tolerance = 1.0E-4
    residual_absolute_tolerance = 1.0E-9
    max_iteration = 10
    
    class linear_solver_config:
      solver_type = "AMGCL"
      scaling = True
      max_iteration = 200
      tolerance = 1e-5
      smoother_type = "ILU0"  #other options are "DAMPED_JACOBI" and "SPAI0"
      krylov_type = "CG" #other options are "BICGSTAB" and "CG"
      coarsening_type = "AGGREGATION"
      verbosity = 1 #OPTIONAL! set to 1 to have some output
      provide_coordinates = True
      
    #class linear_solver_config:
        #solver_type = "Conjugate gradient"
        #tolerance = 1E-5
        #max_iteration = 5000
        #scaling = False
        #verbosity = 0
        ##Pastix Iterative Solver:
        #gmres_krylov_space_dimension = 100
        #ilu_level_of_fill            = 3
        ##GMRES or CG:
        #preconditioner_type          = "None"
        ##Deflated CG:
        #assume_constant_structure    = True
        #max_reduced_size             = 1000
        ##AMG: (requires block_builder)
        #smoother_type  = "ILU0" #"DAMPED_JACOBI"
        #krylov_type    = "GMRES"
      
    #class linear_solver_config:
        #solver_type = "BiConjugate gradient stabilized"
        #tolerance = 1E-5
        #max_iteration = 1000
        #scaling = False
        #verbosity = 0
        ##Pastix Iterative Solver:
        #gmres_krylov_space_dimension = 100
        #ilu_level_of_fill            = 3
        ##GMRES or CG:
        #preconditioner_type          = "None"
        ##Deflated CG:
        #assume_constant_structure    = True
        #max_reduced_size             = 1000
        ##AMG: (requires block_builder)
        #smoother_type  = "ILU0" #"DAMPED_JACOBI"
        #krylov_type    = "GMRES"


#Constraints Data
#################################################

Incremental_Load = "False"
Incremental_Displacement = "False"

#PostProcess Data
#################################################

nodal_results=["DISPLACEMENT","REACTION"]
gauss_points_results=["X_PHYS"]

# GiD output configuration
class GidOutputConfiguration:
    GiDPostMode = "Binary"
    GiDWriteMeshFlag = False
    GiDWriteConditionsFlag = False
    GiDWriteParticlesFlag = False
    GiDMultiFileFlag = "Single"

GiDWriteFrequency = 0.01
WriteResults = "PreMeshing"
echo_level = 1

# graph_options
PlotGraphs = "False"
PlotFrequency = 0 

# list options
PrintLists = "True"
file_list = [] 

# restart options
SaveRestart = False
RestartFrequency = 0
LoadRestart = False
Restart_Step = 0

problem_name="TopOptStructuralAnalysis" # this is the mdpa file
problem_path="/home/ericg/test/TEMPLATE.gid"
