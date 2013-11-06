/*
==============================================================================
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain

Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNER.

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
//   Last Modified by:    $Author:    J.M.Carbonell$
//   Date:                $Date:          6/11/2013$
//   Revision:            $Revision:           1.7 $
//


#if !defined(KRATOS_CONDITION_H_INCLUDED )
#define  KRATOS_CONDITION_H_INCLUDED

// System includes
#include <string>
#include <iostream>
#include <sstream>
#include <cstddef>

// External includes

// Project includes
#include "includes/define.h"
#include "includes/node.h"
#include "geometries/geometry.h"
#include "includes/properties.h"
#include "includes/process_info.h"
#include "includes/geometrical_object.h"
#include "utilities/indexed_object.h"
#include "containers/flags.h"


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

/// Base class for all Conditions.

/**
 * This is the base class for all conditions used in KRATOS
 * Conditions inherited from this class have to reimplement
 * all public functions that are needed to perform their designated
 * tasks. Due to a dummy implementation of every function though,
 * not all of them have to be implemented if they are not needed for
 * the actual problem
 */
class Condition : public GeometricalObject, public Flags
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of Condition
    KRATOS_CLASS_POINTER_DEFINITION(Condition);

    ///definition of condition type
    typedef Condition ConditionType;

    ///base type: an GeometricalObject that automatically has a unique number
    typedef GeometricalObject BaseType;

    ///definition of node type (default is: Node<3>)
    typedef Node < 3 > NodeType;

    /**
     * Properties are used to store any parameters
     * related to the constitutive law
     */
    typedef Properties PropertiesType;

    ///definition of the geometry type with given NodeType
    typedef Geometry<NodeType> GeometryType;

    ///definition of nodes container type, redefined from GeometryType
    typedef Geometry<NodeType>::PointsArrayType NodesArrayType;

    typedef Vector VectorType;

    typedef Matrix MatrixType;

    typedef std::size_t IndexType;

    typedef std::size_t SizeType;

    typedef std::vector<std::size_t> EquationIdVectorType;

    typedef std::vector< Dof<double>::Pointer > DofsVectorType;

    typedef PointerVectorSet<Dof<double>, IndexedObject> DofsArrayType;

    typedef VectorMap<IndexType, DataValueContainer> SolutionStepsConditionalDataContainerType;

    ///Type definition for integration methods
    typedef GeometryData::IntegrationMethod IntegrationMethod;

    typedef GeometryData GeometryDataType;
    ///@}


    ///@}
    ///@name Life Cycle
    ///@{

    /**
     * CONDITIONS inherited from this class have to implement next 
     * contructors, copy constructors and destructor: MANDATORY
     */

    /**
     * Constructor.
     */
    Condition(IndexType NewId = 0)
        : BaseType(NewId)
        , Flags()
        , mpProperties(new PropertiesType)
    {
    }

    /**
     * Constructor using an array of nodes
     */
    Condition(IndexType NewId, const NodesArrayType& ThisNodes)
        : BaseType(NewId,GeometryType::Pointer(new GeometryType(ThisNodes)))
        , Flags()
        , mpProperties(new PropertiesType)
    {
    }

    /**
     * Constructor using Geometry
     */
    Condition(IndexType NewId, GeometryType::Pointer pGeometry)
        : BaseType(NewId,pGeometry)
        , Flags()
        , mpProperties(new PropertiesType)
    {
    }

    /**
     * Constructor using Properties
     */
    Condition(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties)
        : BaseType(NewId,pGeometry)
        , Flags()
        , mpProperties(pProperties)
    {
    }

    /// Copy constructor.

    Condition(Condition const& rOther)
        : BaseType(rOther)
        , Flags(rOther)
        , mpProperties(rOther.mpProperties)
    {
    }

    /// Destructor.

    virtual ~Condition()
    {
    }


    ///@}
    ///@name Operators
    ///@{

    /**
     * CONDITIONS inherited from this class have to implement next 
     * assigment operator: MANDATORY
     */

    /// Assignment operator.

    Condition & operator=(Condition const& rOther)
    {
        BaseType::operator=(rOther);
        Flags::operator =(rOther);
        mpProperties = rOther.mpProperties;

        return *this;
    }
   
    ///@}
    ///@name Operations
    ///@{

    /**
     * CONDITIONS inherited from this class have to implement next 
     * Create and Clone methods: MANDATORY
     */

    /**
     * creates a new condition pointer
     * @param NewId: the ID of the new condition
     * @param ThisNodes: the nodes of the new condition
     * @param pProperties: the properties assigned to the new condition
     * @return a Pointer to the new condition
     */
    virtual Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes, 
			   PropertiesType::Pointer pProperties) const
    {
        return Condition::Pointer(new Condition(NewId, GetGeometry().Create(ThisNodes), pProperties));
    }


    /**
     * creates a new condition pointer and clones the previous condition data
     * @param NewId: the ID of the new condition
     * @param ThisNodes: the nodes of the new condition
     * @param pProperties: the properties assigned to the new condition
     * @return a Pointer to the new condition
     */
    virtual Pointer Clone (IndexType NewId, NodesArrayType const& ThisNodes) const
    {
        KRATOS_TRY
	std::cout<<" Call base class condition Clone "<<std::endl;
        return Condition::Pointer(new Condition(NewId, GetGeometry().Create(ThisNodes), pGetProperties()));
        KRATOS_CATCH("");
    }

    /**
     * CONDITIONS inherited from this class have to implement next 
     * EquationIdVector and GetDofList methods: MANDATORY
     */

    /**
     * this determines the condition equation ID vector for all condition
     * DOFs
     * @param rResult: the condition equation ID vector
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void EquationIdVector(EquationIdVectorType& rResult, 
				  ProcessInfo& rCurrentProcessInfo)
    {
        if (rResult.size() != 0)
            rResult.resize(0);
    }

    /**
     * determines the condition list of DOFs
     * @param ConditionDofList: the list of DOFs
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void GetDofList(DofsVectorType& rConditionDofList, 
			    ProcessInfo& rCurrentProcessInfo)
    {
        if (rConditionDofList.size() != 0)
            rConditionDofList.resize(0);
    }

    /**
     * returns the used integration method. In the general case this is the
     * default integration method of the used geometry. I an other integration
     * method is used the method has to be overwritten within the condition
     * @return default integration method of the used Geometry
     * this method is: OPTIONAL ( is recommended to reimplement it in the derived class )
     */
    virtual IntegrationMethod GetIntegrationMethod()
    {
        return pGetGeometry()->GetDefaultIntegrationMethod();
    }

    /**
     * CONDITIONS inherited from this class must implement this methods
     * if they need the values of the time derivatives of any of the dof 
     * set by the condition. If the derivatives do not exist can set to zero
     * these methods are: MANDATORY ( when compatibility with dynamics is required )
     */

    /**
     * Getting method to obtain the variable which defines the degrees of freedom
     */
    virtual void GetValuesVector(Vector& values, int Step = 0)
    {
    }

    /**
     * Getting method to obtain the time derivative of variable which defines the degrees of freedom
     */
    virtual void GetFirstDerivativesVector(Vector& values, int Step = 0)
    {
    }

    /**
     * Getting method to obtain the second time derivative of variable which defines the degrees of freedom
     */
    virtual void GetSecondDerivativesVector(Vector& values, int Step = 0)
    {
    }

    /**
     * CONDITIONS inherited from this class must implement next methods
     * Initialize, ResetConstitutiveLaw, CleanMemory
     * if the condition needs to perform any operation before any calculation is done
     * reset material and constitutive parameters
     * or clean memory deleting obsolete variables
     * this methods are: OPTIONAL
     */
   
    /**
     * is called to initialize the condition
     * if the condition needs to perform any operation before any calculation is done
     * the condition variables will be initialized and set using this method
     */
    virtual void Initialize()
    {
    }

    /**
     * is called to reset the constitutive law parameters and the material properties
     * the condition variables will be changed and reset using this method
     */
    virtual void ResetConstitutiveLaw()
    {
    }

    /**
     * deletes all obsolete data from memory
     */
    virtual void CleanMemory()
    {
    }

    /**
     * CONDITIONS inherited from this class must implement next methods
     * InitializeSolutionStep, FinalizeSolutionStep,
     * InitializeNonLinearIteration, FinalizeNonLinearIteration
     * if the condition needs to perform any operation before and after the solution step
     * if the condition needs to perform any operation before and after the solution iteration
     * this methods are: OPTIONAL
     */

    /**
     * this is called for non-linear analysis at the beginning of the iteration process
     */
    virtual void InitializeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called for non-linear analysis at the end of the iteration process
     */
    virtual void FinalizeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called in the beginning of each solution step
     */
    virtual void InitializeSolutionStep(ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called at the end of each solution step
     */
    virtual void FinalizeSolutionStep(ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * CONDITIONS inherited from this class have to implement next 
     * CalculateLocalSystem, CalculateLeftHandSide and CalculateRightHandSide methods
     * they can be managed internally with a private method to do the same calculations 
     * only once: MANDATORY
     */

    /**
     * this is called during the assembling process in order
     * to calculate all condition contributions to the global system
     * matrix and the right hand side
     * @param rLeftHandSideMatrix: the condition left hand side matrix
     * @param rRightHandSideVector: the condition right hand side
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix, 
				      VectorType& rRightHandSideVector, 
				      ProcessInfo& rCurrentProcessInfo)
    {
        if (rLeftHandSideMatrix.size1() != 0)
            rLeftHandSideMatrix.resize(0, 0);
        if (rRightHandSideVector.size() != 0)
            rRightHandSideVector.resize(0);
    }
    
    /**
     * this function provides a more general interface to the condition.
     * it is designed so that rLHSvariables and rRHSvariables are passed TO the condition
     * thus telling what is the desired output
     * @param rLeftHandSideMatrices: container with the output left hand side matrices
     * @param rLHSVariables: paramter describing the expected LHSs
     * @param rRightHandSideVectors: container for the desired RHS output
     * @param rRHSVariables: parameter describing the expected RHSs
     */
    virtual void CalculateLocalSystem(std::vector< MatrixType >& rLeftHandSideMatrices,
                                      const std::vector< Variable< MatrixType > >& rLHSVariables,
                                      std::vector< VectorType >& rRightHandSideVectors,
                                      const std::vector< Variable< VectorType > >& rRHSVariables,
                                      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called during the assembling process in order
     * to calculate the condition left hand side matrix only
     * @param rLeftHandSideMatrix: the condition left hand side matrix
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void CalculateLeftHandSide(MatrixType& rLeftHandSideMatrix, 
				       ProcessInfo& rCurrentProcessInfo)
    {
        if (rLeftHandSideMatrix.size1() != 0)
            rLeftHandSideMatrix.resize(0, 0);
    }

    /**
     * this is called during the assembling process in order
     * to calculate the condition right hand side vector only
     * @param rRightHandSideVector: the condition right hand side vector
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void CalculateRightHandSide(VectorType& rRightHandSideVector, 
					ProcessInfo& rCurrentProcessInfo)
    {
        if (rRightHandSideVector.size() != 0)
            rRightHandSideVector.resize(0);
    }
    
    /**
     * CONDITIONS inherited from this class must implement this methods
     * if they need to add dynamic condition contributions 
     * MassMatrix, AddMassMatrix, DampMatrix, AddInertiaForces and 
     * CalculateLocalVelocityContribution methods are: OPTIONAL
     */
    
    /**
     * this is called during the assembling process in order
     * to calculate the condition mass matrix
     * @param rMassMatrix: the condition mass matrix
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void MassMatrix(MatrixType& rMassMatrix, ProcessInfo& rCurrentProcessInfo)
    {
        if (rMassMatrix.size1() != 0)
            rMassMatrix.resize(0, 0);
    }

    /**
     * adds the mass matrix scaled by a given factor to the LHS
     * @param rLeftHandSideMatrix: the condition LHS matrix
     * @param coeff: the given factor
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void AddMassMatrix(MatrixType& rLeftHandSideMatrix, double coeff, ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called during the assembling process in order
     * to calculate the condition damping matrix
     * @param rDampMatrix: the condition damping matrix
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void DampMatrix(MatrixType& rDampMatrix, ProcessInfo& rCurrentProcessInfo)
    {
        if (rDampMatrix.size1() != 0)
            rDampMatrix.resize(0, 0);
    }

    /**
     * adds the inertia forces to the RHS --> performs residua = static_residua - coeff*M*acc
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void AddInertiaForces(VectorType& rRightHandSideVector, double coeff, ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * Calculate Damp matrix and add velocity contribution to RHS
     * @param rDampMatrix: the velocity-proportional "damping" matrix
     * @param rRightHandSideVector: the condition right hand side matrix
     * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void CalculateLocalVelocityContribution(MatrixType& rDampMatrix, VectorType& rRightHandSideVector, ProcessInfo& rCurrentProcessInfo)
    {
        if (rDampMatrix.size1() != 0)
            rDampMatrix.resize(0, 0);
    }

    /**
     * CONDITIONS inherited from this class must implement this methods
     * if they need to write something at the condition geometry nodes
     * AddExplicitContribution methods are: OPTIONAL ( avoid to use them is not needed )
     */

    /**
     * this is called during the assembling process in order
     * to calculate the condition contribution in explicit calculation.
     * NodalData is modified Inside the function, so the 
     * The "AddEXplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION
     * IS ALLOWED TO WRITE ON ITS NODES.
     * the caller is expected to ensure thread safety hence
     * SET/UNSETLOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
      * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void AddExplicitContribution(ProcessInfo& rCurrentProcessInfo)
    {
    }
    
    /**
     * this function is designed to make the condition to assemble an rRHS vector
     * identified by a variable rRHSVariable by assembling it to the nodes on the variable
     * rDestinationVariable.
     * The "AddEXplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION
     * IS ALLOWED TO WRITE ON ITS NODES.
     * the caller is expected to ensure thread safety hence
     * SET/UNSETLOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
     * @param rRHSVector: input variable containing the RHS vector to be assembled
     * @param rRHSVariable: variable describing the type of the RHS vector to be assembled
     * @param rDestinationVariable: variable in the database to which the rRHSvector will be assembled 
      * @param rCurrentProcessInfo: the current process info instance
     */
    virtual void AddExplicitContribution(const VectorType& rRHSVector, const Variable<VectorType>& rRHSVariable, Variable<double >& rDestinationVariable, const ProcessInfo& rCurrentProcessInfo)
    {
        KRATOS_ERROR(std::logic_error, "base condition classes is not able to assemble rRHS to the desired variable. destination variable is ",rDestinationVariable)
    }
    
    virtual void AddExplicitContribution(const VectorType& rRHS, const Variable<VectorType>& rRHSVariable, Variable<array_1d<double,3> >& rDestinationVariable, const ProcessInfo& rCurrentProcessInfo)
    {
         KRATOS_ERROR(std::logic_error, "base condition classes is not able to assemble rRHS to the desired variable. destination variable is ",rDestinationVariable)
    }     

    /**
     * Calculate a Condition variable usually associated to a integration point
     * the Output is given on integration points and characterizes the condition
     * Calculate(..) methods are: OPTIONAL
     */

    virtual void Calculate(const Variable<double >& rVariable, 
			   double& Output, 
			   const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void Calculate(const Variable< array_1d<double,3> >& rVariable, 
			   array_1d<double,3>& Output, 
			   const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void Calculate(const Variable<Vector >& rVariable, 
			   Vector& Output, 
			   const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void Calculate(const Variable<Matrix >& rVariable, 
			   Matrix& Output, 
			   const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * Calculate variables on Integration points.
     * This gives access to variables computed in the constitutive law on each integration point.
     * Specialisations of condition must specify the actual interface to the integration points!
     * Note, that these functions expect a std::vector of values for the specified variable type that
     * contains a value for each integration point!
     * CalculateValueOnIntegrationPoints: calculates the values of given Variable.
     * these methods are: OPTIONAL
     */

    virtual void CalculateOnIntegrationPoints(const Variable<double>& rVariable, 
					      std::vector<double>& rOutput, 
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void CalculateOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable, 
					      std::vector< array_1d<double, 3 > >& Output,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void CalculateOnIntegrationPoints(const Variable<Vector >& rVariable,
					      std::vector< Vector >& Output,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void CalculateOnIntegrationPoints(const Variable<Matrix >& rVariable, 
					      std::vector< Matrix >& Output, 
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * Access for variables on Integration points.
     * This gives access to variables stored in the constitutive law on each integration point.
     * Specializations of condition must specify the actual interface to the integration points!
     * Note, that these functions expect a std::vector of values for the specified variable type that
     * contains a value for each integration point!
     * SetValueOnIntegrationPoints: set the values for given Variable.
     * GetValueOnIntegrationPoints: get the values for given Variable.
     * these methods are: OPTIONAL
     */

    //SET ON INTEGRATION POINTS - METHODS 

    virtual void SetValueOnIntegrationPoints(const Variable<double>& rVariable, 
					     std::vector<double>& rValues, 
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void SetValueOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					     std::vector<array_1d<double, 3 > > rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void SetValueOnIntegrationPoints(const Variable<Vector>& rVariable,
					     std::vector<Vector>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void SetValueOnIntegrationPoints(const Variable<Matrix>& rVariable,
					     std::vector<Matrix>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    //GET ON INTEGRATION POINTS METHODS 

    virtual void GetValueOnIntegrationPoints(const Variable<double>& rVariable,
					     std::vector<double>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValueOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					     std::vector<array_1d<double, 3 > >& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValueOnIntegrationPoints(const Variable<array_1d<double, 6 > >& rVariable,
					     std::vector<array_1d<double, 6 > >& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValueOnIntegrationPoints(const Variable<Vector>& rVariable,
					     std::vector<Vector>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValueOnIntegrationPoints(const Variable<Matrix>& rVariable,
					     std::vector<Matrix>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }


    /**
     * This method provides the place to perform checks on the completeness of the input
     * and the compatibility with the problem options as well as the contitutive laws selected
     * It is designed to be called only once (or anyway, not often) typically at the beginning
     * of the calculations, so to verify that nothing is missing from the input
     * or that no common error is found.
     * @param rCurrentProcessInfo
     * this method is: MANDATORY
     */

    virtual int Check(const ProcessInfo& rCurrentProcessInfo)
    {
        KRATOS_TRY
        if (this->Id() < 1)
        {
            KRATOS_ERROR(std::logic_error, "Condition found with Id 0 or negative","")
        }
        if (this->GetGeometry().Area() < 0)
        {
            std::cout << "error on condition -> " << this->Id() << std::endl;
            KRATOS_ERROR(std::logic_error, "Area cannot be less than 0","")
        }
        return 0;

        KRATOS_CATCH("");
    }

    //METHODS TO BE CLEANED: RUBBISH start

    /* virtual void CalculateOnIntegrationPoints(const Variable<double>& rVariable, Vector& Output, const ProcessInfo& rCurrentProcessInfo) */
    /* { */
    /* }  */

    /* /\** */
    /*  * this is called during the assembling process in order */
    /*  * to calculate the condition left hand side vector only */
    /*  * @param rLeftHandSideVector: the condition left hand side vector */
    /*  * @param rCurrentProcessInfo: the current process info instance */
    /*  *\/ */
    /* virtual void CalculateLeftHandSide(VectorType& rLeftHandSideVector, ProcessInfo& rCurrentProcessInfo) */
    /* { */
    /*     if (rLeftHandSideVector.size() != 0) */
    /*         rLeftHandSideVector.resize(0); */
    /* } */

    /* /\** */
    /*  * this is called during the assembling process in order */
    /*  * to calculate the condition right hand side matrix only */
    /*  * @param rRightHandSideMatrix: the condition right hand side matrix */
    /*  * @param rCurrentProcessInfo: the current process info instance */
    /*  *\/ */
    /* virtual void CalculateRightHandSide(MatrixType& rRightHandSideMatrix, ProcessInfo& rCurrentProcessInfo) */
    /* { */
    /*     if (rRightHandSideMatrix.size1() != 0) */
    /*         rRightHandSideMatrix.resize(0, 0); */
    /* } */


    /* /\** */
    /*  * this is called during the assembling process in order */
    /*  * to calculate all condition contributions to the global system */
    /*  * matrix and the right hand sides for a matrix-formed RHS */
    /*  * @param rLeftHandSideMatrix: the condition left hand side matrix */
    /*  * @param rDampMatrix: the condition damping matrix */
    /*  * @param rRightHandSideVector1: the condition right hand side (1) */
    /*  * @param rRightHandSideVector2: the condition right hand side (1) */
    /*  * @param rCurrentProcessInfo: the current process info instance */
    /*  *\/ */
    /* virtual void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix, */
    /*                                   MatrixType& rDampMatrix, */
    /*                                   VectorType& rRightHandSideVector1, */
    /*                                   VectorType& rRightHandSideVector2, */
    /*                                   ProcessInfo& rCurrentProcessInfo) */
    /* { */
    /* } */

    //METHODS TO BE CLEANED: RUBBISH end

    ///@}
    ///@name Access
    ///@{

    PropertiesType::Pointer pGetProperties()
    {
        return mpProperties;
    }

    const PropertiesType::Pointer pGetProperties() const
    {
        return mpProperties;
    }

    PropertiesType& GetProperties()
    {
        return *mpProperties;
    }

    PropertiesType const& GetProperties() const
    {
        return *mpProperties;
    }


    ///@}
    ///@name Condition Data
    ///@{

    /**
     * Access Data:
     */
    DataValueContainer& Data()
    {
        return mData;
    }

    /**
     * Check if the Data exists with Has(..) methods:
     */
    template<class TDataType> bool Has(const Variable<TDataType>& rThisVariable) const
    {
        return mData.Has(rThisVariable);
    }

    template<class TAdaptorType> bool Has(
        const VariableComponent<TAdaptorType>& rThisVariable) const
    {
        return mData.Has(rThisVariable);
    }

    /**
     * Set Data with SetValue and the Variable to set:
     */
    template<class TVariableType> void SetValue(
        const TVariableType& rThisVariable,
        typename TVariableType::Type const& rValue)
    {
        mData.SetValue(rThisVariable, rValue);
    }

    /**
     * Get Data with GetValue and the Variable to get:
     */
    template<class TVariableType> typename TVariableType::Type& GetValue(
        const TVariableType& rThisVariable)
    {
        return mData.GetValue(rThisVariable);
    }

    template<class TVariableType> typename TVariableType::Type const& GetValue(
        const TVariableType& rThisVariable) const
    {
        return mData.GetValue(rThisVariable);
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
        buffer << "Condition #" << Id();
        return buffer.str();
    }

    /// Print information about this object.

    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << "Condition #" << Id();
    }

    /// Print object's data.

    virtual void PrintData(std::ostream& rOStream) const
    {
        pGetGeometry()->PrintData(rOStream);
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

    /**
     * pointer to the data related to this condition
     */
    DataValueContainer mData;
    
    /**
     * pointer to the condition properties
     */
    Properties::Pointer mpProperties;

    ///@}
    ///@name Private Operators
    ///@{
    ///@}
    ///@name Private Operations
    ///@{
    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    virtual void save(Serializer& rSerializer) const
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, GeometricalObject );
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Flags );
        rSerializer.save("Condition Data", mData);
        rSerializer.save("Condition Properties", mpProperties);
    }

    virtual void load(Serializer& rSerializer)
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, GeometricalObject );
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Flags );
        rSerializer.load("Condition Data", mData);
        rSerializer.load("Condition Properties", mpProperties);
    }

    ///@}
    ///@name Private  Access
    ///@{
    ///@}
    ///@name Private Inquiry
    ///@{
    ///@}
    ///@name Un accessible methods
    ///@{
    ///@}

}; // Class Condition

///@}
///@name Type Definitions
///@{
///@}
///@name Input and output
///@{

/// input stream function
inline std::istream & operator >>(std::istream& rIStream,
                                  Condition& rThis);

/// output stream function

inline std::ostream & operator <<(std::ostream& rOStream,
                                  const Condition& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << " : " << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

/**
 * definition of condition specific variables
 */
KRATOS_DEFINE_VARIABLE(WeakPointerVector< Condition >, NEIGHBOUR_CONDITIONS)

} // namespace Kratos.
#endif // KRATOS_CONDITION_H_INCLUDED  defined 


