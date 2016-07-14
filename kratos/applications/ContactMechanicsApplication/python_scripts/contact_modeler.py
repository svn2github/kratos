from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7
#import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.PfemBaseApplication as KratosPfemBase

# Check that KratosMultiphysics was imported in the main script
KratosMultiphysics.CheckForPreviousImport()

# Import the mesh modeler (the base class for the modeler derivation)
import mesh_modeler

def CreateMeshModeler(main_model_part, meshing_parameters, mesh_id):
    return ContactModeler(main_model_part, meshing_parameters, mesh_id)

class ContactModeler(mesh_modeler.MeshModeler):
    
    #
    def __init__(self, main_model_part, meshing_parameters, mesh_id): 
        
        self.echo_level             = 1
        self.mesh_id                = mesh_id
        self.main_model_part        = main_model_part 
        self.MeshingParameters      = meshing_parameters

        self.imposed_walls          = []
        self.consider_imposed_walls = False      

        print("Construction of Contact Modeler finished")
  
    #
    def Initialize(self, domain_size):
        
        self.domain_size   =  domain_size

        # set mesh modeler
        if(self.domain_size == 2):
            self.mesher = KratosPfemBase.ContactDomain2DModeler()
        elif(self.domain_size == 3):
            self.mesher = KratosPfemBase.ContactDomain3DModeler()

        self.mesher.SetEchoLevel(self.echo_level)
        self.mesher.SetMeshingParameters(self.MeshingParameters,self.mesh_id)

        self.SetPreMeshingProcesses()
        self.SetPostMeshingProcesses()    

        self.mesher.Initialize()

    #
    def InitializeMeshing(self):

        meshing_options = self.MeshingParameters.GetOptions()
        
        # set execution flags: to set the options to be executed in methods and processes
        execution_options = KratosMultiphysics.Flags()

        execution_options.Set(KratosPfemBase.ModelerUtilities.INITIALIZE_MESHER_INPUT, True)
        execution_options.Set(KratosPfemBase.ModelerUtilities.FINALIZE_MESHER_INPUT, True)

        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_NODES_TO_MESHER, True)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_ELEMENTS_TO_MESHER, False)
        execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_NEIGHBOURS_TO_MESHER, False)

        if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
            execution_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER_KRATOS_FACES_TO_MESHER, True)
                              
        execution_options.Set(KratosPfemBase.ModelerUtilities.SELECT_TESSELLATION_ELEMENTS, True)
        execution_options.Set(KratosPfemBase.ModelerUtilities.KEEP_ISOLATED_NODES, False)


        self.MeshingParameters.SetExecutionOptions(execution_options)
        
        # set modeler flags: to set options for the mesher (triangle 2D, tetgen 3D)
        # RECONNECT
            
        modeler_flags = ""
        modeler_info  = "Reconnect a cloud of points"
        if( self.domain_size == 2 ):
           
            if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
                modeler_flags = "pBYYQ"  
            else:
                modeler_flags = "QNP"

            
        elif( self.domain_size == 3 ):

            if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
                modeler_flags = "pBJFMYYQ"
            else:
                modeler_flags = "JFMQO4/4"

        self.MeshingParameters.SetTessellationFlags(modeler_flags)
        self.MeshingParameters.SetTessellationInfo(modeler_info)

    #
    def SetPreMeshingProcesses(self):
        #nothing to do: only reconnection    

        ## in the mesh_utilities: set ids consecutives and calculate boundary normals

        #clear contacts

        pass
     

    #
    def SetPostMeshingProcesses(self):

        # The order set is the order of execution:
   
        # build contact conditions

        ## in the mesh_utilities: set ids not shared with elements to write in GiD
    #
    def FinalizeMeshing(self):
        
        # reset execution flags: to unset the options to be executed in methods and processes
        execution_options = KratosMultiphysics.Flags() 
        # all false
        self.MeshingParameters.SetExecutionOptions(execution_options)