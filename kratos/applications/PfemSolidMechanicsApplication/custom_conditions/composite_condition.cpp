//
//   Project Name:        KratosPfemSolidMechanicsApplication $
//   Last modified by:    $Author:                JMCarbonell $
//   Date:                $Date:                    July 2013 $
//   Revision:            $Revision:                      0.0 $
//
//

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/kratos_flags.h"
#include "custom_conditions/composite_condition.hpp"

#include "pfem_solid_mechanics_application.h"

namespace Kratos
{

//******************************CONSTRUCTOR*******************************************
//************************************************************************************

CompositeCondition::CompositeCondition( IndexType NewId, GeometryType::Pointer pGeometry )
    : Condition( NewId, pGeometry )
{
  //DO NOT ADD DOFS HERE!!!
  this->Set(BOUNDARY);
}


//******************************CONSTRUCTOR*******************************************
//************************************************************************************

CompositeCondition::CompositeCondition( IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties )
    : Condition( NewId, pGeometry, pProperties )
{
  this->Set(BOUNDARY);
}


//******************************COPY CONSTRUCTOR**************************************
//************************************************************************************

CompositeCondition::CompositeCondition( CompositeCondition const& rOther)
    :Condition(rOther)
{

  //mpChildConditions = rOther.mpChildConditions;

  //deep copy
  mpChildConditions->clear();
  for (ConditionIterator cn = rOther.mpChildConditions->begin() ; cn != rOther.mpChildConditions->end(); ++cn)
    {
      mpChildConditions->push_back(*cn);
    }

  this->Set(BOUNDARY);

}


//*******************************ASSIGMENT OPERATOR***********************************
//************************************************************************************

CompositeCondition&  CompositeCondition::operator=(CompositeCondition const& rOther)
{
  Condition::operator=(rOther);

  //mpChildConditions = rOther.mpChildConditions;

  //deep copy
  mpChildConditions->clear();
  for (ConditionIterator cn = rOther.mpChildConditions->begin() ; cn != rOther.mpChildConditions->end(); ++cn)
    {
      mpChildConditions->push_back(*cn);
    }

  this->Set(BOUNDARY);

  return *this;
}


//*********************************CREATE*********************************************
//************************************************************************************

Condition::Pointer CompositeCondition::Create( IndexType NewId, NodesArrayType const& ThisNodes, PropertiesType::Pointer pProperties ) const
{
    return Condition::Pointer(new CompositeCondition( NewId, GetGeometry().Create( ThisNodes ), pProperties ) );
}


//************************************CLONE*******************************************
//************************************************************************************

Condition::Pointer CompositeCondition::Clone( IndexType NewId, NodesArrayType const& rThisNodes ) const
{
  return this->Create( NewId, rThisNodes, pGetProperties() );
}



//*******************************DESTRUCTOR*******************************************
//************************************************************************************


CompositeCondition::~CompositeCondition()
{
}



//************* GETTING METHODS
//************************************************************************************
//************************************************************************************

CompositeCondition::IntegrationMethod CompositeCondition::GetIntegrationMethod()
{
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      return cn->GetIntegrationMethod();
    }
  
  return this->GetIntegrationMethod();
}


//************************************************************************************
//************************************************************************************

void CompositeCondition::GetDofList( DofsVectorType& rConditionalDofList, ProcessInfo& rCurrentProcessInfo )
{
  rConditionalDofList.clear();
  rConditionalDofList.resize(0);

  DofsVectorType LocalConditionalDofList;

  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn, rCurrentProcessInfo) ){

	cn->GetDofList(LocalConditionalDofList,rCurrentProcessInfo);
	
	for(unsigned int i=0; i<LocalConditionalDofList.size(); i++)
	  {
	    rConditionalDofList.push_back(LocalConditionalDofList[i]);
	  }
      }
      
    }
  
}

//************************************************************************************
//************************************************************************************

void CompositeCondition::EquationIdVector( EquationIdVectorType& rResult, ProcessInfo& rCurrentProcessInfo )
{
  
  rResult.clear();
  rResult.resize(0);

  EquationIdVectorType LocalResult;
   
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn, rCurrentProcessInfo) ){

	cn->EquationIdVector(LocalResult,rCurrentProcessInfo);

	for(unsigned int i=0; i<LocalResult.size(); i++)
	  {
	    rResult.push_back(LocalResult[i]);
	  }
      }
      
    }
  
}


//*********************************SET VALUE TO CHILDREN******************************
//************************************************************************************

bool CompositeCondition::IsActive(ConditionIterator iChildCondition, const ProcessInfo& rCurrentProcessInfo )
{
  if(rCurrentProcessInfo.Is(THERMAL) && iChildCondition->Is(THERMAL))
    return true;

  if(rCurrentProcessInfo.IsNot(THERMAL) && iChildCondition->IsNot(THERMAL))
    return true;
  
  return false;
}



//*********************************DISPLACEMENT***************************************
//************************************************************************************

void CompositeCondition::GetValuesVector( Vector& rValues, int Step )
{
  Vector LocalValues;

  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->GetValuesVector(LocalValues,Step);
      if(LocalValues.size() != rValues.size())
	rValues.resize(LocalValues.size(),false);
      
      rValues+=LocalValues;
  }
}


//************************************VELOCITY****************************************
//************************************************************************************

void CompositeCondition::GetFirstDerivativesVector( Vector& rValues, int Step )
{
  Vector LocalValues;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->GetFirstDerivativesVector(LocalValues,Step);
      if(LocalValues.size() != rValues.size())
	rValues.resize(LocalValues.size(),false);

      rValues+=LocalValues;
    }
}

//*********************************ACCELERATION***************************************
//************************************************************************************

void CompositeCondition::GetSecondDerivativesVector( Vector& rValues, int Step )
{
  Vector LocalValues;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->GetSecondDerivativesVector(LocalValues,Step);
      if(LocalValues.size() != rValues.size())
	rValues.resize(LocalValues.size(),false);

      rValues+=LocalValues;
  }
}


//*********************************SET VECTOR VALUE***********************************
//************************************************************************************

void CompositeCondition::SetValueOnIntegrationPoints( const Variable<Vector>& rVariable, std::vector<Vector>& rValues, const ProcessInfo& rCurrentProcessInfo )
{
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->SetValueOnIntegrationPoints(rVariable,rValues,rCurrentProcessInfo);
    }
}


//*********************************SET MATRIX VALUE***********************************
//************************************************************************************

void CompositeCondition::SetValueOnIntegrationPoints( const Variable<Matrix>& rVariable,
        std::vector<Matrix>& rValues,
        const ProcessInfo& rCurrentProcessInfo )
{
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->SetValueOnIntegrationPoints(rVariable,rValues,rCurrentProcessInfo);
    }
}

//*********************************GET DOUBLE VALUE***********************************
//************************************************************************************

void CompositeCondition::GetValueOnIntegrationPoints( const Variable<double>& rVariable,
							    std::vector<double>& rValues,
							    const ProcessInfo& rCurrentProcessInfo )
{

  std::vector< double > LocalValues;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->GetValueOnIntegrationPoints(rVariable,LocalValues,rCurrentProcessInfo);

    if ( LocalValues.size() != rValues.size() )
      rValues.resize( LocalValues.size(), false );

    for(unsigned int i=0; i<LocalValues.size(); i++)
      rValues[i] += LocalValues[i];
  }

}


//**********************************GET VECTOR VALUE**********************************
//************************************************************************************

void CompositeCondition::GetValueOnIntegrationPoints( const Variable<Vector>& rVariable,
							    std::vector<Vector>& rValues,
							    const ProcessInfo& rCurrentProcessInfo )
{
  std::vector< Vector > LocalValues;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->GetValueOnIntegrationPoints(rVariable,LocalValues,rCurrentProcessInfo);

    if ( LocalValues.size() != rValues.size() )
      rValues.resize(LocalValues.size());

    for(unsigned int i=0; i<LocalValues.size(); i++)
      {     
	if ( LocalValues[i].size() != rValues[i].size() )
	  rValues[i].resize( LocalValues[i].size(), false );
      }

    for(unsigned int i=0; i<LocalValues.size(); i++)
      rValues[i] += LocalValues[i];
 
  }
}

//***********************************GET MATRIX VALUE*********************************
//************************************************************************************

void CompositeCondition::GetValueOnIntegrationPoints( const Variable<Matrix>& rVariable,
							 std::vector<Matrix>& rValues, 
							 const ProcessInfo& rCurrentProcessInfo )
{
  std::vector< Matrix > LocalValues;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->GetValueOnIntegrationPoints(rVariable,LocalValues,rCurrentProcessInfo);

    if ( LocalValues.size() != rValues.size() )
      rValues.resize(LocalValues.size());

    for(unsigned int i=0; i<LocalValues.size(); i++)
      {     
	if ( LocalValues[i].size2() != rValues[i].size2() )
	  rValues[i].resize( LocalValues[i].size1(), LocalValues[i].size2(), false );
      }

    for(unsigned int i=0; i<LocalValues.size(); i++)
      rValues[i] += LocalValues[i];
  }
}



//************* STARTING - ENDING  METHODS
//************************************************************************************
//************************************************************************************

void CompositeCondition::Initialize()
{
    KRATOS_TRY
      
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      SetValueToChildren(MASTER_ELEMENTS);
      SetValueToChildren(MASTER_NODES);
	
      cn->Initialize();
      
    }
 

    KRATOS_CATCH( "" )
}



////************************************************************************************
////************************************************************************************

void CompositeCondition::InitializeSolutionStep( ProcessInfo& rCurrentProcessInfo )
{

  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){

	SetValueToChildren(MASTER_ELEMENTS);
	SetValueToChildren(MASTER_NODES);

	//cn->SetId(this->Id());
	cn->pGetGeometry() = this->pGetGeometry();

	cn->InitializeSolutionStep(rCurrentProcessInfo);

      }
    }
}

////************************************************************************************
////************************************************************************************

void CompositeCondition::InitializeNonLinearIteration( ProcessInfo& rCurrentProcessInfo )
{
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	cn->InitializeNonLinearIteration(rCurrentProcessInfo);
    }
  }
}


//************************************************************************************
//************************************************************************************

void CompositeCondition::FinalizeSolutionStep( ProcessInfo& rCurrentProcessInfo )
{
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	cn->FinalizeSolutionStep(rCurrentProcessInfo);
      }
    }
}


//************************************************************************************
//************************************************************************************

void CompositeCondition::AddExplicitContribution(const VectorType& rRHS, 
						 const Variable<VectorType>& rRHSVariable, 
						 Variable<array_1d<double,3> >& rDestinationVariable, 
						 const ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY

    for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
      {
	cn->AddExplicitContribution(rRHS, rRHSVariable, rDestinationVariable, rCurrentProcessInfo);
      }
    
    KRATOS_CATCH( "" )
}


//************* COMPUTING  METHODS
//************************************************************************************
//************************************************************************************


//***********************COMPUTE LOCAL SYSTEM CONTRIBUTIONS***************************
//************************************************************************************


void CompositeCondition::CalculateLocalSystem( MatrixType& rLeftHandSideMatrix, VectorType& rRightHandSideVector, ProcessInfo& rCurrentProcessInfo )
{
  rLeftHandSideMatrix.clear();
  rLeftHandSideMatrix.resize(0,0);

  rRightHandSideVector.clear();
  rRightHandSideVector.resize(0);

  //std::cout<<" Calculate local system Skin "<<std::endl;
  VectorType LocalRightHandSideVector;
  MatrixType LocalLeftHandSideMatrix;

  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	
	cn->CalculateLocalSystem(LocalLeftHandSideMatrix,LocalRightHandSideVector,rCurrentProcessInfo);
	//std::cout<<" LocalRightHandSideVector "<<LocalRightHandSideVector<<std::endl;

	//resizing as needed the LHS
	unsigned int GlobalSize1 = rLeftHandSideMatrix.size1();
	unsigned int LocalSize1  = LocalLeftHandSideMatrix.size1();
	unsigned int MatSize1    = GlobalSize1+LocalSize1;

	unsigned int GlobalSize2 = rLeftHandSideMatrix.size2();
	unsigned int LocalSize2  = LocalLeftHandSideMatrix.size2();
	unsigned int MatSize2    = GlobalSize2+LocalSize2;

	rLeftHandSideMatrix.resize( MatSize1, MatSize2, true );

	unsigned int indexi = 0;
	for(unsigned int i=GlobalSize1; i<MatSize1; i++)
	  {
	    unsigned int indexj = 0;
	    for(unsigned int j=GlobalSize2; j<MatSize2; j++)
	      {
		rLeftHandSideMatrix(i,j)=LocalLeftHandSideMatrix(indexi,indexj);
		indexj++;
	      }
	    indexi++;
	  }
    
	//resizing as needed the RHS
	GlobalSize1 = rRightHandSideVector.size();
	LocalSize1  = LocalRightHandSideVector.size();
	MatSize1    = GlobalSize1+LocalSize1;
    
	rRightHandSideVector.resize( MatSize1, true );
    
	indexi = 0;
	for(unsigned int i=GlobalSize1; i<MatSize1; i++)
	  {
	    rRightHandSideVector[i]=LocalRightHandSideVector[indexi];
	    indexi++;
	  }
    
      }
    }

  //std::cout<<" Skin "<<this->Id()<<" rRightHandSideVector "<<rRightHandSideVector<<std::endl;

}

//************************************************************************************
//************************************************************************************


void CompositeCondition::CalculateRightHandSide( VectorType& rRightHandSideVector, ProcessInfo& rCurrentProcessInfo )
{
  //std::cout<<" Calculate local rhs system Skin "<<std::endl;
  rRightHandSideVector.clear();
  rRightHandSideVector.resize(0);


  VectorType LocalRightHandSideVector;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	
	cn->CalculateRightHandSide(LocalRightHandSideVector,rCurrentProcessInfo);

	//resizing as needed the RHS
	unsigned int GlobalSize  = rRightHandSideVector.size();
	unsigned int LocalSize   = LocalRightHandSideVector.size();
	unsigned int MatSize     = GlobalSize+LocalSize;
    
	rRightHandSideVector.resize( MatSize, true );
    
	unsigned int indexi = 0;
	for(unsigned int i=GlobalSize; i<MatSize; i++)
	  {
	    rRightHandSideVector[i]=LocalRightHandSideVector[indexi];
	    indexi++;
	  }
      }
    }

  //std::cout<<" rRightHandSideVector "<<rRightHandSideVector<<std::endl;
}


//************************************************************************************
//************************************************************************************


void CompositeCondition::CalculateLeftHandSide( MatrixType& rLeftHandSideMatrix, ProcessInfo& rCurrentProcessInfo )
{
  rLeftHandSideMatrix.clear();
  rLeftHandSideMatrix.resize(0,0);

  MatrixType LocalLeftHandSideMatrix;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	
	cn->CalculateLeftHandSide(LocalLeftHandSideMatrix,rCurrentProcessInfo);

	//resizing as needed the LHS
	unsigned int GlobalSize1 = rLeftHandSideMatrix.size1();
	unsigned int LocalSize1  = LocalLeftHandSideMatrix.size1();
	unsigned int MatSize1    = GlobalSize1+LocalSize1;

	unsigned int GlobalSize2 = rLeftHandSideMatrix.size2();
	unsigned int LocalSize2  = LocalLeftHandSideMatrix.size2();
	unsigned int MatSize2    = GlobalSize2+LocalSize2;

	rLeftHandSideMatrix.resize( MatSize1, MatSize2, true );

	unsigned int indexi = 0;
	for(unsigned int i=GlobalSize1; i<MatSize1; i++)
	  {
	    unsigned int indexj = 0;
	    for(unsigned int j=GlobalSize2; j<MatSize2; j++)
	      {
		rLeftHandSideMatrix(i,j)=LocalLeftHandSideMatrix(indexi,indexj);
		indexj++;
	      }
	    indexi++;
	  }
      }
    }
}


//***********************COMPUTE LOCAL SYSTEM CONTRIBUTIONS***************************
//************************************************************************************


void CompositeCondition::CalculateLocalSystem( std::vector< MatrixType >& rLeftHandSideMatrices,
					       const std::vector< Variable< MatrixType > >& rLHSVariables,
					       std::vector< VectorType >& rRightHandSideVectors,
					       const std::vector< Variable< VectorType > >& rRHSVariables,
					       ProcessInfo& rCurrentProcessInfo )
{
  for( unsigned int i=0; i< rLeftHandSideMatrices.size(); i++)
    {
      rLeftHandSideMatrices[i].clear();
      rLeftHandSideMatrices[i].resize(0,0);
    }

  for( unsigned int i=0; i< rRightHandSideVectors.size(); i++)
    {
      rRightHandSideVectors[i].clear();
      rRightHandSideVectors[i].resize(0);
    }

  //std::cout<<" Calculate local system Skin "<<std::endl;
  std::vector< MatrixType > LocalLeftHandSideMatrices;
  std::vector< VectorType > LocalRightHandSideVectors;

  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	
	cn->CalculateLocalSystem(LocalLeftHandSideMatrices, rLHSVariables, LocalRightHandSideVectors, rRHSVariables, rCurrentProcessInfo);
	//std::cout<<" LocalRightHandSideVector "<<LocalRightHandSideVector<<std::endl;

	//resizing as needed the LHSs
	for( unsigned int i=0; i< rLeftHandSideMatrices.size(); i++)
	  {
	    unsigned int GlobalSize1 = rLeftHandSideMatrices[i].size1();
	    unsigned int LocalSize1  = LocalLeftHandSideMatrices[i].size1();
	    unsigned int MatSize1    = GlobalSize1+LocalSize1;
	    
	    unsigned int GlobalSize2 = rLeftHandSideMatrices[i].size2();
	    unsigned int LocalSize2  = LocalLeftHandSideMatrices[i].size2();
	    unsigned int MatSize2    = GlobalSize2+LocalSize2;

	    rLeftHandSideMatrices[i].resize( MatSize1, MatSize2, true );
	  


	    unsigned int indexi = 0;
	    for(unsigned int i=GlobalSize1; i<MatSize1; i++)
	      {
		unsigned int indexj = 0;
		for(unsigned int j=GlobalSize2; j<MatSize2; j++)
		  {
		    rLeftHandSideMatrices[i](i,j)=LocalLeftHandSideMatrices[i](indexi,indexj);
		    indexj++;
		  }
		indexi++;
	      }

	  }

    
	//resizing as needed the RHSs
	for( unsigned int i=0; i< rRightHandSideVectors.size(); i++)
	  {

	    unsigned int GlobalSize1 = rRightHandSideVectors[i].size();
	    unsigned int LocalSize1  = LocalRightHandSideVectors[i].size();
	    unsigned int MatSize1    = GlobalSize1+LocalSize1;
    
	    rRightHandSideVectors[i].resize( MatSize1, true );
    
	    unsigned int indexi = 0;
	    for(unsigned int j=GlobalSize1; j<MatSize1; j++)
	      {
		rRightHandSideVectors[i][j]=LocalRightHandSideVectors[i][indexi];
		indexi++;
	      }

	  }
    
      }
    }

}

//************************************************************************************
//************************************************************************************


void CompositeCondition::CalculateRightHandSide( std::vector< VectorType >& rRightHandSideVectors, const std::vector< Variable< VectorType > >& rRHSVariables, ProcessInfo& rCurrentProcessInfo )
{
  //std::cout<<" Calculate local rhs system Skin "<<std::endl;
  for( unsigned int i=0; i< rRightHandSideVectors.size(); i++)
    {
      rRightHandSideVectors[i].clear();
      rRightHandSideVectors[i].resize(0);
    }


  std::vector< VectorType > LocalRightHandSideVectors;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      if( IsActive(cn,rCurrentProcessInfo) ){
	
	cn->CalculateRightHandSide(LocalRightHandSideVectors, rRHSVariables, rCurrentProcessInfo);

	//resizing as needed the RHSs
	for( unsigned int i=0; i< rRightHandSideVectors.size(); i++)
	  {

	    unsigned int GlobalSize  = rRightHandSideVectors[i].size();
	    unsigned int LocalSize   = LocalRightHandSideVectors[i].size();
	    unsigned int MatSize     = GlobalSize+LocalSize;
    
	    rRightHandSideVectors[i].resize( MatSize, true );
    
	    unsigned int indexi = 0;
	    for(unsigned int j=GlobalSize; j<MatSize; j++)
	      {
		rRightHandSideVectors[i][j]=LocalRightHandSideVectors[i][indexi];
		indexi++;
	      }
	  }
      }
    }

}



//************************************************************************************
//************************************************************************************


void CompositeCondition:: MassMatrix(MatrixType& rMassMatrix, ProcessInfo& rCurrentProcessInfo)
{
  // MatrixType LocalMassMatrix;
  // for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
  //    {
  //      cn->MassMatrix(LocalMassMatrix,rCurrentProcessInfo);
  //   rMassMatrix+=LocalMassMatrix;
  // }
}


//************************************************************************************
//************************************************************************************


void CompositeCondition:: DampMatrix(MatrixType& rDampMatrix, ProcessInfo& rCurrentProcessInfo)
{
  // MatrixType LocalDampMatrix;
  // for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
  //    {
  //      cn->DampMatrix(LocalDampMatrix,rCurrentProcessInfo);
  //   rDampMatrix+=LocalDampMatrix;
  // }
}

//************************************************************************************
//************************************************************************************

void CompositeCondition::CalculateOnIntegrationPoints( const Variable<double>& rVariable, std::vector<double>& rOutput, const ProcessInfo& rCurrentProcessInfo )
{
  std::vector<double> LocalOutput;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->CalculateOnIntegrationPoints(rVariable,LocalOutput,rCurrentProcessInfo);

      if ( LocalOutput.size() != rOutput.size() )
	rOutput.resize(LocalOutput.size(),true);

      for(unsigned int i=0; i<LocalOutput.size(); i++)
	{     
	  rOutput[i]+=LocalOutput[i];
	}

    }
}


//************************************************************************************
//************************************************************************************

void CompositeCondition::CalculateOnIntegrationPoints( const Variable<Vector>& rVariable, std::vector<Vector>& rOutput, const ProcessInfo& rCurrentProcessInfo )
{
  std::vector< Vector > LocalOutput;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->CalculateOnIntegrationPoints(rVariable,LocalOutput,rCurrentProcessInfo);

      if ( LocalOutput.size() != rOutput.size() )
	rOutput.resize(LocalOutput.size());

      for(unsigned int i=0; i<LocalOutput.size(); i++)
	{     
	  if ( LocalOutput[i].size() != rOutput[i].size() )
	    rOutput[i].resize( LocalOutput[i].size(), false );
	}

      for(unsigned int i=0; i<LocalOutput.size(); i++)
	{     
	  rOutput[i]+=LocalOutput[i];
	}
    }
}

//************************************************************************************
//************************************************************************************

void CompositeCondition::CalculateOnIntegrationPoints( const Variable<Matrix >& rVariable, std::vector< Matrix >& rOutput, const ProcessInfo& rCurrentProcessInfo )
{
  std::vector< Matrix > LocalOutput;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->CalculateOnIntegrationPoints(rVariable,LocalOutput,rCurrentProcessInfo);

      if ( LocalOutput.size() != rOutput.size() )
	rOutput.resize(LocalOutput.size());

      for(unsigned int i=0; i<LocalOutput.size(); i++)
	{     
	  if ( LocalOutput[i].size2() != rOutput[i].size2() )
	    rOutput[i].resize( LocalOutput[i].size1(), LocalOutput[i].size2(), false );
	}

      for(unsigned int i=0; i<LocalOutput.size(); i++)
	{     
	  rOutput[i]+=LocalOutput[i];
	}
    }
 
}


//************************************************************************************
//************************************************************************************

void CompositeCondition::Calculate( const Variable<double>& rVariable, double& rOutput, const ProcessInfo& rCurrentProcessInfo )
{
  double LocalOutput;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      cn->Calculate(rVariable,LocalOutput,rCurrentProcessInfo);
      rOutput+=LocalOutput;
    }
  
}



//************************************************************************************
//************************************************************************************
/**
 * This function provides the place to perform checks on the completeness of the input.
 * It is designed to be called only once (or anyway, not often) typically at the beginning
 * of the calculations, so to verify that nothing is missing from the input
 * or that no common error is found.
 * @param rCurrentProcessInfo
 */
int  CompositeCondition::Check( const ProcessInfo& rCurrentProcessInfo )
{
  int check = 1;
  int child_check = 1;
  for (ConditionIterator cn = mpChildConditions->begin() ; cn != mpChildConditions->end(); ++cn)
    {
      child_check = cn->Check(rCurrentProcessInfo);
      if(child_check == 0)
	check = 0;
    }
    
  return check;
}


void CompositeCondition::save( Serializer& rSerializer ) const
{
    KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, Condition )
    rSerializer.save("mpChildConditions",mpChildConditions);
}

void CompositeCondition::load( Serializer& rSerializer )
{
    KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, Condition )
    rSerializer.load("mpChildConditions",mpChildConditions);
}



} // Namespace Kratos


