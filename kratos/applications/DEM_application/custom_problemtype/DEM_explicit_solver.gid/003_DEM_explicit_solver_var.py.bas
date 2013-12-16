# Model Type

ContinuumOption                  = "*GenData(Continuum)"
RotationOption                   = "*GenData(Rotation)"
HomogeneousMaterialOption        = "*GenData(Homogeneous_Material)"
ElementType                      = "*GenData(DEM_Element_Type)"
Dempack                          = "*GenData(Dempack)"

# Meshing Settings

CleanIndentationsOption          = "*GenData(Clean_Initial_Indentations)"

# General Settings

*format "%10.5e"
FinalTime                        = *GenData(Calculation_Time)
*format "%10.5e"
GravityX                         = *GenData(Gravity_x)
*format "%10.5e"
GravityY                         = *GenData(Gravity_y)
*format "%10.5e"
GravityZ                         = *GenData(Gravity_z)
BoundingBoxOption                = "*GenData(Bounding_Box_option)"
*format "%10.5e"
BoundingBoxEnlargementFactor     = *GenData(Bounding_Box_Enlargement_Factor)
AutomaticBoundingBoxOption       = "*GenData(Automatic_Calculation_Of_Bounding_Box)"
*format "%10.5e"
BoundingBoxMaxX                  = *GenData(Max_X)
*format "%10.5e"
BoundingBoxMaxY                  = *GenData(Max_Y)
*format "%10.5e"
BoundingBoxMaxZ                  = *GenData(Max_Z)
*format "%10.5e"
BoundingBoxMinX                  = *GenData(Min_X)
*format "%10.5e"
BoundingBoxMinY                  = *GenData(Min_Y)
*format "%10.5e"
BoundingBoxMinZ                  = *GenData(Min_Z)
Dimension                        = *GenData(Domain_Dimension)
OutputFileType                   = "*GenData(Output_file_type)"
Multifile                        = "*GenData(Multifile)"
PrintNeighbourLists              = "*GenData(Print_Neighbour_Lists)"
ModelDataInfo                    = "*GenData(Model_Data_Info)"

# Special features

VirtualMassOption                = "*GenData(Virtual_Mass_Option)"
*format "%10.5e"
VirtualMassCoefficient           = *GenData(Virtual_Mass_Coefficient)
*format "%10.5e"
MagicFactor                      = *GenData(Magic_Factor)
*format "%10.5e"
MagicFactorPoisson               = *GenData(Magic_Factor_Poisson)
DeltaOption                      = "*GenData(Set_Search_Tolerance)"
*format "%10.5e"
SearchTolerance                  = *GenData(Search_Tolerance_Absolute)
*format "%10.5e"
CoordinationNumber               = *GenData(Coordination_Number)
*format "%10.5e"
AmplifiedSearchRadiusExtension   = *GenData(Amplified_Continuum_Search_Radius_Extension)
FixVelocitiesOption              = "*GenData(Fix_Velocities_At_Predetermined_Time)"
HorizontalFixVel                 = "*GenData(Fix_Horizontal_Velocities)"
*format "%10.5e"
TotalTimePercentageFixVelocities = *GenData(Time_Step_Constrain_DOFs_Percentage)

# Time Discretization Settings

IntegrationScheme               = "*GenData(Integration_Scheme)"
*format "%10.5e"
TimeStepsPerSearchStep           = *GenData(Time_Steps_Per_Search_Step)
AutoReductionOfTimeStepOption    = "*GenData(Automatic_Time_Step_Reduction_Option)"
*format "%10.5e"
DeltaTimeSafetyFactor            = *GenData(Time_Step_Safety_Factor)
*format "%10.5e"
MaxTimeStep                      = *GenData(Time_Step)
*format "%10.5e"
OutputTimeStep                   = *GenData(Output_Time_Step)
*format "%10.5e"
ControlTime                      = *GenData(Control_Time)

# Material Model

NormalForceCalculationType       = "*GenData(Normal_Force_Calculation)"
NormalDampingType                = "*GenData(Normal_Contact_Damp)"
TangentialDampingType            = "*GenData(Tangential_Contact_Damp)"
FailureCriterionType             = "*GenData(Failure_Criterion)"
*format "%10.5e"
DempackDamping                   = *GenData(Dempack_Damping)
*format "%10.5e"
DempackGlobalDamping             = *GenData(Dempack_Global_Damping)
*format "%10.5e"
AreaFactor                       = *GenData(Area_Factor)
*format "%10.5e"
TauZero                          = *GenData(TSTREN)
*format "%10.5e"
SigmaMax                         = *GenData(Sigma_Max)
*format "%10.5e"
SigmaMin                         = *GenData(NTSTR)
*format "%10.5e"
InternalFriction                 = *GenData(Internal_Friction)

*format "%10.5e"
C1                               = *GenData(NCSTR1)
*format "%10.5e"
N1                               = *GenData(YNGRT1)
*format "%10.5e"
C2                               = *GenData(NCSTR2)
*format "%10.5e"
N2                               = *GenData(YNGRT2)
*format "%10.5e"
C3                               = *GenData(NCSTR3)
*format "%10.5e"
N3                               = *GenData(YNGRT3)
*format "%10.5e"
PlasticYoungModulusRatio         = *GenData(Plastic_Young_Modulus_Ratio)
*format "%10.5e"
PlasticYieldStress               = *GenData(YIELDS)
*format "%10.5e"
DamageDeformationFactor          = *GenData(AXIFRC)
*format "%10.5e"
G1                               = *GenData(Gamma_1)
*format "%10.5e"
G2                               = *GenData(Gamma_2)
*format "%10.5e"
G3                               = *GenData(Gamma_3)
*format "%10.5e"
MaxDef                           = *GenData(Max_Deformation)

RotationalSpringOption           = "*GenData(Rotation_Spring)"
RotaDampingType                  = "*GenData(Rota_Damp_Type)"

# Global Material Parameters

*format "%10.5e"
GeneralDensity                   = *GenData(General_Density)
*format "%10.5e"
GeneralYoungModulus              = *GenData(General_Young_Modulus)
*format "%10.5e"
GeneralPoissonRatio              = *GenData(General_Poisson_Ratio)
*format "%10.5e"
GeneralCohesion                  = *GenData(General_Cohesion)
*format "%10.5e"
GeneralRollingFriction           = *GenData(General_Rolling_Friction)
*format "%10.5e"
GeneralTension                   = *GenData(General_Tension)
*format "%10.5e"
GeneralRotaDampRatio             = *GenData(General_Rota_Damp_Ratio)
*format "%10.5e"
GeneralStaticFrictionCoef        = *GenData(General_Static_Friction_Coef)
*format "%10.5e"
GeneralDynamicFrictionCoef       = *GenData(General_Dynamic_Friction_Coef)
*format "%10.5e"
GeneralRestitutionCoef           = *GenData(General_Restitution_Coef)
*format "%10.5e"
GeneralColour                    = *GenData(General_Colour)
GlobalVariablesOption            = "*GenData(Globally_Specified_Variables)"
*format "%10.5e"
GlobalKn                         = *GenData(Global_KN)
*format "%10.5e"
GlobalKt                         = *GenData(Global_KT)
*format "%10.5e"
GlobalKr                         = *GenData(Global_KR)
*format "%10.5e"
GlobalRn                         = *GenData(Global_RN)
*format "%10.5e"
GlobalRT                         = *GenData(Global_RT)
*format "%10.5e"
GlobalRr                         = *GenData(Global_RR)
*format "%10.5e"
GlobalFrictionAngle              = *GenData(Global_FRI_ANG)

# Continuum Options

StressStrainOperationsOption     = "*GenData(Stress_Strain_Operations)"
ContactMeshOption                = "*GenData(Contact_Mesh)"
ConcreteTestOption               = "*GenData(Concrete_Test)"
BtsOption                        = "*GenData(Brazilian_test)"
GraphOption                      = "*GenData(Graph_Option)"
MonitoringOption                 = "*GenData(Monitoring_Option)"
PoissonMeasure                   = "*GenData(Poisson_Measure)"
*format "%10.5e"
SpecimenHeight                   = *GenData(Specimen_height)
*format "%10.5e"
SpecimenWidth                    = *GenData(Specimen_width)
*format "%10.5e"
MeasuringSurface                 = *GenData(Measuring_Surface)
*format "%10.5e"
MeasuringWidth                   = *GenData(Measuring_Width)
RealTimeGraphOption              = "*GenData(Real_Time_Graph)"
TriaxialOption                   = "*GenData(Triaxial_Option)"
*format "%10.5e"
ConfinementPressure              = *GenData(Confinement_Pressure)
*format "%10.5e"
TotalTimePercentAsForceAplTime   = *GenData(Total_Time_Percentage_As_Force_Aplication_Time)

#POSTPROCES

PostVelocity                     = "*GenData(VELOCITY)"
PostDisplacement                 = "*GenData(DISPLACEMENT)"
PostRadialDisplacement           = "*GenData(RADIAL_DISPLACEMENT)"
PostRHS                          = "*GenData(RHS)"
PostTotalForces                  = "*GenData(TOTAL_FORCES)"
PostDampForces                   = "*GenData(DAMP_FORCES)"
PostAppliedForces                = "*GenData(APPLIED_FORCES)"
PostRadius                       = "*GenData(RADIUS)"
PostParticleCohesion             = "*GenData(PARTICLE_COHESION)"
PostParticleTension              = "*GenData(PARTICLE_TENSION)"
PostGroupId                      = "*GenData(GROUP_ID)"
PostExportId                     = "*GenData(EXPORT_ID)"
PostExportParticleFailureId      = "*GenData(EXPORT_PARTICLE_FAILURE_ID)"
PostExportSkinSphere             = "*GenData(EXPORT_SKIN_SPHERE)"
PostLocalContactForce            = "*GenData(LOCAL_CONTACT_FORCE)"
PostFailureCriterionState        = "*GenData(FAILURE_CRITERION_STATE)"
PostUnidimensionalDamage         = "*GenData(UNIDIMENSIONAL_DAMAGE)"
PostContactFailure               = "*GenData(CONTACT_FAILURE)"
PostContactTau                   = "*GenData(CONTACT_TAU)"
PostContactSigma                 = "*GenData(CONTACT_SIGMA)"
PostAngularVelocity              = "*GenData(ANGULAR_VELOCITY)"
PostParticleMoment               = "*GenData(PARTICLE_MOMENT)"
PostEulerAngles                  = "*GenData(EULER_ANGLES)"
PostRepresentativeVolume         = "*GenData(REPRESENTATIVE_VOLUME)"
PostMeanContactArea              = "*GenData(MEAN_CONTACT_AREA)"
PostStressTensor                 = "*GenData(STRESS_TENSOR)"

#FROM CND:

*Set cond volume_SET_SKIN_MANUALLY *elems
*Add cond surface_SET_SKIN_MANUALLY *elems
*Add cond INHERIT_SKIN_FROM_SURFACE *elems
*if(CondNumEntities(int))
PredefinedSkinOption             = "ON"
*else
PredefinedSkinOption             = "OFF"
*endif

TotalElementsVolume              = *tcl(DEM::Get_Mass_Elements)
MeanRadius                       = *tcl(DEM::Get_Mean_Radius)

# For RigidFace
ComputeMovementOption            =  "*GenData(ComputeMovementOption)"
RotationSpeed                    = *GenData(RotationSpeed)
AxialSpeed                       = *GenData(AxialSpeed)
PropID                           = *GenData(PropID)
GLOBAL_X_VEL                     = *GenData(GLOBAL_X_VEL)
GLOBAL_Y_VEL                     = *GenData(GLOBAL_Y_VEL)
GLOBAL_Z_VEL                     = *GenData(GLOBAL_Z_VEL)
ROTA_ORIGIN_COORD_X              = *GenData(ROTA_ORIGIN_COORD_X)
ROTA_ORIGIN_COORD_Y              = *GenData(ROTA_ORIGIN_COORD_Y)
ROTA_ORIGIN_COORD_Z              = *GenData(ROTA_ORIGIN_COORD_Z)
ROTA_AXIAL_NORMAL_X              = *GenData(ROTA_AXIAL_NORMAL_X)
ROTA_AXIAL_NORMAL_Y              = *GenData(ROTA_AXIAL_NORMAL_Y)
ROTA_AXIAL_NORMAL_Z              = *GenData(ROTA_AXIAL_NORMAL_Z)
BEGIN_TIME                       = *GenData(BEGIN_TIME)
END_TIME                         = *GenData(END_TIME)



# Declare Python Variables
