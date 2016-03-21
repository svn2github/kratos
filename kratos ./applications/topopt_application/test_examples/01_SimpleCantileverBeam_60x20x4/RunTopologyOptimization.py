from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
# Activate it to import in the gdb path:
# import sys
# sys.path.append('/home/jmaria/kratos')
# x = raw_input("stopped to allow debug: set breakpoints and press enter to continue");

#
# ***************GENERAL MAIN OF THE ANALISYS****************###
#

# time control starts
from time import *
print(ctime())
# measure process time
t0p = clock()
# measure wall time
# t0w = time()

# ----------------------------------------------------------------#
# --CONFIGURATIONS START--####################
# Import the general variables read from the GiD
import ProjectParameters as general_variables

# setting the domain size for the problem to be solved
domain_size = general_variables.domain_size

# including kratos path
from KratosMultiphysics import *

# including Applications paths
from KratosMultiphysics.ExternalSolversApplication import *
from KratosMultiphysics.TopOptApplication import *

# import the python utilities:
import restart_utility as restart_utils
import gid_output_utility as gid_utils

import conditions_python_utility as condition_utils
import list_files_python_utility as files_utils

import time_operation_utility as operation_utils


# ------------------------#--FUNCTIONS START--#------------------#
# ---------------------------------------------------------------#
# --TIME MONITORING START--##################
def StartTimeMeasuring():
    # measure process time
    time_ip = clock()
    return time_ip


def StopTimeMeasuring(time_ip, process):
    # measure process time
    time_fp = clock()
    print(process, " [ spent time = ", time_fp - time_ip, "] ")
# --TIME MONITORING END --###################

# --SET NUMBER OF THREADS --#################


def SetParallelSize(num_threads):
    parallel = OpenMPUtils()
    print("Num Threads = ", num_threads)
    parallel.SetNumThreads(int(num_threads))
# --SET NUMBER OF THREADS --#################

# ------------------------#--FUNCTIONS END--#--------------------#
# ---------------------------------------------------------------#

# defining the number of threads:
num_threads = general_variables.NumberofThreads
SetParallelSize(num_threads)

# defining the type, the name and the path of the problem:
problem_type = general_variables.ProblemType
problem_name = general_variables.problem_name
problem_path = general_variables.problem_path

# defining a model part
model_part = ModelPart("SolidDomain")

# defining the model size to scale
length_scale = 1.0

# --DEFINE MAIN SOLVER START--################

SolverSettings = general_variables.SolverSettings

# import solver file
solver_constructor = __import__(SolverSettings.solver_type)

# construct the solver
main_step_solver = solver_constructor.CreateSolver(model_part, SolverSettings)

# --DEFINE MAIN SOLVER END--##################

# --READ AND SET MODEL FILES--###############

# set the restart of the problem
restart_step = general_variables.Restart_Step
problem_restart = restart_utils.RestartUtility(model_part, problem_path, problem_name)

# set the results file list of the problem (managed by the problem_restart and gid_print)
print_lists = general_variables.PrintLists
output_mode = general_variables.GidOutputConfiguration.GiDPostMode
list_files = files_utils.ListFilesUtility(problem_path, problem_name, print_lists, output_mode)
list_files.Initialize(general_variables.file_list)

# --READ AND SET MODEL FILES END--############

# --DEFINE CONDITIONS START--#################
incr_disp = general_variables.Incremental_Displacement
incr_load = general_variables.Incremental_Load
rotation_dofs = SolverSettings.RotationDofs
conditions = condition_utils.ConditionsUtility(model_part, domain_size, incr_disp, incr_load, rotation_dofs)

# --DEFINE CONDITIONS END--###################


# --GID OUTPUT OPTIONS START--###############
# set gid print options
gid_print = gid_utils.GidOutputUtility(problem_name, general_variables.GidOutputConfiguration)

# --GID OUTPUT OPTIONS END--##################


# --CONFIGURATIONS END--######################
# ----------------------------------------------------------------#


# --START SOLUTION--######################
#
# initialize problem : load restart or initial start
load_restart = general_variables.LoadRestart
save_restart = general_variables.SaveRestart

# set buffer size
buffer_size = 3

# define problem variables:
solver_constructor.AddVariables(model_part, SolverSettings)


# --- READ MODEL ------#
if(load_restart == False):

    # remove results, restart, graph and list previous files
    problem_restart.CleanPreviousFiles()
    list_files.RemoveListFiles()

    # reading the model
    name = general_variables.problem_name
    gid_mode = GiDPostMode.GiD_PostBinary
    multifile = MultiFileFlag.MultipleFiles
    deformed_mesh_flag = WriteDeformedMeshFlag.WriteUndeformed
    write_conditions = WriteConditionsFlag.WriteElementsOnly
    gid_io = GidIO(name, gid_mode, multifile, deformed_mesh_flag, write_conditions)
    
    model_part_io = ModelPartIO(problem_name)
    model_part_io.ReadModelPart(model_part)

    # set the buffer size
    model_part.SetBufferSize(buffer_size)
    # Note: the buffer size should be set once the mesh is read for the first time

    # set the degrees of freedom
    solver_constructor.AddDofs(model_part, SolverSettings)

    # set the constitutive law
    import constitutive_law_python_utility as constitutive_law_utils

    constitutive_law = constitutive_law_utils.ConstitutiveLawUtility(model_part, domain_size);
    constitutive_law.Initialize();
    
else:

    # reading the model from the restart file
    problem_restart.Load(restart_step);

    # remove results, restart, graph and list posterior files
    problem_restart.CleanPosteriorFiles(restart_step)
    list_files.ReBuildListFiles()

# set mesh searches and modeler
# modeler.InitializeDomains();

# if(load_restart == False):
    # find nodal h
    # modeler.SearchNodalH();


# --- PRINT CONTROL --- #
print(model_part)
print(model_part.Properties[1])

# --INITIALIZE--###########################
#

# set delta time in process info
model_part.ProcessInfo[DELTA_TIME] = general_variables.time_step

# solver initialize
main_step_solver.Initialize()
main_step_solver.SetRestart(load_restart) #calls strategy initialize if no restart

# initial contact search
# modeler.InitialContactSearch()

#define time steps and loop range of steps
time_step = model_part.ProcessInfo[DELTA_TIME]

# define time steps and loop range of steps
if(load_restart):

    buffer_size = 0

else:

    model_part.ProcessInfo[TIME]                = 0
    model_part.ProcessInfo[TIME_STEPS]          = 0
    model_part.ProcessInfo[PREVIOUS_DELTA_TIME] = time_step;

    conditions.Initialize(time_step);


# initialize step operations
starting_step  = model_part.ProcessInfo[TIME_STEPS]
starting_time  = model_part.ProcessInfo[TIME]
#ending_step    = general_variables.nsteps
ending_time    = general_variables.end_time


output_print = operation_utils.TimeOperationUtility()
gid_time_frequency = general_variables.GiDWriteFrequency
output_print.InitializeTime(starting_time, ending_time, time_step, gid_time_frequency)

restart_print = operation_utils.TimeOperationUtility()
restart_time_frequency = general_variables.RestartFrequency
restart_print.InitializeTime(starting_time, ending_time, time_step, restart_time_frequency)


# --TIME INTEGRATION--#######################
#

# writing a single file
gid_print.initialize_results(model_part)

#initialize time integration variables
current_time = starting_time
current_step = starting_step

# filling the buffer
for step in range(0,buffer_size):

  model_part.CloneTimeStep(current_time)
  model_part.ProcessInfo[DELTA_TIME] = time_step
  model_part.ProcessInfo[TIME_STEPS] = step-buffer_size


# This is just to set up the data tree and to get ELEM_CENTER_X, ELEM_CENTER_Y, ELEM_CENTER_Z...
# Eventually this should be where the data tree for the filtering should be created, so that it isn't created on each iteration.
main_step_solver.FilterSensitivities()


#-----------------------------------------------------------------------------------
#------------------------TOPOLOGY OPTIMIZATION SECTION------------------------------
#-----------------------------------------------------------------------------------

# --USER SETTINGS--#####################

# Exit Conditions
tolerance =  0.01 	# termination criterion
maxloop = 100		# Max number of iterations
obj_fcn_exit	= 0	# Criterion to exit once OBJ_FCN stops decreasing: Activated=1, Deactivated=0

# User Defined Values
volfrac 	= 0.3	# Volume Fraction
penalty 	= 3	# Penalty Factor - Recomended: 3
searchradius 	= 1.5	# Filer Radius Size - Good value includes at least 18 neighboring elements
greyscale 	= 0	# Greyscale Filter: Activated=1, Deactivated=0
qmax 		= 2	# If Greyscale Filter is activated, set maximum q value - Recomended: 2
continuation 	= 0	# Continutation Strategy: Activated=1, Deactivated=0
youngsmodulus	= 100  	# Elastic modulus. To get value from model, use: model_part.Properties[1].GetValue(YOUNG_MODULUS)
Emin	= 0.000000001	# Elastic modulus of void material

#--Eof USER SETTINGS--##################


print("")
print("*********************************************************")
print("LOOP EXIT CONDITIONS:")
print("")
print("Tolerance is set at:", tolerance*100,"%")
print("MaxLoop is set at:", maxloop)
if (obj_fcn_exit == 1 and continuation==0):
  print("Loop exits when Obj_Fcn stops decreasing")
elif (obj_fcn_exit == 1 and continuation==1):
  print("Loop does not exit when Obj_Fcn stops decreasing since Continuation Strategy is activated")
elif (obj_fcn_exit == 0):
  print("Loop does not exit when Obj_Fcn stops decreasing")
print("*********************************************************")
print("")

print("*********************************************************")
print("USER DEFINED VALUES:")
print("")
print("Volume Fraction:", volfrac)
print("Penalty Factor:", penalty)
print("Filter Radius:", searchradius)
print("Young's Modulus:", youngsmodulus)
if greyscale == 1:
  print("Grey Scale Filter Activated")
elif greyscale == 0:
  print("Grey Scale Filter Deactivated")
if continuation == 1:
  print("Continuation Strategy Activated")
elif continuation == 0:
  print("Continuation Strategy Deactivated")
print("*********************************************************")
print("")


# Set Elemental TOPOPT values
for element_i in model_part.Elements:
  element_i.SetValue(E_MIN, Emin)
  element_i.SetValue(PENAL, penalty)
  element_i.SetValue(X_PHYS, volfrac)
  element_i.SetValue(YOUNG_MODULUS, youngsmodulus)
  element_i.SetValue(SEARCHRADIUS, searchradius)
  
  # Set Active or Passive elements:  0=normal, 1=solid(active), 2=void(passive)
  # EXAMPLE: This sets all elements with a center y-coordinate between 19 and 20 to have a density of 0, which is a passive element. 
  #center_coord_x  = element_i.GetValue(ELEM_CENTER_X)
  #center_coord_y  = element_i.GetValue(ELEM_CENTER_Y)
  #center_coord_z  = element_i.GetValue(ELEM_CENTER_Z)
  #if center_coord_y < 20 and center_coord_y > 19:
    #element_i.SetValue(SOLID_VOID, 2)

# Initialize Variables
change 	= 1
loop 	= 0
q 	= 1
pmax 	= penalty 	# Maximum penalty value used for continuation strategy
Objective_Function = 0
Objective_Function_old = 0
Obj_Fcn_change = 1
Obj_Fcn_tolerance = 0

# Write initial state results file
current_id = 0
gid_io.PrintOnGaussPoints(X_PHYS, model_part, current_time)
gid_print.write_results(model_part, general_variables.nodal_results, general_variables.gauss_points_results, current_time, current_step, current_id)
list_files.PrintListFiles(current_id);

file = open("ObjectiveFunctionData.txt", "w")

## --- BEGIN ITERATIONS --- ######################################

while(change > tolerance and loop < maxloop and Obj_Fcn_change>Obj_Fcn_tolerance):
  
  print("################################################################################")
  print("######################### Optimization Loop ", loop, "Results #########################")
  print("################################################################################")
  loop = loop + 1
  
  iteration_clock_time = StartTimeMeasuring();

  # store previous time step
  model_part.ProcessInfo[PREVIOUS_DELTA_TIME] = time_step
  # set new time step ( it can change when solve is called )
  time_step = model_part.ProcessInfo[DELTA_TIME]

  current_time = current_time + time_step
  current_step = current_step + 1
  model_part.CloneTimeStep(current_time)
  model_part.ProcessInfo[TIME] = current_time
  
  # SET X_PHYS_OLD
  for element_i in model_part.Elements:
    element_i.SetValue(X_PHYS_OLD, element_i.GetValue(X_PHYS))
    # CONTINUATION STRATEGY (TO AVOID LOCAL MIN)
    if continuation == 1:
        if loop < 20:
            element_i.SetValue(PENAL, 1)
        else:
            element_i.SetValue(PENAL, min(pmax,1.02*element_i.GetValue(PENAL)))

  # SOLVER
  # solve time step non-linear system
  solver_clock_time = StartTimeMeasuring();
  print("--------> Starting Main Solver...")
  main_step_solver.Solve()
  StopTimeMeasuring(solver_clock_time, "solver_clock_time:");

  # incremental load
  conditions.SetIncrementalLoad(current_step, time_step);
        
  conditions.RestartImposedDisp()
    
  # Sensitivity Analysis ###
  ComputeSensitivities_clock_time = StartTimeMeasuring();
  print("--------> Starting Sensitivity Computations...")
  main_step_solver.ComputeSensitivities()
  StopTimeMeasuring(ComputeSensitivities_clock_time, "ComputeSensitivities_clock_time:");
  
  # Sensitivity filtering
  Filter_clock_time = StartTimeMeasuring();
  print("--------> Starting Filter Process...")
  main_step_solver.FilterSensitivities()
  StopTimeMeasuring(Filter_clock_time, "Filter_clock_time:");
  
  # Compute Objective Function
  Objective_Function = 0
  for element_i in model_part.Elements:
    Objective_Function = Objective_Function + element_i.GetValue(OBJ_FCN)
  
  # Design variable (Density) Update ###
  UpdateDensities_clock_time = StartTimeMeasuring();
  print("--------> Starting Density Update...")
  
  if greyscale == 1:   # Grey Scale Filter
    if loop < 15:
      q=1
    else:
      q=min(qmax,1.01*q)

  main_step_solver.UpdateDensities(volfrac,q)

  print("OBJ FCN = ", Objective_Function)
  
  # Write OBJ_FCN data to file
  file.write(str(Objective_Function) + "\n")
        
  # Calculate % change of OBJ_FCN
  if loop > 1:
    Obj_Fcn_change = -(Objective_Function-Objective_Function_old)/Objective_Function_old
    print("OBJ FCN CHANGE = ", Obj_Fcn_change*100,"%")
    if obj_fcn_exit==0:
      Obj_Fcn_change=1
    if continuation==1: 
      Obj_Fcn_change=1
  Objective_Function_old=Objective_Function
  
  StopTimeMeasuring(UpdateDensities_clock_time, "UpdateDensities_clock_time:");
  
  # print the results at the end of the step
  execute_write = output_print.perform_time_operation(current_time)
  if(execute_write):
      clock_time = StartTimeMeasuring();
      gid_io.PrintOnGaussPoints(X_PHYS, model_part, current_time) #write X_PHYS to gausspoints
      current_id = output_print.operation_id()
      # print gid output file
      gid_print.write_results(model_part, general_variables.nodal_results, general_variables.gauss_points_results, current_time, current_step, current_id)
      # print on list files
      list_files.PrintListFiles(current_id);
      StopTimeMeasuring(clock_time, "Write Results Time:");

  # print restart file
  if(save_restart):
      execute_save = restart_print.perform_time_operation(current_time)
      if(execute_save):
          clock_time = StartTimeMeasuring();
          current_id = output_print.operation_id()
          problem_restart.Save(current_time, current_step, current_id);
          StopTimeMeasuring(clock_time, "Write Restart Time:");
  
  StopTimeMeasuring(iteration_clock_time, "Total Iteration Solve Time:");

# Eof Optimization Loop ######################################################################
  

# --FINALIZE--###

# writing a single file
gid_print.finalize_results()
file.close()

print("################################################################################")
print("############################## Analysis Finalized ##############################")
print("################################################################################")

# measure process time
tfp = clock()
print(ctime())
print("Analysis Completed  [Process Time = ", tfp - t0p, "] ")

# --END--###############################