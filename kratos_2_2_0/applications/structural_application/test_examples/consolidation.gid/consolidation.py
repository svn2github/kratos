##################################################################
##### ekate - Enhanced KRATOS for Advanced Tunnel Enineering #####
##### copyright by CIMNE, Barcelona, Spain                   #####
#####          and Janosch Stascheit for TUNCONSTRUCT        #####
##### all rights reserved                                    #####
##################################################################
#setting the domain size for the problem to be solved
domain_size = 3
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

##################################################################
##################################################################
sys.path.append('./consolidation.gid')
import consolidation_include
from consolidation_include import *
# calculate insitu-stress for geology_virgin.gid
model = consolidation_include.Model('consolidation','./')
linear_elastic = False
model.InitializeModel( linear_elastic )

##################################################################
###  SIMULATION  #################################################
##################################################################
initial_water_pressure = 1907.53
surface_load = -1000.0
free_node_list_water = []
free_node_list_air = []
model.FixPressureNodes(free_node_list_water, free_node_list_air)
model.FreePressureNodes( free_node_list_water, free_node_list_air )
for node in model.model_part.Nodes:
    node.SetSolutionStepValue( WATER_PRESSURE, initial_water_pressure )
    node.SetSolutionStepValue( WATER_PRESSURE_NULL, initial_water_pressure )
    node.SetSolutionStepValue( WATER_PRESSURE_EINS, initial_water_pressure )

time = 0.0
delta_time = 0.01
#time = time + delta_time
#model.Solve(time, 0, 0, 0, 0)
#model.WriteOutput( time )

#loading phase
for node in model.layer_nodes_sets['surface']:
    model.model_part.Nodes[node].SetSolutionStepValue( FACE_LOAD_Z, surface_load )
#consolidation phase
for node in model.layer_nodes_sets['surface']:
    model.model_part.Nodes[node].Fix( WATER_PRESSURE )
for node in model.layer_nodes_sets['surface']:
    model.model_part.Nodes[node].SetSolutionStepValue( WATER_PRESSURE, 0.0 )
    model.model_part.Nodes[node].SetSolutionStepValue( WATER_PRESSURE_NULL, 0.0  )
    model.model_part.Nodes[node].SetSolutionStepValue( WATER_PRESSURE_EINS, 0.0  )

delta_time = 0.001
for outer in range( 0, 26 ):
    delta_time = delta_time * 1.7
    for step in range( 0, 40 ):
        for inner_step in range( 0, 1 ):
            time = time + delta_time
            model.Solve( time, 0, 0, 0, 0 )
    model.WriteOutput( time )
print("all done.")
