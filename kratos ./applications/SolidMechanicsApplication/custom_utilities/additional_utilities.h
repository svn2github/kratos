//
//   Project Name:        Kratos
//   Last Modified by:    $Author:   Daniel Baumgärtner$
//   Date:                $Date:     January 2016$
//   Revision:            $Revision: 1.0 $
//
//

#ifndef ADDITIONAL_UTILITIES_H
#define ADDITIONAL_UTILITIES_H

// System includes
#include <iostream>
#include <string>
#include <algorithm>

// External includes
#include <boost/python.hpp>

// Project includes

// Note that in the following "../kratos/" was inserted to allow for proper code-highlighting and following in qtCreator
// Might be necessary to exclude it again when committed

#include "../kratos/includes/define.h"
#include "../kratos/includes/define.h"
#include "../kratos/processes/process.h"
#include "../kratos/includes/node.h"
#include "../kratos/includes/element.h"
#include "../kratos/includes/model_part.h"
#include "../kratos/includes/kratos_flags.h"

#include "../kratos/geometries/point_3d.h"
#include "../custom_conditions/point_load_3D_condition.hpp"
#include "../kratos/includes/condition.h"

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

class AdditionalUtilities
{
public:
    ///@name Type Definitions
    ///@{
    typedef Geometry<ModelPart::NodeType> GeometryType;

    /// Pointer definition of AdditionalUtilities
    KRATOS_CLASS_POINTER_DEFINITION(AdditionalUtilities);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    AdditionalUtilities(ModelPart& model_part)
        : mr_model_part(model_part)
    {
    }

    /// Destructor.
    virtual ~AdditionalUtilities()
    {
    }


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    //***********************************************************************************
    //***********************************************************************************
    void create_PointLoad3DCondition( int node_Id )
    {
        KRATOS_TRY;

//        ModelPart::NodeType& given_node = mr_model_part.Nodes()[node_Id];
//        GeometryType::Pointer pGeom(new Point3D <ModelPart::NodeType>(given_node));
//        ModelPart::IndexType biggest_condition_Id;
//        if(mr_model_part.Conditions().size()!=0)
//        {
//            biggest_condition_Id = mr_model_part.Conditions().back().Id();
//        }
//        else
//        {
//           biggest_condition_Id = 0;
//        }
//        PointLoad3DCondition new_condition(++biggest_condition_Id,pGeom);
//        mr_model_part.Conditions().push_back( new_condition );

        ModelPart::NodeType& given_node = mr_model_part.Nodes()[node_Id];
        Condition const& r_clone_condition = KratosComponents<Condition>::Get("PointLoadCondition3D1N");
        Condition::NodesArrayType temp_condition_nodes;
        ModelPart::PropertiesType::Pointer pProperty = mr_model_part.ElementsBegin()->pGetProperties();
        ModelPart::IndexType biggest_condition_Id;
        if(mr_model_part.Conditions().size()!=0)
        {
            biggest_condition_Id = mr_model_part.Conditions().back().Id();
        }
        else
        {
            biggest_condition_Id = 0;
        }
        temp_condition_nodes.push_back(given_node);
        mr_model_part.Conditions().push_back(r_clone_condition.Create(++biggest_condition_Id, temp_condition_nodes, pProperty));



        KRATOS_CATCH("");
    }

    //***********************************************************************************
    //***********************************************************************************


    //***********************************************************************************
    //***********************************************************************************

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
        return "AdditionalUtilities";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << "AdditionalUtilities";
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
    }


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
    ModelPart& mr_model_part;

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
//		AdditionalUtilities& operator=(AdditionalUtilities const& rOther);

    /// Copy constructor.
//		AdditionalUtilities(AdditionalUtilities const& rOther);


    ///@}

}; // Class AdditionalUtilities

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}


}  // namespace Kratos.

#endif // ADDITIONAL_UTILITIES_H
