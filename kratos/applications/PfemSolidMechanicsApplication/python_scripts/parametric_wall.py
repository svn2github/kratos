from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7
#import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.PfemSolidMechanicsApplication as KratosPfemSolid

# Check that KratosMultiphysics was imported in the main script
KratosMultiphysics.CheckForPreviousImport()

def CreateParametricWall(main_model_part, custom_settings):
    return ParametricWall(main_model_part, custom_settings)

class ParametricWall(object):
    
    ##constructor. the constructor shall only take care of storing the settings 
    ##and the pointer to the main_model part.
    ##
    ##real construction shall be delayed to the function "Initialize" which 
    ##will be called once the modeler is already filled
    def __init__(self, main_model_part, custom_settings):
        
        self.main_model_part = main_model_part    
        
        ##settings string in json format
        default_settings = KratosMultiphysics.Parameters("""
        {
            "python_file_name": "parametric_wall",
            "mesh_id": 1,
            "domain_size": 2,
            "echo_level": 1,
            "wall_parameters_list":[{
               "radius": "meshing_strategy",
               "center": [0.0, 0.0, 0.0],
               "rake_angle": 0.0,
               "clearance_angle": 0.0,
               "convexity": 0
            }],
            "contact_search_strategy":{
               "python_file_name": "contact_search_strategy",
               "search_frequency": 0,
               "contact_parameters":{
                   "contact_condition_type": "PointContactCondition2D1N",
                   "friction_active": false,
                   "friction_law_type": "MorhCoulomb",
                   "variables_of_properties":{
                      "MU_STATIC": 0.3,
                      "MU_DYNAMIC": 0.2,
                      "PENALTY_PARAMETER": 1000
                   }
               }
            },
            "constraints_process_list":[{
               "implemented_in_file"   : "apply_velocity_process",
               "implemented_in_module" : "KratosMultiphysics.SolidMechanicsApplication",
               "process_name"          : "ApplyVelocityProcess",
               "parameters"            : {
                    "mesh_id"        : 0,
                    "model_part_name : "ParametricWallPart",
                    "variable_name"  : "VELOCITY",
                    "is_fixed_x"     : true,
                    "is_fixed_y"     : true,
                    "is_fixed_z"     : true,
                    "value"          : [0.0, 0.0, 0.0]
               }
            }],
            "rigid_body_element_type": "TranslatoryRigidElement3D1N"
        }
        """)
        
        ##overwrite the default settings with user-provided parameters
        self.settings = custom_settings
        self.settings.ValidateAndAssignDefaults(default_settings)
        
        #construct the solving strategy
        meshing_module = __import__(self.settings["contact_search_strategy"]["python_file_name"].GetString())
        self.SearchStrategy = meshing_module.CreateSearchStrategy(self.main_model_part, self.settings["contact_search_strategy"])

        print("Construction of Parametric Wall finished")
        

    #### 

    def Initialize(self):

        print("::[Mesh Domain]:: -START-")
        
        self.domain_size = self.settings["domain_size"].GetInt()
        self.mesh_id     = self.settings["mesh_id"].GetInt()

       
        # Meshing Stratety
        self.SearchStrategy.Initialize(self.MeshingParameters, self.domain_size, self.mesh_id)
        
        print("::[Mesh Domain]:: -END- ")

        
