//
//   Project Name:        Kratos
//   Last Modified by:    $Author: clabra $
//   Date:                $Date: 2007-03-29 19:37:47 $
//   Revision:            $Revision: 1.2 $
//
//

#if !defined(KRATOS_OMP_DEM_SEARCH_H_INCLUDED )
#define  KRATOS_OMP_DEM_SEARCH_H_INCLUDED

// System includes
#include <string>
#include <iostream> 

// include kratos definitions
#include "includes/define.h"

// Project includes
#include "spatial_containers/bins_dynamic_objects.h"
#include "spatial_containers/dem_search.h"
#include "discrete_particle_configure.h"

#include "doubledispatch/discrete_sphere_configure.h"
#include "doubledispatch/discrete_tetrahedron_configure.h"

// External includes

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

class OMP_DEMSearch : public DEMSearch<OMP_DEMSearch>
{
    public:
      ///@name Type Definitions
      ///@{
        
      enum SearchType { 
        ELEM2ELEM = 1,
        ELEM2NODE = 2,
        ELEM2COND = 4,
        NODE2ELEM = 8,
        NODE2NODE = 16,
        NODE2COND = 32,
        COND2ELEM = 64,
        COND2NODE = 128,
        COND2COND = 256
      };
    
      /// Pointer definition of OMP_DEMSearch
      KRATOS_CLASS_POINTER_DEFINITION(OMP_DEMSearch);
      
      //typedef SpatialSearch                 BaseType;
      typedef DiscreteParticleConfigure<3>  Configure;
      typedef BinsObjectDynamic<Configure>  BinsType;
     
      ///@}
      ///@name Life Cycle 
      ///@{
      
      /// Default constructor.
      OMP_DEMSearch(){}

      /// Destructor.
      ~OMP_DEMSearch(){}
      

      ///@}
      ///@name Operators 
      ///@{
      
      
      ///@}
      ///@name Operations
      ///@{
        
      void SearchElementsInRadiusExclusiveImplementation (
          ModelPart& rModelPart,
          ElementsContainerType rElements,
          const RadiusArrayType & Radius, 
          VectorResultElementsContainerType& rResults, 
          VectorDistanceType& rResultsDistance )
      {     
          KRATOS_TRY

          ElementsContainerType::ContainerType& elements_array     = rElements.GetContainer();
          ElementsContainerType::ContainerType& elements_ModelPart = rModelPart.GetCommunicator().LocalMesh().ElementsArray();
          
          int MaxNumberOfElements = elements_ModelPart.size();
        
          BinsType bins(elements_ModelPart.begin(), elements_ModelPart.end());
          
          #pragma omp parallel
          {
              ResultElementsContainerType   localResults(MaxNumberOfElements);
              DistanceType                  localResultsDistances(MaxNumberOfElements);
              std::size_t                   NumberOfResults = 0;
              
              //NOTE: This is a mistake? V-- It should be elements_array.size():
              #pragma omp for
              for(int i = 0; i < MaxNumberOfElements; i++)
              {
                  ResultIteratorType     ResultsPointer          = localResults.begin();
                  DistanceType::iterator ResultsDistancesPointer = localResultsDistances.begin();
                
                  NumberOfResults = bins.SearchObjectsInRadiusInner(elements_array[i],Radius[i],ResultsPointer,ResultsDistancesPointer,MaxNumberOfElements);
                      
                  rResults[i].insert(rResults[i].begin(),localResults.begin(),localResults.begin()+NumberOfResults);
                  rResultsDistance[i].insert(rResultsDistance[i].begin(),localResultsDistances.begin(),localResultsDistances.begin()+NumberOfResults);
              }
          }
          
          KRATOS_CATCH("")
      }
      
      void SearchElementsInRadiusInclusiveImplementation (
          ModelPart& rModelPart,
          ElementsContainerType rElements,
          const RadiusArrayType& Radius, 
          VectorResultElementsContainerType& rResults, 
          VectorDistanceType& rResultsDistance )
      {     
          KRATOS_TRY

          ElementsContainerType::ContainerType& elements_array     = rElements.GetContainer();
          ElementsContainerType::ContainerType& elements_ModelPart = rModelPart.GetCommunicator().LocalMesh().ElementsArray();
          
          int MaxNumberOfElements = elements_ModelPart.size();
        
          BinsType bins(elements_ModelPart.begin(), elements_ModelPart.end());
          
          #pragma omp parallel
          {
              ResultElementsContainerType   localResults(MaxNumberOfElements);
              DistanceType                  localResultsDistances(MaxNumberOfElements);
              std::size_t                   NumberOfResults = 0;
              
              //NOTE: This is a mistake? V-- It should be elements_array.size():
              #pragma omp for
              for(int i = 0; i < MaxNumberOfElements; i++)
              {
                  ResultIteratorType     ResultsPointer          = localResults.begin();
                  DistanceType::iterator ResultsDistancesPointer = localResultsDistances.begin();
                
                  NumberOfResults = bins.SearchObjectsInRadius(elements_array[i],Radius[i],ResultsPointer,ResultsDistancesPointer,MaxNumberOfElements);
                      
                  rResults[i].insert(rResults[i].begin(),localResults.begin(),localResults.begin()+NumberOfResults);
                  rResultsDistance[i].insert(rResultsDistance[i].begin(),localResultsDistances.begin(),localResultsDistances.begin()+NumberOfResults);
              }
          }
          
          KRATOS_CATCH("")        
      }

      void SearchElementsInRadiusExclusiveImplementation (
          ModelPart& rModelPart,
          ElementsContainerType rElements,
          const RadiusArrayType & Radius, 
          VectorResultElementsContainerType& rResults )
      {     
          KRATOS_TRY

          ElementsContainerType::ContainerType& elements_array     = rElements.GetContainer();
          ElementsContainerType::ContainerType& elements_ModelPart = rModelPart.GetCommunicator().LocalMesh().ElementsArray();
          
          int MaxNumberOfElements = elements_ModelPart.size();
        
          BinsType bins(elements_ModelPart.begin(), elements_ModelPart.end());
          
          #pragma omp parallel
          {
              ResultElementsContainerType   localResults(MaxNumberOfElements);
              std::size_t                   NumberOfResults = 0;
              
              //NOTE: This is a mistake? V-- It should be elements_array.size():
              #pragma omp for
              for(int i = 0; i < MaxNumberOfElements; i++)
              {
                  ResultIteratorType     ResultsPointer          = localResults.begin();

                  NumberOfResults = bins.SearchObjectsInRadiusInner(elements_array[i],Radius[i],ResultsPointer,MaxNumberOfElements);
                      
                  rResults[i].insert(rResults[i].begin(),localResults.begin(),localResults.begin()+NumberOfResults);
              }
          }
          
          KRATOS_CATCH("")
      }
      
      void SearchElementsInRadiusInclusiveImplementation (
          ModelPart& rModelPart,
          ElementsContainerType rElements,
          const RadiusArrayType& Radius, 
          VectorResultElementsContainerType& rResults )
      {     
          KRATOS_TRY

          ElementsContainerType::ContainerType& elements_array     = rElements.GetContainer();
          ElementsContainerType::ContainerType& elements_ModelPart = rModelPart.GetCommunicator().LocalMesh().ElementsArray();
          
          int MaxNumberOfElements = elements_ModelPart.size();
        
          BinsType bins(elements_ModelPart.begin(), elements_ModelPart.end());
          
          #pragma omp parallel
          {
              ResultElementsContainerType   localResults(MaxNumberOfElements);
              std::size_t                   NumberOfResults = 0;
              
              //NOTE: This is a mistake? V-- It should be elements_array.size():
              #pragma omp for
              for(int i = 0; i < MaxNumberOfElements; i++)
              {
                  ResultIteratorType     ResultsPointer          = localResults.begin();

                  NumberOfResults = bins.SearchObjectsInRadius(elements_array[i],Radius[i],ResultsPointer,MaxNumberOfElements);
                      
                  rResults[i].insert(rResults[i].begin(),localResults.begin(),localResults.begin()+NumberOfResults);
              }
          }
          
          KRATOS_CATCH("")        
      }
      
//       void SearchElementsAndConditionsInRadiusExclusiveImplementation(
//           ModelPart& rModelPart,
//           ElementsContainerType rElements,
//           ConditionsContainerType rConditions,
//           const RadiusArrayType& Radius,
//           const unsigned int& Flags, 
//           VectorResultElementsContainerType& rElementsResults,
//           VectorResultConditionsContainerType& rConditionsResults,
//           VectorDistanceType& rElementsResultsDistance,
//           VectorDistanceType& rConditionsResultsDistance
//       )
//       {
//           KRATOS_TRY
//           
//           // Checking the falgs in order to decide which bins are going to be generated
//           if(Flags & ( ELEM2ELEM | NODE2ELEM | COND2ELEM ) )
//           {
//               // Initialize Element based Bins
//               ElementsContainerType::ContainerType& elements_ModelPart  = rModelPart.GetCommunicator().LocalMesh().ElementsArray();
//               BinsType bins(elements_ModelPart.begin(), elements_ModelPart.end());
//               
//               int MaxNumberOfElements = elements_ModelPart.size();
//               
//               // Performing every search over the element bins
//               if(Flags & ELEM2ELEM) // Element vs Element
//               {
//                   ElementsContainerType::ContainerType& elements_array = rElements.GetContainer();
// //                   InRadiusInnerWrapper(bins,elements_array,Radius,rElementsResults,rElementsResultsDistance,MaxNumberOfElements);
//               }
//               if(Flags & COND2ELEM) // Condition - Element
//               {
//                   ConditionsContainerType::ContainerType& conditions_array = rConditions.GetContainer();
// //                   InRadiusInnerWrapper(bins,conditions_array,Radius,rConditionsResults,rConditionsResultsDistance,MaxNumberOfElements);
//               }
//           }
//           
//           KRATOS_CATCH("")  
//       }
      
//       template<class BinsType, class SearchObjectType, class ResultObjectType>
//       void InRadiusInnerWrapper(BinsType& bins, 
//                                 SearchObjectType& searchObjects,
//                                 const RadiusArrayType& Radius,
//                                 ResultObjectType& rResults, 
//                                 VectorDistanceType& rResultsDistance,
//                                 const int MaxNumberOfElements)
//       {
//           #pragma omp parallel
//           {
//               typename ResultObjectType::value_type     localResults(MaxNumberOfElements);
//               DistanceType                              localResultsDistances(MaxNumberOfElements);
//               std::size_t                               NumberOfResults = 0;
//               
//               #pragma omp parallel for
//               for(int i = 0; i < searchObjects.size(); i++)
//               {
//                   typename SearchObjectType::iterator     ResultsPointer          = localResults.begin();
//                   DistanceType::iterator ResultsDistancesPointer = localResultsDistances.begin();
//                 
//                   Configure::PointerType value = searchObjects[i]; 
//                   NumberOfResults = bins.SearchObjectsInRadius(value,Radius[i],ResultsPointer,ResultsDistancesPointer,MaxNumberOfElements);
//                   
//                   rResults[i].insert(rResults[i].begin(),localResults.begin(),localResults.begin()+NumberOfResults);
//                   rResultsDistance[i].insert(rResultsDistance[i].begin(),localResultsDistances.begin(),localResultsDistances.begin()+NumberOfResults);      
//               }
//           }
//       }
      
      
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
          std::stringstream buffer;
          buffer << "OpenMPDemSearch" ;
          
          return buffer.str();
      }
      
      /// Print information about this object.
      virtual void PrintInfo(std::ostream& rOStream) const {rOStream << "OpenMPDemSearch";}

      /// Print object's data.
      virtual void PrintData(std::ostream& rOStream) const {}
      
            
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
      
      /// Assignment operator.
      OMP_DEMSearch& operator=(OMP_DEMSearch const& rOther)
      {
          return *this;
      }

      /// Copy constructor.
      OMP_DEMSearch(OMP_DEMSearch const& rOther)
      {
          *this = rOther;
      }

        
      ///@}    
        
    }; // Class DEMSearch

  ///@} 
  
  ///@name Type Definitions       
  ///@{ 
  
  
  ///@} 
  ///@name Input and output 
  ///@{ 
        
  /// input stream function
//   inline std::istream& operator >> (std::istream& rIStream, 
//                     DEMSearch& rThis){return rIStream;}
// 
//   /// output stream function
//   inline std::ostream& operator << (std::ostream& rOStream, 
//                     const DEMSearch& rThis)
//   {
//     rThis.PrintInfo(rOStream);
//     rOStream << std::endl;
//     rThis.PrintData(rOStream);
// 
//     return rOStream;
//   }
  
  ///@}

  ///@} addtogroup block
  
}  // namespace Kratos.

#endif // KRATOS_DEM_SEARCH_H_INCLUDED  defined 


