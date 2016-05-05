from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7
# importing the Kratos Library
import KratosMultiphysics as kratos_core
import KratosMultiphysics.SolidMechanicsApplication as solid_application
# Check that KratosMultiphysics was imported in the main script
kratos_core.CheckForPreviousImport()


def CreateSolver(main_model_part, custom_settings):
    return SolidMechanicsSolver(main_model_part, custom_settings)

class SolidMechanicsSolver:
    
    ##constructor. the constructor shall only take care of storing the settings 
    ##and the pointer to the main_model part. This is needed since at the point of constructing the 
    ##model part is still not filled and the variables are not yet allocated
    ##
    ##real construction shall be delayed to the function "Initialize" which 
    ##will be called once the model is already filled
    def __init__(self, main_model_part, custom_settings): 
        
        #TODO: shall obtain the compute_model_part from the MODEL once the object is implemented
        self.main_model_part = main_model_part    
        
        ##settings string in json format
        default_settings = kratos_core.Parameters("""
        {
            "solver_type": "static_structural_solver",
            "problem_is_linear": false,
            "convergence_criteria_settings": {
                "criteria_type" : "DisplacementCriteria",
                "parameters" : {
                    "relative_tolerance" : 1e-6,
                    "absolute_tolerance" : 1e-9
                }
            }
            "maximum_iterations": 3,
            "echo_level": 1,
            "compute_reactions": true,
            "reform_dofs_at_each_iteration": false,
            "volume_model_part_name" : "volume_model_part",
            "skin_parts":[""],
            "model_import_settings": {
                    "input_type": "mdpa",
                    "input_filename": "unknown_name"
            },
            "linear_solver_settings": {
                    "solver_type": "Super LU",
                    "max_iteration": 500,
                    "tolerance": 1e-9,
                    "scaling": false,
                    "verbosity": 1
            }
        }""")
        
        ##overwrite the default settings with user-provided parameters
        self.settings = custom_settings
        self.settings.ValidateAndAssignDefaults(default_settings)
        
        #construct the linear solvers
        import linear_solver_factory
        self.linear_solver = linear_solver_factory.ConstructSolver(self.settings["linear_solver_settings"])

        
    def GetMinimumBufferSize(self):
        return 1;
    
    def AddVariables(self):
        self.main_model_part.AddNodalSolutionStepVariable(kratos_core.DISPLACEMENT)
        if(self.settings["compute_reactions"].GetBool() == True):
            self.main_model_part.AddNodalSolutionStepVariable(kratos_core.REACTION)
        
        if(self.settings["rotation_dofs_needed"].GetBool() == True):
            self.main_model_part.AddNodalSolutionStepVariable(kratos_core.ROTATION)
            if(self.settings["compute_reactions"].GetBool() == True):
                self.main_model_part.AddNodalSolutionStepVariable(kratos_core.TORQUE)
    
        if(self.settings["pressure_dofs_needed"].GetBool() == True):
            self.main_model_part.AddNodalSolutionStepVariable(kratos_core.PRESSURE)
            if(self.settings["compute_reactions"].GetBool() == True):
                self.main_model_part.AddNodalSolutionStepVariable(kratos_core.PRESSURE_REACTION)
        
        print("variables for the  monolithic solver symbolic added correctly")


    def AddDofs(self):
        for node in self.main_model_part.Nodes:
            node.AddDof(kratos_core.DISPLACEMENT_X, kratos_core.REACTION_X)
            node.AddDof(kratos_core.DISPLACEMENT_Y, kratos_core.REACTION_Y)
            node.AddDof(kratos_core.DISPLACEMENT_Z, kratos_core.REACTION_Z)
            
        if(self.settings["rotation_dofs_needed"].GetBool() == True):
            for node in model_part.Nodes:
                node.AddDof(kratos_core.ROTATION_X, kratos_core.TORQUE_X)
                node.AddDof(kratos_core.ROTATION_Y, kratos_core.TORQUE_Y)
                node.AddDof(kratos_core.ROTATION_Z, kratos_core.TORQUE_Z)
                
        if(self.settings["pressure_dofs_needed"].GetBool() == True):
            for node in model_part.Nodes:
                node.AddDof(kratos_core.PRESSURE, kratos_core.PRESSURE_REACTION)
                
        if(setting.PressureDofs == True):
            node.AddDof(kratos_core.PRESSURE, kratos_core.PRESSURE_REACTION)
        
    #TODO: ImportModelPart shall be inherited from a base class
    def ImportModelPart(self):
        
        #add variables (always before importing the model part)
        self.AddVariables()

        if(self.settings["model_import_settings"]["input_type"].GetString() == "mdpa"):
            #here it would be the place to import restart data if required
            kratos_core.ModelPartIO(self.settings["model_import_settings"]["input_filename"].GetString()).ReadModelPart(self.main_model_part)
            
            ##here we shall check that the input read has the shape we like
            aux_params = kratos_core.Parameters("{}")
            aux_params.AddValue("volume_model_part_name",self.settings["volume_model_part_name"])
            aux_params.AddValue("skin_parts",self.settings["skin_parts"])
            
            #here we could do things to prepare the model part. Probably not needed for the structure
            self.compute_model_part = self.main_model_part
            
            ##here we must construct correctly the constitutive law
            print("don't forget constructing the constitutive law!!!!!!!!!!!!!!")
            
            import constitutive_law_python_utility
            constitutive_law = constitutive_law_python_utility.ConstitutiveLawUtility(main_model_part, self.settings["DomainSize"]);
            constitutive_law.Initialize();
            
        elif(self.settings["model_import_settings"]["input_type"].GetString() == "other_model_part"): ##generate from another ModelPart 
            origin_model_part = self.main_model_part.GetSubModelPart( self.settings["model_import_settings"]["origin_model_part_name"].GetString() )
            
            new_model_part_name = self.settings["model_import_settings"]["new_model_part_name"].GetString()
            if( not self.main_model_part.HasSubModelPart( new_model_part_name ) ):
                self.main_model_part.CreateSubModelPart( new_model_part_name )
            new_model_part = self.main_model_part.GetSubModelPart( new_model_part_name )
            
            #fill the model_part to be used by the termal solver
            kratos_core.ConnectivityPreserveModeler().GenerateModelPart(origin_model_part, new_model_part, "PlasticConvectionDiffusionElement3D", "PlasticThermalFace3D")
            
            #here we could do things to prepare the model part. Probably not needed for the structure
            self.compute_model_part = self.main_model_part
            
            ##here we must construct correctly the constitutive law
            print("don't forget constructing the constitutive law!!!!!!!!!!!!!!")
            
            import constitutive_law_python_utility
            constitutive_law = constitutive_law_python_utility.ConstitutiveLawUtility(main_model_part, self.settings["DomainSize"]);
            constitutive_law.Initialize();
            
        else:
            raise Exception("input from restart not yet implemented")
        
        current_buffer_size = self.main_model_part.GetBufferSize()
        if(self.GetMinimumBufferSize() > current_buffer_size):
            self.main_model_part.SetBufferSize( self.GetMinimumBufferSize() )
                
        print ("model reading finished")


        #add dofs (always after importing the model part) (it must be integrated in the ImportModelPart)
        self.AddDofs()

        
    def Initialize(self):

        computing_model_part = self.GetComputeModelPart()
                       
        time_scheme = kratos_core.ResidualBasedIncrementalUpdateStaticScheme() 
        
        builder_and_solver = kratos_core.ResidualBasedBlockBuilderAndSolver(self.linear_solver)
        
        move_mesh_flag = True #user should NOT configure this
        
        if( self.settings["problem_is_linear"].GetBool() == True): #linear case

            self.solver = kratos_core.ResidualBasedLinearStrategy(computing_model_part, 
                                                                  time_scheme,
                                                                  self.linear_solver, 
                                                                  builder_and_solver, 
                                                                  self.settings["compute_reactions"].GetBool(), 
                                                                  self.settings["reform_dofs_at_each_step"].GetBool(), 
                                                                  move_mesh_flag)
        else: #nonlinear case
            
            #TODO: we shall construct a factory for the convergence criteria, like what is done for the linear solver
            convergence_criteria_settings = self.settings["convergence_criteria_settings"]["parameters"]
            convergence_criterion = kratos_core.DisplacementCriteria(
                                                                 convergence_criteria_settings["relative_tolerance"].GetDouble(),
                                                                 convergence_criteria_settings["absolute_tolerance"].GetDouble()
                                                                 )
                    
            self.solver = kratos_core.ResidualBasedNewtonRaphsonStrategy(computing_model_part, 
                                                                 time_scheme, 
                                                                 self.linear_solver, 
                                                                 convergence_criterion,
                                                                 builder_and_solver, 
                                                                 self.settings["max_iter"].GetInt(), 
                                                                 self.settings["compute_reactions"].GetBool(), 
                                                                 self.settings["reform_dofs_at_each_step"].GetBool(), 
                                                                 move_mesh_flag)
            
        (self.solver).SetEchoLevel(self.settings["echo_level"])
        self.solver.Check()
        self.solver.Initialize()

        print("Construction stokes solver finished")
        
    def GetComputeModelPart(self):
        return self.compute_model_part
        
    def GetOutputVariables(self):
        pass
        
    def ComputeDeltaTime(self):
        pass
        
    def SaveRestart(self):
        pass #one should write the restart file here
       
    #TODO: we may inherit the next 4 methods from the base class
    def Solve(self):
        #TODO: this shall be divided in Predict,InitializeSolutionStep, SolveSolutionStep, FinalizeSolutionStep
        self.solver.Solve()

    def SetEchoLevel(self, level):
        self.solver.SetEchoLevel(level)

    def Clear(self):
        self.solver.Clear()
        
    def Check(self):
        self.solver.Check()

