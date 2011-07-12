##################################################################
######################## include.py   ############################
##################################################################
##### ekate - Enhanced KRATOS for Advanced Tunnel Enineering #####
##### copyright by CIMNE, Barcelona, Spain                   #####
#####          and Janosch Stascheit for TUNCONSTRUCT        #####
##### all rights reserved                                    #####
##################################################################
##### note: KRATOS is released under LGPL                    #####
##################################################################
##################################################################
##################################################################
## ATTENTION: here the order is important                    #####
##################################################################
## including kratos path                                     #####
## ATTENTION: the following lines have to be adapted to      #####
##            match your acrtual configuration               #####
##################################################################
## calypso:
kratos_root_path = '../../../..'
##setting up paths
kratos_libs_path = kratos_root_path+'/libs' ##kratos_root/libs
kratos_applications_path = kratos_root_path+'/applications' ##kratos_root/applications
##################################################################
##################################################################
import sys
sys.path.append(kratos_libs_path)
sys.path.append(kratos_applications_path)

#importing Kratos main library
from Kratos import *
kernel = Kernel()   #defining kernel

#importing applications
import applications_interface
applications_interface.Import_StructuralApplication = True
applications_interface.Import_KratosMKLSolversApplication = True
applications_interface.ImportApplications(kernel, kratos_applications_path)
from KratosStructuralApplication import *
from KratosMKLSolversApplication import *
##################################################################
##################################################################
class Model:
        def __init__( self, problem_name, path ):
                #setting the domain size for the problem to be solved
                self.domain_size = 3
                ##################################################################
                ## DEFINE MODELPART ##############################################
                ##################################################################
                self.model_part = ModelPart("ekate_simulation")
                self.path = path
                self.problem_name = problem_name
                ##################################################################
                ## DEFINE SOLVER #################################################
                ##################################################################
                # reading simulation parameters
                number_of_time_steps = 10
                self.analysis_parameters = []
                # content of analysis_parameters:
                # perform_contact_analysis_flag
                # penalty value for normal contact
                # maximum number of uzawa iterations
                # friction coefficient
                # penalty value for frictional contact
                # contact_double_check_flag
                # contact_ramp_penalties_flag
                # maximum penalty value for normal contact
                # ramp criterion for normal contact
                # ramp factor for normal contact
                # maximum penalty value for frictional contact
                # ramp criterion for frictional contact
                # ramp factor for frictional contact
                perform_contact_analysis_flag = False
                penalty = 0.0
                maxuzawa = 0.0
                friction = 0.0
                frictionpenalty = 0.0
                contact_double_check_flag = False
                contact_ramp_penalties_flag = False
                maxpenalty = 0.0
                rampcriterion = 0.0
                rampfactor = 0.0
                fricmaxpenalty = 0.0
                fricrampcriterion = 0.0
                fricrampfactor = 0.0
                self.analysis_parameters.append(perform_contact_analysis_flag)
                self.analysis_parameters.append(penalty)
                self.analysis_parameters.append(maxuzawa)
                self.analysis_parameters.append(friction)
                self.analysis_parameters.append(frictionpenalty)
                self.analysis_parameters.append(contact_double_check_flag)
                self.analysis_parameters.append(contact_ramp_penalties_flag)
                self.analysis_parameters.append(maxpenalty)
                self.analysis_parameters.append(rampcriterion)
                self.analysis_parameters.append(rampfactor)
                self.analysis_parameters.append(fricmaxpenalty)
                self.analysis_parameters.append(fricrampcriterion)
                self.analysis_parameters.append(fricrampfactor)
                #PrintSparsityInfoFlag
                self.analysis_parameters.append(False)
                
                abs_tol =        1e-06
                #rel_tol =       0.0001
                rel_tol = 1e-10
                
                ## generating solver
                import structural_solver_advanced
                self.solver = structural_solver_advanced.SolverAdvanced( self.model_part, self.domain_size, number_of_time_steps, self.analysis_parameters, abs_tol, rel_tol, kratos_applications_path )
                #import ekate_solver_parallel
                #self.solver = ekate_solver_parallel.EkateSolver( self.model_part, self.domain_size, number_of_time_steps, self.analysis_parameters, abs_tol, rel_tol, kratos_applications_path )
                structural_solver_advanced.AddVariables( self.model_part )
                #ekate_solver_parallel.AddVariables( self.model_part )
                ##################################################################
                ## READ MODELPART ################################################
                ##################################################################
                #reading a model
                write_deformed_flag = WriteDeformedMeshFlag.WriteUndeformed
                write_elements = WriteConditionsFlag.WriteConditions
                #write_elements = WriteConditionsFlag.WriteElementsOnly
                post_mode = GiDPostMode.GiD_PostBinary
                multi_file_flag = MultiFileFlag.MultipleFiles
                self.gid_io = StructuralGidIO( self.path+self.problem_name, post_mode, multi_file_flag, write_deformed_flag, write_elements )
                self.gid_io.ReadModelPart(self.model_part)
                self.meshWritten = False

                ## READ DEACTIVATION FILE ########################################
                self.deac_file = open(self.path+self.problem_name+".deac",'r' )
                self.activation_flags = [0]
                for line in self.deac_file:
                        val_set = line.split(' ')
                        elem_num = int(val_set[0])
                        act_level = int(val_set[1])
                        self.activation_flags.append(act_level)
                self.cond_file = open(self.path+self.problem_name+".cond",'r' )
                self.cond_activation_flags = []
                for line in self.cond_file:
                        if "//belongs_to:" in line:
                                val_set = line.split(':')
                                self.cond_activation_flags.append(int(val_set[1]))
                print "input data read OK"
                #print "+++++++++++++++++++++++++++++++++++++++"
                #for node in self.model_part.Nodes:
                #        print node
                #print "+++++++++++++++++++++++++++++++++++++++"
                
                #the buffer size should be set up here after the mesh is read for the first time
                self.model_part.SetBufferSize(2)

                ##################################################################
                ## ADD DOFS ######################################################
                ##################################################################                
                for node in self.model_part.Nodes:
                        node.AddDof( WATER_PRESSURE )
                structural_solver_advanced.AddDofs( self.model_part )
                #ekate_solver_parallel.AddDofs( self.model_part )

                ##################################################################
                ## INITIALISE SOLVER FOR PARTICULAR SOLUTION #####################
                ##################################################################
                #defining linear solver
                plinear_solver = MKLPardisoSolver()
                #plinear_solver = ParallelMKLPardisoSolver()
                self.solver.structure_linear_solver = plinear_solver
                self.solver.Initialize()
                (self.solver.solver).SetEchoLevel(2);

                ##################################################################
                ## INITIALISE RESTART UTILITY ####################################
                ##################################################################
                #restart_utility= RestartUtility( self.problem_name )
                
        def SetUpActivationLevels( self, model_part, activation_list, cond_activation_list ):
                for element in self.model_part.Elements:
                        element.SetValue(ACTIVATION_LEVEL, activation_list[element.Id])

        def FixPressureNodes( self, free_node_list_water, free_node_list_air):
            for node in self.model_part.Nodes:
                if (node.IsFixed(WATER_PRESSURE)==0):                
                    node.Fix(WATER_PRESSURE)
                    free_node_list_water.append(node)
                if (node.IsFixed(AIR_PRESSURE)==0):
                    node.Fix(AIR_PRESSURE)
                    free_node_list_air.append(node)                                

        def ApplyInsituWaterPressure( self, free_node_list_water, free_node_list_air, z_zero, gravity_z):
                water_density=1000.0;
                for node in self.model_part.Nodes:                              
                        water_pressure= water_density*gravity_z*(z_zero-(node.Z-node.GetSolutionStepValue(DISPLACEMENT_Z,0)))
                        node.SetSolutionStepValue(WATER_PRESSURE, water_pressure)
                        node.SetSolutionStepValue(WATER_PRESSURE_EINS, water_pressure)
                        node.SetSolutionStepValue(WATER_PRESSURE_NULL, water_pressure)
                for node in self.model_part.Nodes:              
                        node.SetSolutionStepValue(AIR_PRESSURE, 0.0)
                        node.SetSolutionStepValue(AIR_PRESSURE_EINS, 0.0)
                        node.SetSolutionStepValue(AIR_PRESSURE_NULL, 0.0)

        def FreePressureNodes(self,free_node_list_water, free_node_list_air):
                for item in free_node_list_water:
                        #self.model_part.Nodes[item].Free(WATER_PRESSURE)
                        item.Free(WATER_PRESSURE)
                for item in free_node_list_air:
                        #self.model_part.Nodes[item].Free(AIR_PRESSURE)
                        item.Free(AIR_PRESSURE)
                        
        def WriteMaterialParameters( self, time, indices ):
                self.gid_io.OpenResultFile( self.path+self.problem_name, GiDPostMode.GiD_PostBinary)
                #self.gid_io.ChangeOutputName( self.path+self.problem_name +str(time), GiDPostMode.GiD_PostBinary )
                for index in indices:
                        self.gid_io.SuperPrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, index)
                self.gid_io.CloseResultFile()
                
        def WriteOutput( self, time ):
                self.gid_io.InitializeMesh( time )
                mesh = self.model_part.GetMesh()
                self.gid_io.WriteNodeMesh( mesh )
                self.gid_io.WriteMesh( mesh )
                print("mesh written...")
                self.gid_io.FinalizeMesh()
                self.gid_io.InitializeResults( time, self.model_part.GetMesh() )
                #self.gid_io.PrintOnGaussPoints(PLASTICITY_INDICATOR, self.model_part, time, 0)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 0)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 1)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 2)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 3)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 4)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 5)
                #self.gid_io.PrintOnGaussPoints(MATERIAL_PARAMETERS, self.model_part, time, 6)
                print("write nodal displacements")
                self.gid_io.WriteNodalResults(DISPLACEMENT, self.model_part.Nodes, time, 0)
                self.gid_io.PrintOnGaussPoints(WATER_PRESSURE, self.model_part, time)
                #self.gid_io.WriteNodalResults(WATER_PRESSURE, self.model_part.Nodes, time, 0)
                self.gid_io.PrintOnGaussPoints(SATURATION, self.model_part, time)
                #self.gid_io.PrintOnGaussPoints(CONTACT_PENETRATION, self.model_part, time)
                #self.gid_io.PrintOnGaussPoints(NORMAL, self.model_part, time, 0)
                self.gid_io.FinalizeResults()
                                
        def InitializeModel( self, linear_elastic ):
                ##################################################################
                ## INITIALISE CONSTITUTIVE LAWS ##################################
                ##################################################################
                #set material parameters
                append_manual_data = False
                params1 = Vector(1)
                self.model_part.Properties[1].SetValue(MATERIAL_PARAMETERS, params1 )
                self.model_part.Properties[1].SetValue(CONSTITUTIVE_LAW, Isotropic3D())
                #if( append_manual_data ):
                        #from set_material_data import *
                        #mdp = MaterialDataProvider()
                        #print(" 0 ")

                ##################################################################
                ## STORE LAYER SETS ##############################################
                ##################################################################
                ## ELEMENTS on layers ############################################
                self.layer_sets = {}
                layer_elements_list = [
                1 ,
                2 ,
                3 ,
                4 ,
                5 ,
                6 ,
                7 ,
                8 ,
                9 ,
                10 ,
                ]
                self.layer_sets['Layer0'] = layer_elements_list
                layer_elements_list = [
                ]
                self.layer_sets['surface'] = layer_elements_list
                ## ELEMENTS on inner boundaries ##################################
                self.inner_boundary_elements = [
                ]
                ## NODES on layers ###############################################
                self.layer_nodes_sets = {}
                layer_nodes_list = [
            1 ,
            2 ,
            3 ,
            4 ,
            5 ,
            6 ,
            7 ,
            8 ,
            9 ,
            10 ,
            11 ,
            12 ,
            13 ,
            14 ,
            15 ,
            16 ,
            17 ,
            18 ,
            19 ,
            20 ,
            21 ,
            22 ,
            23 ,
            24 ,
            25 ,
            26 ,
            27 ,
            28 ,
            29 ,
            30 ,
            31 ,
            32 ,
            33 ,
            34 ,
            35 ,
            36 ,
            37 ,
            38 ,
            39 ,
            40 ,
            41 ,
            42 ,
            43 ,
            44 ,
            45 ,
            46 ,
            47 ,
            48 ,
            49 ,
            50 ,
            51 ,
            52 ,
            53 ,
            54 ,
            55 ,
            56 ,
            57 ,
            58 ,
            59 ,
            60 ,
            61 ,
            62 ,
            63 ,
            64 ,
            65 ,
            66 ,
            67 ,
            68 ,
            69 ,
            70 ,
            71 ,
            72 ,
            73 ,
            74 ,
            75 ,
            76 ,
            77 ,
            78 ,
            79 ,
            80 ,
            81 ,
            82 ,
            83 ,
            84 ,
            85 ,
            86 ,
            87 ,
            88 ,
            89 ,
            90 ,
            91 ,
            92 ,
            93 ,
            94 ,
            95 ,
            96 ,
            97 ,
            98 ,
            99 ,
            100 ,
            102 ,
            103 ,
            104 ,
            107 ,
            108 ,
            109 ,
            110 ,
            111 ,
            112 ,
            113 ,
            114 ,
            115 ,
            116 ,
            117 ,
            120 ,
            121 ,
            124 ,
            125 ,
            126 ,
            127 ,
                ]
                self.layer_nodes_sets['Layer0'] = layer_nodes_list
                layer_nodes_list = [
            101 ,
            105 ,
            106 ,
            118 ,
            119 ,
            122 ,
            123 ,
            128 ,
                ]
                self.layer_nodes_sets['surface'] = layer_nodes_list
                ## CONTACT MASTER NODES ##########################################
                self.contact_master_nodes = [
                ]
                ## CONTACT SLAVE NODES ###########################################
                self.contact_slave_nodes = [
                ]
                ## INNER BOUNDARY NODES ##########################################
                self.inner_boundary_nodes = [
                ]
                ##################################################################
                print "layer sets stored"                
                ##################################################################
                ## STORE NODES ON GROUND SURFACE #################################
                ##################################################################
                self.top_surface_nodes = []
                print "nodes on ground surface stored"
                ##################################################################
                ## STORE NODES CORRECTLY FOR CONDITIONS ##########################
                ##################################################################
                self.node_groups = {}
                ##################################################################
                ## ACTIVATION ####################################################
                ##################################################################
                self.deac = DeactivationUtility()
                self.SetUpActivationLevels( self.model_part, self.activation_flags, self.cond_activation_flags )
                self.deac.Initialize( self.model_part )
                print "activation utility initialized"
                print "model successfully initialized"
                self.SetCalculateInSituStress( False )

                
        def FinalizeModel( self ):
                self.gid_io.CloseResultFile()
                
        def SetCalculateInSituStress( self, calculation_flag ):
                self.insitu_stress_flag = calculation_flag

        def Solve( self, time, from_deac, to_deac, from_reac, to_reac ):
                self.deac.Reactivate( self.model_part, from_reac, to_reac )
                self.deac.Deactivate( self.model_part, from_deac, to_deac )
                self.model_part.CloneTimeStep(time)
                self.model_part.ProcessInfo.SetValue( CALCULATE_INSITU_STRESS, self.insitu_stress_flag )
                self.solver.Solve()
##################################################################
