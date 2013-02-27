/*
==============================================================================
KratosIncompressibleFluidApplication 
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi
pooyan@cimne.upc.edu 
rrossi@cimne.upc.edu
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain


Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/
 
//   
//   Project Name:        Kratos       
//   Last Modified by:    $Author: kkamran $
//   Date:                $Date: 2008-10-13 06:58:23 $
//   Revision:            $Revision: 1.2 $
//
//


#if !defined(KRATOS_BIPHASIC_FILLING_UTILITIES_INCLUDED )
#define  KRATOS_BIPHASIC_FILLING_UTILITIES_INCLUDED



// System includes
#include <string>
#include <iostream> 
#include <algorithm>

// External includes 


// Project includes
#include "includes/define.h"
#include "includes/model_part.h"
#include "includes/node.h"
#include "utilities/geometry_utilities.h"
#include "utilities/enrichment_utilities.h"
//#include "geometries/tetrahedra_3d_4.h"
#include "geometries/point.h"
#include "thermo_mechanical_application.h"
// #include "custom_conditions/environment_contact.h"
//#include "includes/variables.h"



namespace Kratos
{
 	
class BiphasicFillingUtilities
 {
  public:

		//**********************************************************************************************
		//**********************************************************************************************
		double CreateAutoExitAssignAirSmagorinsky(ModelPart& ThisModelPart, double y_wall, double C_Smagorinsky)
		{			
			KRATOS_TRY;
			int node_size = ThisModelPart.Nodes().size();
			for (int ii = 0; ii < node_size; ii++)
			 {
                 ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
				 double str_flag = it->GetValue(IS_STRUCTURE);
				 double slip_flag = it->GetSolutionStepValue(IS_SLIP);

				 if (str_flag == 0.0 && slip_flag>=10.0)
					 return 1.0;
			 }
			// if there is no dry node
		    double is_dry_node = 0.0;
#pragma omp parallel for firstprivate(node_size)
			for (int ii = 0; ii < node_size; ii++)
			 {
                ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
				double dist = it->FastGetSolutionStepValue(DISTANCE);
				double slip_flag = it->GetSolutionStepValue(IS_SLIP);
				double str_flag = it->GetValue(IS_STRUCTURE);

				if(dist > 0.0){
					is_dry_node = 1.0;
					//slip_flag=10.0 refers to the boundary nodes with well-defined normal
					if(slip_flag==10.0 && str_flag!=0.0){
						it->SetValue(IS_STRUCTURE,0.0); 
						it->SetValue(Y_WALL,y_wall); 
					}
				}
			 }

			//assign smagorinsky at air element
			AirSmagorinskey(ThisModelPart, C_Smagorinsky);

		    return is_dry_node;
		
			KRATOS_CATCH("")
		}
	//**********************************************************************************************
/*for node in fluid_model_part.Nodes:
	slip_flag = node.GetSolutionStepValue(IS_SLIP)
	nd_dist = node.GetSolutionStepValue(DISTANCE)
        if((slip_flag == 20.0 or slip_flag == 30.0 )):# 
	  if(nd_dist< 0.0):
	    node.SetValue(IS_STRUCTURE,1.0)	  
	    node.SetValue(Y_WALL,y_wall_val)
	  else:
	    node.SetValue(IS_STRUCTURE,0.0)	  
	    node.SetValue(Y_WALL,y_wall_val*y_wall_fac)	    

	if(slip_flag == 10.0):
	  if(nd_dist< 0.0):
	    node.SetValue(Y_WALL,y_wall_val)	      
	    node.SetValue(IS_STRUCTURE,1.0)	  
	  else:
	    node.SetValue(Y_WALL,y_wall_val*y_wall_fac)
	    node.SetValue(IS_STRUCTURE,0.0)*/
	//**********************************************************************************************
    double AssignSmoothBoundaryAirExit(ModelPart& ThisModelPart, bool air_exit_flag, const double y_wall_val, const double  y_wall_fac)
		{			
		  KRATOS_TRY;
			int node_size = ThisModelPart.Nodes().size();
			double is_str = 1.0;
			if(air_exit_flag) is_str = 0.0;

			int wet_nodes = 0;

#pragma omp parallel for firstprivate(node_size)
			for (int ii = 0; ii < node_size; ii++)
			 {

                 ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
				double dist = it->FastGetSolutionStepValue(DISTANCE);
				double slip_flag = it->GetSolutionStepValue(IS_SLIP);
				if(dist<0.0){
				 #pragma omp atomic
				 wet_nodes++;}

				if(slip_flag == 20.0 || slip_flag == 30.0 )//edges(20) and corners(30) are automatic air exits till they are wetten
				  if(dist<0.0){
					    it->SetValue(IS_STRUCTURE,1.0);	  
					    it->SetValue(Y_WALL,y_wall_val);}
				  else{
						it->SetValue(IS_STRUCTURE,0.0);	  
						it->SetValue(Y_WALL,y_wall_val*y_wall_fac);}	
				else if(slip_flag == 10.0)//smooth boundaries(10), if dry, can be air exit or not
				  {
				  if(dist<0.0){
					    it->SetValue(IS_STRUCTURE,1.0);	  
					    it->SetValue(Y_WALL,y_wall_val);}
				  else{
						it->SetValue(IS_STRUCTURE,is_str);	  
						it->SetValue(Y_WALL,y_wall_val*y_wall_fac);}	
				  }
			}
			double filling_percent = 0.0;

			if(wet_nodes != 0) filling_percent = 100.0*double(wet_nodes)/double(node_size);

			return filling_percent;
		  KRATOS_CATCH("")
		}
	//**********************************************************************************************
	//**********************************************************************************************
    void ApplyFluidProperties(ModelPart& ThisModelPart, const double water_mu, const double water_density ,const double air_mu,const double air_density)
		{	
		  KRATOS_TRY;
		 int node_size = ThisModelPart.Nodes().size();

#pragma omp parallel for firstprivate(node_size)
		 for (int ii = 0; ii < node_size; ii++)
		 {
           ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
		   double dist = it->FastGetSolutionStepValue(DISTANCE);
		   if(dist<=0.0){
                it->FastGetSolutionStepValue(DENSITY) = water_density;
                it->FastGetSolutionStepValue(VISCOSITY) = water_mu;
		   }
		   else
		   {
                it->FastGetSolutionStepValue(DENSITY) = air_density;
                it->FastGetSolutionStepValue(VISCOSITY) = air_mu;
		   }
		 }
		  KRATOS_CATCH("")
		}
	//**********************************************************************************************
	//**********************************************************************************************
    void DistanceFarRegionCorrection(ModelPart& ThisModelPart, const double max_distance)
		{	
		  KRATOS_TRY;
		    int node_size = ThisModelPart.Nodes().size();

    #pragma omp parallel for firstprivate(node_size)
		    for (int ii = 0; ii < node_size; ii++)
		    {
		      ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
		      double& current_dist = it->FastGetSolutionStepValue(DISTANCE);		  
		      const double old_dist = it->FastGetSolutionStepValue(DISTANCE,1);	
		      
		      if( fabs(old_dist) >= 0.8*max_distance && current_dist*old_dist <= 0.0)
			current_dist = old_dist;
		    }
		  KRATOS_CATCH("")
		}
	//**********************************************************************************************
	//**********************************************************************************************
    void VolumeCorrection(ModelPart& ThisModelPart, const double Net_volume)
	    {	
	      KRATOS_TRY;	
	      
	      double wet_volume = 0.0;
	      double cutted_area = 0.0;
	      int node_size = ThisModelPart.Nodes().size();	      
// #pragma omp parallel for firstprivate(node_size) reduction(+:wet_volume,cutted_area )
// 		for (int ii = 0; ii < node_size; ii++)
// 		{
// 		  ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
// 		  
// 		  wet_volume += it->FastGetSolutionStepValue(WET_VOLUME);
// 		  cutted_area += it->FastGetSolutionStepValue(CUTTED_AREA);
// 		}
		
		ComputeWetVolumeAndCuttedArea(ThisModelPart, wet_volume, cutted_area);
		
		double volume_difference = fabs(Net_volume) - wet_volume;
		volume_difference /= cutted_area;

#pragma omp parallel for firstprivate(node_size)		
		for (int ii = 0; ii < node_size; ii++)
		{
		  ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
		  
		  it->FastGetSolutionStepValue(DISTANCE) -= volume_difference;
		  
		}
std::cout << "Volume Correction " << " Net volume: "<< fabs(Net_volume) << " wet volume: " << wet_volume << " percent: "<< wet_volume/fabs(Net_volume)<< " Area: "<< cutted_area << std::endl;
	      KRATOS_CATCH("")
	    }
	//**********************************************************************************************
	//**********************************************************************************************
    void ComputeNetInletVolume(ModelPart& ThisModelPart)
	    {	
	      KRATOS_TRY;
	      double net_input = 0.0;	      
	      int node_size = ThisModelPart.Nodes().size();	      
#pragma omp parallel for firstprivate(node_size) reduction(+:net_input)
		for (int ii = 0; ii < node_size; ii++)
		{
		  ModelPart::NodesContainerType::iterator it = ThisModelPart.NodesBegin() + ii;
		  double str_flag = it->GetValue(IS_STRUCTURE);
		  double slip_flag = it->GetSolutionStepValue(IS_SLIP);
		  double distance = it->GetSolutionStepValue(DISTANCE);
		  
		  if ( (str_flag != 0.0 || slip_flag == 0.0) && distance < 0.0 ){
		  const array_1d<double, 3> vel = it->FastGetSolutionStepValue(VELOCITY);
		  const array_1d<double, 3> normal = it->FastGetSolutionStepValue(NORMAL);
		  
		  net_input += inner_prod(vel,normal);
		  }
		}
		
               ProcessInfo& CurrentProcessInfo = ThisModelPart.GetProcessInfo();
	       const double delta_t = CurrentProcessInfo[DELTA_TIME];
               double& net_volume = CurrentProcessInfo[NET_INPUT_MATERIAL];
	       net_volume += (net_input*delta_t);
	      
	      KRATOS_CATCH("")
	    }	      
	//**********************************************************************************************
	//**********************************************************************************************		  
  private:
 	
	void AirSmagorinskey(ModelPart& ThisModelPart, double C_Smagorinsky)
	{
        int elem_size = ThisModelPart.Elements().size();

        #pragma omp parallel for firstprivate(elem_size) 
        for(int ii = 0; ii<elem_size; ii++)
         {
            PointerVector< Element>::iterator iel=ThisModelPart.ElementsBegin()+ii;
			double dist_sign = 1.0;
            Geometry< Node<3> >& geom = iel->GetGeometry();
            for(unsigned int i =0; i<geom.size(); i++)
            {
				double dist = geom[i].FastGetSolutionStepValue(DISTANCE);
				if(dist_sign*dist < 0.0){
					dist_sign = -1.0;
					break;}
			}
			// to be sure to not apply to the cutted elements and just to all air elements
			if(dist_sign == 1.0)
				iel->SetValue(C_SMAGORINSKY, C_Smagorinsky );

		}
	}
	//**********************************************************************************************
	//**********************************************************************************************	
	void ComputeWetVolumeAndCuttedArea(ModelPart& ThisModelPart, double& wet_volume, double& cutted_area)
	{
	   KRATOS_TRY;
	    int elem_size = ThisModelPart.Elements().size();
	    double wetvol = 0.0;
	    double cutare = 0.0;

	    #pragma omp parallel for firstprivate(elem_size) reduction(+:wetvol,cutare)
	    for(int ii = 0; ii<elem_size; ii++)
	    {
	      PointerVector< Element>::iterator iel=ThisModelPart.ElementsBegin()+ii;	  

	      // Calculate this element's geometric parameters
	      double Area;
	      array_1d<double, 4> N;
	      boost::numeric::ublas::bounded_matrix<double, 4, 3> DN_DX;
	      GeometryUtils::CalculateGeometryData(iel->GetGeometry(), DN_DX, N, Area);
	      //get position of the cut surface
	      Vector distances(4);
	      Matrix Nenriched(6, 1);
	      Vector volumes(6);
	      Matrix coords(4, 3);
	      Matrix Ngauss(6, 4);
	      Vector signs(6);
	      std::vector< Matrix > gauss_gradients(6);
	      //fill coordinates
	      for (unsigned int i = 0; i < 4; i++)
	      {
		  const array_1d<double, 3 > & xyz = iel->GetGeometry()[i].Coordinates();
		  volumes[i] = 0.0;
		  distances[i] = iel->GetGeometry()[i].FastGetSolutionStepValue(DISTANCE);
		  for (unsigned int j = 0; j < 3; j++)
		      coords(i, j) = xyz[j];
	      }
	      for (unsigned int i = 0; i < 6; i++)
		  gauss_gradients[i].resize(1, 3, false);
	      unsigned int ndivisions = EnrichmentUtilities::CalculateTetrahedraEnrichedShapeFuncions(coords, DN_DX, distances, volumes, Ngauss, signs, gauss_gradients, Nenriched);      
	  
	      if(ndivisions == 1){		
		if( signs[0] < 0.0)
		    wetvol += volumes[0];}	      
	      else{
		double ele_wet_volume=0.0;
		for (unsigned int kk = 0; kk < ndivisions; kk++)
		{
		  if( signs[kk]<0.0 )
		    ele_wet_volume += volumes[kk];		  
		}
		wetvol += ele_wet_volume;
	        cutare += 7.205621731 * pow(ele_wet_volume,0.666666666667); // equilateral tetrahedraon is considered
	      }				
	    }
	    
	    wet_volume = wetvol;
	    cutted_area = cutare;
	    
	   KRATOS_CATCH("")	  
	}


 };

}  // namespace Kratos.

#endif // KRATOS_BIPHASIC_FILLING_UTILITIES_INCLUDED  defined 


