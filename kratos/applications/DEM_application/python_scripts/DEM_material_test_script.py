from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
from KratosMultiphysics import *
from KratosMultiphysics.DEMApplication import *
# from KratosMultiphysics.MetisApplication import *
# from KratosMultiphysics.mpi import *

import math
import datetime
import shutil


class MaterialTest:

  def __init__(self, DEM_parameters, procedures, solver, graphs_path, post_path, balls_model_part, RigidFace_model_part):
    
      self.parameters = DEM_parameters
      self.graphs_path = graphs_path
      self.post_path = post_path
      self.balls_model_part = balls_model_part
      self.RigidFace_model_part = RigidFace_model_part
      self.Procedures = procedures
      self.solver = solver
      
      self.top_mesh_nodes = []; self.bot_mesh_nodes = []; self.top_mesh_fem_nodes = []; self.bot_mesh_fem_nodes = [];
      
      self.SKIN = list()
      self.LAT = list()
      self.BOT = list()
      self.TOP = list()
      self.XLAT = list()  # only lat, not the corner ones
      self.XTOP = list()  # only top, not corner ones...
      self.XBOT = list()
      self.XTOPCORNER = list()
      self.XBOTCORNER = list()
      
      
      self.bond_00_05 = list(); self.bond_05_10 = list(); self.bond_10_15 = list(); self.bond_15_20 = list(); self.bond_20_25 = list(); self.bond_25_30 = list(); self.bond_30_35 = list()
      self.bond_35_40 = list(); self.bond_40_45 = list(); self.bond_45_50 = list(); self.bond_50_55 = list(); self.bond_55_60 = list(); self.bond_60_65 = list(); self.bond_65_70 = list()
      self.bond_70_75 = list(); self.bond_75_80 = list(); self.bond_80_85 = list(); self.bond_85_90 = list()
      
      self.sizes = [];
      
      self.sigma_mean_table = []; self.tau_mean_table = []; self.sigma_rel_std_dev_table = []; self.tau_rel_std_dev_table = []; self.sigma_ratio_table = [];

      for i in range(0,18):
          self.sizes.append(0.0)
          self.sigma_mean_table.append(0.0)
          self.tau_mean_table.append(0.0)
          self.sigma_rel_std_dev_table.append(0.0)
          self.tau_rel_std_dev_table.append(0.0)
          self.sigma_ratio_table.append(0.0)      

      self.graph_counter = 0; self.renew_pressure = 0; self.Pressure = 0.0; self.pressure_to_apply = 0.0;
      
      self.length_correction_factor = 1.0

      self.graph_frequency        = int(self.parameters.GraphExportFreq/balls_model_part.ProcessInfo.GetValue(DELTA_TIME))
      
      self.strain = 0.0; self.strain_bts = 0.0; self.volumetric_strain = 0.0; self.radial_strain = 0.0; self.first_time_entry = 1; self.first_time_entry_2 = 1
      self.total_stress_top = 0.0; self.total_stress_bot = 0.0; self.total_stress_mean = 0.0;

      
      self.new_strain = 0.0
      
      # for the graph plotting    
      self.loading_velocity = 0.0
      self.height = self.parameters.SpecimenLength
      self.diameter = self.parameters.SpecimenDiameter

      self.initial_time = datetime.datetime.now()

      os.chdir(self.graphs_path)
      self.chart = open(self.parameters.problem_name + "_Parameter_chart.grf", 'w')
           
      if(self.parameters.TestType == "BTS"):

        self.bts_export = open(self.parameters.problem_name + "_bts" + ".grf", 'w');
        self.BtsSkinDetermination()

      else:

        self.graph_export = open(self.parameters.problem_name +"_graph.grf", 'w')
        self.graph_export_1 = open(self.parameters.problem_name +"_graph_top.grf", 'w')
        self.graph_export_2 = open(self.parameters.problem_name +"_graph_bot.grf", 'w')
        self.graph_export_volumetric = open(self.parameters.problem_name+"_graph_VOL.grf",'w')
        self.graph_export_radial1 = open(self.parameters.problem_name+"_graph_1.grf",'w')
        self.graph_export_radial2 = open(self.parameters.problem_name+"_graph_2.grf",'w')
        self.graph_export_radial3 = open(self.parameters.problem_name+"_graph_3.grf",'w')

        print ('Initial Height of the Model: ' + str(self.height)+'\n')
        
        if(self.parameters.PredefinedSkinOption == "ON" ):
          print ("ERROR: in Concrete Test Option the Skin is automatically predefined. Switch the Predefined Skin Option OFF")

        (xtop_area,xbot_area,xlat_area,xtopcorner_area,xbotcorner_area,y_top_total,weight_top, y_bot_total, weight_bot) = self.CylinderSkinDetermination()

  		#xtop_area_gath        = mpi.allgather(mpi.world, xtop_area) 
        #xbot_area_gath        = mpi.allgather(mpi.world, xbot_area) 
        #xlat_area_gath        = mpi.allgather(mpi.world, xlat_area) 
        #xtopcorner_area_gath  = mpi.allgather(mpi.world, xtopcorner_area) 
        #xbotcorner_area_gath  = mpi.allgather(mpi.world, xbotcorner_area) 
        
        #xtop_area = reduce(lambda x, y: x + y, xtop_area_gath)
        #xbot_area = reduce(lambda x, y: x + y, xbot_area_gath)
        #xlat_area = reduce(lambda x, y: x + y, xlat_area_gath)
        #xtopcorner_area = reduce(lambda x, y: x + y, xtopcorner_area_gath)
        #xbotcorner_area = reduce(lambda x, y: x + y, xbotcorner_area_gath)
      
        #weight_top_gath = mpi.gather(mpi.world, weight_top, 0)
        #weight_bot_gath = mpi.gather(mpi.world, weight_bot, 0)
        #y_top_total_gath = mpi.gather(mpi.world, y_top_total, 0)
        #y_bot_total_gath = mpi.gather(mpi.world, y_bot_total, 0)

         #if(mpi.rank == 0):
 
          #weight_top = reduce(lambda x, y: x + y, weight_top_gath)
          #weight_bot = reduce(lambda x, y: x + y, weight_bot_gath)
          #y_top_total = reduce(lambda x, y: x + y, y_top_total_gath)
          #y_bot_total = reduce(lambda x, y: x + y, y_bot_total_gath)
      
        initial_height_top = y_top_total/weight_top 
        initial_height_bot = y_bot_total/weight_bot
    
        inner_initial_height = initial_height_top - initial_height_bot

        specimen_length = self.parameters.SpecimenLength
        extended_length = specimen_length + (specimen_length - inner_initial_height)
          
        self.length_correction_factor = specimen_length/extended_length
      
      ##Oedometric

      if(self.parameters.TestType == "Oedometric"):

        for node in self.LAT:

          node.SetSolutionStepValue(VELOCITY_X, 0.0);
          node.SetSolutionStepValue(VELOCITY_Z, 0.0);
          node.Fix(VELOCITY_X);
          node.Fix(VELOCITY_Z);
      
      ##Pressure
      
      if ( ( self.parameters.TestType == "Triaxial") or ( self.parameters.TestType == "Hydrostatic") ):

        #Correction Coefs
        self.alpha_top = 3.141592*self.diameter*self.diameter*0.25/(xtop_area + 0.70710678*xtopcorner_area)
        self.alpha_bot = 3.141592*self.diameter*self.diameter*0.25/(xbot_area + 0.70710678*xbotcorner_area)
        self.alpha_lat = 3.141592*self.diameter*self.height/(xlat_area + 0.70710678*xtopcorner_area + 0.70710678*xbotcorner_area) 
        
      ##Fixing horizontally top and bot
      
      if(self.parameters.TestType != "BTS"):
        
        for node in self.TOP:
          
          node.SetSolutionStepValue(VELOCITY_X, 0.0);
          node.SetSolutionStepValue(VELOCITY_Z, 0.0);
          node.Fix(VELOCITY_X);
          node.Fix(VELOCITY_Z);
          
        for node in self.BOT:
          
          node.SetSolutionStepValue(VELOCITY_X, 0.0);
          node.SetSolutionStepValue(VELOCITY_Z, 0.0);
          node.Fix(VELOCITY_X);
          node.Fix(VELOCITY_Z);
      
  #-------------------------------------------------------------------------------------#

  def CylinderSkinDetermination(self): #model_part, solver, DEM_parameters):

        # SKIN DETERMINATION
        total_cross_section = 0.0

        # Cylinder dimensions

        h = self.parameters.SpecimenLength
        d = self.parameters.SpecimenDiameter

        eps = 2.0

        surface = 2 * (3.141592 * d * d * 0.25) + (3.141592 * d * h)

        xlat_area = 0.0
        xbot_area = 0.0
        xtop_area = 0.0
        xbotcorner_area = 0.0
        xtopcorner_area = 0.0
        
        y_top_total = 0.0
        y_bot_total = 0.0
        
        weight_top = 0.0
        weight_bot = 0.0

        for element in self.balls_model_part.Elements:

            element.SetValue(SKIN_SPHERE, 0)

            node = element.GetNode(0)
            r = node.GetSolutionStepValue(RADIUS)
            x = node.X
            y = node.Y
            z = node.Z
            
            cross_section = 3.141592 * r * r

            if ((x * x + z * z) >= ((d / 2 - eps * r) * (d / 2 - eps * r))):

                element.SetValue(SKIN_SPHERE, 1)
                self.LAT.append(node)

                if ((y > eps * r) and (y < (h - eps * r))):

                    self.SKIN.append(element)
                    self.XLAT.append(node)
                    
                    xlat_area = xlat_area + cross_section

            if ((y <= eps * r) or (y >= (h - eps * r))):

                element.SetValue(SKIN_SPHERE, 1)
                self.SKIN.append(element)

                if (y <= eps * r):

                    self.BOT.append(node)
                    y_bot_total += y*r
                    weight_bot += r

                elif (y >= (h - eps * r)):

                    self.TOP.append(node)
                    
                    y_top_total += y*r
                    weight_top += r
         

                if ((x * x + z * z) >= ((d / 2 - eps * r) * (d / 2 - eps * r))):

                    if (y > h / 2):

                        self.XTOPCORNER.append(node)
                        xtopcorner_area = xtopcorner_area + cross_section

                    else:

                        self.XBOTCORNER.append(node)
                        xbotcorner_area = xbotcorner_area + cross_section
                else:

                    if (y <= eps * r):

                        self.XBOT.append(node)
                        xbot_area = xbot_area + cross_section

                    elif (y >= (h - eps * r)):

                        self.XTOP.append(node)
                        xtop_area = xtop_area + cross_section
        #checks:
            
        if(len(self.XLAT)==0):
            
            print("ERROR! in Cylinder Skin Determination - NO LATERAL PARTICLES", "\n")
        
        else:
          
            print("End ", h, "x", d, "Cylinder Skin Determination", "\n")

        return (xtop_area, xbot_area, xlat_area, xtopcorner_area, xbotcorner_area, y_top_total, weight_top, y_bot_total, weight_bot)
  
  #-------------------------------------------------------------------------------------#

  def BtsSkinDetermination(self):

      # SKIN DETERMINATION

      # Cylinder dimensions

      h = self.parameters.SpecimenLength
      d = self.parameters.SpecimenDiameter
      eps = 2.0

      for element in self.balls_model_part.Elements:

          element.SetValue(SKIN_SPHERE, 0)

          node = element.GetNode(0)
          r = node.GetSolutionStepValue(RADIUS)
          x = node.X
          y = node.Y
          z = node.Z

          if ((x * x + y * y) >= ((d / 2 - eps * r) * (d / 2 - eps * r))):

              element.SetValue(SKIN_SPHERE, 1)

          if ((z <= eps * r) or (z >= (h - eps * r))):

              element.SetValue(SKIN_SPHERE, 1)

      print("End 30x15 Bts Skin Determination", "\n")
     
  #-------------------------------------------------------------------------------------#
  
  def PrepareDataForGraph(self):
    
    prepare_check = [0,0,0,0]
    self.total_check = 0
    
    for mesh_number in range(1, self.RigidFace_model_part.NumberOfMeshes()):
      
      if(self.RigidFace_model_part.GetMesh(mesh_number)[TOP]):
        
        self.top_mesh_nodes = self.RigidFace_model_part.GetMesh(mesh_number).Nodes
        prepare_check[0] = 1
        
      if(self.RigidFace_model_part.GetMesh(mesh_number)[BOTTOM]):
        
        self.bot_mesh_nodes = self.RigidFace_model_part.GetMesh(mesh_number).Nodes
        prepare_check[1] = 1
      
    for mesh_number in range(1, self.balls_model_part.NumberOfMeshes()):
      
      if(self.balls_model_part.GetMesh(mesh_number)[TOP]):
        
        self.top_mesh_nodes = self.balls_model_part.GetMesh(mesh_number).Nodes
        prepare_check[2] = -1
        
      if(self.balls_model_part.GetMesh(mesh_number)[BOTTOM]):
        
        self.bot_mesh_nodes = self.balls_model_part.GetMesh(mesh_number).Nodes
        prepare_check[3] = -1
        
    for it in range(len(prepare_check)):
      
      self.total_check += prepare_check[it]
      
    if(math.fabs(self.total_check)!=2):
      
      print(" ERROR in the definition of TOP BOT groups. Both groups are required to be defined, they have to be either on FEM groups or in DEM groups")
   
  #-------------------------------------------------------------------------------------#
  
  def MeasureForcesAndPressure(self):
    
    dt = self.balls_model_part.ProcessInfo.GetValue(DELTA_TIME)

    self.strain += -100*self.length_correction_factor*1.0*self.parameters.LoadingVelocityTop*dt/self.parameters.SpecimenLength

    if( self.parameters.TestType =="BTS"):

      total_force_bts = 0.0
      
      for node in self.top_mesh_nodes:

        force_node_y = node.GetSolutionStepValue(ELASTIC_FORCES)[1] 
        total_force_bts += force_node_y
        
      self.total_stress_bts = 2.0*total_force_bts/(3.14159*self.parameters.SpecimenLength*self.parameters.SpecimenDiameter*1e6)
      self.strain_bts += -100*2*self.parameters.LoadingVelocityTop*dt/self.parameters.SpecimenDiameter
    else:

      radial_strain = -100*self.MeasureRadialStrain()
      self.volumetric_strain = self.strain + 2.0*radial_strain

      total_force_top = 0.0
      total_force_bot = 0.0
      
      for node in self.top_mesh_nodes:

        force_node_y = node.GetSolutionStepValue(ELASTIC_FORCES)[1]

        total_force_top += force_node_y
        
      self.total_stress_top = total_force_top/(self.parameters.MeasuringSurface*1000000)

      for node in self.bot_mesh_nodes:

        force_node_y = -node.GetSolutionStepValue(ELASTIC_FORCES)[1]

        total_force_bot += force_node_y

      self.total_stress_bot = total_force_bot/(self.parameters.MeasuringSurface*1000000)

      self.total_stress_mean = 0.5*(self.total_stress_bot + self.total_stress_top)

      
      if( ( (self.parameters.TestType == "Triaxial") or (self.parameters.TestType == "Hydrostatic") ) and (self.parameters.ConfinementPressure != 0.0) ):
      
          self.Pressure = min(self.total_stress_mean*1e6, self.parameters.ConfinementPressure * 1e6)
          
          self.ApplyLateralPressure(self.Pressure, self.XLAT, self.XBOT, self.XTOP, self.XBOTCORNER, self.XTOPCORNER,self.alpha_top,self.alpha_bot,self.alpha_lat)
            
    
  #-------------------------------------------------------------------------------------#
  
  def PrintGraph(self,step):
    
    if(self.graph_counter == self.graph_frequency):
      
      self.graph_counter = 0
      
      if(self.parameters.TestType == "BTS"):
        
        self.bts_export.write(str(self.strain_bts)+"  "+str(self.total_stress_bts)+'\n')
        self.bts_export.flush()
      
      else:
      
        self.graph_export.write(str(self.strain)+"    "+str(self.total_stress_mean)+'\n')
        self.graph_export_1.write(str(self.strain)+"    "+str(self.total_stress_top)+'\n')
        self.graph_export_2.write(str(self.strain)+"    "+str(self.total_stress_bot)+'\n')
        self.graph_export.flush()
        self.graph_export_1.flush()
        self.graph_export_2.flush()
        
        self.graph_export_volumetric.write(str(self.volumetric_strain)+"    "+str(self.total_stress_mean)+'\n')
        self.graph_export_radial1.write(str(self.total_stress_mean)+"    "+str(self.radial_strain)+'\n')
        self.graph_export_radial2.write(str(self.total_stress_mean)+"    "+str(self.strain)+'\n')
        self.graph_export_radial3.write(str(self.total_stress_mean)+"    "+str(self.volumetric_strain)+'\n')
        self.graph_export_volumetric.flush()
        self.graph_export_radial1.flush()
        self.graph_export_radial2.flush()
        self.graph_export_radial3.flush()
        
    self.graph_counter += 1 
    
   #-------------------------------------------------------------------------------------#  
   
  def PrintChart(self):
    
    loading_velocity = self.parameters.LoadingVelocityTop
  
    print ('************DEM VIRTUAL LAB******************'+'\n')
    print ('Loading velocity: ' + str(loading_velocity) + '\n')
    print ('Expected maximum deformation: ' + str(-loading_velocity*self.parameters.FinalTime/self.height*100) +'%'+'\n'+'\n'  )

    self.chart.write(("***********PARAMETERS*****************")+'\n')
    self.chart.write( "                                    " +'\n')
    self.chart.write( "    DENSI  = " + (str(self.parameters.w_densi))+" Kg/m3     "+'\n')
    self.chart.write( "    STAFRC = " + (str(self.parameters.InternalFriction))+"           "+'\n')
    self.chart.write( "    DYNFRC = " + (str(self.parameters.w_dynfrc))+"          " +'\n')
    self.chart.write( "    YOUNG  = " + (str(self.parameters.w_young/1e9))+" GPa"+"     " +'\n')
    self.chart.write( "    POISS  = " + (str(self.parameters.w_poiss))+"           " +'\n')
    self.chart.write( "    FTS    = " + (str(self.parameters.NormalTensileStrength))+" Mpa        " +'\n')
    self.chart.write( "    LCS1   = " + (str(self.parameters.LCS1))+" Mpa       " +'\n')
    self.chart.write( "    LCS2   = " + (str(self.parameters.LCS2))+" Mpa       " +'\n')
    self.chart.write( "    LCS3   = " + (str(self.parameters.LCS3))+" Mpa       " +'\n')
    self.chart.write( "    YRC1   = " + (str(self.parameters.YRC1))+"           " +'\n')
    self.chart.write( "    YRC2   = " + (str(self.parameters.YRC2))+"           " +'\n')
    self.chart.write( "    YRC3   = " + (str(self.parameters.YRC3))+"           " +'\n')
    self.chart.write( "    FSS    = " + (str(self.parameters.TangentialStrength))+" Mpa       " +'\n')
    self.chart.write( "    YEP    = " + (str(self.parameters.PlasticYoungModulus/1e9))+" GPa"+"     " +'\n')
    self.chart.write( "    YIELD  = " + (str(self.parameters.PlasticYieldStress))+" Mpa       " +'\n')
    self.chart.write( "    EDR    = " + (str(self.parameters.DamageDeformationFactor))+"           " +'\n')
    self.chart.write( "    GDAMP  = " + (str(self.parameters.GlobalForceReduction))+"           " +'\n')
    self.chart.write( "    LDAMP  = " + (str(self.parameters.LocalDampingFactor))+"           " +'\n')
    self.chart.write( "    ALPHA  = " + str(1.00) +"           " +'\n')
    self.chart.write( "                                    " +'\n')
    self.chart.write( "**************************************" +'\n')

    self.chart.close()
    
    a_chart = open(self.parameters.problem_name + "_Parameter_chart.grf","r")
    for line in a_chart.readlines():
      print(line)
    a_chart.close()

  #-------------------------------------------------------------------------------------#  
  
  def FinalizeGraphs(self):
  
    os.chdir(self.graphs_path)

    #Create a copy and renaming
    
    for filename in os.listdir("."):
      
      if filename.startswith(self.parameters.problem_name + "_graph.grf"):
        shutil.copy(filename, filename+"COPY")
        os.rename(filename+"COPY", self.parameters.problem_name + "_graph_" + str(self.initial_time) + ".grf")
      
      if filename.startswith(self.parameters.problem_name + "_bts.grf"):
        shutil.copy(filename, filename+"COPY")
        os.rename(filename+"COPY", self.parameters.problem_name + "_bts_" + str(self.initial_time) + ".grf")
      
      if filename.startswith(self.parameters.problem_name + "_graph_VOL.grf"):
        shutil.copy(filename, filename+"COPY")
        os.rename(filename+"COPY", self.parameters.problem_name + "_graph_VOL" + str(self.initial_time) + ".grf")

    if(self.parameters.TestType == "BTS"):
      
      self.bts_export.close()
      self.bts_stress_export.close()
    
    else:
    
      self.graph_export.close()
      self.graph_export_volumetric.close()
   
  #-------------------------------------------------------------------------------------#  
  
  def OrientationStudy(self,contact_model_part,step):
    
    os.chdir(self.post_path)
    
    OrientationChart = open("OrientationChart_"+str(step), 'w')
    
    counter = 1
    
    for element in contact_model_part.Elements:

      u1 = element.GetNode(1).X - element.GetNode(0).X
      u2 = element.GetNode(1).Y - element.GetNode(0).Y
      u3 = element.GetNode(1).Z - element.GetNode(0).Z
      
      alpha = abs(math.asin(abs(u2)/math.sqrt((u1*u1)+(u2*u2)+(u3*u3))))
      
      alpha_deg = alpha/math.pi*180
      
      element.SetValue(CONTACT_ORIENTATION,alpha_deg)
      
      sigma = element.GetValue(CONTACT_SIGMA) 
    
      OrientationChart.write(str(counter)+"    "+str(sigma/(self.total_stress_mean*1e6))+'\n')
      counter += 1
    
      if(alpha_deg >= 0.0 and alpha_deg < 5.0):
        self.bond_00_05.append(element)

      if(alpha_deg >= 5.0 and alpha_deg < 10.0):
        self.bond_05_10.append(element)
        
      if(alpha_deg >= 10.0 and alpha_deg < 15.0):
        self.bond_10_15.append(element)
        
      if(alpha_deg >= 15.0 and alpha_deg < 20.0):
        self.bond_15_20.append(element)
        
      if(alpha_deg >= 20.0 and alpha_deg < 25.0):
        self.bond_20_25.append(element)
        
      if(alpha_deg >= 25.0 and alpha_deg < 30.0):
        self.bond_25_30.append(element)
        
      if(alpha_deg >= 30.0 and alpha_deg < 35.0):
        self.bond_30_35.append(element)
      
      if(alpha_deg >= 35.0 and alpha_deg < 40.0):
        self.bond_35_40.append(element)
      
      if(alpha_deg >= 40.0 and alpha_deg < 45.0):
        self.bond_40_45.append(element)
        
      if(alpha_deg >= 45.0 and alpha_deg < 50.0):
        self.bond_45_50.append(element)
        
      if(alpha_deg >= 50.0 and alpha_deg < 55.0):
        self.bond_50_55.append(element)
        
      if(alpha_deg >= 55.0 and alpha_deg < 60.0):
        self.bond_55_60.append(element)
      
      if(alpha_deg >= 60.0 and alpha_deg < 65.0):
        self.bond_60_65.append(element)
        
      if(alpha_deg >= 65.0 and alpha_deg < 70.0):
        self.bond_65_70.append(element)
        
      if(alpha_deg >= 70.0 and alpha_deg < 75.0):
        self.bond_70_75.append(element)
            
      if(alpha_deg >= 75.0 and alpha_deg < 80.0):
        self.bond_75_80.append(element)
        
      if(alpha_deg >= 80.0 and alpha_deg < 85.0):
        self.bond_80_85.append(element)
        
      if(alpha_deg >= 85.0 and alpha_deg < 90.0):
        self.bond_85_90.append(element)
    

    ii=0 
    for item in [self.bond_00_05, self.bond_05_10, self.bond_10_15, self.bond_15_20, self.bond_20_25, self.bond_25_30, self.bond_30_35, self.bond_35_40, self.bond_40_45,  self.bond_45_50, self.bond_50_55, self.bond_55_60, self.bond_60_65, self.bond_65_70, self.bond_70_75, self.bond_75_80, self.bond_80_85, self.bond_85_90]:
      
      self.sizes[ii] = len(item)  
      
      i = 0.0
      sigma_sum =0.0
      tau_sum = 0.0
      
      sigma_total_sum_squared = 0
      tau_total_sum_squared = 0.0
      
      volume = 0.0
      area = 0.0

      for element in item:
        
        sigma_normal = element.GetValue(CONTACT_SIGMA)
        sigma_tau = element.GetValue(CONTACT_TAU)
        
        sigma_sum += sigma_normal
        tau_sum += sigma_tau
        
        sigma_partial_sum_squared = sigma_normal ** 2.0
        sigma_total_sum_squared += sigma_partial_sum_squared
        
        tau_partial_sum_squared = sigma_tau ** 2.0
        tau_total_sum_squared += tau_partial_sum_squared
        
        i += 1.0

      sigma_mean = sigma_sum / len(item)
      sigma_var = sigma_total_sum_squared / len(item) - sigma_mean ** 2.0
      
      sigma_std_dev = 0.0

      if(abs(sigma_var) > 1e-9):
          std_dev = sigma_var ** 0.5

      sigma_rel_std_dev = sigma_std_dev / sigma_mean
      
      tau_mean = tau_sum/ len(item)
      tau_var = tau_total_sum_squared / len(item) - tau_mean ** 2.0
      
      tau_std_dev = 0.0

      if(abs(tau_var) > 1e-9):
          tau_std_dev = tau_var ** 0.5

      tau_rel_std_dev = tau_std_dev / tau_mean
      
      self.sigma_mean_table[ii] = sigma_mean
      self.sigma_rel_std_dev_table[ii] = sigma_rel_std_dev
      self.tau_mean_table[ii] = tau_mean   
      self.tau_rel_std_dev_table[ii] = tau_rel_std_dev
      self.sigma_ratio_table[ii]=sigma_mean/(self.total_stress_mean*1e6)
      ii+=1
      
    print(self.sigma_ratio_table)
    OrientationChart.close()
    
  #-------------------------------------------------------------------------------------#  
    
  def ApplyLateralPressure(self, Pressure, XLAT, XBOT, XTOP, XBOTCORNER, XTOPCORNER, alpha_top, alpha_bot, alpha_lat):

      for node in XLAT:
          
          r = node.GetSolutionStepValue(RADIUS)
          x = node.X
          y = node.Y
          z = node.Z

          values = Array3()
          vect = Array3()
          
          cross_section = 3.141592 * r * r

          # vector normal al centre:
          vect_moduli = math.sqrt(x * x + z * z)

          if(vect_moduli > 0.0):
              vect[0] = -x / vect_moduli
              vect[1] = 0
              vect[2] = -z / vect_moduli

          values[0] = cross_section * alpha_lat * Pressure * vect[0]
          values[1] = 0.0
          values[2] = cross_section * alpha_lat * Pressure * vect[2]
          
          node.SetSolutionStepValue(EXTERNAL_APPLIED_FORCE, values)
                  
      for node in XTOPCORNER:

          r = node.GetSolutionStepValue(RADIUS)
          x = node.X
          y = node.Y
          z = node.Z

          values = Array3()
          vect = Array3()
          
          cross_section = 3.141592 * r * r

          # vector normal al centre:
          vect_moduli = math.sqrt(x * x + z * z)

          if(vect_moduli > 0.0):
              vect[0] = -x / vect_moduli
              vect[1] = 0
              vect[2] = -z / vect_moduli

          values[0] = cross_section * alpha_lat * Pressure * vect[0] * 0.70710678
          values[1] = 0.0
          values[2] = cross_section * alpha_lat * Pressure * vect[2] * 0.70710678

          node.SetSolutionStepValue(EXTERNAL_APPLIED_FORCE, values)

      for node in XBOTCORNER:

          r = node.GetSolutionStepValue(RADIUS)
          x = node.X
          y = node.Y
          z = node.Z

          values = Array3()
          vect = Array3()
          
          cross_section = 3.141592 * r * r

          # vector normal al centre:
          vect_moduli = math.sqrt(x * x + z * z)

          if(vect_moduli > 0.0):
              vect[0] = -x / vect_moduli
              vect[1] = 0
              vect[2] = -z / vect_moduli

          values[0] = cross_section * alpha_lat * Pressure * vect[0] * 0.70710678
          values[1] = 0.0
          values[2] = cross_section * alpha_lat * Pressure * vect[2] * 0.70710678

          node.SetSolutionStepValue(EXTERNAL_APPLIED_FORCE, values)

  #-------------------------------------------------------------------------------------#  
  
  def MeasureRadialStrain(self):
      
    mean_radial_strain = 0.0
    radial_strain = 0.0
    weight = 0.0
    
    for node in self.XLAT:
      
      r = node.GetSolutionStepValue(RADIUS)
      x = node.X
      z = node.Z
      
      x0 = node.X0
      z0 = node.Z0
      
      dist_initial = math.sqrt(x0 * x0 + z0 * z0)
      dist_now = math.sqrt(x * x + z * z)    
      node_radial_strain = (dist_now - dist_initial) / dist_initial      
      mean_radial_strain += node_radial_strain
      
      weight += 1.0
    radial_strain = mean_radial_strain/weight
    return radial_strain
    
    #-------------------------------------------------------------------------------------#  

  def PoissonMeasure(self):
    
    print("Not Working now")
    #left_nodes = list()
    #right_nodes = list()

    #xleft_weight  = 0.0         
    #xright_weight  = 0.0

    #left_counter = 0.0
    #right_counter = 0.0

    #if(self.parameters.PoissonMeasure == "ON"):
            
          #for node in balls_model_part.Nodes:
            
            #if (node.GetSolutionStepValue(GROUP_ID)==4):
              
              #left_nodes.append(node)
              #xleft_weight = +(node.X0 - node.GetSolutionStepValue(RADIUS))*node.GetSolutionStepValue(RADIUS)
              #left_counter = +node.GetSolutionStepValue(RADIUS)
              
            #elif(node.GetSolutionStepValue(GROUP_ID)==8):
              
              #right_nodes.append(node)
              #xright_weight = +(node.X + node.GetSolutionStepValue(RADIUS))*node.GetSolutionStepValue(RADIUS)
              #right_counter = +node.GetSolutionStepValue(RADIUS)
              
          #width_ini = xright_weight/right_counter - xleft_weight/left_counter
          
          
   ##################################POISSON##################################
    
    #if(self.parameters.PoissonMeasure == "ON"):
                
      #xleft_weight  = 0.0         
      #xright_weight  = 0.0

      #left_counter = 0.0
      #right_counter = 0.0

      #for node in left_nodes:
        
        #xleft_weight = +(node.X - node.GetSolutionStepValue(RADIUS))*node.GetSolutionStepValue(RADIUS)
        #left_counter = +node.GetSolutionStepValue(RADIUS)
        
      #for node in right_nodes:
        
        #xright_weight = +(node.X + node.GetSolutionStepValue(RADIUS))*node.GetSolutionStepValue(RADIUS)
        #right_counter = +node.GetSolutionStepValue(RADIUS)
      
      #width_now = xright_weight/right_counter - xleft_weight/left_counter

      #measured_poisson =  ((width_now-width_ini)/width_ini)/strain
      
      #graph_export_poisson.write(str(strain)+"  "+str(measured_poisson)+'\n')
        
  
  #-------------------------------------------------------------------------------------#  
    
   
 
