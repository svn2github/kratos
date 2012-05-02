//   
//   Project Name:        Kratos       
//   Last Modified by:    $Author: Nelson Lafontaine $
//   Date:                $Date: 2008-10-13 08:56:42 $
//   Revision:            $Revision: 1.5 $
//
//

#if !defined(KRATOS_EXPLICIT_SOLVER_STRATEGY)
#define  KRATOS_EXPLICIT_SOLVER_STRATEGY

// /* External includes */
// #include "boost/smart_ptr.hpp"

// System includes

// Project includes
#include "utilities/timer.h"
#include "custom_utilities/neighbours_calculator.h"
#include "custom_utilities/spheric_particle_hertzian.h"
#include "custom_elements/spheric_particle.h" //M: le afegit jo.. no hi era. cal que hi sigui oi???

/* System includes */
#include <limits>
#include<iostream>
#include<iomanip>
#include <iostream>

/* External includes */
#ifdef _OPENMP
#include <omp.h>
#endif

#include "boost/smart_ptr.hpp"

/* Project includes */
#include "includes/define.h"
#include "utilities/openmp_utils.h"
#include "includes/model_part.h"
#include "solving_strategies/strategies/solving_strategy.h"
#include "solving_strategies/schemes/scheme.h"


#include "custom_utilities/neighbours_calculator.h"


namespace Kratos
{
  ///@addtogroup ApplicationNameApplication
  ///@{

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
  enum Constraint_Enforcement{Penalty_Methods, Lagrange_Multiplier_Methods};  
  template<
  class TSparseSpace,
  class TDenseSpace, 
  class TLinearSolver> 
  class ExplicitSolverStrategy : public  SolvingStrategy<TSparseSpace,TDenseSpace,TLinearSolver>
     {
      public:
      ///@name Type Definitions
      ///@{
 
	  typedef SolvingStrategy<TSparseSpace,TDenseSpace,TLinearSolver> BaseType;
	  typedef typename BaseType::TDataType TDataType;
	  typedef typename BaseType::TBuilderAndSolverType TBuilderAndSolverType;
	  typedef typename BaseType::TSchemeType TSchemeType;
	  typedef typename BaseType::DofsArrayType DofsArrayType;
	  typedef typename Element::DofsVectorType DofsVectorType;
	  
	  typedef ModelPart::NodesContainerType NodesArrayType;
	  typedef ModelPart::ElementsContainerType ElementsArrayType;
	  typedef ModelPart::ConditionsContainerType ConditionsArrayType;	  
	  
	  typedef ModelPart::NodesContainerType::ContainerType      NodesContainerType;
	  typedef ModelPart::ElementsContainerType::ContainerType   ElementsContainerType;
	  typedef ModelPart::ConditionsContainerType::ContainerType ConditionsContainerType;
	  
      /// Pointer definition of ExplicitSolverStrategy
      KRATOS_CLASS_POINTER_DEFINITION(ExplicitSolverStrategy);
 
      
      ///@}
      ///@name Life Cycle 
      ///@{ 
      
      /// Default constructor.
       ExplicitSolverStrategy(){}
      
       ExplicitSolverStrategy(ModelPart& model_part,   
			      const int        dimension,
			      const double     damping_ratio,       ///para calcular la matriz de amortiguamiento proporcional a la masa
                              const double     fraction_delta_time,
                              const double     max_delta_time,
		              const bool       MoveMeshFlag,
		              typename         TSchemeType::Pointer pScheme
			)
			: SolvingStrategy<TSparseSpace,TDenseSpace,TLinearSolver>(model_part, MoveMeshFlag),
                        
                            mdimension(dimension) //inicialitzacio de variables const. no poden inicialitzarse a l'esquerra d'un igual.

                        {
			 
                           mElementsAreInitialized    = false;
			   mConditionsAreInitialized  = false;
			   mCalculateOldTime          = false;
                           mSolutionStepIsInitialized = false; 
			   mInitializeWasPerformed    = false;
                           mComputeTime               = false;
			   mInitialConditions         = false;
                           mdamping_ratio             = damping_ratio;
                           mfraction_delta_time       = fraction_delta_time; 
                           mmax_delta_time            = max_delta_time;
                           molddelta_time             = 0.00;
                           mtimestep                  = 0.00;
			}
      /// Destructor.
      virtual ~ExplicitSolverStrategy(){}
      

      ///@}
      ///@name Operators 
      ///@{
      
      
      ///@}
      ///@name Operations
      ///@{
      
	/*
      void Search_Neighbours()
          {
             Neighbours_Calculator<TDim, ParticleType, ParticlePointerType, ParticleVectorType, ParticleWeakVectorType, ParticlePointerVectorType,
             ParticleWeakIteratorType, ParticleIteratorType, ParticlePointerIteratorType, DistanceVectorType, DistanceIteratorType>::
             Search_Neighbours(mListOfParticlePointers, *mModelPart, mRadiusSearch, mProximityTol);
          }	 
       */
	
      double Solve()
      {
	KRATOS_TRY
        std::cout<<std::fixed<<std::setw(15)<<std::scientific<<std::setprecision(5);
        ModelPart& r_model_part              = BaseType::GetModelPart();
	ProcessInfo& CurrentProcessInfo      = r_model_part.GetProcessInfo();
	
	
	#ifdef _OPENMP
	double start_prod = omp_get_wtime();   
	#endif
	
        std::cout<<"------------------------------------------------------------------------"<<std::endl;
        std::cout<<"                 KRATOS DEM APPLICATION. TIME STEPS = "           << CurrentProcessInfo[TIME_STEPS]     <<std::endl;                                                    
	std::cout<<"------------------------------------------------------------------------"<<std::endl;
	
        ///Initialize solution step
	InitializeSolutionStep(); 
	
        ComputeCriticalTime();
	
	if(mInitialConditions==false){
	   ComputeInitialConditions();
	   GetForce();
	}
	
        /// Predict displacements
	ComputeIntermedialVelocityAndNewDisplacement();
	
	/// Computa los valores del paso de las velocidades y aceleraciones	
        ComputeOldVelocitiesAndAccelerations();
	
	/// Actualizacion de los desplazamientos
        MoveMesh();
	
        ///Computing The new internal and external forces  for n+1 step
	GetForce();
	
        ///Finalize solution step
	FinalizeSolutionStep();
	
	///Computing energies
	CalculateEnergies();
	
        #ifdef _OPENMP
	double stop_prod = omp_get_wtime();
	std::cout << "  Time solving                                 = "<<   stop_prod - start_prod    << "  seconds" << std::endl; 
	#endif
	
	std::cout <<"FINISHED SOLVE"<<std::endl;
	return 0.00;   
	KRATOS_CATCH("")
	
      }
	
	void Initialize()
        {
            KRATOS_TRY

       //M: faig una primera búsqueda abans de inicialitzar elements pk allí guardaré veins inicials i altres coses.

        ModelPart& r_model_part           = BaseType::GetModelPart();
          
        typedef DiscreteElement                                                 ParticleType;
        typedef ParticleType::Pointer                                           ParticlePointerType;
        typedef ElementsContainerType                                           ParticleContainerType;
        typedef WeakPointerVector<Element>                                      ParticleWeakVectorType;  //hauria de ser Discrete_Element
        typedef typename std::vector<ParticlePointerType>                       ParticlePointerVectorType;
        typedef WeakPointerVector<Element>::iterator                            ParticleWeakIteratorType;
        typedef typename std::vector<ParticleType>::iterator                    ParticleIteratorType;
        typedef typename std::vector<ParticlePointerType>::iterator             ParticlePointerIteratorType;
        typedef std::vector<double>                                             DistanceVectorType;
        typedef std::vector<double>::iterator                                   DistanceIteratorType;


        if (mdimension == 2)
            Neighbours_Calculator<2, ParticleType>::Search_Neighbours(r_model_part);
        else if (mdimension == 3)
            Neighbours_Calculator<3, ParticleType>::Search_Neighbours(r_model_part);
        

         ///Initializing elements
	  if(mElementsAreInitialized == false)
	     InitializeElements();
	  mInitializeWasPerformed   = true;


          KRATOS_CATCH("")
	}
	
      void  ComputeCriticalTime()
	{
	}
	
	void ComputeInitialConditions()
	{
	}
	
	void InitializeSolutionStep()
	{
	}
	
	void GetForce()
	{
	}
	
	void ComputeIntermedialVelocityAndNewDisplacement()
	{
	}
	  
	void ComputeOldVelocitiesAndAccelerations()
	{
	}
	  
	void MoveMesh()
	{
	}

	void FinalizeSolutionStep()
	{
	}
	  
	void CalculateEnergies()
	{
	}
	
      
      ///@}
      ///@name Access
      ///@{ 
      
      
      ///@}
      ///@name Inquiry
      ///@{
      
      
      ///@}      
      ///@name Input and output
      ///@{

      /* 
      /// Turn back information as a string.
      virtual std::string Info() const;
      
      /// Print information about this object.
      virtual void PrintInfo(std::ostream& rOStream) const;

      /// Print object's data.
      virtual void PrintData(std::ostream& rOStream) const;
      */
            
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
        

    const unsigned int    mdimension;
    unsigned int    minitial_conditions_size;  
    unsigned int    mcontact_conditions_size;  
    bool   mInitialCalculations;
    bool   mElementsAreInitialized;
    bool   mConditionsAreInitialized;
    bool   mCalculateOldTime;
    bool   mSolutionStepIsInitialized;
    bool   mComputeTime;
    bool   mInitializeWasPerformed;
    bool   mInitialConditions;
    double mdamping_ratio;
    double malpha_damp;
    double mbeta_damp; 
    double mfraction_delta_time;
    double mmax_delta_time;
    double molddelta_time;
    double mtimestep;  /// la suma de los delta time

      ///@} 
      ///@name Private Operators
      ///@{ 
        
      ///@} 
      ///@name Private Operations
      ///@{ 
           
      void InitializeElements()
{
      KRATOS_TRY
      ModelPart& r_model_part          = BaseType::GetModelPart();
      ProcessInfo& CurrentProcessInfo  = r_model_part.GetProcessInfo();  
      ElementsArrayType& pElements     = r_model_part.Elements(); 

      Matrix MassMatrix;
      #ifdef _OPENMP
      int number_of_threads = omp_get_max_threads();
      #else
      int number_of_threads = 1;
       #endif

      vector<unsigned int> element_partition;
      OpenMPUtils::CreatePartition(number_of_threads, pElements.size(), element_partition);
      unsigned int index = 0;
      
      #pragma omp parallel for private(index, MassMatrix)  //M. proba de compilar sense mass matrix??
      for(int k=0; k<number_of_threads; k++)
      {
	typename ElementsArrayType::iterator it_begin=pElements.ptr_begin()+element_partition[k];
	typename ElementsArrayType::iterator it_end=pElements.ptr_begin()+element_partition[k+1];
	for (ElementsArrayType::iterator it= it_begin; it!=it_end; ++it)
	  {
	    Element::GeometryType& geom = it->GetGeometry(); 
	    (it)->Initialize(); 
	    (it)->MassMatrix(MassMatrix, CurrentProcessInfo); //NELSON: fa falta????
           
            /*
	    const unsigned int& dim   = geom.WorkingSpaceDimension();
	    index = 0;
	    for (unsigned int i = 0; i <geom.size(); i++)
	     {
	        double& mass = geom(i)->FastGetSolutionStepValue(NODAL_MASS);
		index = i*dim;
		mass  = mass + MassMatrix(index,index);
	     }
	     */
	 }
      }
      
     //r_model_part.GetCommunicator().AssembleCurrentData(NODAL_MASS); 
     mElementsAreInitialized   = true;
     KRATOS_CATCH("")
}
      
       /*
      void Set_Initial_Contacts()
      {
	 ///particula->GetValue(NEIGHBOUR_ELEMENTS)).push_back( *(particula.base()));
	//  busqueda vecinos inicial amb tolerancia
        //aqui he de guardar els veins inicials i per cada un la distancia delta de la tolerancia.

       KRATOS_TRY
      
       ModelPart& r_model_part          = BaseType::GetModelPart();  
       ProcessInfo& CurrentProcessInfo  = r_model_part.GetProcessInfo();  //M: ho necesitu aki per algoo?? per treure la tolerancia porser
       ElementsArrayType& pElements     = r_model_part.Elements(); 
       
      #ifdef _OPENMP
      int number_of_threads = omp_get_max_threads();
      #else
      int number_of_threads = 1;
       #endif

      vector<unsigned int> element_partition;
      OpenMPUtils::CreatePartition(number_of_threads, pElements.size(), element_partition);
     
      unsigned int index = 0;
      
      #pragma omp parallel for private(index)
      for(int k=0; k<number_of_threads; k++)
      {
	typename ElementsArrayType::iterator it_begin=pElements.ptr_begin()+element_partition[k];
	typename ElementsArrayType::iterator it_end=pElements.ptr_begin()+element_partition[k+1];
	for (ElementsArrayType::iterator it= it_begin; it!=it_end; ++it)
	  {
	    Element::GeometryType& geom = it->GetGeometry(); 
	    (it)->Initialize(); //add , set initals...
	    KRATOS_WATCH(geom)
       
       int size = pElements.size();

       ///pragma omp parallel for

          

         } // loop over particle_it
 
            
        KRATOS_CATCH("")

	//  get continun vecinos
      }
   */
      
      ///@} 
      ///@name Private  Access 
      ///@{ 
        
        
      ///@}    
      ///@name Private Inquiry 
      ///@{ 
        
        
      ///@}    
      ///@name Un accessible methods 
      ///@{ 
      
      /// Assignment operator.
      //ExplicitSolverStrategy& operator=(ExplicitSolverStrategy const& rOther);

      /// Copy constructor.
      //ExplicitSolverStrategy(ExplicitSolverStrategy const& rOther);

        
      ///@}    
        
    }; // Class ExplicitSolverStrategy 

  ///@} 
  
  ///@name Type Definitions       
  ///@{ 
  
  
  ///@} 
  ///@name Input and output 
  ///@{ 
        
 /*
  /// input stream function
  inline std::istream& operator >> (std::istream& rIStream, 
				    ExplicitSolverStrategy& rThis){return rIStream;}

  /// output stream function
  inline std::ostream& operator << (std::ostream& rOStream, 
				    const ExplicitSolverStrategy& rThis)
    {
      rThis.PrintInfo(rOStream);
      rOStream << std::endl;
      rThis.PrintData(rOStream);

      return rOStream;
    }
    */
  ///@}

  ///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_FILENAME_H_INCLUDED  defined 




