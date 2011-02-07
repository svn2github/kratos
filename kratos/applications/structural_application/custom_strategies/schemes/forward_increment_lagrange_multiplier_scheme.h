//   
//   Project Name:        Kratos       
//   Last Modified by:    $Author: Nelson $
//   Date:                $Date: 2011-01-21  $
//   Revision:            $Revision: 1.0 $
//
//


#if !defined(FORWARD_INCREMENT_LAGRANGE_MULTIPLIER_SCHEME )
#define  FORWARD_INCREMENT_LAGRANGE_MULTIPLIER_SCHEME



// System includes
#include <string>
#include <iostream> 
#include <cmath>

// External includes 
#include "boost/smart_ptr.hpp"


// Project includes
#include "includes/define.h"
#include "includes/ublas_interface.h"
#include "includes/model_part.h"
#include "utilities/math_utils.h"
#include "custom_utilities/sd_math_utils.h"



namespace Kratos
{

  ///@name Kratos Globals
  ///@{ 
  
  ///@} 
  ///@name Type Definitions
  ///@{ 
  
  ///@} 
  ///@name  Enum's
  ///@{
      
  ///@}
  ///@name  Functions 
  ///@{
      
  ///@}
  ///@name Kratos Classes
  ///@{
  
  /// Short class definition.
  /** Detail class definition.
  */
  class ForwardIncrementLagrangeMultiplierScheme
    {
    public:
      ///@name Type Definitions
      ///@{
      
      typedef ModelPart::ConditionsContainerType::ContainerType ConditionsContainerType;
      typedef ConditionsContainerType::iterator                 ConditionsContainerIterator;
      typedef ConditionsContainerType::value_type               ConditionsPointerType;
      
	
      /// Pointer definition of ForwardIncrementLagrangeMultiplierScheme
      KRATOS_CLASS_POINTER_DEFINITION(ForwardIncrementLagrangeMultiplierScheme);
  
      ///@}
      ///@name Life Cycle 
      ///@{ 
      
      /// Default constructor.
      ForwardIncrementLagrangeMultiplierScheme(){}
      ForwardIncrementLagrangeMultiplierScheme(ModelPart& model_part, const unsigned int& dimension
      ) : mr_model_part(model_part), mrdimension(dimension) 
      {
      }

      /// Destructor.
      virtual ~ForwardIncrementLagrangeMultiplierScheme(){}
      
      
      
      
      void CalculateContactForceAndDisplacementCorrections(
         ConditionsContainerIterator const& end_previos,   
         ConditionsContainerIterator const& end_actual 
      )
      {
      
	  std::cout<< "      Simultaneous Jacobi Iteration Method" << std::endl; 
	  ProcessInfo& CurrentProcessInfo      =  mr_model_part.GetProcessInfo();
	  //ConditionsContainerType& pConditions =  mr_model_part.ConditionsArray();   
	  //const double current_delta_time      =  CurrentProcessInfo[DELTA_TIME]; 
	  const unsigned int   max             =  100;  
	  unsigned int   iter                  =  0;  
	  

	  #ifdef _OPENMP
	  int number_of_threads = 1; //omp_get_max_threads();
	  #else
	  int number_of_threads = 1;
	  #endif

	  vector<unsigned int> condition_partition;
	  int distance   = std::distance(end_previos, end_actual);
	  
	  CreatePartition(number_of_threads, distance, condition_partition);
	  
	  double Rigth_Term, Left_Term;
	  bool   is_converged = false;
	  const double EPS   = 1E-7;
	  while(is_converged ==false &&   iter++ < max )
	  { 
	      Rigth_Term = 0.00; 
	      Left_Term  = 0.00;
                   
 	      /// WARNING = STEP1
	      #pragma omp parallel for shared(CurrentProcessInfo, Rigth_Term, Left_Term)
	      for(int k=0; k<number_of_threads; k++)
	      {
		ConditionsContainerType::iterator it_begin=end_previos + condition_partition[k];
		ConditionsContainerType::iterator it_end=end_previos+condition_partition[k+1];

		for (ConditionsContainerIterator it= it_begin; it!=it_end; ++it)
		{
		  JacobiIteration(*it, CurrentProcessInfo);  
		}
		
		UpadateDisplacement();
		
	        for (ConditionsContainerIterator it= it_begin; it!=it_end; ++it)
		  { 
		    CkeckConvergence(*it, Rigth_Term, Left_Term);
		  }

	      }
	      
	      is_converged = IsConverged(EPS, Rigth_Term, Left_Term);
	      std::cout << "         Contact Residual =  " << Left_Term  << std::endl;
	      
          }
          
              MoveMeshAgain();
              std::cout << "         Contact Residual =  " << Left_Term  << std::endl;
	      std::cout << "         Tolerance        =  " << EPS  << std::endl;
	      std::cout << "         Iteration Number =  " << iter       << std::endl; 
	     
              if (iter>max) 
	      {
		KRATOS_WATCH(" NOT CONVERGENCE FOR CONTACT!!!!!!!!")
	        //KRATOS_ERROR( std::logic_error,  " NOT CONVERGENCE FOR CONTACT!!!!!!!!" , "" );
	      }
              
      }
      
      
      /// Compute Lamdas and Contact Dislplacement
      void JacobiIteration(const ConditionsPointerType& rCond, ProcessInfo& CurrentProcessInfo)
      {
	
	        const double current_delta_time      =  CurrentProcessInfo[DELTA_TIME]; 
		double  lamda_old                    =  0.00;
	
		Vector Constraint;
		Matrix Constraint_Matrix;
		Matrix Mass;
		Matrix InvMass;
		Matrix Aux;
		Matrix InvAux;
		Vector Displ;
	
		Vector& lambdas                =  (rCond)->GetValue(LAMBDAS); 
		Vector& delta_lambdas          =  (rCond)->GetValue(DELTA_LAMBDAS);
		
		
		
		ComputeConstraintVector(rCond,  Constraint);
		Constraint_Matrix = ZeroMatrix(1, Constraint.size());
		
		Constraint_Matrix(0,0) = Constraint[0]; 
		Constraint_Matrix(0,1) = Constraint[1];
		Constraint_Matrix(0,2) = Constraint[2];
		Constraint_Matrix(0,3) = Constraint[3];
		Constraint_Matrix(0,4) = Constraint[4];
		Constraint_Matrix(0,5) = Constraint[5];
	
		rCond->MassMatrix(Mass, CurrentProcessInfo);
		InvertDiagonalMatrix(Mass , InvMass);
		
		Aux           = ZeroMatrix(Constraint_Matrix.size1(), Constraint_Matrix.size1());
	        noalias(Aux)  = prod(Matrix(prod(Constraint_Matrix,InvMass)), (trans(Constraint_Matrix)));
		
		CheckMatrix(Aux);
		
		InvAux        = ZeroMatrix(Aux.size1(), Aux.size1());
		SD_MathUtils<double>::InvertMatrix(Aux,InvAux);
		
		GetNodeDisplacement(rCond, Displ); 
		noalias(delta_lambdas) = (1.00/(current_delta_time * current_delta_time))  * prod(InvAux, Vector( prod(Constraint_Matrix, Displ) ) );
		lamda_old              = lambdas[0]; 
		noalias(lambdas)       += delta_lambdas;  
                
		if (lambdas[0] > 0.00) 
		{
		  lambdas[0]       = 0.00;
		  delta_lambdas[0] = lamda_old;
		  
		}

		CalculateContactDisplacement(rCond, delta_lambdas, Constraint, InvMass);
		
      }
      
      
      void CkeckConvergence(const ConditionsPointerType& rCond, double& Rigth_Term,
	                                                         double&  Left_Term)
      {
	Vector& delta_lambdas   =  (rCond)->GetValue(DELTA_LAMBDAS);
	Vector& lambdas         =  (rCond)->GetValue(LAMBDAS);
	Left_Term              +=  norm_2(delta_lambdas);
	Rigth_Term             +=  norm_2(lambdas);
	
	unsigned int size_delta_lambdas =  delta_lambdas.size();
        noalias(delta_lambdas)          = ZeroVector(size_delta_lambdas);
      }
      
      bool IsConverged(const double& EPS, const double& Rigth_Term,  const double& Left_Term)
      {
	
	return (Left_Term < EPS);
      }
      
      void InvertDiagonalMatrix(const Matrix& rMatrix   ,Matrix& rResult)
      {
	rResult = ZeroMatrix(rMatrix.size1(), rMatrix.size1());
	for (unsigned int i = 0; i<rMatrix.size1(); i++ )
	   rResult(i,i) = 1.00 / rMatrix(i,i);   
	
	return;
      }
      
      
        void CheckMatrix(Matrix& rMatrix)
      {
	const double toler = 1.0E-10; 
	for (unsigned int i = 0; i<rMatrix.size1(); i++ )
	   if(std::fabs(rMatrix(i,i)) < toler)  { rMatrix(i,i) = 1E-14;}   
	return;
      }

     void GetNodeDisplacement(const ConditionsPointerType& rCond, Vector& Displ) 
      {
	Condition::GeometryType& geom       = rCond->GetGeometry(); 
	const unsigned int dimension        = geom.WorkingSpaceDimension();
        const unsigned int dim2             = geom.size()*dimension;
	Displ                               = ZeroVector(dim2);
	unsigned int count                  = 0;
	for(unsigned int i = 0; i<geom.size(); i++){
	   //array_1d<double,3>& actual_displacement   = geom[i].FastGetSolutionStepValue(DISPLACEMENT);
	   Displ[count]   =  geom[i].X0() + geom[i].GetSolutionStepValue(DISPLACEMENT_X);   // geom[i].X();    // actual_displacement[0];
	   Displ[count+1] =  geom[i].Y0() + geom[i].GetSolutionStepValue(DISPLACEMENT_Y);   // geom[i].Y();   // actual_displacement[1];
	   count = count + 2;
	   if(dim2==3){
	        Displ[count+2]  =  geom[i].Z0() + geom[i].GetSolutionStepValue(DISPLACEMENT_Z);
		count +=3;
	        }
	}
	return;
      }
      
       /// Computa el incremento de desplazamiento producido por el contacto
       void CalculateContactDisplacement(        const ConditionsPointerType& rCond, 
                                                 const Vector& delta_lambdas, 
					         const Vector& Constraint,
	                                         const Matrix& InvMass)
      {
	
	ProcessInfo& CurrentProcessInfo     =  mr_model_part.GetProcessInfo();
        Condition::GeometryType& geom       =  rCond->GetGeometry(); 
	const unsigned int dimension        =  geom.WorkingSpaceDimension();
        const unsigned int dim2             =  geom.size()*dimension;
	const double current_delta_time     =  CurrentProcessInfo[DELTA_TIME];
	
	Vector Displ                        =  ZeroVector(dim2); 
	noalias(Displ)                      =  -current_delta_time * current_delta_time * delta_lambdas[0] * prod(InvMass, Constraint);
	
	unsigned int count                  = 0;
	for(unsigned int i = 0; i<geom.size(); i++){
	   geom[i].SetLock();
	   array_1d<double,3>&  Contact_Displ  =  geom[i].FastGetSolutionStepValue(DISPLACEMENT_OLD); /// WARNING CONTACT DISPLACEMENT
	   Contact_Displ[0]  +=     Displ[0+count];
	   Contact_Displ[1]  +=     Displ[1+count];
	   Contact_Displ[2]  +=     0.00;
	   count             +=     2;
	   if(dim2==3){
	        Contact_Displ[2]  +=  Displ[2+count];
		count +=3;
	        }
	  geom[i].UnSetLock();   
	          
	}
	return;
      }
      
      void ComputeConstraintVector(const ConditionsPointerType& rCond,  Vector& Constraint)
      {
	
	Condition::GeometryType& geom       = rCond->GetGeometry(); 
        const unsigned int dimension        = geom.WorkingSpaceDimension();
        const unsigned int dim2             = geom.size()*dimension;
	ProcessInfo& CurrentProcessInfo     =  mr_model_part.GetProcessInfo();
        
	Constraint = ZeroVector(dim2);
	rCond->Calculate(CONSTRAINT_VECTOR, Constraint, CurrentProcessInfo);
      }
      
       
      void UpadateDisplacement()
      
      {
	KRATOS_TRY
	
	for(ModelPart::NodeIterator i = mr_model_part.NodesBegin() ; 
	i != mr_model_part.NodesEnd() ; ++i)
        {
	 array_1d<double,3>&  Contact_Displ         =  i->FastGetSolutionStepValue(DISPLACEMENT_OLD);
	 array_1d<double,3>&  actual_displacement   =  i->FastGetSolutionStepValue(DISPLACEMENT);
	 if( (i->pGetDof(DISPLACEMENT_X))->IsFixed() == false )
	    { 
	      actual_displacement[0]+=Contact_Displ[0];
	    }
	 
	 if( (i->pGetDof(DISPLACEMENT_Y))->IsFixed() == false )
	    { 
	      actual_displacement[1]+=Contact_Displ[1];
	    }
	 
	 if (mrdimension==3)
	     {  
	       if( (i->pGetDof(DISPLACEMENT_Z))->IsFixed() == false )
	        { 
	           actual_displacement[2]+=Contact_Displ[2];
	        }
	     }
	         
	    Contact_Displ = ZeroVector(3);
        }

        KRATOS_CATCH("")

	}
      
      
       void MoveMeshAgain()
      
      {
	KRATOS_TRY
	
	for(ModelPart::NodeIterator i = mr_model_part.NodesBegin() ; 
	i != mr_model_part.NodesEnd() ; ++i)
        {
	 array_1d<double,3>&  actual_displacement   =  i->FastGetSolutionStepValue(DISPLACEMENT);
	 if( (i->pGetDof(DISPLACEMENT_X))->IsFixed() == false )
	    { 
	      (i)->X() = (i)->X0() + i->GetSolutionStepValue(DISPLACEMENT_X);
	    }
	 
	 if( (i->pGetDof(DISPLACEMENT_Y))->IsFixed() == false )
	    { 
	      (i)->Y() = (i)->Y0() + i->GetSolutionStepValue(DISPLACEMENT_Y);
	    }
	 
	 if (mrdimension==3)
	     {  
	       if( (i->pGetDof(DISPLACEMENT_Z))->IsFixed() == false )
	        { 
	           (i)->Z() = (i)->Z0() + i->GetSolutionStepValue(DISPLACEMENT_Z);
	        }
	     }
	         
        }

        KRATOS_CATCH("")

	}
    

      ///@}
      ///@name Operators 
      ///@{
      
      
      ///@}
      ///@name Operations
      ///@{
      
      
      ///@}
      ///@name Access
      ///@{ 
      
      
      ///@}
      ///@name Inquiry
      ///@{
      
      
      ///@}      
      ///@name Input and output
      ///@{

      /// Turn back information as a string.
      
      virtual std::string Info() const
      { 
	return "  ForwardIncrementLagrangeMultiplierScheme ";
      }
      
      /// Print information about this object.
      virtual void PrintInfo(std::ostream& rOStream) const{}

      /// Print object's data.
      virtual void PrintData(std::ostream& rOStream) const{}
      
            
      ///@}      
      ///@name Friends
      ///@{
      
            
      ///@}
      
    protected:
      ///@name Protected static Member Variables 
      ///@{ 
        
        
      ///@} 
      ///@name Protected member Variables 
      ///@{ 
        
        
      ///@} 
      ///@name Protected Operators
      ///@{ 
        
        
      ///@} 
      ///@name Protected Operations
      ///@{ 
        
        
      ///@} 
      ///@name Protected  Access 
      ///@{ 
        
        
      ///@}      
      ///@name Protected Inquiry 
      ///@{ 
        
        
      ///@}    
      ///@name Protected LifeCycle 
      ///@{ 
      
            
      ///@}
      
    private:
      ///@name Static Member Variables 
      ///@{ 
        
        
      ///@} 
      ///@name Member Variables 
      ///@{ 
        
      ModelPart  mr_model_part;
      unsigned int  mrdimension;
 
      
        
      ///@} 
      ///@name Private Operators
      ///@{ 
        
        
      ///@} 
      ///@name Private Operations
      ///@{ 
        
     
      ///@} 
      ///@name Private  Access 
      ///@{ 
        
        
      ///@}    
      ///@name Private Inquiry 
      ///@{ 
        
        
      ///@}    
      ///@name Un accessible methods 
      ///@{ 
      
      inline void CreatePartition(unsigned int number_of_threads, const int number_of_rows, vector<unsigned int>& partitions)
    {
      partitions.resize(number_of_threads+1);
      int partition_size = number_of_rows / number_of_threads;
      partitions[0] = 0;
      partitions[number_of_threads] = number_of_rows;
      for(unsigned int i = 1; i<number_of_threads; i++)
      partitions[i] = partitions[i-1] + partition_size ;
  } 
	
      /*
      /// Assignment operator.
      ForwardIncrementLagrangeMultiplierScheme& operator=(ForwardIncrementLagrangeMultiplierScheme const& rOther){}

      /// Copy constructor.
      ForwardIncrementLagrangeMultiplierScheme(ForwardIncrementLagrangeMultiplierScheme const& rOther){}
      */
        
      ///@}    
        
    }; // Class ForwardIncrementLagrangeMultiplierScheme 

  ///@} 
  
  ///@name Type Definitions       
  ///@{ 
  
  
  ///@} 
  ///@name Input and output 
  ///@{ 
        
 /*
  /// input stream function
  inline std::istream& operator >> (std::istream& rIStream, 
				    ForwardIncrementLagrangeMultiplierScheme& rThis);

  /// output stream function
  inline std::ostream& operator << (std::ostream& rOStream, 
				    const ForwardIncrementLagrangeMultiplierScheme& rThis)
    {
      rThis.PrintInfo(rOStream);
      rOStream << std::endl;
      rThis.PrintData(rOStream);

      return rOStream;
    }
  ///@} 
  */
  
}  // namespace Kratos.

#endif // FORWARD_INCREMENT_LAGRANGE_MULTIPLIER_SCHEME  defined 


