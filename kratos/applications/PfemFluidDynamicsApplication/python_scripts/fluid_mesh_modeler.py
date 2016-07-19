from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7
#import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.PfemBaseApplication as KratosPfemBase
import KratosMultiphysics.PfemFluidDynamicsApplication as KratosPfemFluid

# Check that KratosMultiphysics was imported in the main script
KratosMultiphysics.CheckForPreviousImport()

import mesh_modeler

def CreateMeshModeler(main_model_part, meshing_parameters, mesh_id):
    return FluidMeshModeler(main_model_part, meshing_parameters, mesh_id)

class FluidMeshModeler(mesh_modeler.MeshModeler):
    
    #
    def __init__(self, main_model_part, meshing_parameters, mesh_id): 
        
        self.echo_level        = 1
        self.mesh_id           = mesh_id
        self.main_model_part   = main_model_part 
        self.MeshingParameters = meshing_parameters

    #
 
    def Initialize(self, domain_size):

        print("::[fluid_mesh_modeler]:: -START Initialize-")
 
        self.domain_size   =  domain_size

        # set mesh modeler
        if(self.domain_size == 2):
            self.mesher = KratosPfemBase.TriangularMesh2DModeler()
        elif(self.domain_size == 3):
            self.mesher = KratosPfemBase.TetrahedralMesh3DModeler()

        self.mesher.SetEchoLevel(self.echo_level)
        self.mesher.SetMeshingParameters(self.MeshingParameters,self.mesh_id)

        self.SetPreMeshingProcesses()
        self.SetPostMeshingProcesses()    

        self.mesher.Initialize()
    #
    def InitializeMeshing(self):

        self.MeshingParameters.InitializeMeshing()

        
        # set execution flags: to set the options to be executed in methods and processes
        execution_options = KratosMultiphysics.Flags()

        execution_options.Set(KratosPfemBase.ModelerUtilities.INITIALIZE_MESHER_INPUT, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.FINALIZE_MESHER_INPUT, False)

        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_NODES_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_ELEMENTS_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_NEIGHBOURS_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_FACES_TO_MESHER, False)

        execution_options.Set(KratosPfemBase.ModelerUtilities.SELECT_TESSELLATION_ELEMENTS, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.KEEP_ISOLATED_NODES, False)

        self.MeshingParameters.SetExecutionOptions(execution_options)
        
        # set modeler flags: to set options for the mesher (triangle 2D, tetgen 3D)
        if( self.domain_size == 2 ):
            pass
            #REFINE
            #ADD NODES
            #to add_nodes automatically and refine the mesh ("q"-quality mesh and "a"-area constraint switches)
            # "YYJaqrn" "YJq1.4arn" "Jq1.4arn"
            #refine
            #modeler_flags = "YJq1.4arnQ" 
            #refine constrained
            #modeler_flags = "pYJq1.4arnCQ"
            
            #INSERT NODES
            #to insert a set of given points and refine the mesh
            # "rinYYJQ" "rinYYJQ" "rinJQ" "rinQ"
            #refine
            #modeler_flags = "rinJQ" 
            #refine constrained
            #modeler_flags = "rinYYJQ"
            
            #refine without adding nodes
            #modeler_flags = "YJrnQ" 
            
            #RECONNECT
            #to reconnect a set of points only
            #modeler_flags = "nQP"
            #constrained
            #modeler_flags = "pnBYYQ"
            
            #BOUNDARY SEARCH
            #to get conectivities, boundaries and neighbours only
            #modeler_flags = "ncEBQ" 
            
        if( self.domain_size == 3 ):
            #other flags
            pass
            
    #
    def SetPreMeshingProcesses(self):
        
        # The order set is the order of execution:
        print("::[fluid_mesh_modeler]:: -START SetPreMeshingProcesses-")


        # process to refine elements /refine boundary
        refine_mesh_elements  = KratosPfemBase.SetElementsToRefineOnThreshold(self.main_model_part, self.RefiningParameters, self.mesh_id, self.echo_level)
        self.mesher.SetPreMeshingProcess(refine_mesh_elements)

        #refine_mesh_boundary = RefineMeshBoundary(self.main_model_part, self.RefiningParameters, self.mesh_id, self.echo_level)            
        #self.mesher.SetPreMeshingProcess(refine_mesh_boundary)
                
        #set imposed walls (rigid walls)
        rigid_walls_container = KratosPfemBase.BoundingBoxContainer()

        if( self.imposed_walls.RigidWallActive() ):
            rigid_wall_bbox = self.imposed_walls.RigidWallBoundingBoxes()
            for sizei in range(0, len(rigid_wall_bbox)):
                rigid_walls_container.PushBack( rigid_wall_bbox[sizei] )

        #refine_mesh_boundary
        refine_mesh_boundary = KratosPfemBase.ContactRefineMeshBoundary(self.main_model_part, rigid_walls_container, self.MeshingParameters, self.mesh_id, self.echo_level)
        self.mesher.SetPreMeshingProcess(refine_mesh_boundary)

        # process to remove nodes / remove boundary
        remove_mesh_nodes = KratosPfemBase.RemoveMeshNodes(self.main_model_part, self.MeshingParameters,  self.mesh_id, self.echo_level)
        self.mesher.SetPreMeshingProcess(remove_mesh_nodes)

        
    #
    def SetPostMeshingProcesses(self):

        # The order set is the order of execution:

        #select mesh elements
        generate_particles  = KratosPfemBase.GenerateNewNodes(self.main_model_part, self.MeshingParameters, self.mesh_id, self.echo_level)
        self.mesher.SetPostMeshingProcess(generate_particles)

        #select mesh elements
        #select_mesh_elements  = KratosPfemBase.SelectMeshElements(self.main_model_part, self.MeshingParameters, self.mesh_id, self.echo_level)
        #self.mesher.SetPostMeshingProcess(select_mesh_elements)
        select_mesh_elements  = KratosPfemFluid.SelectMeshElements(self.main_model_part, self.MeshingParameters, self.mesh_id, self.echo_level)
        self.mesher.SetPostMeshingProcess(select_mesh_elements)

        #rebuild elements
        rebuild_mesh_elements = KratosPfemBase.BuildMeshElements(self.main_model_part, self.MeshingParameters, self.mesh_id, self.echo_level)
        self.mesher.SetPostMeshingProcess(rebuild_mesh_elements)

        #rebuild boundary
        rebuild_mesh_boundary = KratosPfemBase.ReconstructMeshBoundary(self.main_model_part, self.MeshingParameters, self.mesh_id, self.echo_level)
        self.mesher.SetPostMeshingProcess(rebuild_mesh_boundary)

    #
    def FinalizeMeshing(self):
        
        # reset execution flags: to unset the options to be executed in methods and processes
        execution_options = KratosMultiphysics.Flags()
        
        # all flags
        execution_options.Set(KratosPfemBase.ModelerUtilities.INITIALIZE_MESHER_INPUT, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.FINALIZE_MESHER_INPUT, False)

        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_NODES_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_ELEMENTS_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_NEIGHBOURS_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_FACES_TO_MESHER, False)

        execution_options.Set(KratosPfemBase.ModelerUtilities.SELECT_TESSELLATION_ELEMENTS, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.KEEP_ISOLATED_NODES, False)

        self.MeshingParameters.SetExecutionOptions(execution_options)

        self.MeshingParameters.FinalizeMeshing()

    #
