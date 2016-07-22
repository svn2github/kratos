//
//   Project Name:        KratosPfemBaseApplication $
//   Created by:          $Author:      JMCarbonell $
//   Last modified by:    $Co-Author:               $
//   Date:                $Date:      February 2016 $
//   Revision:            $Revision:            0.0 $
//
//

// Project includes
#include "includes/kratos_flags.h"
#include "custom_utilities/mesh_data_transfer_utilities.hpp"

#include "pfem_base_application_variables.h"

namespace Kratos
{

       KRATOS_CREATE_LOCAL_FLAG( MeshDataTransferUtilities, NODE_TO_ELEMENT,        0 );
       KRATOS_CREATE_LOCAL_FLAG( MeshDataTransferUtilities, ELEMENT_TO_NODE,        1 );
       KRATOS_CREATE_LOCAL_FLAG( MeshDataTransferUtilities, ELEMENT_TO_ELEMENT,     2 );
       KRATOS_CREATE_LOCAL_FLAG( MeshDataTransferUtilities, INITIALIZE_MASTER_CONDITION,        3 );
       KRATOS_CREATE_LOCAL_FLAG( MeshDataTransferUtilities, MASTER_ELEMENT_TO_MASTER_CONDITION, 4 );

  

	//*******************************************************************************************
	//*******************************************************************************************

        void MeshDataTransferUtilities::TransferData(ModelPart& rModelPart,
						     const Element & rReferenceElement,
						     PointPointerVector &list_of_new_centers,
						     std::vector<Geometry<Node<3> > >& list_of_new_vertices,
						     Flags Options,
						     ModelPart::IndexType MeshId)
	  
	{
	  KRATOS_TRY

	  ModelPart::MeshesContainerType Meshes = rModelPart.GetMeshes();
	  
	  
	    if(Options.Is(MeshDataTransferUtilities::NODE_TO_ELEMENT))
	      {
		TransferNodalValuesToElements(rModelPart,rReferenceElement,list_of_new_centers,list_of_new_vertices,MeshId);
	      }
	    else
	      {
		if(Options.Is(MeshDataTransferUtilities::ELEMENT_TO_ELEMENT)){
		  TransferElementalValuesToElements(rModelPart,rReferenceElement,list_of_new_centers,list_of_new_vertices,MeshId);
		}
		else
		  {
		    if(Options.Is(MeshDataTransferUtilities::ELEMENT_TO_NODE)){
		      TransferElementalValuesToNodes(rModelPart,rReferenceElement,list_of_new_centers,list_of_new_vertices,MeshId);
		    }
		  }
	      }
	    
	  KRATOS_CATCH( "" )

	}



	//*******************************************************************************************
	//*******************************************************************************************

        void MeshDataTransferUtilities::InitializeBoundaryData(Condition::Pointer rCondition,
							       const TransferParameters& rTransferVariables)
	{

	  KRATOS_TRY

	  unsigned int StrainSize;
	  const unsigned int dimension = rCondition->GetGeometry().WorkingSpaceDimension();
	  
	  if ( dimension == 2 )
	    StrainSize = 3;
	  else
	    StrainSize = 6;


	  double DoubleVariable = 0;
	  array_1d<double, 3> Array1DVariable;
	  Vector VectorVariable = ZeroVector(StrainSize);
	  Matrix MatrixVariable = identity_matrix<double>( dimension );


	  //double
	  for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
	    {
	      rCondition->SetValue(*(rTransferVariables.DoubleVariables[i]),DoubleVariable);
	    }
	  
	  //array_1d
	  for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
	    {
	      rCondition->SetValue(*(rTransferVariables.Array1DVariables[i]),Array1DVariable);
	    }
	  
	  //Vector
	  for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
	    {
	      rCondition->SetValue(*(rTransferVariables.VectorVariables[i]),VectorVariable);
	    }
	  
	  //Matrix
	  for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
	    {
	      rCondition->SetValue(*(rTransferVariables.MatrixVariables[i]),MatrixVariable);
	    }
	  
		    
	  KRATOS_CATCH( "" )

	}



	//*******************************************************************************************
	//*******************************************************************************************

        void MeshDataTransferUtilities::TransferBoundaryData(Condition::Pointer rCurrentCondition, 
							     Condition::Pointer rReferenceCondition, 
							     const TransferParameters& rTransferVariables)
	{
	  KRATOS_TRY

	  //double
	  for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
	    {
	      rCurrentCondition->SetValue(*(rTransferVariables.DoubleVariables[i]), rReferenceCondition->GetValue(*(rTransferVariables.DoubleVariables[i])) );
	    }
	  
	  //array_1d
	  for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
	    {
	      rCurrentCondition->SetValue(*(rTransferVariables.Array1DVariables[i]), rReferenceCondition->GetValue(*(rTransferVariables.Array1DVariables[i])) );
	    }
	  
	  //Vector
	  for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
	    {
	      rCurrentCondition->SetValue(*(rTransferVariables.VectorVariables[i]), rReferenceCondition->GetValue(*(rTransferVariables.VectorVariables[i])) );
	    }
	  
	  //Matrix
	  for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
	    {
	      rCurrentCondition->SetValue(*(rTransferVariables.MatrixVariables[i]), rReferenceCondition->GetValue(*(rTransferVariables.MatrixVariables[i])) );
	    }
	  
		    
	  KRATOS_CATCH( "" )

	}

	//*******************************************************************************************
	//*******************************************************************************************

        void MeshDataTransferUtilities::TransferBoundaryData(const TransferParameters& rTransferVariables,
							     ModelPart& rModelPart,
							     ModelPart::IndexType MeshId)
	{
	    KRATOS_TRY

	    if(rTransferVariables.Options.Is(MeshDataTransferUtilities::MASTER_ELEMENT_TO_MASTER_CONDITION))
	      {
		
		std::cout<<"  TRANSFER MASTER_ELEMENT_TO_MASTER_CONDITION "<<std::endl;

		ProcessInfo& CurrentProcessInfo = rModelPart.GetProcessInfo();
		unsigned int StrainSize;
		ModelPart::ElementsContainerType::iterator element_begin = rModelPart.ElementsBegin();
		const unsigned int dimension = element_begin->GetGeometry().WorkingSpaceDimension();
		
		if ( dimension == 2 )
		  StrainSize = 3;
		else
		  StrainSize = 6;

		double DoubleVariable = 0;
		array_1d<double, 3> Array1DVariable;
		Vector VectorVariable = ZeroVector(StrainSize);
		Matrix MatrixVariable = identity_matrix<double>( dimension );

		
		//initialize to zero all skin master-nodes
		for(ModelPart::ConditionsContainerType::iterator ic = rModelPart.ConditionsBegin(); ic!= rModelPart.ConditionsEnd(); ic++)
		  {
		    //double
		    for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.DoubleVariables[i]),DoubleVariable);
		      }
		    
		    //array_1d
		    for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.Array1DVariables[i]),Array1DVariable);
		      }
		    
		    //Vector
		    for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.VectorVariables[i]),VectorVariable);
		      }

		    //Matrix
		    for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.MatrixVariables[i]),MatrixVariable);
		      }
		    
		  }
		
	      


		//store that information in all body skin if there is a Contact Condition;
		for(ModelPart::ConditionsContainerType::iterator ic = rModelPart.ConditionsBegin(); ic!= rModelPart.ConditionsEnd(); ic++)
		  {

		    if(ic->Is(CONTACT) && ic->Is(ACTIVE)){
		      
		      //std::cout<<" Transfer: Cond: "<<ic->Id()<<" is Active "<<std::endl;

		      Element::ElementType& MasterElement    = ic->GetValue(MASTER_ELEMENTS)[0];
		      Condition::Pointer MasterCondition     = ic->GetValue(MASTER_CONDITION);

		      unsigned int integration_points_number = (MasterElement.pGetGeometry())->IntegrationPointsNumber(MasterElement.GetIntegrationMethod());
		      
		      std::vector<double> DoubleVariableArray (integration_points_number);
		      std::vector<array_1d<double,3> > Array1DVariableArray (integration_points_number);
		      std::vector<Vector> VectorVariableArray (integration_points_number);
		      std::vector<Matrix> MatrixVariableArray (integration_points_number);
		      
		      
		      //double
		      for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
			{
			  MasterElement.GetValueOnIntegrationPoints(*(rTransferVariables.DoubleVariables[i]),DoubleVariableArray,CurrentProcessInfo);

			  //if there is more than one integration point, an average or an interpolation is need		
			  DoubleVariable = DoubleVariableArray[0];
			  for(unsigned int j=1; j<integration_points_number; j++)
			    {
			      DoubleVariable  += DoubleVariableArray[j];
			    }
			  DoubleVariable *= (1.0/double(integration_points_number));
			  MasterCondition->SetValue(*(rTransferVariables.DoubleVariables[i]),DoubleVariable);
			}
		    
		      //array_1d
		      for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
			{
			  MasterElement.GetValueOnIntegrationPoints(*(rTransferVariables.Array1DVariables[i]),Array1DVariableArray,CurrentProcessInfo);

			  //if there is more than one integration point, an average or an interpolation is need		
			  Array1DVariable = Array1DVariableArray[0];
			  for(unsigned int j=1; j<integration_points_number; j++)
			    {
			      Array1DVariable += Array1DVariableArray[j];
			    }
			  Array1DVariable *= (1.0/double(integration_points_number));
			  MasterCondition->SetValue(*(rTransferVariables.Array1DVariables[i]),Array1DVariable);
			}
		      
		      //Vector
		      for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
			{
			
			  MasterElement.GetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),VectorVariableArray,CurrentProcessInfo);

			  //std::cout<<" TRANSFER EN "<<MasterElement.Id()<<" "<<*rTransferVariables.VectorVariables[i]<<" "<<VectorVariableArray[0]<<std::endl;

			  //if there is more than one integration point, an average or an interpolation is need		
			  VectorVariable = VectorVariableArray[0];
			  for(unsigned int j=1; j<integration_points_number; j++)
			    {
			      VectorVariable  += VectorVariableArray[j];
			    }
			  VectorVariable *= (1.0/double(integration_points_number));
			  MasterCondition->SetValue(*(rTransferVariables.VectorVariables[i]),VectorVariable);
			}

		      //Matrix
		      for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
			{

			  MasterElement.GetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),MatrixVariableArray,CurrentProcessInfo);

			  //std::cout<<" TRANSFER EN "<<MasterElement.Id()<<" "<<*rTransferVariables.MatrixVariables[i]<<" "<<MatrixVariableArray[0]<<std::endl;
			  
			  //if there is more than one integration point, an average or an interpolation is need		
			  MatrixVariable = MatrixVariableArray[0];
			  for(unsigned int j=1; j<integration_points_number; j++)
			    {
			      MatrixVariable  += MatrixVariableArray[i];
			    }
			  MatrixVariable *= (1.0/double(integration_points_number));
			  MasterCondition->SetValue(*(rTransferVariables.MatrixVariables[i]),MatrixVariable);
			}
		    
     

		      //std::cout<<" MasterCond: "<<MasterCondition->Id()<<" is Active "<<std::endl;		    

		    }
		  }


		    

	      }

	    if(rTransferVariables.Options.Is(MeshDataTransferUtilities::INITIALIZE_MASTER_CONDITION))		
	      {
		
		std::cout<<"  TRANSFER INITIALIZE MASTER CONDITION "<<std::endl;

		unsigned int StrainSize;
		
		//I don't use condition because when I create the boundary skin I must assign the 
		//correct geometry to the boundary face, not only some PointsArray
		//It must be done.
		ModelPart::ElementsContainerType::iterator element_begin = rModelPart.ElementsBegin();
		const unsigned int dimension = element_begin->GetGeometry().WorkingSpaceDimension();
		
		if ( dimension == 2 )
		  StrainSize = 3;
		else
		  StrainSize = 6;


		double DoubleVariable = 0;
		array_1d<double, 3> Array1DVariable;
		Vector VectorVariable = ZeroVector(StrainSize);
		Matrix MatrixVariable = identity_matrix<double>( dimension );

		
		//initialize to zero all skin master-nodes
		for(ModelPart::ConditionsContainerType::iterator ic = rModelPart.ConditionsBegin(); ic!= rModelPart.ConditionsEnd(); ic++)
		  {
		    //double
		    for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.DoubleVariables[i]),DoubleVariable);
		      }
		    
		    //array_1d
		    for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.Array1DVariables[i]),Array1DVariable);
		      }
		    
		    //Vector
		    for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.VectorVariables[i]),VectorVariable);
			//std::cout<<" TRANSFER "<<*rTransferVariables.VectorVariables[i]<<" "<<VectorVariable<<std::endl;
		      }

		    //Matrix
		    for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
		      {
			ic->SetValue(*(rTransferVariables.MatrixVariables[i]),MatrixVariable);
			//std::cout<<" TRANSFER "<<*rTransferVariables.MatrixVariables[i]<<" "<<MatrixVariable<<std::endl;
		      }
		    
		  }

	      }

	  KRATOS_CATCH( "" )

	}

  
      //*******************************************************************************************
      //*******************************************************************************************
	
      void MeshDataTransferUtilities::TransferNodalValuesToElements(const TransferParameters& rTransferVariables,
								    ModelPart& rModelPart,
								    ModelPart::IndexType MeshId)
	{

	    KRATOS_TRY
		    
	    //std::cout<<" [ Data Transfer NODE to ELEMENT ] :"<<std::endl;

	    double alpha = 0.25; //[0,1] //smoothing level of the Jacobian	      

	    Geometry<Node<3> >& rGeom = rModelPart.ElementsBegin(MeshId)->GetGeometry();
	    GeometryData::IntegrationMethod IntegrationMethod =  rGeom.GetDefaultIntegrationMethod();
	    unsigned int integration_points_number = rGeom.IntegrationPointsNumber( IntegrationMethod );

	    std::vector<double> NodesDoubleVariableArray (integration_points_number);
	    std::vector<array_1d<double,3> > NodesArray1DVariableArray (integration_points_number);
	    std::vector<Vector> NodesVectorVariableArray (integration_points_number);
	    std::vector<Matrix> NodesMatrixVariableArray (integration_points_number);

	    std::vector<double> ElementDoubleVariableArray (integration_points_number);
	    std::vector<array_1d<double,3> > ElementArray1DVariableArray (integration_points_number);
	    std::vector<Vector> ElementVectorVariableArray (integration_points_number);
	    std::vector<Matrix> ElementMatrixVariableArray (integration_points_number);
		      

	    ProcessInfo& CurrentProcessInfo = rModelPart.GetProcessInfo();

	    for(ModelPart::ElementsContainerType::const_iterator ie = rModelPart.ElementsBegin(MeshId); ie != rModelPart.ElementsEnd(MeshId); ie++)
	      {
		
		Geometry<Node<3> > & rGeometry = (ie)->GetGeometry();
		IntegrationMethod =  rGeometry.GetDefaultIntegrationMethod();
		integration_points_number = rGeometry.IntegrationPointsNumber( IntegrationMethod );

		const Matrix& Ncontainer = rGeometry.ShapeFunctionsValues( IntegrationMethod );

		//shape functions
		Vector N;

		//double
		for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
		  {
		    	      
		    //elemental value
		    (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.DoubleVariables[i]),ElementDoubleVariableArray,CurrentProcessInfo);

		    std::fill(NodesDoubleVariableArray.begin(), NodesDoubleVariableArray.end(), 0.0 );

		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			N = row( Ncontainer, j );

			//nodal value
			for( unsigned int k=0 ; k<rGeometry.size(); k++)
			  {
			    NodesDoubleVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.DoubleVariables[i]));
			  }
			
			NodesDoubleVariableArray[j] *= (alpha);
			NodesDoubleVariableArray[j] += (1-alpha) * ElementDoubleVariableArray[j];
						
		      }
		    
	    
		    (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.DoubleVariables[i]),NodesDoubleVariableArray,CurrentProcessInfo);

		  }

		//array_1d
		for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
		  {
		    	      
		    //elemental value
		    (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.Array1DVariables[i]),ElementArray1DVariableArray,CurrentProcessInfo);

		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			NodesArray1DVariableArray[j].clear();
		      }

		    
		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			N = row( Ncontainer, j );

			//nodal value
			for( unsigned int k=0 ; k<rGeometry.size(); k++)
			  {
			    NodesArray1DVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.Array1DVariables[i]));
			  }
			
			NodesArray1DVariableArray[j] *= (alpha);
			NodesArray1DVariableArray[j] += (1-alpha) * ElementArray1DVariableArray[j];
						
		      }
		    
	    
		    (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.Array1DVariables[i]),NodesArray1DVariableArray,CurrentProcessInfo);

		  }
		      
		//Vector
		for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
		  {
		    	      
		    //elemental value
		    (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),ElementVectorVariableArray,CurrentProcessInfo);


		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			// if( ElementVectorVariableArray[j].size() != 0 )
			//   NodesVectorVariableArray[j] = ZeroVector(ElementVectorVariableArray[j].size());
			// else
			  NodesVectorVariableArray[j] = ZeroVector(); //¿value?
		      }

		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			N = row( Ncontainer, j );

			//nodal value
			for( unsigned int k=0 ; k<rGeometry.size(); k++)
			  {
			    NodesVectorVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.VectorVariables[i]));
			  }
			
			NodesVectorVariableArray[j] *= (alpha);
			NodesVectorVariableArray[j] += (1-alpha) * ElementVectorVariableArray[j];
						
		      }
		    
	    
		    (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),NodesVectorVariableArray,CurrentProcessInfo);

		  }


		//Matrix

		for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
		  {
		    	      
		    //elemental value
		    (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),ElementMatrixVariableArray,CurrentProcessInfo);

		    
		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			// if( ElementMatrixVariableArray[j].size1() != 0 && ElementMatrixVariableArray[j].size2() != 0 )
			//   NodesMatrixVariableArray[j] = ZeroMatrix(ElementMatrixVariableArray[j].size1(),ElementMatrixVariableArray[j].size2());
			// else
			  NodesMatrixVariableArray[j] = ZeroMatrix(); //¿value?
		      }


		    for(unsigned int j=0; j<integration_points_number; j++)
		      {
			N = row( Ncontainer, j );

			//nodal value
			for( unsigned int k=0 ; k<rGeometry.size(); k++)
			  {
			    NodesMatrixVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i]));
			  }
			
			NodesMatrixVariableArray[j] *= (alpha);
			NodesMatrixVariableArray[j] += (1-alpha) * ElementMatrixVariableArray[j];
						
		      }
		    
	    
		    (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),NodesMatrixVariableArray,CurrentProcessInfo);

		  }


	      }

	    
	    //std::cout<<" [ Finished NODE to ELEMENT Transfer ]"<<std::endl;
	    
	    KRATOS_CATCH( "" )
	}
      

      //*******************************************************************************************
      //*******************************************************************************************
	
      //KRATOS MESH INPUT
      void MeshDataTransferUtilities::TransferNodalValuesToElements(const TransferParameters& rTransferVariables,
								    const Variable<double>& rCriticalVariable,
								    const double& rCriticalValue,
								    ModelPart& rModelPart,
								    ModelPart::IndexType MeshId)
	{

	    KRATOS_TRY
		    

	    //std::cout<<" [ Data Transfer NODE to ELEMENT ] : based on critical values of "<<rCriticalVariable<<std::endl;

	    double alpha = 0.25; //[0,1] //smoothing level of the Jacobian	      

	    Geometry<Node<3> >& rGeom = rModelPart.ElementsBegin(MeshId)->GetGeometry();
	    GeometryData::IntegrationMethod IntegrationMethod =  rGeom.GetDefaultIntegrationMethod();
	    unsigned int integration_points_number = rGeom.IntegrationPointsNumber( IntegrationMethod );


            std::vector<double> ComputedValues(integration_points_number);
	    double computed_value=0;
	    double critical_value=0;

	    ProcessInfo& CurrentProcessInfo = rModelPart.GetProcessInfo();

	    for(ModelPart::ElementsContainerType::const_iterator ie = rModelPart.ElementsBegin(MeshId); ie != rModelPart.ElementsEnd(MeshId); ie++)
	      {
	     
	    	(ie)->GetValueOnIntegrationPoints(rCriticalVariable,ComputedValues,CurrentProcessInfo);

		computed_value = ComputedValues[0];

		for(unsigned int j=1; j<integration_points_number; j++)
		  {
		    computed_value += ComputedValues[j];
		  }
		
		computed_value *= (ie)->GetGeometry().Area()/double(integration_points_number);

	    	critical_value = rCriticalValue;

	    	if( computed_value > critical_value )
	    	  {
	    	    for(unsigned int i = 0; i<ie->GetGeometry().size(); i++)
	    	      {
	    		(ie)->GetGeometry()[i].Set(TO_REFINE);
	    	      }
	    	  }
	      }

	    std::vector<double> NodesDoubleVariableArray (integration_points_number);
	    std::vector<array_1d<double,3> > NodesArray1DVariableArray (integration_points_number);
	    std::vector<Vector> NodesVectorVariableArray (integration_points_number);
	    std::vector<Matrix> NodesMatrixVariableArray (integration_points_number);

	    std::vector<double> ElementDoubleVariableArray (integration_points_number);
	    std::vector<array_1d<double,3> > ElementArray1DVariableArray (integration_points_number);
	    std::vector<Vector> ElementVectorVariableArray (integration_points_number);
	    std::vector<Matrix> ElementMatrixVariableArray (integration_points_number);
		
	    int counter = 0;

	    for(ModelPart::ElementsContainerType::const_iterator ie = rModelPart.ElementsBegin(MeshId); ie != rModelPart.ElementsEnd(MeshId); ie++)
	      {
		
		Geometry<Node<3> > & rGeometry = (ie)->GetGeometry();
		IntegrationMethod =  rGeometry.GetDefaultIntegrationMethod();
		integration_points_number = rGeometry.IntegrationPointsNumber( IntegrationMethod );
		const Matrix& Ncontainer = rGeometry.ShapeFunctionsValues( IntegrationMethod );

		//shape functions
		Vector N;

		bool apply_smoothing = false;
		for( unsigned int k=0 ; k<rGeometry.size(); k++)
		  {
		    if(rGeometry[k].Is(TO_REFINE))
		      apply_smoothing = true;
		  }
		
		if( apply_smoothing ){

		  counter++;
		  //double
		  for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
		    {
		    	      
		      //elemental value
		      (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.DoubleVariables[i]),ElementDoubleVariableArray,CurrentProcessInfo);

		      std::fill(NodesDoubleVariableArray.begin(), NodesDoubleVariableArray.end(), 0.0 );

		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  N = row( Ncontainer, j );

			  //nodal value
			  for( unsigned int k=0 ; k<rGeometry.size(); k++)
			    {
			      NodesDoubleVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.DoubleVariables[i]));
			      //std::cout<<" Node var ["<<k<<"] "<<rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.DoubleVariables[i]))<<std::endl;
			    }
			
			  NodesDoubleVariableArray[j] *= (alpha);
			  NodesDoubleVariableArray[j] += (1-alpha) * ElementDoubleVariableArray[j];
						
			}
		    
		      //std::cout<<" transfer ["<<ie->Id()<<"] "<<NodesDoubleVariableArray[0]<<" element "<<ElementDoubleVariableArray[0]<<std::endl;

		      (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.DoubleVariables[i]),NodesDoubleVariableArray,CurrentProcessInfo);

		    }

		  //array_1d
		  for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
		    {
		    	      
		      //elemental value
		      (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.Array1DVariables[i]),ElementArray1DVariableArray,CurrentProcessInfo);
		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  NodesArray1DVariableArray[j].clear();
			}

		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  N = row( Ncontainer, j );

			  //nodal value
			  for( unsigned int k=0 ; k<rGeometry.size(); k++)
			    {
			      NodesArray1DVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.Array1DVariables[i]));
			    }
			
			  NodesArray1DVariableArray[j] *= (alpha);
			  NodesArray1DVariableArray[j] += (1-alpha) * ElementArray1DVariableArray[j];
						
			}
		    
	    
		      (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.Array1DVariables[i]),NodesArray1DVariableArray,CurrentProcessInfo);

		    }
		      
		  //Vector
		  for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
		    {
		    	      
		      //elemental value
		      (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),ElementVectorVariableArray,CurrentProcessInfo);

		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  // if(ElementVectorVariableArray[j].size() != 0)
			  //   NodesVectorVariableArray[j] = ZeroVector(ElementVectorVariableArray[j].size());
			  // else
			    NodesVectorVariableArray[j] = ZeroVector(); //¿value?
			}

		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  N = row( Ncontainer, j );

			  //nodal value
			  for( unsigned int k=0 ; k<rGeometry.size(); k++)
			    {
			      NodesVectorVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.VectorVariables[i]));
			    }
			
			  NodesVectorVariableArray[j] *= (alpha);
			  NodesVectorVariableArray[j] += (1-alpha) * ElementVectorVariableArray[j];
						
			}
		    
	    
		      (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),NodesVectorVariableArray,CurrentProcessInfo);

		    }


		  //Matrix

		  for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
		    {
		    	      
		      //elemental value
		      (ie)->GetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),ElementMatrixVariableArray,CurrentProcessInfo);

		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  // if(ElementMatrixVariableArray[j].size1() !=0 && ElementMatrixVariableArray[j].size2() != 0)
			  //   NodesMatrixVariableArray[j]= ZeroMatrix(ElementMatrixVariableArray[j].size1(),ElementMatrixVariableArray[j].size2());
			  // else
			    NodesMatrixVariableArray[j] = ZeroMatrix(); //¿value?
			}

		      for(unsigned int j=0; j<integration_points_number; j++)
			{
			  N = row( Ncontainer, j );

			  //nodal value
			  for( unsigned int k=0 ; k<rGeometry.size(); k++)
			    {
			      NodesMatrixVariableArray[j] += N [k] * rGeometry[k].FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i]));
			    }
			
			  NodesMatrixVariableArray[j] *= (alpha);
			  NodesMatrixVariableArray[j] += (1-alpha) * ElementMatrixVariableArray[j];
						
			}
		    
	    
		      (ie)->SetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),NodesMatrixVariableArray,CurrentProcessInfo);

		    }

		}
	      }

	    for(ModelPart::NodesContainerType::const_iterator in = rModelPart.NodesBegin(MeshId); in!=rModelPart.NodesEnd(MeshId); in++)
	      {
	    	in->Reset(TO_REFINE);
	      }

	    //std::cout<<" [ Finished NODE to ELEMENT Transfer ] : ( Performed "<<counter<<" transfers of "<<rModelPart.NumberOfElements(MeshId)<<" possible )"<<std::endl;

	    
	    KRATOS_CATCH( "" )
	}
      

	//*******************************************************************************************
	//*******************************************************************************************

        //KRATOS MESH INPUT
        void MeshDataTransferUtilities::TransferElementalValuesToNodes( const TransferParameters& rTransferVariables,
									ModelPart& rModelPart,
									ModelPart::IndexType MeshId)
	{

	    KRATOS_TRY
	     
	    //std::cout<<" [ Data Transfer ELEMENT to NODE ] :"<<std::endl;

	    ProcessInfo& CurrentProcessInfo = rModelPart.GetProcessInfo();
	    NodesContainerType& rNodes      = rModelPart.Nodes(MeshId);

	    Geometry<Node<3> >& rGeom = rModelPart.ElementsBegin(MeshId)->GetGeometry();
	    GeometryData::IntegrationMethod IntegrationMethod =  rGeom.GetDefaultIntegrationMethod();
	    unsigned int integration_points_number = rGeom.IntegrationPointsNumber( IntegrationMethod );

	    std::vector<double> NodesDoubleVariableArray (integration_points_number);
	    std::vector<array_1d<double,3> > NodesArray1DVariableArray (integration_points_number);
	    std::vector<Vector> NodesVectorVariableArray (integration_points_number);
	    std::vector<Matrix> NodesMatrixVariableArray (integration_points_number);

	    std::vector<double> ElementDoubleVariableArray (integration_points_number);
	    std::vector<array_1d<double,3> > ElementArray1DVariableArray (integration_points_number);
	    std::vector<Vector> ElementVectorVariableArray (integration_points_number);
	    std::vector<Matrix> ElementMatrixVariableArray (integration_points_number);
      
	    unsigned int buffer_size = rModelPart.GetBufferSize();
	    VariablesList& variables_list = rModelPart.GetNodalSolutionStepVariablesList();
	    

	    for(NodesContainerType::iterator in = rNodes.begin(); in!=rNodes.end(); in++)
	      {

		if(in->IsNot(RIGID)){
		  
		  //fill variables that are non assigned vectors
		  this->FillVectorData(variables_list, *(in.base()));	  

	    	  WeakPointerVector<Element >& neighb_elems = in->GetValue(NEIGHBOUR_ELEMENTS);

	    	  double Area         = 0;
	    	  double ElementArea  = 0;
		  std::fill( NodesDoubleVariableArray.begin(), NodesDoubleVariableArray.end(), 0.0);
		  
		  //Initialize variables
		  
		  //double
		  std::fill( NodesDoubleVariableArray.begin(), NodesDoubleVariableArray.end(), 0.0);
		  

		  //Array1D
		  for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
		    {			  
		      NodesArray1DVariableArray[i].clear();
		    }


		  //Vector
		  for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
		    {			  
		      // (rModelPart.Elements(MeshId).begin())->GetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),ElementVectorVariableArray,CurrentProcessInfo);
		      // if( ElementVectorVariableArray[i].size() != 0)
		      // 	NodesVectorVariableArray[i] = ZeroVector(ElementVectorVariableArray[i].size());
		      // else
			NodesVectorVariableArray[i] = ZeroVector(); //¿value?
		    }

		  //Matrix
		  for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
		    {	
		      // (rModelPart.Elements(MeshId).begin())->GetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),ElementMatrixVariableArray,CurrentProcessInfo);
		      // if( ElementMatrixVariableArray[i].size1() != 0  && ElementMatrixVariableArray[i].size2() != 0 )
		      // 	NodesMatrixVariableArray[i] = ZeroMatrix(ElementMatrixVariableArray[i].size1(),ElementMatrixVariableArray[i].size2());
		      // else
			NodesMatrixVariableArray[i] = ZeroMatrix(); //¿value?
		    }


	    	  for(unsigned int ne=0; ne < neighb_elems.size(); ne++)
	    	    {		    

		      Geometry<Node<3> > & rGeometry   = neighb_elems[ne].GetGeometry();
		      ElementArea = rGeometry.Area();
		      Area += ElementArea;			 
		      
		      //double
		      for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
			{			  
			  //elemental value
			  neighb_elems[ne].GetValueOnIntegrationPoints(*(rTransferVariables.DoubleVariables[i]),ElementDoubleVariableArray,CurrentProcessInfo);   
			  for(unsigned int j=0; j<integration_points_number; j++)
			    {
			      NodesDoubleVariableArray[i] += ElementDoubleVariableArray[j] * ElementArea/double(integration_points_number);
			    }			  	 
			}

		      //Array1D
		      for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
			{			  
			  //elemental value
			  neighb_elems[ne].GetValueOnIntegrationPoints(*(rTransferVariables.Array1DVariables[i]),ElementArray1DVariableArray,CurrentProcessInfo);
			  for(unsigned int j=0; j<integration_points_number; j++)
			    {
			      NodesArray1DVariableArray[i] += ElementArray1DVariableArray[j] * ElementArea/double(integration_points_number);
			    }
			}

		      //Vector
		      for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
			{			  
			  //elemental value
			  neighb_elems[ne].GetValueOnIntegrationPoints(*(rTransferVariables.VectorVariables[i]),ElementVectorVariableArray,CurrentProcessInfo);
			  for(unsigned int j=0; j<integration_points_number; j++)
			    {
			      NodesVectorVariableArray[i] += ElementVectorVariableArray[j] * ElementArea/double(integration_points_number);
			    }
			}

		      //Matrix
		      for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
			{			  
			  //elemental value
			  neighb_elems[ne].GetValueOnIntegrationPoints(*(rTransferVariables.MatrixVariables[i]),ElementMatrixVariableArray,CurrentProcessInfo);
			  for(unsigned int j=0; j<integration_points_number; j++)
			    {
			      NodesMatrixVariableArray[i] += ElementMatrixVariableArray[j] * ElementArea/double(integration_points_number);
			    }
			}

	    	    }
		
	    	  if(Area!=0){
		    //double
		    for(unsigned int i=0; i<rTransferVariables.DoubleVariables.size(); i++)
		      {			  
			NodesDoubleVariableArray[i] /= Area;

			//std::cout<<" node ["<<in->Id()<<"] "<<NodesDoubleVariableArray[i]<<" Area "<<Area<<std::endl;
			if( (in)->SolutionStepsDataHas(*(rTransferVariables.DoubleVariables[i]))){
			  (in)->FastGetSolutionStepValue(*(rTransferVariables.DoubleVariables[i])) = NodesDoubleVariableArray[i];
			}
			else{
			  std::cout<<" ERROR TR: Something Wrong in node ["<<in->Id()<<"] : variable "<<*(rTransferVariables.DoubleVariables[i])<<" was not defined "<<std::endl;
			}
		      }
		    //Array1D
		    for(unsigned int i=0; i<rTransferVariables.Array1DVariables.size(); i++)
		      {			  
			NodesArray1DVariableArray[i] /= Area;

			if( (in)->SolutionStepsDataHas(*(rTransferVariables.Array1DVariables[i]))){
			  (in)->FastGetSolutionStepValue(*(rTransferVariables.Array1DVariables[i])) = NodesArray1DVariableArray[i];
			}
			else{
			  std::cout<<" ERROR TR: Something Wrong in node ["<<in->Id()<<"] : variable "<<*(rTransferVariables.Array1DVariables[i])<<" was not defined "<<std::endl;
			}
		      }
		    //Vector
		    for(unsigned int i=0; i<rTransferVariables.VectorVariables.size(); i++)
		      {			  
			NodesVectorVariableArray[i] /= Area;

			if( (in)->SolutionStepsDataHas(*(rTransferVariables.VectorVariables[i]))){
			  (in)->FastGetSolutionStepValue(*(rTransferVariables.VectorVariables[i])) = NodesVectorVariableArray[i];
			  //fill buffer if empty
			  for(unsigned int step = 1; step<buffer_size; step++)
			    {
			      if((in)->FastGetSolutionStepValue(*(rTransferVariables.VectorVariables[i]), step).size() == 0){
				(in)->FastGetSolutionStepValue(*(rTransferVariables.VectorVariables[i]), step) = NodesVectorVariableArray[i];
				(in)->FastGetSolutionStepValue(*(rTransferVariables.VectorVariables[i]), step).clear();
			      }
			      
			    }
			}
			else{
			  std::cout<<" ERROR TR: Something Wrong in node ["<<in->Id()<<"] : variable "<<*(rTransferVariables.VectorVariables[i])<<" was not defined "<<std::endl;
			}

		      }
		    //Matrix
		    for(unsigned int i=0; i<rTransferVariables.MatrixVariables.size(); i++)
		      {			  
			NodesMatrixVariableArray[i] /= Area;

			if( (in)->SolutionStepsDataHas(*(rTransferVariables.MatrixVariables[i]))){
			  (in)->FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i])) = NodesMatrixVariableArray[i];
			  //fill buffer if empty
			  for(unsigned int step = 1; step<buffer_size; step++)
			    {
			      if((in)->FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i]), step).size1() == 0 &&
				 (in)->FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i]), step).size2() == 0 ){
				(in)->FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i]), step) = NodesMatrixVariableArray[i];
				(in)->FastGetSolutionStepValue(*(rTransferVariables.MatrixVariables[i]), step).clear();
			      }
			    }
			}
			else{
			  std::cout<<" ERROR TR: Something Wrong in node ["<<in->Id()<<"] : variable "<<*(rTransferVariables.MatrixVariables[i])<<" was not defined "<<std::endl;
			}

		      }		    
		  }
	    	  else{
	    	    std::cout<<" ERROR TR: Something Wrong in node ["<<(in)->Id()<<"] : Area = 0 (neighbours: "<<neighb_elems.size()<<") "<<std::endl;
		  }

		    
	    	}

	      }
    

	    //std::cout<<" [ Finished ELEMENT to NODE Transfer ]"<<std::endl;
		   
	    KRATOS_CATCH( "" )
        }



	//*******************************************************************************************
	//*******************************************************************************************


        //CENTERS AND NODES INPUT
	void MeshDataTransferUtilities::TransferNodalValuesToElements(ModelPart& rModelPart,
								      const Element& rReferenceElement,
								      PointPointerVector &list_of_new_centers,
								      std::vector<Geometry<Node<3> > >& list_of_new_vertices,
								      ModelPart::IndexType MeshId)
	{

	    KRATOS_TRY
		    
	    std::cout<<" [ Data Transfer NODE to ELEMENT: NOT IMPLEMENTED YET ]"<<std::endl;
	    std::cout<<" [ Finished NODE to ELEMENT Transfer: NOT IMPLEMENTED YET ]"<<std::endl;
	    
	    KRATOS_CATCH( "" )

	}


	//*******************************************************************************************
	//*******************************************************************************************

        //CENTERS AND NODES INPUT
	void MeshDataTransferUtilities::TransferElementalValuesToNodes(ModelPart& rModelPart,
								       const Element & rReferenceElement,
								       PointPointerVector &list_of_new_centers,
								       std::vector<Geometry<Node<3> > >& list_of_new_vertices,
								       ModelPart::IndexType MeshId)
	{

	    KRATOS_TRY

	    std::cout<<" [ Data Transfer ELEMENT to NODE: NOT IMPLEMENTED YET ]"<<std::endl;
	    std::cout<<" [ Finished ELEMENT to NODE Transfer: NOT IMPLEMENTED YET ]"<<std::endl;

    	    KRATOS_CATCH( "" )

	}


	//*******************************************************************************************
	//*******************************************************************************************


        //CENTERS AND NODES INPUT
	void MeshDataTransferUtilities::TransferElementalValuesToElements(ModelPart& rModelPart,
									  const Element & rReferenceElement,
									  PointPointerVector &list_of_new_centers,
									  std::vector<Geometry<Node<3> > >& list_of_new_vertices,				       
									  ModelPart::IndexType MeshId)
	{

	    KRATOS_TRY

	    //std::cout<<" [ Data Transfer ELEMENT to ELEMENT ]"<<std::endl;

	    //definitions for spatial search
  	    typedef Node<3>                                  PointType;
	    typedef Node<3>::Pointer                  PointPointerType;
	    typedef std::vector<PointPointerType>   PointPointerVector;
	    //typedef std::vector<PointType>             PointTypeVector;
	    typedef PointPointerVector::iterator         PointIterator;
	    typedef std::vector<double>                 DistanceVector;
	    typedef std::vector<double>::iterator     DistanceIterator;
	    typedef Bucket<3, PointType, PointPointerVector, PointPointerType, PointIterator, DistanceIterator > BucketType;
	    typedef Tree< KDTreePartition<BucketType> >     KdtreeType; //Kdtree
	    //definitions for spatial search


	    ElementsContainerType& rPreElements = rModelPart.Elements(MeshId);
		
	    //creating an auxiliary list for the pre integration points
	    PointPointerVector list_of_pre_centers;
	    	    

	    //find the center and "radius" of the element
	    double xc=0;
	    double yc=0;
	    double zc=0;
	    double radius=0;

	    // CREATE LIST OF PREVIOUS ELEMENT CENTERS
	    for(ElementsContainerType::iterator ie = rPreElements.begin(); ie != rPreElements.end(); ie++)
	    {
		PointsArrayType& vertices=ie->GetGeometry().Points();

		if( vertices.size() == 3 ){
		  CalculateCenterAndSearchRadius( vertices[0].X(), vertices[0].Y(),
						  vertices[1].X(), vertices[1].Y(),
						  vertices[2].X(), vertices[2].Y(),
						  xc,yc,radius);
		}
		else if( vertices.size() == 4 ){
		  CalculateCenterAndSearchRadius( vertices[0].X(), vertices[0].Y(), vertices[0].Z(),
						  vertices[1].X(), vertices[1].Y(), vertices[1].Z(),
						  vertices[2].X(), vertices[2].Y(), vertices[2].Z(),
						  vertices[3].X(), vertices[3].Y(), vertices[3].Z(),
						  xc,yc,zc,radius);
		}
		else{
		  KRATOS_THROW_ERROR( std::logic_error, "Wrong Number of Nodes for the Element in Transfer",*this );
		}

		int id= ie->Id();
		PointPointerType p_center = PointPointerType( new PointType(id,xc,yc,zc) );

		// if ((*ie.base())->GetOptions().Is(Element::THERMAL))
		//   std::cout<<" is thermal "<<std::endl;

		// if ((*ie.base())->GetOptions().Is(Element::MECHANICAL))
		//   std::cout<<" is mechanical "<<std::endl;

		list_of_pre_centers.push_back( p_center );

		//std::cout<<" id pre elems "<<list_of_pre_centers.back()->Id()<<std::endl;
	    }

	    //std::cout<<" list of pre centers "<<list_of_pre_centers.size()<<" list of new centers "<<list_of_new_centers.size()<<std::endl;
	    

	    // SEARCH PREVIOUS CENTER NEARER TO THE NEW CENTER

	    //creating an auxiliary list for the pre integration points
	    unsigned int   bucket_size = 40;
	    KdtreeType     nodes_tree(list_of_pre_centers.begin(),list_of_pre_centers.end(),bucket_size);	
	    PointType      work_point(0,0.0,0.0,0.0);
	    double         ResultDistance;

	    //make a loop on temporal elements
	    ElementsContainerType temporal_elements;
	    temporal_elements.reserve(rModelPart.Elements(MeshId).size());    
	    temporal_elements.swap(rModelPart.Elements(MeshId));


	    int count=0;
	    int moved_transfers = 0;
	    
	    unsigned int on_distance = 0;
	    unsigned int on_connectivities = 0;
	    unsigned int same_selection = 0;

	    for(PointIterator i_center = list_of_new_centers.begin() ; i_center != list_of_new_centers.end() ; i_center++)
	      {
		count++;
		
		//std::cout<<" INTEGRATION POINT  "<<count<<std::endl;

		//find the nearest integration point to the new integration point (work_point)
		PointType& work_point= (**i_center);


		// find integration point (Element) which values will be transferred
		PointPointerType result_point = nodes_tree.SearchNearestPoint(work_point,ResultDistance);

		ElementsContainerType::iterator pe; //element corresponding to the result point

		bool extra_search = false;
		if(list_of_new_vertices[(*i_center)->Id()-1].size() == 4 && extra_search) //try no improve transfer in 3D trying to find another result point
		  { 
		    // find integration points in radius (Candidate Elements)
		    xc = 0;
		    yc = 0;
		    zc = 0;
		    radius = 0;
		    if( list_of_new_vertices[(*i_center)->Id()-1].size() == 3 ){
		      CalculateCenterAndSearchRadius( list_of_new_vertices[(*i_center)->Id()-1][0].X(), list_of_new_vertices[(*i_center)->Id()-1][0].Y(),
						      list_of_new_vertices[(*i_center)->Id()-1][1].X(), list_of_new_vertices[(*i_center)->Id()-1][1].Y(),
						      list_of_new_vertices[(*i_center)->Id()-1][2].X(), list_of_new_vertices[(*i_center)->Id()-1][2].Y(),
						      xc,yc,radius);
		    }
		    else if( list_of_new_vertices[(*i_center)->Id()-1].size() == 4 ){
		      CalculateCenterAndSearchRadius( list_of_new_vertices[(*i_center)->Id()-1][0].X(), list_of_new_vertices[(*i_center)->Id()-1][0].Y(), list_of_new_vertices[(*i_center)->Id()-1][0].Z(),
						      list_of_new_vertices[(*i_center)->Id()-1][1].X(), list_of_new_vertices[(*i_center)->Id()-1][1].Y(), list_of_new_vertices[(*i_center)->Id()-1][1].Z(),
						      list_of_new_vertices[(*i_center)->Id()-1][2].X(), list_of_new_vertices[(*i_center)->Id()-1][2].Y(), list_of_new_vertices[(*i_center)->Id()-1][2].Z(),
						      list_of_new_vertices[(*i_center)->Id()-1][3].X(), list_of_new_vertices[(*i_center)->Id()-1][3].Y(), list_of_new_vertices[(*i_center)->Id()-1][3].Z(),
						      xc,yc,zc,radius);
		    }
		    else{
		      KRATOS_THROW_ERROR( std::logic_error, "Wrong Number of Nodes for the Element in Transfer",*this );
		    }

		    //std::cout<<" Radius "<<radius<<std::endl;

		    unsigned int max_points = 50;
		    unsigned int num_points_in_radius = 0;
		    PointPointerVector list_of_points_in_radius(max_points);
		    DistanceVector list_of_distances(max_points);

		    num_points_in_radius = nodes_tree.SearchInRadius(work_point,radius,list_of_points_in_radius.begin(),list_of_distances.begin(),max_points);
		
		    std::vector<int> coincident_nodes;
		    int coincident_node = 0;

		    for(PointIterator i_candidate = list_of_points_in_radius.begin(); i_candidate!=list_of_points_in_radius.begin()+ num_points_in_radius; i_candidate++)
		      {
			ElementsContainerType::iterator ce = temporal_elements.find( (*i_candidate)->Id() );
			PointsArrayType& vertices          = ( *ce.base() )->GetGeometry().Points();
			coincident_node = 0;
			for(unsigned int i=0; i<list_of_new_vertices[(*i_center)->Id()-1].size(); i++)
			  {
			    for(unsigned int j=0; j<vertices.size(); j++)
			      {
				if( list_of_new_vertices[(*i_center)->Id()-1][i].Id() == vertices[j].Id() )
				  {
				    coincident_node+=1;
				  }
			      }
			  }

			coincident_nodes.push_back(coincident_node);
		      }
		
		    //check if there is an element with more coincident connectivities
		    int coincident = 0;
		    int candidate = 0;
		    for(unsigned int j=0; j<coincident_nodes.size(); j++)
		      {
			if(coincident_nodes[j]>coincident){
			  candidate = j;
			  coincident = coincident_nodes[j];
			}

		      }

		    //check if is the only one
		    //std::sort(coincident_nodes.begin(), coincident_nodes.end(), std::greater<int>());
		    int num_candidates = 0;
		    std::vector<int> coincident_candidates;
		    for(unsigned int j=0; j<coincident_nodes.size(); j++)
		      {
			if( coincident == coincident_nodes[j] ){
			  num_candidates += 1;
			  coincident_candidates.push_back(j);
			}		    
		      }
		
		    if(num_candidates > 1){
		      int distance_candidates = list_of_distances[coincident_candidates[0]];
		      candidate = coincident_candidates[0];
		      for(unsigned int j=1; j<coincident_candidates.size(); j++)
			{
			  if(list_of_distances[coincident_candidates[j]] < distance_candidates)
			    candidate = coincident_candidates[j];
			}
		    }
		
		    unsigned int result_id = result_point->Id();

		    //select point
		    if( result_point->Id() == (*(list_of_points_in_radius.begin()+candidate))->Id() ){
		      same_selection += 1;
		    }
		    else{
		      
		      // standart case no radius is needed
		      // if( ResultDistance > list_of_distances[candidate] ){
		      // 	result_id = (*(list_of_points_in_radius.begin()+candidate))->Id();
		      // 	on_connectivities += 1;
		      // }
		      // else{
		      // 	result_id = result_point->Id();
		      // 	on_distance += 1;
		      // }

		      // alternative case if connectivities coincides have a bigger weight
		      if( num_candidates > 1 ){
		        result_id = result_point->Id();
		        on_distance += 1;
		      }
		      else{
		        result_id = (*(list_of_points_in_radius.begin()+candidate))->Id();
		        on_connectivities += 1;
		      }
		      
		    }
		    

		    //element selected
		    pe = temporal_elements.find( result_id );

		    //std::cout<<" ResultPointDistance "<<result_point->Id()<<" ResultPointConnectivities "<<(*(list_of_points_in_radius.begin()+candidate))->Id()<<std::endl;

		  }
		else{

		  //result point is a previous center
		  pe = temporal_elements.find( result_point->Id() );
		}

		Element::Pointer PreviousElement   = Element::Pointer( *pe.base() );
		PointsArrayType& vertices          = PreviousElement->GetGeometry().Points();

		if( fabs(ResultDistance) > 1e-30 ){

		  // // Connectivities Ids
		  // std::cout<<" New Id: "<<(*i_center)->Id()<<" Connectivities : ["<<list_of_new_vertices[(*i_center)->Id()-1][0].Id();
		  // for(unsigned int pn=1; pn<list_of_new_vertices[(*i_center)->Id()-1].size(); pn++){
		  //   std::cout<<", "<<list_of_new_vertices[(*i_center)->Id()-1][pn].Id();
		  // }
		  // std::cout<<"] "<<std::endl;

		  // std::cout<<" Pre Id: "<<PreviousElement->Id()<<" Connectivities : ["<<vertices[0].Id();
		  // for(unsigned int pn=1; pn<vertices.size(); pn++){
		  //   std::cout<<", "<<vertices[pn].Id();
		  // }
		  // std::cout<<"] "<<std::endl;

		  // // Connectivities coordinates
		  // // std::cout<<" New Id: "<<(*i_center)->Id()<<" Connectivities : ["<<list_of_new_vertices[(*i_center)->Id()-1][0].Coordinates();
		  // // for(unsigned int pn=1; pn<vertices.size(); pn++){
		  // //   std::cout<<", "<<list_of_new_vertices[(*i_center)->Id()-1][pn].Coordinates();
		  // // }
		  // // std::cout<<"] "<<std::endl;

		  // // std::cout<<" Pre Id: "<<PreviousElement->Id()<<" Connectivities : ["<<vertices[0].Coordinates();
		  // // for(unsigned int pn=1; pn<vertices.size(); pn++){
		  // //   std::cout<<", "<<vertices[pn].Coordinates();
		  // // }
		  // // std::cout<<"] "<<std::endl;

		  // std::cout<<" New Center :"<<work_point<<std::endl;
		  // std::cout<<" Old Center :"<<*result_point<<std::endl;
		  // std::cout<<" [Distance: "<<ResultDistance<<"]"<<std::endl;

		  moved_transfers ++;
		}
	      

		//***************************************************
		// CREATE ELEMENT WITH THE TRANSFERED VALUES :: START

		// inside of the loop create the element, set the variables, and push_back the new element to the model part
		Element::Pointer new_element = PreviousElement->Clone((*i_center)->Id(), list_of_new_vertices[(*i_center)->Id()-1]);	
	      
		//set transfer variables
		new_element->SetValue(DOMAIN_LABEL,vertices[0].GetValue(DOMAIN_LABEL)); //DOMAIN_LABEL set as a variable
		new_element->AssignFlags(*PreviousElement);	

		//check
		//new_element->PrintInfo(std::cout);

		//setting new elements
		(rModelPart.Elements(MeshId)).push_back(new_element);

		// CREATE ELEMENT WITH THE TRANSFERED VALUES :: END
		//***************************************************


		//ELEMENT TRANSFER CHECK//
		// std::cout<<" Write Stress [element: "<<new_element->Id()<<"]: ";
		// std::cout.flush();
		// std::vector<Matrix >  StressMatrix;
		// Matrix Stress;
		// Stress.clear();
		// StressMatrix.push_back(Stress);
		// ProcessInfo& CurrentProcessInfo = rModelPart.GetProcessInfo();
		// new_element->GetValueOnIntegrationPoints(CAUCHY_STRESS_TENSOR,StressMatrix,CurrentProcessInfo);
		// std::cout<<StressMatrix[0]<<std::endl;
		//ELEMENT TRANSFER CHECK//
	      }

	    //std::cout<<" on distances "<<on_distance<<" on connectitivies "<<on_connectivities<<" same selection "<<same_selection<<std::endl;
	    //std::cout<<" [ MOVED TRANSFERS: "<<moved_transfers<<" ]"<<std::endl;
	    //std::cout<<" [ Finished ELEMENT to ELEMENT Transfer ]"<<std::endl;


	    KRATOS_CATCH( "" )
	}
	



        //*******************************************************************************************
        //*******************************************************************************************

        void MeshDataTransferUtilities::Interpolate2Nodes( Geometry<Node<3> > &geom,
							   const std::vector<double>& N,
							   VariablesList& rVariablesList,
							   Node<3>& pnode)
	{ 
	  
	  KRATOS_TRY

	  unsigned int buffer_size = pnode.GetBufferSize();
	     
	  if (N[0]==0.0 && N[1]==0.0){
	    KRATOS_THROW_ERROR( std::logic_error,"SOMETHING is wrong with interpolated 2 Nodes Shape Functions", "" )
	      }
     
	  for(VariablesList::const_iterator i_variable =  rVariablesList.begin();  i_variable != rVariablesList.end() ; i_variable++)
	    {
	      //std::cout<<" name "<<i_variable->Name()<<std::endl;
	      //std::cout<<" type "<<typeid(*i_variable).name()<<std::endl;
	      std::string variable_name = i_variable->Name();
	      if(KratosComponents<Variable<double> >::Has(variable_name))
		{
		  //std::cout<<"double"<<std::endl;
		  Variable<double> variable = KratosComponents<Variable<double> >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      double& node_data = pnode.FastGetSolutionStepValue(variable, step);
		  
		      double& node0_data = geom[0].FastGetSolutionStepValue(variable, step);
		      double& node1_data = geom[1].FastGetSolutionStepValue(variable, step);
		  
		      node_data = (N[0]*node0_data + N[1]*node1_data);
		  
		    }
		}
	      else if(KratosComponents<Variable<array_1d<double, 3> > >::Has(variable_name))
		{
		  //std::cout<<"array1d"<<std::endl;
		  Variable<array_1d<double, 3> > variable = KratosComponents<Variable<array_1d<double, 3> > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      array_1d<double, 3>& node_data = pnode.FastGetSolutionStepValue(variable, step);
		  
		      array_1d<double, 3>& node0_data = geom[0].FastGetSolutionStepValue(variable, step);
		      array_1d<double, 3>& node1_data = geom[1].FastGetSolutionStepValue(variable, step);
		  
		      node_data = (N[0]*node0_data + N[1]*node1_data);		  
		    }

		}
	      else if(KratosComponents<Variable<int > >::Has(variable_name))
		{
		  //std::cout<<"int"<<std::endl;
		  //NO INTERPOLATION
		}
	      else if(KratosComponents<Variable<bool > >::Has(variable_name))
		{
		  //std::cout<<"bool"<<std::endl;
		  //NO INTERPOLATION
		}
	      else if(KratosComponents<Variable<Matrix > >::Has(variable_name))
		{
		  //std::cout<<"Matrix"<<std::endl;
		  Variable<Matrix > variable = KratosComponents<Variable<Matrix > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      Matrix& node_data = pnode.FastGetSolutionStepValue(variable, step);
		  
		      Matrix& node0_data = geom[0].FastGetSolutionStepValue(variable, step);
		      Matrix& node1_data = geom[1].FastGetSolutionStepValue(variable, step);
		  
		      if( node_data.size1() > 0 && node_data.size2() ){
			if( node_data.size1() == node0_data.size1() && node_data.size2() == node0_data.size2() &&
			    node_data.size1() == node1_data.size1() && node_data.size2() == node1_data.size2() ) {
		      
			  node_data = (N[0]*node0_data + N[1]*node1_data);	       
			}
		      }
		    }

		}
	      else if(KratosComponents<Variable<Vector > >::Has(variable_name))
		{
		  //std::cout<<"Vector"<<std::endl;
		  Variable<Vector >variable = KratosComponents<Variable<Vector > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      Vector& node_data = pnode.FastGetSolutionStepValue(variable, step);
		  
		      Vector& node0_data = geom[0].FastGetSolutionStepValue(variable, step);
		      Vector& node1_data = geom[1].FastGetSolutionStepValue(variable, step);
		  
		      if( node_data.size() > 0 ){
			if( node_data.size() == node0_data.size() &&
			    node_data.size() == node1_data.size()) {
		      
			  node_data = (N[0]*node0_data + N[1]*node1_data);	       
			}
		      }
		    }
		}

	    }

	  KRATOS_CATCH( "" )

      }


      //*******************************************************************************************
      //*******************************************************************************************
  
      VariablesListDataValueContainer MeshDataTransferUtilities::InterpolateVariables( Geometry<Node<3> > &geom,
										       const std::vector<double>& N,
										       VariablesList& rVariablesList,
										       Node<3>::Pointer pnode,
										       double& alpha)
      {

	  KRATOS_TRY

	  //Copy Variables List
	  // std::cout<<" node["<<pnode->Id()<<"] Data "<<(pnode)->SolutionStepData()<<std::endl;

	  VariablesListDataValueContainer PreviousVariablesListData = (pnode)->SolutionStepData();

	  // std::cout<<" CopiedData "<<PreviousVariablesListData<<std::endl;

      
	  Interpolate( geom, N, rVariablesList, pnode, alpha);

	  VariablesListDataValueContainer CurrentVariablesListData = (pnode)->SolutionStepData();

	  (pnode)->SolutionStepData() = PreviousVariablesListData;
      
	  // std::cout<<" PreviousVariables "<<PreviousVariablesListData<<std::endl;
	  // std::cout<<" CurrentVariables "<<CurrentVariablesListData<<std::endl;

	  return CurrentVariablesListData;

	  KRATOS_CATCH( "" )

      }


      //*******************************************************************************************
      //*******************************************************************************************
  
      void MeshDataTransferUtilities::FillVectorData(VariablesList& rVariablesList,
						     Node<3>::Pointer pnode)
      {

	  KRATOS_TRY

	  unsigned int buffer_size = pnode->GetBufferSize();
     
	  for(VariablesList::const_iterator i_variable =  rVariablesList.begin();  i_variable != rVariablesList.end() ; i_variable++)
	    {
	      //std::cout<<" name "<<i_variable->Name()<<std::endl;
	      //std::cout<<" type "<<typeid(*i_variable).name()<<std::endl;
	      std::string variable_name = i_variable->Name();
	      if(KratosComponents<Variable<Vector > >::Has(variable_name))
		{
		  //std::cout<<"Vector"<<std::endl;
		  Variable<Vector> variable = KratosComponents<Variable<Vector > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      Vector& node_data = pnode->FastGetSolutionStepValue(variable, step);
		      
		      // if( variable == CONSTRAINT_LABELS ){
		      // 	std::cout<<" node ["<<pnode->Id()<<"]"<<std::endl;
		      // 	std::cout<<" variable "<<variable<<std::endl;
		      // 	std::cout<<" node data "<<node_data<<std::endl;
		      // }

		      if( node_data.size() == 0 ){
			node_data = ZeroVector(1);	       		      
		      }
		      
		    }
		}
	      
	    }

	    KRATOS_CATCH( "" )

      }


      //*******************************************************************************************
      //*******************************************************************************************
  
      void MeshDataTransferUtilities::Interpolate( Geometry<Node<3> > &geom,
						   const std::vector<double>& N,
						   VariablesList& rVariablesList,
						   Node<3>::Pointer pnode,
						   double& alpha)
      {

	  KRATOS_TRY

	  unsigned int buffer_size = pnode->GetBufferSize();
	     
	  bool all_null = true;
	  for(unsigned int i=0; i<N.size(); i++)
	    if(N[i]!=0)
	      all_null = false;

	  if( all_null )
	      KRATOS_THROW_ERROR( std::logic_error,"SOMETHING is wrong with the Interpolation Functions", "" )

     
	  for(VariablesList::const_iterator i_variable =  rVariablesList.begin();  i_variable != rVariablesList.end() ; i_variable++)
	    {
	      //std::cout<<" name "<<i_variable->Name()<<std::endl;
	      //std::cout<<" type "<<typeid(*i_variable).name()<<std::endl;
	      std::string variable_name = i_variable->Name();
	      if(KratosComponents<Variable<double> >::Has(variable_name))
		{
		  Variable<double> variable = KratosComponents<Variable<double> >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      double& node_data = pnode->FastGetSolutionStepValue(variable, step);
		      
		      std::vector<double* > nodes_data;
		      for(unsigned int i=0; i<geom.size(); i++)
			nodes_data.push_back(&geom[i].FastGetSolutionStepValue(variable, step));
		  
		      if(alpha != 1 ){
			node_data *= (1-alpha);
			for(unsigned int i=0; i<geom.size(); i++)
			  node_data += (alpha) * (N[i]*(*nodes_data[i]));		       
		      }
		      else{
			
			node_data = (N[0]*(*nodes_data[0]));
			for(unsigned int i=1; i<geom.size(); i++)
			  node_data += (N[i]*(*nodes_data[i]));
		      }

		    }
		}
	      else if(KratosComponents<Variable<array_1d<double, 3> > >::Has(variable_name))
		{
		  //std::cout<<"array1d"<<std::endl;
		  Variable<array_1d<double, 3> > variable = KratosComponents<Variable<array_1d<double, 3> > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      array_1d<double, 3>& node_data = pnode->FastGetSolutionStepValue(variable, step);
		  
		      std::vector<array_1d<double, 3>* > nodes_data;
		      for(unsigned int i=0; i<geom.size(); i++)
			nodes_data.push_back(&geom[i].FastGetSolutionStepValue(variable, step));
		  
		      if(alpha != 1 ){

			node_data *= (1-alpha);
			for(unsigned int i=0; i<geom.size(); i++)
			  node_data += (alpha) * (N[i]*(*nodes_data[i]));
			
		      }
		      else{
			
			node_data = (N[0]*(*nodes_data[0]));
			for(unsigned int i=1; i<geom.size(); i++)
			  node_data += (N[i]*(*nodes_data[i]));
		      }
		    }

		}
	      else if(KratosComponents<Variable<int > >::Has(variable_name))
		{
		  //std::cout<<"int"<<std::endl;
		  //NO INTERPOLATION
		}
	      else if(KratosComponents<Variable<bool > >::Has(variable_name))
		{
		  //std::cout<<"bool"<<std::endl;
		  //NO INTERPOLATION
		}
	      else if(KratosComponents<Variable<Matrix > >::Has(variable_name))
		{
		  //std::cout<<"Matrix"<<std::endl;
		  Variable<Matrix> variable = KratosComponents<Variable<Matrix > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      Matrix& node_data = pnode->FastGetSolutionStepValue(variable, step);
		      
		      std::vector<Matrix* > nodes_data;
		      for(unsigned int i=0; i<geom.size(); i++)
			nodes_data.push_back(&geom[i].FastGetSolutionStepValue(variable, step));
		      
		      if( node_data.size1() > 0 && node_data.size2() ){
			bool same_size = true;

			for(unsigned int i=0; i<geom.size(); i++)
			  if(node_data.size1() != (*nodes_data[i]).size1() && node_data.size2() != (*nodes_data[i]).size2())
			    same_size = false;

			if( same_size ) {
			
			  if(alpha != 1 ){
			    
			    node_data *= (1-alpha);
			    for(unsigned int i=0; i<geom.size(); i++)
			      node_data += (alpha) * (N[i]*(*nodes_data[i]));
			    
			  }
			  else{
			    
			    node_data = (N[0]*(*nodes_data[0]));
			    for(unsigned int i=1; i<geom.size(); i++)
			      node_data += (N[i]*(*nodes_data[i]));
			  }
			  
			}
		      }
		    }

		}
	      else if(KratosComponents<Variable<Vector > >::Has(variable_name))
		{
		  //std::cout<<"Vector"<<std::endl;
		  Variable<Vector> variable = KratosComponents<Variable<Vector > >::Get(variable_name);
		  for(unsigned int step = 0; step<buffer_size; step++)
		    {
		      //getting the data of the solution step
		      Vector& node_data = pnode->FastGetSolutionStepValue(variable, step);
		  
		      std::vector<Vector* > nodes_data;
		      for(unsigned int i=0; i<geom.size(); i++)
			nodes_data.push_back(&geom[i].FastGetSolutionStepValue(variable, step));
		  
		      if( variable != CONSTRAINT_LABELS && variable != LOAD_LABELS && variable != MARKER_LABELS )
			{
			  // std::cout<<" node ["<<pnode->Id()<<"]"<<std::endl;
			  // std::cout<<" variable "<<variable<<std::endl;
			  // std::cout<<" node data "<<node_data<<std::endl;
			  if( node_data.size() > 0 ){
			    bool same_size = true;

			    for(unsigned int i=0; i<geom.size(); i++)
			      if(node_data.size() != (*nodes_data[i]).size())
				same_size = false;

			    if( same_size ) {
			      
			      if(alpha != 1 ){

				node_data *= (1-alpha);
				for(unsigned int i=0; i<geom.size(); i++)
				  node_data += (alpha) * (N[i]*(*nodes_data[i]));
			      }
			      else{

				node_data = (N[0]*(*nodes_data[0]));
				for(unsigned int i=1; i<geom.size(); i++)
				  node_data += (N[i]*(*nodes_data[i]));

			      }
			      
			    }
			  }
			}

		    }
		}

	    }

	    KRATOS_CATCH( "" )

	}


	//*******************************************************************************************
	//*******************************************************************************************

	void MeshDataTransferUtilities::InterpolateData( Geometry<Node<3> >& geom,
							 const std::vector<double>& N,
							 unsigned int step_data_size,
							 Node<3>::Pointer pnode,
							 double& alpha)
	{

   	    KRATOS_TRY

	    unsigned int buffer_size = pnode->GetBufferSize();

	    //alpha [0,1] //smoothing level of the interpolation

	    for(unsigned int step = 0; step<buffer_size; step++)
	      {
		//getting the data of the solution step
		double* step_data = (pnode)->SolutionStepData().Data(step);
		
		std::vector<double* > nodes_data;
		for(unsigned int i=0; i<geom.size(); i++)
		  {
		    nodes_data.push_back(geom[i].SolutionStepData().Data(step));
		  }
		
		//copying this data in the position of the vector we are interested in
		for(unsigned int j= 0; j<step_data_size; j++)
		  {
		    step_data[j] *= (1-alpha);
		    for(unsigned int i=0; i<geom.size(); i++)
		      step_data[j] += (alpha) * (N[i]*nodes_data[i][j]);
		  }
	      }

	    KRATOS_CATCH( "" )
	}

	//*******************************************************************************************
	//*******************************************************************************************
  
        VariablesListDataValueContainer MeshDataTransferUtilities::InterpolateVariablesData( Geometry<Node<3> >& geom,
											     const std::vector<double>& N,
											     unsigned int step_data_size,
											     Node<3>::Pointer pnode,
											     double& alpha)
	  
	{
	    KRATOS_TRY

	    //Copy Variables List
	    VariablesListDataValueContainer PreviousVariablesListData = (pnode)->SolutionStepData();
      
	    InterpolateData( geom, N, step_data_size, pnode, alpha);
	    
	    VariablesListDataValueContainer CurrentVariablesListData = (pnode)->SolutionStepData();
	  
	    (pnode)->SolutionStepData() = PreviousVariablesListData;
      
	    //std::cout<<" PreviousVariables "<<PreviousVariablesListData<<std::endl;
	    //std::cout<<" CurrentVariables "<<CurrentVariablesListData<<std::endl;
	    
	    return CurrentVariablesListData;
	    
	    KRATOS_CATCH( "" )
	    
	}


}  // namespace Kratos.

