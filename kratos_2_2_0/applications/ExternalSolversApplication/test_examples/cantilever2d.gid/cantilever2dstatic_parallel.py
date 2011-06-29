##################################################################
##################################################################
#setting the domain size for the problem to be solved
domain_size = 2

##################################################################
##################################################################
## ATTENTION: here the order is important

#including kratos path
kratos_libs_path = '../../../../libs' ##kratos_root/libs
kratos_applications_path = '../../../../applications' ##kratos_root/applications
import sys
sys.path.append(kratos_libs_path)
sys.path.append(kratos_applications_path)

#importing Kratos main library
from Kratos import *
kernel = Kernel()   #defining kernel

#importing applications
import applications_interface
applications_interface.Import_StructuralApplication = True
applications_interface.ImportApplications(kernel, kratos_applications_path)
from KratosStructuralApplication import *

## from now on the order is not anymore crucial
##################################################################
##################################################################

#defining a model part
model_part = ModelPart("FluidPart");  

#adding of Variables to Model Part should be here 
import structural_solver_static_parallel
structural_solver_static_parallel.AddVariables(model_part)

#reading a model
gid_io = GidIO("cantilever2d",GiDPostMode.GiD_PostBinary)
gid_io.ReadModelPart(model_part)
gid_io.WriteMesh((model_part).GetMesh(),domain_size,GiDPostMode.GiD_PostBinary);
print model_part

#writing the mesh
#gid_io.WriteMesh(model_part.GetMesh(),domain_size,GiDPostMode.GiD_PostBinary);

#the buffer size should be set up here after the mesh is read for the first time
model_part.SetBufferSize(2)

#importing the solver files
structural_solver_static_parallel.AddDofs(model_part)

#creating a fluid solver object
solver = structural_solver_static_parallel.StaticStructuralSolver(model_part,domain_size)
##pDiagPrecond = ParallelDiagonalPreconditioner()
##solver.model_linear_solver =  ParallelCGSolver(1e-8, 5000,pDiagPrecond)


model_part.Properties[1].SetValue(CONSTITUTIVE_LAW, Isotropic2D() )
print "Linear elastic model selected"

solver.Initialize()
solver.SetEchoLevel(1)
solver.structure_linear_solver =  ParallelSkylineLUFactorizationSolver()

Dt = 0.02
nsteps = 10

for step in range(0,nsteps):
    print "line49"

    time = Dt*step
    model_part.CloneTimeStep(time)

    print time
    #print model_part.ProcessInfo()[TIME]

    #solving the fluid problem
    if(step > 3):
        solver.Solve()

        #print the results
        gid_io.WriteNodalResults(DISPLACEMENT,model_part.Nodes,time,0)
        gid_io.PrintOnGaussPoints(PK2_STRESS_TENSOR,model_part,time,domain_size)
    

          
        

