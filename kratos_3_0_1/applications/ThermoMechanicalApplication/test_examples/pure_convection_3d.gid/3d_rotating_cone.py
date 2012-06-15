##################################################################
##################################################################
#setting the domain size for the problem to be solved
domain_size = 2

##################################################################
##################################################################
## ATTENTION: here the order is important

#including kratos path
kratos_path = '../../../..'
#kratos_benchmarking_path = '../../../../benchmarking' ##kratos_root/benchmarking

import sys
sys.path.append(kratos_path)
sys.path.append(kratos_benchmarking_path)
#import benchmarking

##################################################################
##################################################################
 # importing kratos
from KratosMultiphysics import *
from KratosMultiphysics.ThermoMechanicalApplication import *
from KratosMultiphysics.IncompressibleFluidApplication import *


#defining a model part
model_part = ModelPart("FluidPart");  


## from now on the order is not anymore crucial
##################################################################
##################################################################

from KratosThermoMechanicalApplication import *
##########################################################
thermal_settings = ConvectionDiffusionSettings()
thermal_settings.SetDensityVariable(DENSITY)
thermal_settings.SetDiffusionVariable(CONDUCTIVITY)
thermal_settings.SetUnknownVariable(TEMPERATURE)
thermal_settings.SetVolumeSourceVariable(HEAT_FLUX)
thermal_settings.SetSurfaceSourceVariable(FACE_HEAT_FLUX)
thermal_settings.SetMeshVelocityVariable(MESH_VELOCITY)
thermal_settings.SetConvectionVariable(VELOCITY)
##########################################################

#importing the solver files
import thermo_monolithic_solver_eulerian_3d
thermo_monolithic_solver_eulerian_3d.AddVariables(model_part,thermal_settings)




#adding of Variables to Model Part should be here when the "very fix container will be ready"
#reading a model
gid_mode = GiDPostMode.GiD_PostBinary
multifile = MultiFileFlag.MultipleFiles
deformed_mesh_flag = WriteDeformedMeshFlag.WriteUndeformed
write_conditions = WriteConditionsFlag.WriteConditions
gid_io = GidIO("pure_convection_3d",gid_mode,multifile,deformed_mesh_flag, write_conditions)
gid_io.ReadModelPart(model_part)

mesh_name = 0.0
gid_io.InitializeMesh( mesh_name );
gid_io.WriteMesh((model_part).GetMesh());
gid_io.FinalizeMesh()
print model_part


#the buffer size should be set up here after the mesh is read for the first time
model_part.SetBufferSize(3)

#importing the solver files
thermo_monolithic_solver_eulerian_3d.AddDofs(model_part,thermal_settings)
print "111111111111111111111111111111111111111111"

#creating a fluid solver object
solver = thermo_monolithic_solver_eulerian_3d.MonolithicSolver(model_part,domain_size,thermal_settings)
#pDiagPrecond = DiagonalPreconditioner()
#solver.linear_solver =  BICGSTABSolver(1e-6, 5000,pDiagPrecond)
solver.Initialize()

#assigning the fluid properties
#conductivity = 0.0;
density = 1.0;
specific_heat = 100.0;
for node in model_part.Nodes:
    #node.SetSolutionStepValue(CONDUCTIVITY,0,conductivity);
    node.SetSolutionStepValue(DENSITY,0,density);
    node.SetSolutionStepValue(SPECIFIC_HEAT,0,specific_heat);

#assigning a rotational velocity field
vel = Vector(3);
for node in model_part.Nodes:
      vel[0] = 0.0
      vel[1] = -node.Z
      vel[2] = node.Y
  #else:
      #vel[0] = 0.0
      #vel[1] = 0.0
      #vel[2] = 0.0    
##   if(node.X**2 + node.Y**2 > 0.24):
##        vel[0] = 0.0
##        vel[1] = 0.0        
      node.SetSolutionStepValue(thermal_settings.GetConvectionVariable(),0,vel);

#assigning a cone shaped temperature distribution
xc = 0.0
yc = 0.0
zc = 1.00/6.00
sigma = 0.2
import math
for node in model_part.Nodes:
    X1 = (node.X - xc) / sigma
    X2 = (node.Y - yc) / sigma
    X3 = (node.Z - zc) / sigma    
    if( (X1**2 + X2**2 + X3**2) <= 1.00  ):
        temp = 0.25*(1.00+math.cos(math.pi*X1) )*(1.00+math.cos(math.pi*X2) )*(1.00+math.cos(math.pi*X3) )
        node.SetSolutionStepValue(TEMPERATURE,0,temp)
    else:
        node.SetSolutionStepValue(TEMPERATURE,0,0.0)
    if(node.Z == 0.5 or node.Z == -0.5 or node.Y== 0.5 or node.Y == -0.5):
        node.SetSolutionStepValue(TEMPERATURE,0,0.0)
        node.Fix(TEMPERATURE)
 
print "222222222222222222222222222222222222222222222222222222"

#settings to be changed
nsteps = 100
output_step = 1

Dt = 2.00*math.pi/200.0;

out = 0


for step in range(0,nsteps):
    print "line49"
    time = Dt*step
    model_part.CloneTimeStep(time)

    print time
    #print model_part.ProcessInfo()[TIME]    #solving the fluid problem
    if(step > 3):
        solver.Solve()

    #print the results
    if(out == output_step):
        gid_io.WriteNodalResults(TEMPERATURE,model_part.Nodes,time,0)
        gid_io.WriteNodalResults(thermal_settings.GetConvectionVariable(),model_part.Nodes,time,0)
        gid_io.WriteNodalResults(NODAL_AREA,model_part.Nodes,time,0)
        #gid_io.WriteNodalResults(SPECIFIC_HEAT,model_part.Nodes,time,0) 
        #gid_io.WriteNodalResults(DENSITY,model_part.Nodes,time,0) 
        #gid_io.WriteNodalResults(CONDUCTIVITY,model_part.Nodes,time,0)      
        gid_io.WriteNodalResults(MESH_VELOCITY,model_part.Nodes,time,0)   
        gid_io.WriteNodalResults(NORMAL,model_part.Nodes,time,0)           
        out = 0
    out = out + 1

          
        

