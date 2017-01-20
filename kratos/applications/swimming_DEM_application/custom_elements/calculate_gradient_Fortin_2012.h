//
//   Project Name:        Kratos
//   Last Modified by:    $Author: gcasas $
//   Date:                $Date: 2016-03-12
//

#if !defined(KRATOS_COMPUTE_GRADIENT_FORTIN_2012_H_INCLUDED )
#define  KRATOS_COMPUTE_GRADIENT_FORTIN_2012_H_INCLUDED

// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "containers/array_1d.h"
#include "includes/define.h"
#include "includes/element.h"
#include "includes/serializer.h"
#include "geometries/geometry.h"
#include "utilities/math_utils.h"
#include "utilities/geometry_utilities.h"

// Application includes
#include "includes/variables.h"
#include "fluid_dynamics_application_variables.h"
#include "calculate_component_gradient_simplex_element.h"

namespace Kratos
{

///@addtogroup SwimmingDEMApplication
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

/// A post-processing element to recover the Laplacian from the velocity solution.
/**
 */
template< unsigned int TDim,
          unsigned int TNumNodes = TDim + 1 >
class ComputeGradientFortin2012 : public ComputeComponentGradientSimplex<TDim, TNumNodes>
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of ComputeGradientFortin2012
    KRATOS_CLASS_POINTER_DEFINITION(ComputeGradientFortin2012);

    typedef ComputeComponentGradientSimplex<TDim, TNumNodes> BaseType;
    /// Node type (default is: Node<3>)
    typedef Node <3> NodeType;

    /// Geometry type (using with given NodeType)
    typedef Geometry<NodeType> GeometryType;

    /// Definition of nodes container type, redefined from GeometryType
    typedef Geometry<NodeType>::PointsArrayType NodesArrayType;

    /// Vector type for local contributions to the linear system
    typedef Vector VectorType;

    /// Matrix type for local contributions to the linear system
    typedef Matrix MatrixType;

    typedef Properties PropertiesType;

    typedef std::size_t IndexType;

    typedef std::size_t SizeType;

    typedef std::vector<std::size_t> EquationIdVectorType;

    typedef std::vector< Dof<double>::Pointer > DofsVectorType;

    typedef PointerVectorSet<Dof<double>, IndexedObject> DofsArrayType;

    typedef VectorMap<IndexType, DataValueContainer> SolutionStepsElementalDataContainerType;

    /// Type for shape function values container
    typedef Kratos::Vector ShapeFunctionsType;

    /// Type for a matrix containing the shape function gradients
    typedef Kratos::Matrix ShapeFunctionDerivativesType;

    /// Type for an array of shape function gradient matrices
    typedef GeometryType::ShapeFunctionsGradientsType ShapeFunctionDerivativesArrayType;

    ///@}
    ///@name Life Cycle
    ///@{

    //Constructors.

    /// Default constuctor.
    /**
     * @param NewId Index number of the new element (optional)
     */
    ComputeGradientFortin2012(IndexType NewId = 0) :
        BaseType(NewId)
    {}

    /// Constructor using an array of nodes.
    /**
     * @param NewId Index of the new element
     * @param ThisNodes An array containing the nodes of the new element
     */
    ComputeGradientFortin2012(IndexType NewId, const NodesArrayType& ThisNodes) :
        BaseType(NewId, ThisNodes)
    {}	

    /// Constructor using a geometry object.
    /**
     * @param NewId Index of the new element
     * @param pGeometry Pointer to a geometry object
     */
    ComputeGradientFortin2012(IndexType NewId, GeometryType::Pointer pGeometry) :
        BaseType(NewId, pGeometry)
    {}

    /// Constuctor using geometry and properties.
    /**
     * @param NewId Index of the new element
     * @param pGeometry Pointer to a geometry object
     * @param pProperties Pointer to the element's properties
     */
    ComputeGradientFortin2012(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties) :
       BaseType(NewId, pGeometry, pProperties)
    {}

    /// Destructor.
    virtual ~ComputeGradientFortin2012()
    {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    /// Create a new element of this type
    /**
     * Returns a pointer to a new ComputeGradientFortin2012 element, created using given input
     * @param NewId: the ID of the new element
     * @param ThisNodes: the nodes of the new element
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes,
                            PropertiesType::Pointer pProperties) const
    {
        return Element::Pointer(new ComputeGradientFortin2012(NewId, this->GetGeometry().Create(ThisNodes), pProperties));
    }

    /// Calculate the element's local contribution to the system for the current step. It is a combination of the
    /// RHS of the Base class (weighed by a small parameter to stabilize the system) and the contribution described in
    /// the Fortin 2012 paper
    virtual void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
                                      VectorType& rRightHandSideVector,
                                      ProcessInfo& rCurrentProcessInfo);


    /// Provides the global indices for each one of this element's local rows
    /**
     * this determines the elemental equation ID vector for all elemental
     * DOFs
     * @param rResult A vector containing the global Id of each row
     * @param rCurrentProcessInfo the current process info object (unused)
     */
    virtual void EquationIdVector(EquationIdVectorType& rResult,
                                  ProcessInfo& rCurrentProcessInfo)
    {

        const unsigned int NumNodes(TDim+1), LocalSize(TDim * NumNodes);
        unsigned int LocalIndex = 0;
        unsigned int pos = this->GetGeometry()[0].GetDofPosition(VELOCITY_Z_GRADIENT_X);

        if (rResult.size() != LocalSize)
            rResult.resize(LocalSize, false);

        for (unsigned int iNode = 0; iNode < NumNodes; ++iNode)
        {
            rResult[LocalIndex++] = this->GetGeometry()[iNode].GetDof(VELOCITY_Z_GRADIENT_X,pos).EquationId();
            rResult[LocalIndex++] = this->GetGeometry()[iNode].GetDof(VELOCITY_Z_GRADIENT_Y,pos+1).EquationId();
            rResult[LocalIndex++] = this->GetGeometry()[iNode].GetDof(VELOCITY_Z_GRADIENT_Z,pos+2).EquationId();
        }
//Z
    }

    /// Returns a list of the element's Dofs
    /**
     * @param ElementalDofList the list of DOFs
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void GetDofList(DofsVectorType& rElementalDofList,
                            ProcessInfo& rCurrentProcessInfo)
    {

//        unsigned int number_of_nodes = TDim+1;
// G
//        if (rElementalDofList.size() != number_of_nodes)
//            rElementalDofList.resize(number_of_nodes);

//        for (unsigned int i = 0; i < number_of_nodes; i++)
//            rElementalDofList[i] = GetGeometry()[i].pGetDof(DISTANCE);
        const unsigned int NumNodes(TDim+1), LocalSize(TDim * NumNodes);

        if (rElementalDofList.size() != LocalSize)
            rElementalDofList.resize(LocalSize);

        unsigned int LocalIndex = 0;

        for (unsigned int iNode = 0; iNode < NumNodes; ++iNode)
        {
            rElementalDofList[LocalIndex++] = this->GetGeometry()[iNode].pGetDof(VELOCITY_Z_GRADIENT_X);
            rElementalDofList[LocalIndex++] = this->GetGeometry()[iNode].pGetDof(VELOCITY_Z_GRADIENT_Y);
            rElementalDofList[LocalIndex++] = this->GetGeometry()[iNode].pGetDof(VELOCITY_Z_GRADIENT_Z);
        }
//Z

    }



    /// Obtain an array_1d<double,3> elemental variable, evaluated on gauss points.
    /**
     * @param rVariable Kratos vector variable to get
     * @param Output Will be filled with the values of the variable on integrartion points
     * @param rCurrentProcessInfo Process info instance
     */
//    virtual void GetValueOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
//            std::vector<array_1d<double, 3 > >& rValues,
//            const ProcessInfo& rCurrentProcessInfo)
//    {
//
//    }


    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Elemental Data
    ///@{

    /// Checks the input and that all required Kratos variables have been registered.
    /**
     * This function provides the place to perform checks on the completeness of the input.
     * It is designed to be called only once (or anyway, not often) typically at the beginning
     * of the calculations, so to verify that nothing is missing from the input
     * or that no common error is found.
     * @param rCurrentProcessInfo The ProcessInfo of the ModelPart that contains this element.
     * @return 0 if no errors were found.
     */
    virtual int Check(const ProcessInfo& rCurrentProcessInfo)
{
        KRATOS_TRY

        // Perform basic element checks
        int ErrorCode = Kratos::Element::Check(rCurrentProcessInfo);
        if(ErrorCode != 0) return ErrorCode;

        if(this->GetGeometry().size() != TDim+1)
            KRATOS_THROW_ERROR(std::invalid_argument,"wrong number of nodes for element",this->Id());

        // Check that all required variables have been registered
//G
//        if(DISTANCE.Key() == 0)

//            //KRATOS_THROW_ERROR(std::invalid_argument,"DISTANCE Key is 0. Check if the application was correctly registered.","");


//        // Checks on nodes

//        // Check that the element's nodes contain all required SolutionStepData and Degrees of freedom
//        for(unsigned int i=0; i<this->GetGeometry().size(); ++i)
//        {
//            if(this->GetGeometry()[i].SolutionStepsDataHas(DISTANCE) == false)
//                KRATOS_THROW_ERROR(std::invalid_argument,"missing DISTANCE variable on solution step data for node ",this->GetGeometry()[i].Id());
//        }

        if(VELOCITY_Z_GRADIENT.Key() == 0)

            KRATOS_THROW_ERROR(std::invalid_argument,"VELOCITY_Z_GRADIENT Key is 0. Check if the application was correctly registered.","");

        // Checks on nodes

        // Check that the element's nodes contain all required SolutionStepData and Degrees of freedom
        for(unsigned int i=0; i<this->GetGeometry().size(); ++i)
        {
            if(this->GetGeometry()[i].SolutionStepsDataHas(VELOCITY_Z_GRADIENT) == false)
                KRATOS_THROW_ERROR(std::invalid_argument,"missing VELOCITY_Z_GRADIENT variable on solution step data for node ",this->GetGeometry()[i].Id());
        }
//Z
        return 0;

        KRATOS_CATCH("");
    }


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
        buffer << "ComputeGradientFortin2012 #" << this->Id();
        return buffer.str();
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << "ComputeGradientFortin2012" << TDim << "D";
    }

//        /// Print object's data.
//        virtual void PrintData(std::ostream& rOStream) const;

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
    ///@name Serialization
    ///@{

    friend class Serializer;

    virtual void save(Serializer& rSerializer) const
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Element );
    }

    virtual void load(Serializer& rSerializer)
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Element);
    }

    ///@}
    ///@name Private Operators
    ///@{

    /// Computes local contributions to the mass matrix
    /**
     * Provides the local contributions to the mass matrix, which is defined here
     * as the matrix associated to velocity derivatives. Note that the mass
     * matrix implemented here is lumped.
     * @param rMassMatrix Will be filled with the elemental mass matrix
     * @param rCurrentProcessInfo the current process info instance
     */

    virtual void AddFortin2012LHS(MatrixType& rLeftHandSideMatrix, ProcessInfo& rCurrentProcessInfo){
        const int NEdges = 3 * TNumNodes - 6; // works in 2D and 3D
        int edges[NEdges][2];

        int i_edge = 0;
        for (int i = 0; i < TNumNodes - 1; ++i){
            for (int j = i + 1; j < TNumNodes; ++j){
                edges[i_edge][0]   = i;
                edges[i_edge++][1] = j;
            }
        }

        array_1d<array_1d<double, 3>, NEdges> EdgeVectors; // stores the [lx, ly(, lz)] vectors for all edges
        array_1d<double, NEdges> edge_lengths_inv;
        const GeometryType& rGeom = this->GetGeometry();

        for (int e = 0; e < NEdges; ++e){
            array_1d<double, 3>& le = EdgeVectors[e];
            noalias(le) = rGeom[edges[e][1]].Coordinates() - rGeom[edges[e][0]].Coordinates();
            const double he_inv = 1.0 / std::sqrt(le[0] * le[0] + le[1] * le[1] + le[2] * le[2]);
            edge_lengths_inv[e] = he_inv;
            le *= he_inv;
            AssembleEdgeLHSContribution(edges[e], le, rLeftHandSideMatrix);
        }
    }

    void AssembleEdgeLHSContribution(const int edge[2], const array_1d<double, 3>& edge_normalized_vector, MatrixType& rLeftHandSideMatrix)
    {
        for (int node_e = 0; node_e < 2; ++node_e){
            for (int i = 0; i < TDim; ++i){
                for (int node_f = 0; node_f < 2; ++node_f){
                    for (int j = 0; j < TDim; ++j){
                        rLeftHandSideMatrix(TDim * edge[node_e] + i, TDim * edge[node_f] + j) += edge_normalized_vector[i] * edge_normalized_vector[j];
                    }
                }
            }
        }
    }

    virtual void AddFortin2012RHS(VectorType& F, ProcessInfo& rCurrentProcessInfo)
    {
        const int NEdges = 3 * TNumNodes - 6; // works in 2D and 3D

        int edges[NEdges][2];

        int i_edge = 0;
        for (int i = 0; i < TNumNodes - 1; ++i){
            for (int j = i + 1; j < TNumNodes; ++j){
                edges[i_edge][0]   = i;
                edges[i_edge++][1] = j;
            }
        }

        array_1d<array_1d<double, 3>, NEdges> EdgeVectors; // stores the [lx, ly(, lz)] vectors for all edges
        array_1d<double, NEdges> edge_lengths_inv;
        const GeometryType& rGeom = this->GetGeometry();

        for (int e = 0; e < NEdges; ++e){
            array_1d<double, 3>& le = EdgeVectors[e];
            noalias(le) = rGeom[edges[e][1]].Coordinates() - rGeom[edges[e][0]].Coordinates();
            const double he_inv = 1.0 / std::sqrt(le[0] * le[0] + le[1] * le[1] + le[2] * le[2]);
            edge_lengths_inv[e] = he_inv;
            le *= he_inv;

            if (this->mCurrentComponent == 'X'){
                AssembleEdgeRHSContributionX(edges[e], he_inv, le, F);
            }

            else if (this->mCurrentComponent == 'Y'){
                AssembleEdgeRHSContributionY(edges[e], he_inv, le, F);
            }

            else {
                AssembleEdgeRHSContributionZ(edges[e], he_inv, le, F);
            }
        }
    }

    void AssembleEdgeRHSContributionX(const int edge[2], const double h_edge_inv, const array_1d<double, 3>& edge_normalized_vector, VectorType& F)
    {
        const double vel_component_variation_along_edge = this->GetGeometry()[edge[1]].FastGetSolutionStepValue(VELOCITY_X) - this->GetGeometry()[edge[0]].FastGetSolutionStepValue(VELOCITY_X);

        for (int node_e = 0; node_e < 2; ++node_e){
            for (int i = 0; i < TDim; ++i){
                F(TDim * edge[node_e] + i) += 2.0 * h_edge_inv * edge_normalized_vector[i] * vel_component_variation_along_edge;
            }
        }
    }

    void AssembleEdgeRHSContributionY(const int edge[2], const double h_edge_inv, const array_1d<double, 3>& edge_normalized_vector, VectorType& F)
    {
        const double vel_component_variation_along_edge = this->GetGeometry()[edge[1]].FastGetSolutionStepValue(VELOCITY_Y) - this->GetGeometry()[edge[0]].FastGetSolutionStepValue(VELOCITY_Y);

        for (int node_e = 0; node_e < 2; ++node_e){
            for (int i = 0; i < TDim; ++i){
                F(TDim * edge[node_e] + i) += 2.0 * h_edge_inv * edge_normalized_vector[i] * vel_component_variation_along_edge;
            }
        }
    }

    void AssembleEdgeRHSContributionZ(const int edge[2], const double h_edge_inv, const array_1d<double, 3>& edge_normalized_vector, VectorType& F)
    {
        const double vel_component_variation_along_edge = this->GetGeometry()[edge[1]].FastGetSolutionStepValue(VELOCITY_Z) - this->GetGeometry()[edge[0]].FastGetSolutionStepValue(VELOCITY_Z);

        for (int node_e = 0; node_e < 2; ++node_e){
            for (int i = 0; i < TDim; ++i){
                F(TDim * edge[node_e] + i) += 2.0 * h_edge_inv * edge_normalized_vector[i] * vel_component_variation_along_edge;
            }
        }
    }


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
    ComputeGradientFortin2012 & operator=(ComputeGradientFortin2012 const& rOther);

    /// Copy constructor.
    ComputeGradientFortin2012(ComputeGradientFortin2012 const& rOther);

    ///@}

}; // Class ComputeGradientFortin2012

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
template< unsigned int TDim >
inline std::istream& operator >>(std::istream& rIStream,
                                 ComputeGradientFortin2012<TDim>& rThis)
{
    return rIStream;
}

/// output stream function
template< unsigned int TDim >
inline std::ostream& operator <<(std::ostream& rOStream,
                                 const ComputeGradientFortin2012<TDim>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@} // Fluid Dynamics Application group

} // namespace Kratos.

#endif // KRATOS_COMPUTE_GRADIENT_FORTIN_2012_H_INCLUDED  defined
