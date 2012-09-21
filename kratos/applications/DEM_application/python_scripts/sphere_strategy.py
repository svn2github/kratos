
from KratosMultiphysics import *
from KratosMultiphysics.DEMApplication import *
# Check that KratosMultiphysics was imported in the main script
#CheckForPreviousImport(

def AddVariables(model_part):
    #model_part.AddNodalSolutionStepVariable(NUMBER_OF_NEIGHBOURS)
    model_part.AddNodalSolutionStepVariable(GROUP_ID)
    model_part.AddNodalSolutionStepVariable(DISPLACEMENT)
    model_part.AddNodalSolutionStepVariable(DELTA_DISPLACEMENT)
    model_part.AddNodalSolutionStepVariable(DELTA_VELOCITY)
    model_part.AddNodalSolutionStepVariable(VELOCITY)
    model_part.AddNodalSolutionStepVariable(RHS)
    model_part.AddNodalSolutionStepVariable(APPLIED_FORCE)
    model_part.AddNodalSolutionStepVariable(RADIUS)
    model_part.AddNodalSolutionStepVariable(PARTICLE_DENSITY)
    model_part.AddNodalSolutionStepVariable(PARTICLE_STIFFNESS)
    model_part.AddNodalSolutionStepVariable(YOUNG_MODULUS)
    model_part.AddNodalSolutionStepVariable(POISSON_RATIO)
    model_part.AddNodalSolutionStepVariable(NODAL_MASS)
    model_part.AddNodalSolutionStepVariable(VISCO_DAMP_COEFF)
    model_part.AddNodalSolutionStepVariable(RESTITUTION_COEFF)
    #model_part.AddNodalSolutionStepVariable(IS_STRUCTURE)
    model_part.AddNodalSolutionStepVariable(PARTICLE_MATERIAL)
    model_part.AddNodalSolutionStepVariable(PARTICLE_CONTINUUM)
    model_part.AddNodalSolutionStepVariable(PARTICLE_COHESION)
    model_part.AddNodalSolutionStepVariable(PARTICLE_FRICTION)
    model_part.AddNodalSolutionStepVariable(PARTICLE_TENSION)
    model_part.AddNodalSolutionStepVariable(PARTICLE_ROTATION_DAMP_RATIO)
    model_part.AddNodalSolutionStepVariable(EXPORT_PARTICLE_FAILURE_ID)

    model_part.AddNodalSolutionStepVariable(PARTICLE_INERTIA)
    model_part.AddNodalSolutionStepVariable(ANGULAR_VELOCITY)
    model_part.AddNodalSolutionStepVariable(PARTICLE_MOMENT)
    model_part.AddNodalSolutionStepVariable(PARTICLE_MOMENT_OF_INERTIA)
    model_part.AddNodalSolutionStepVariable(PARTICLE_ROTATION_ANGLE)
    model_part.AddNodalSolutionStepVariable(DELTA_ROTA_DISPLACEMENT)
    model_part.AddNodalSolutionStepVariable(EULER_ANGLES)
   

    print "variables for the explicit solver added correctly"

def AddDofs(model_part):
    
    for node in model_part.Nodes:
    #adding dofs
        node.AddDof(DISPLACEMENT_X,REACTION_X);
        node.AddDof(DISPLACEMENT_Y,REACTION_Y);
        node.AddDof(DISPLACEMENT_Z,REACTION_Z);
        node.AddDof(VELOCITY_X,REACTION_X);
        node.AddDof(VELOCITY_Y,REACTION_Y);
        node.AddDof(VELOCITY_Z,REACTION_Z);

    print "dofs for the DEM solution added correctly"

 
class ExplicitStrategy:
    
    def __init__(self,model_part,domain_size):

        self.model_part                     = model_part
        self.domain_size                    = domain_size
        self.damping_ratio                  = 0.00;
        self.penalty_factor                 = 10.00
        self.max_delta_time                 = 0.05;
        self.fraction_delta_time            = 0.90;
        self.MoveMeshFlag                   = True;
        self.time_scheme                    = FowardEulerScheme();
        self.gravity                        = Vector(3)#(0.0,-9.81,0.0)
        self.gravity[0] = 0.0
        self.gravity[1] = -9.81
        self.gravity[2] = 0.0
        self.delta_time                     = 0.00001;
        self.virtual_mass_OPTION            = 0; #its 1/0 xapuza
        self.nodal_mass_coeff               = 0.0;
      
        #type of problem:

        self.critical_time_OPTION           = 0 #its 1/0 xapuza

        self.delta_OPTION                   = False
        self.continuum_simulating_OPTION    = False
        self.case_OPTION                    = 0  #aixo es una xapuza fins que pooyan permeti bools a pyton o tinguis flags.
        self.trihedron_OPTION               = 0

        self.rotation_OPTION                = 0  #its 1/0 xapuza
        self.rotation_spring_OPTION         = 0  #its 1/0 xapuza
        self.bounding_box_OPTION            = 0  #its 1/0 xapuza

        #global parameters
        self.global_variables_OPTION        = 0 #its 1/0 xapuza
        self.global_kn                      = 1000.0
        self.global_kt                      = 1000.0
        self.global_kr                      = 1000.0
        self.global_rn                      = 1000.0
        self.global_rt                      = 1000.0
        self.global_rr                      = 1000.0
        self.global_fri_ang                 = 40

        #problem specific parameters

        self.force_calculation_type_id      =1
        self.damp_id                        =1
        self.rota_damp_id                   =1
        self.search_radius_extension        = 0.0

        self.dummy_switch                   =0

        #problem utilities
        self.enlargement_factor             = 1;
        self.n_step_search                  = 1;
        self.safety_factor                  = 1.0; #for critical time step

        self.create_and_destroy             = particle_destructor_and_constructor();

    ######################################################################

    def Initialize(self):

        #definir les variables del ProcessInfo:
      
        self.model_part.ProcessInfo.SetValue(GRAVITY, self.gravity)

        self.model_part.ProcessInfo.SetValue(DELTA_TIME, self.delta_time)

        #POOOYAAAAN NO EM VAN AKESTS NO ELS CONEIX PYTON DE BOOL A BOOL

        ##################XAPUZA##XAPUZA##XAPUZA#############################


        #LA CONTINUUM OPTION SHA DE LLEGIR MIRANT SI TOTS ELS CONTINUUM GROUPS SON 0 LLAVORS LA OPTION ES FALSE
        #SI HI HA ALGUN KE ES DIFERENT DE 0 LLAVORS LA OPTION ES TRUE.
        
        if(self.delta_OPTION==True):
            if(self.continuum_simulating_OPTION==True): self.case_OPTION = 2
            else: self.case_OPTION = 1
        elif(self.delta_OPTION==False):
            if(self.continuum_simulating_OPTION==False): self.case_OPTION = 0
            else: self.case_OPTION = 3
        self.model_part.ProcessInfo.SetValue(CRITICAL_TIME_OPTION, self.critical_time_OPTION)
        self.model_part.ProcessInfo.SetValue(VIRTUAL_MASS_OPTION, self.virtual_mass_OPTION)
        self.model_part.ProcessInfo.SetValue(NODAL_MASS_COEFF, self.nodal_mass_coeff)
        self.model_part.ProcessInfo.SetValue(CASE_OPTION, self.case_OPTION)
        self.model_part.ProcessInfo.SetValue(ROTATION_OPTION, self.rotation_OPTION)
        self.model_part.ProcessInfo.SetValue(ROTATION_SPRING_OPTION, self.rotation_spring_OPTION)
        self.model_part.ProcessInfo.SetValue(BOUNDING_BOX_OPTION, self.bounding_box_OPTION)
        self.model_part.ProcessInfo.SetValue(TRIHEDRON_OPTION, self.trihedron_OPTION)
        
        #####

        self.model_part.ProcessInfo.SetValue(FORCE_CALCULATION_TYPE, self.force_calculation_type_id)    
        self.model_part.ProcessInfo.SetValue(DAMP_TYPE, self.damp_id)
        self.model_part.ProcessInfo.SetValue(ROTA_DAMP_TYPE, self.rota_damp_id)
        self.model_part.ProcessInfo.SetValue(SEARCH_RADIUS_EXTENSION, self.search_radius_extension)

        self.model_part.ProcessInfo.SetValue(GLOBAL_VARIABLES_OPTION, self.global_variables_OPTION)
        self.model_part.ProcessInfo.SetValue(GLOBAL_KN, self.global_kn)
        self.model_part.ProcessInfo.SetValue(GLOBAL_KT, self.global_kt)
        self.model_part.ProcessInfo.SetValue(GLOBAL_KR, self.global_kr)
        self.model_part.ProcessInfo.SetValue(GLOBAL_RN, self.global_rn)
        self.model_part.ProcessInfo.SetValue(GLOBAL_RT, self.global_rt)
        self.model_part.ProcessInfo.SetValue(GLOBAL_RR, self.global_rr)
        self.model_part.ProcessInfo.SetValue(GLOBAL_FRI_ANG, self.global_fri_ang)

        self.model_part.ProcessInfo.SetValue(DUMMY_SWITCH, self.dummy_switch)
       
        #creating the solution strategy
        self.solver = ExplicitSolverStrategy(self.model_part, self.domain_size, self.enlargement_factor, self.damping_ratio, self.fraction_delta_time, self.delta_time, self.n_step_search, self.safety_factor,
                                            self.MoveMeshFlag, self.delta_OPTION, self.continuum_simulating_OPTION, self.time_scheme)
        #self.solver.Check() #es sa fer sempre un check despres de montar una estrategia.
        self.solver.Initialize() #aqui definirem el initialize dels elements pero tamb funcions que vulguem fer en el primer pras.
       
        self.model_part.ProcessInfo.SetValue(DUMMY_SWITCH, 1)  #the first step has been done
              
            
        #self.solver.SetCohesiveContacts()

 #######################################################################
    def Initial_Critical_Time(self):
        (self.solver).InitialCriticalTime()

    #######################################################################   
    def Solve(self):
        (self.solver).Solve()
    
     #######################################################################

    def Calculate_Model_Surrounding_Bounding_Box(self, model_part, enlargement_factor):
        self.create_and_destroy.calculate_surrounding_bounding_box( model_part, enlargement_factor)

    def Destroy_Particles(self, model_part):
        self.create_and_destroy.destroy_distant_particles( model_part)

        
        
        #self.explicit_solver_object.Search_Neighbours() #after destructing we need to recalculate the neighbours instantly
        ##aixo te el problema de que despres de destruir hem de buscar pero tornarema  buscar en la estrategia??? organitzar-ho be.

          ##M:  be sure that after this we search for neighbours again.