from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7

#import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.PfemBaseApplication as KratosPfemBase

# Check that KratosMultiphysics was imported in the main script
KratosMultiphysics.CheckForPreviousImport()

def CreateMeshingStrategy(main_model_part, custom_settings):
    return MeshingStrategy(main_model_part, custom_settings)

class MeshingStrategy:

    #
    def __init__(self, main_model_part, custom_settings):

        self.main_model_part = main_model_part    
        
        ##settings string in json format
        default_settings = KratosMultiphysics.Parameters("""
        {
             "strategy_type": "meshing_strategy",
             "remesh": false,
             "refine": false,
             "reconnect": false,
             "transfer": false,
             "constrained": false,
             "mesh_smoothing": false,
             "variables_smoothing": false,
             "elemental_variables_to_smooth":[ "DETERMINANT_F" ],
             "reference_element": "Element2D3N",
             "reference_condition": "CompositeCondition2D3N"
        }
        """)
        
        ##overwrite the default settings with user-provided parameters
        self.settings = custom_settings
        self.settings.ValidateAndAssignDefaults(default_settings)
                
        print("Construction of Mesh Modeler finished")
        
    #
    def Initialize(self,meshing_parameters,imposed_walls,domain_size,mesh_id):
        
        #parameters
        self.mesh_id = mesh_id

        self.echo_level = 1
        
        #meshing parameters
        self.MeshingParameters = meshing_parameters  
      
        meshing_options = KratosMultiphysics.Flags()
        
        meshing_options.Set(KratosPfemBase.ModelerUtilities.REMESH, self.settings["remesh"].GetBool())
        meshing_options.Set(KratosPfemBase.ModelerUtilities.REFINE, self.settings["refine"].GetBool())
        meshing_options.Set(KratosPfemBase.ModelerUtilities.RECONNECT, self.settings["reconnect"].GetBool())
        meshing_options.Set(KratosPfemBase.ModelerUtilities.TRANSFER, self.settings["transfer"].GetBool())
        meshing_options.Set(KratosPfemBase.ModelerUtilities.CONSTRAINED, self.settings["constrained"].GetBool())
        meshing_options.Set(KratosPfemBase.ModelerUtilities.MESH_SMOOTHING, self.settings["mesh_smoothing"].GetBool())
        meshing_options.Set(KratosPfemBase.ModelerUtilities.VARIABLES_SMOOTHING, self.settings["variables_smoothing"].GetBool())

        self.MeshingParameters.SetOptions(meshing_options)
        self.MeshingParameters.SetReferenceElement(self.settings["reference_element"].GetString())
        self.MeshingParameters.SetReferenceCondition(self.settings["reference_condition"].GetString())
        
        #set variables to global transfer
        self.MeshDataTransfer   = KratosPfemBase.MeshDataTransferUtilities()
        self.TransferParameters = KratosPfemBase.TransferParameters()
        self.global_transfer    = False
        if( self.settings["variables_smoothing"].GetBool() == True ):
            self.global_transfer = True
            transfer_variables = self.settings["elemental_variables_to_smooth"]
            #for variable in transfer_variables:
            #    self.TransferParameters.SetVariable( KratosMultiphysics.KratosGlobals.GetVariable( variable.GetString() ) )
            for i in range(0, transfer_variables.size() ):            
                self.TransferParameters.SetVariable(KratosMultiphysics.KratosGlobals.GetVariable(transfer_variables[i].GetString()))
                            

        #mesh modelers for the current strategy
        self.mesh_modelers = []
        
        #configure meshers: 
        self.SetMeshModelers();
        
        for mesher in self.mesh_modelers:
            mesher.Initialize(imposed_walls,domain_size)

        self.number_of_nodes      = 0
        self.number_of_elements   = 0
        self.number_of_conditions = 0

    #
    def SetMeshModelers(self):

        modelers = []        
        if( self.settings["remesh"].GetBool() and self.settings["refine"].GetBool() ):
            modelers.append("pre_refining_modeler")
            modelers.append("post_refining_modeler")
        elif( self.settings["remesh"].GetBool() ):
            modelers.append("reconnect_modeler")
        elif( self.settings["transfer"].GetBool() ):
            modelers.append("transfer_modeler")
 
        for modeler in modelers:
            meshing_module =__import__(modeler)      
            mesher = meshing_module.CreateMeshModeler(self.main_model_part,self.MeshingParameters,self.mesh_id) 
            self.mesh_modelers.append(mesher)
  
    #
    def SetInfo(self):
        
        info_parameters = self.MeshingParameters.GetInfoParameters()
    
        current_number_of_nodes      = self.main_model_part.NumberOfNodes(self.mesh_id)
        current_number_of_elements   = self.main_model_part.NumberOfElements(self.mesh_id)
        current_number_of_conditions = self.main_model_part.NumberOfConditions(self.mesh_id)
     
        info_parameters.SetNumberOfNodes(current_number_of_nodes)
        info_parameters.SetNumberOfElements(current_number_of_elements)
        info_parameters.SetNumberOfConditions(current_number_of_conditions)

        info_parameters.SetNumberOfNewNodes(self.number_of_nodes-current_number_of_nodes)
        info_parameters.SetNumberOfNewElements(self.number_of_elements-current_number_of_elements)
        info_parameters.SetNumberOfNewConditions(self.number_of_conditions-current_number_of_elements)
        

    #
    def InitializeMeshGeneration(self):
        
        self.number_of_elements   = 0
        self.number_of_conditions = 0
        self.number_of_nodes      = 0
        
        info_parameters = self.MeshingParameters.GetInfoParameters()
        info_parameters.Initialize()
        
        self.SetInfo()

        if( self.global_transfer == True ):
            self.MeshDataTransfer.TransferElementalValuesToNodes(self.TransferParameters,self.main_model_part,self.mesh_id)

    #
    def FinalizeMeshGeneration(self):

        self.SetInfo()

        info_parameters    = self.MeshingParameters.GetInfoParameters()
        smoothing_required = info_parameters.CheckMechanicalSmoothing()
        
        refining_parameters = self.MeshingParameters.GetRefiningParameters()
        
        if( self.global_transfer == True ):
            if(smoothing_required):
                #smooth only on selected part based on a threshold variable
                self.MeshDataTransfer.TransferNodalValuesToElementsOnThreshold(self.TransferParameters,refining_parameters.GetThresholdVariable(),refining_parameters.GetReferenceThreshold(),self.main_model_part,self.mesh_id)
            else:
                #smooth all domain
                self.MeshDataTransfer.TransferNodalValuesToElements(self.TransferParameters,self.main_model_part,self.mesh_id)                  
                
        

    #
    def GenerateMesh(self):

        self.InitializeMeshGeneration()

        for mesher in self.mesh_modelers:
            mesher.ExecuteMeshing()

        self.FinalizeMeshGeneration()