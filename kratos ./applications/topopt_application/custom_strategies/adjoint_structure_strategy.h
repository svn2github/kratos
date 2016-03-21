//
//   Project Name:        $Project:     TopOpt_Application $
//   Last modified by:    $Author:      Gonzalez, Baumgärtner $
//   Date:                $Date:        October 2015 $
//   Revision:            $Revision:    0.0 $
//


#if !defined(KRATOS_ADJOINT_STRUCTURE_STRATEGY_H_INCLUDED)
#define  KRATOS_ADJOINT_STRUCTURE_STRATEGY_H_INCLUDED

// Project includes
#include <iostream>
#include <vector>
#include "includes/define.h"
#include "includes/element.h"
#include "includes/model_part.h"
#include "includes/process_info.h"
#include "utilities/openmp_utils.h"
#include "topopt_application.h"
#include "spatial_containers/spatial_containers.h" //kd-tree
#include "containers/array_1d.h"
#include "utilities/math_utils.h" // Cross Product
#include <math.h> // sqrt and pow
#include <algorithm> //maxelem
#include <stdlib.h> //abs
#include <vector>


// Application includes
#include "custom_elements/topopt_small_displacement_element.h"

namespace Kratos {

///@addtogroup TopOptApplication
///@{

///@name Kratos Classes
///@{


//*****************************************************************************
//************************* ElementPositionItem Class *************************
//*****************************************************************************

// define an auxiliary structure to hold a pointer to the element of interest and a position in space to be searched for
class ElementPositionItem: public Point<3>
{
public:

    KRATOS_CLASS_POINTER_DEFINITION( ElementPositionItem );

    ElementPositionItem(): // This Constructor is used by the tree
        Point<3>(),
        mpElement()
    {
    }

    ElementPositionItem(array_1d<double,3> Coords, Element::Pointer pElement):
        Point<3>(Coords),
        mpElement(pElement)
    {}

    Element::Pointer GetOriginElement()
    {
        return mpElement;   // TEST FUNCTION
    }


private:
    Element::Pointer mpElement;

};


//*****************************************************************************
//************************* AdjointStructureStrategy Class ********************
//*****************************************************************************


/// Solution strategy for adjoint structure problem.
template<class TSparseSpace, class TDenseSpace, class TLinearSolver>
class AdjointStructureStrategy: public SolvingStrategy<TSparseSpace, TDenseSpace,
        TLinearSolver>
{
public:

    ///@name Type Definitions
    ///@{

    KRATOS_CLASS_POINTER_DEFINITION(AdjointStructureStrategy);
//    tree MyTree;

    typedef SolvingStrategy<TSparseSpace,TDenseSpace,TLinearSolver> BaseType;

    typedef typename Scheme<TSparseSpace,TDenseSpace>::Pointer SchemePointerType;
    typedef typename BuilderAndSolver<TSparseSpace,TDenseSpace,TLinearSolver
    >::Pointer BuilderAndSolverPointerType;

    typedef typename BaseType::LocalSystemVectorType LocalSystemVectorType;
    typedef typename BaseType::LocalSystemMatrixType LocalSystemMatrixType;

    typedef ModelPart::ElementsContainerType ElementsContainerType;
    typedef ModelPart::ElementsContainerType ElementsArrayType;
    typedef ModelPart::ElementIterator ElementIterator;

    typedef ModelPart::NodesContainerType NodesContainerType;
    typedef ModelPart::NodesContainerType NodesArrayType;
    typedef ModelPart::NodeIterator NodeIterator;
    typedef ModelPart::ConditionsContainerType ConditionsArrayType;

    // For FilterSensitivities()
    typedef ElementPositionItem                         PointType;
    typedef ElementPositionItem::Pointer                PointTypePointer;
    typedef std::vector<PointType::Pointer>             ElementPositionVector;
    typedef std::vector<PointType::Pointer>::iterator   ElementPositionIterator;
    typedef std::vector<double>                         DistanceVector;
    typedef std::vector<double>::iterator               DistanceIterator;


    typedef Bucket< 3ul, PointType, ElementPositionVector, PointTypePointer, ElementPositionIterator, DistanceIterator > BucketType;
    typedef Tree< KDTreePartition<BucketType> > tree;



  ///@}
  ///@name Life Cycle
  ///@{

  /**
   * @brief Constructs the adjoint problem from a structure model part and computes the sensitivities
   */
  AdjointStructureStrategy(ModelPart& rStructureModelPart,
                           typename TLinearSolver::Pointer pNewLinearSolver,
                           const int Dimension = 3) : BaseType(rStructureModelPart)
  {
    KRATOS_TRY;

    // If we have to solve an adjoint system --> here we need to create a strategy + assign builderAndSolver & schemes to it

    KRATOS_CATCH("");
  }


  virtual ~AdjointStructureStrategy()
  {}

  ///@}
  ///@name Operations
  ///@{


  /// Solves the adjoint structure problem.
  virtual double Solve()
  {
    KRATOS_TRY;

    // If we have to solve an adjoint system --> here we have to include the actual solving step
    return 0.0;

    KRATOS_CATCH("");
  }


  // ---------------------------------------------------------------------------------------------
  // ----------------------------------COMPUTE SENSITIVITIES--------------------------------------
  // ---------------------------------------------------------------------------------------------

  /// Computes shape sensitivities from the adjoint solution.
  void ComputeSensitivities(ModelPart& rPrimalModelPart)
  {
      KRATOS_TRY;

      double Out = 0.0;

      // Loop over all elements to calculate and assign sensitivity
      for( ModelPart::ElementIterator element_i = rPrimalModelPart.ElementsBegin(); element_i!= rPrimalModelPart.ElementsEnd(); element_i++ )
      {
          element_i->Calculate(DCDX, Out, rPrimalModelPart.GetProcessInfo());
      }

      KRATOS_CATCH("");
  }


  // ---------------------------------------------------------------------------------------------
  // -------------------------------------UPDATE DENSITIES----------------------------------------
  // ---------------------------------------------------------------------------------------------

  void UpdateDensities(ModelPart& rPrimalModelPart, double volfrac, double q)
  {
      KRATOS_TRY;

      double l1     = 0.0;
      double l2     = 1000000000.0;
      double move   = 0.2;
      double dv     = 1.0;     // need to change if density filter implemented


      double sum_X_Phys;
      int nele;
      double x_new = 0.0;
      double lmid = 0.0;
      std::vector<double> elem_change;

      while ((l2-l1)/(l1+l2) > 0.001)
      {
          lmid = 0.5*(l2+l1); // lagrange multiplier
          sum_X_Phys = 0.0;
          nele = 0;
          x_new = 0.0;

          for( ModelPart::ElementIterator element_i = rPrimalModelPart.ElementsBegin(); element_i!= rPrimalModelPart.ElementsEnd(); element_i++ )
          {
              double x_old = element_i->GetValue(X_PHYS_OLD);
              double solid_void = element_i->GetValue(SOLID_VOID);
              double dcdx  = element_i->GetValue(DCDX);

              // OC UPDATE METHOD
              // When q=1, Grey Scale Filter not activated, i.e., results in classic OC update method
              if (solid_void==0) // Normal elements
                  x_new = std::max(0.0, std::max(x_old - move, std::min(1.0, pow(std::min(x_old + move, x_old * sqrt(-dcdx/dv/lmid)),q) )));
              else if (solid_void==1) // Active (solid) elements
                  x_new = 1;
              else if (solid_void==2) // Passive (void) elements
                  x_new = 0;
              else
                  std::cout << "This value for SOLID_VOID does not exist."<< q << std::endl;

              element_i->SetValue(X_PHYS, x_new); // for Sensitivity filter

              sum_X_Phys = sum_X_Phys + x_new;
              elem_change.push_back(x_new - x_old);
              nele = nele + 1;

          }

          if( sum_X_Phys > (volfrac*nele))
              l1=lmid;
          else
              l2=lmid;
      }

      std::vector<double>::iterator change = std::max_element(elem_change.begin(), elem_change.end());
      std::cout << "q = "<< q << std::endl;
      std::cout << "VOLFRAC = "<< sum_X_Phys/nele << std::endl;
      std::cout << "CHANGE = "<< *change*100<< " %"<<std::endl;

      KRATOS_CATCH("");
  }


  // ---------------------------------------------------------------------------------------------
  // ----------------------------------FILTER SENSITIVITIES--------------------------------------
  // ---------------------------------------------------------------------------------------------

  // Computes shape sensitivities from the adjoint solution.
  void FilterSensitivities(ModelPart& rPrimalModelPart)
  {
      KRATOS_TRY;

      // IMPORTANT: Tree data structure is still created on each loop. Should be changed to only calculate one time outside the optimization loop

      // Create placeholder for any element in model. We can assign the center of the element as coordinates to the placeholder
      ElementPositionVector dummy_positions_list;
      for(ModelPart::ElementsContainerType::iterator elem_i = rPrimalModelPart.ElementsBegin(); elem_i!=rPrimalModelPart.ElementsEnd(); elem_i++)
      {
          // here find the center of the element
          array_1d<double,3> center_coord = ZeroVector(3);
          Geometry< Node<3> >& geom = elem_i->GetGeometry();
          for(unsigned int i=0; i<geom.size(); i++)
              noalias(center_coord) += geom[i].Coordinates();
          center_coord /= static_cast<double>(geom.size());

          // new "ElementPositionItem" for every element and assign a pointer to the base element *it.base() )
          PointTypePointer pGP = PointTypePointer(new ElementPositionItem( center_coord, *elem_i.base() ) );
          dummy_positions_list.push_back( pGP );
      }

      // Create a tree space search structure - It will use a copy of mGaussPoinList (a std::vector which contains pointers)
      // Copying the list is required because the tree will reorder it for efficiency
      ElementPositionVector pListOfPositions = dummy_positions_list;
      tree MyTree(pListOfPositions.begin(),pListOfPositions.end(),4);

      const unsigned int MaxResults = 5000; // Maximum number of points to find in a single tree search
      ElementPositionVector Results(MaxResults);
      DistanceVector ResultDistances(MaxResults);

      for(ModelPart::ElementsContainerType::iterator elem_i = rPrimalModelPart.ElementsBegin(); elem_i!=rPrimalModelPart.ElementsEnd(); elem_i++)
      {
          double SearchRadius = elem_i->GetValue(SEARCHRADIUS);

          // here find the center of the element
          array_1d<double,3> center_coord = ZeroVector(3);
          Geometry< Node<3> >& geom = elem_i->GetGeometry();
          for(unsigned int i=0; i<geom.size(); i++)
              noalias(center_coord) += geom[i].Coordinates();
          center_coord /= static_cast<double>(geom.size());

          ElementPositionItem ElemPositionItem(center_coord,*elem_i.base());

          // This is broken. Bug found when using ResultDistances, so we calculate our own Distances
          int Found = MyTree.SearchInRadius(ElemPositionItem,SearchRadius,Results.begin(),ResultDistances.begin(),MaxResults);

          double Hxdc_sum = 0;
          double H_sum = 0;

          for(int ElementPositionItem_j = 0; ElementPositionItem_j < Found; ElementPositionItem_j++)
          {
              // Calculate distances
              array_1d<double,3> elemental_distance = ZeroVector(3);
              elemental_distance = *Results[ElementPositionItem_j] - center_coord;
              double distance = std::sqrt(inner_prod(elemental_distance,elemental_distance));

              // Creation of mesh independent convolution operator (weight factors)
              double H  = std::max(0.0,SearchRadius - distance);
              double Hxdc = H*(Results[ElementPositionItem_j]->GetOriginElement()->GetValue(DCDX))*(Results[ElementPositionItem_j]->GetOriginElement()->GetValue(X_PHYS));
              H_sum     = H_sum     + H;
              Hxdc_sum  = Hxdc_sum  + Hxdc;
          }

          // Calculate filtered sensitivities
          double dcdx_filtered = Hxdc_sum / (H_sum*std::max(0.001,elem_i->GetValue(X_PHYS)) );
          elem_i->SetValue(DCDX,dcdx_filtered);

          // Assign center x,y,z coords to global variables for use in other sections
          elem_i->SetValue(ELEM_CENTER_X,center_coord[0]);
          elem_i->SetValue(ELEM_CENTER_Y,center_coord[1]);
          elem_i->SetValue(ELEM_CENTER_Z,center_coord[2]);
      }

      KRATOS_CATCH("");
  }

  // ---------------------------------------------------------------------------------------------
  // -------------------------------------PREPARE FILTER------------------------------------------
  // ---------------------------------------------------------------------------------------------

  void PrepareFilter(ModelPart& rPrimalModelPart)
  {
      KRATOS_TRY;

      std::cout << "Not yet implemented. Tree data structure is still created on each loop." << std::endl;

      KRATOS_CATCH("");
  }


  ///@}

private:

  ///@name Member Variables
  ///@{

  ModelPart::Pointer mpAdjointModelPart;

  typename BaseType::Pointer mpStrategy;



  int mDimension;

  ///@}
  ///@name Private Operations
  ///@{

  ///@}
}; // class AdjointStructureStrategy

///@} // Kratos classes
///@} // AdjointStructureApplication group
}

#endif	/* KRATOS_ADJOINT_STRUCTURE_STRATEGY_H_INCLUDED */
