//
//   Project Name:        Kratos
//   Last Modified by:    $Author: G.Casas $
//   Date:                $Date: 2006-11-27 16:07:33 $
//   Revision:            $Revision: 1.1.1.1 $
//
//


// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/kratos_flags.h"
#include "spheric_swimming_particle.h"
#include "../applications/DEM_application/custom_utilities/GeometryFunctions.h"
#include "../applications/DEM_application/custom_utilities/AuxiliaryFunctions.h"

namespace Kratos
{
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeAdditionalForces(array_1d<double, 3>& non_contact_force,
                                                                    array_1d<double, 3>& non_contact_moment,
                                                                    const ProcessInfo& r_current_process_info,
                                                                    const array_1d<double,3>& gravity)
{
    KRATOS_TRY

    if (!mCouplingType){
        TBaseElement::ComputeAdditionalForces(non_contact_force, non_contact_moment, r_current_process_info, gravity);
        return;
    }

    NodeType& node = GetGeometry()[0];
    mFluidDensity                           = node.FastGetSolutionStepValue(FLUID_DENSITY_PROJECTED);
    mKinematicViscosity                     = node.FastGetSolutionStepValue(FLUID_VISCOSITY_PROJECTED);
    mFluidFraction                          = node.FastGetSolutionStepValue(FLUID_FRACTION_PROJECTED);
    const array_1d<double, 3>& fluid_vel    = node.FastGetSolutionStepValue(FLUID_VEL_PROJECTED);
    const array_1d<double, 3>& particle_vel = node.FastGetSolutionStepValue(VELOCITY);
    mLastTimeStep = 0.0;

    if (mFluidModelType == 0){ // fluid velocity is modified as a post-process
        noalias(mSlipVel) = fluid_vel / mFluidFraction - particle_vel;
    }

    else {
        noalias(mSlipVel) = fluid_vel - particle_vel;

        if (mDragForceType == 11) {
            const array_1d<double, 3>& fluid_vel_laplacian = node.FastGetSolutionStepValue(FLUID_VEL_LAPL_PROJECTED);
            noalias(mSlipVel) -= mRadius * mRadius / 6.0 * fluid_vel_laplacian; // add Faxen term
        }
    }

    mNormOfSlipVel = SWIMMING_MODULUS_3(mSlipVel);
    double added_mass_coefficient = 0.0; // it is to be modified by those forces that have a contribution proportional to the particle acceleration
    array_1d<double, 3> weight;
    array_1d<double, 3> buoyancy;
    array_1d<double, 3> drag_force;
    array_1d<double, 3> virtual_mass_force;
    array_1d<double, 3> saffman_lift_force;
    array_1d<double, 3> magnus_lift_force;
    array_1d<double, 3> brownian_motion_force;
    array_1d<double, 3>& basset_force = node.FastGetSolutionStepValue(BASSET_FORCE);

    // The decomposition of forces that is considered here follows Jackson (The Dynamics of Fluidized Particles, 2000);
    // so that the role of f_n1 therein is played by non_contact_force here
    TBaseElement::ComputeAdditionalForces(weight, non_contact_moment, r_current_process_info, gravity); // Could add domething else than weight
    ComputeBuoyancy(buoyancy, gravity, r_current_process_info);
    ComputeDragForce(drag_force, r_current_process_info);
    ComputeVirtualMassForce(added_mass_coefficient, virtual_mass_force, r_current_process_info);
    ComputeSaffmanLiftForce(saffman_lift_force, r_current_process_info);
    ComputeMagnusLiftForce(magnus_lift_force, r_current_process_info);
    ComputeHydrodynamicTorque(non_contact_moment, r_current_process_info);
    ComputeBrownianMotionForce(brownian_motion_force, r_current_process_info);

    if (r_current_process_info[TIME_STEPS] >= r_current_process_info[NUMBER_OF_INIT_BASSET_STEPS]){
        ComputeBassetForce(added_mass_coefficient, basset_force, r_current_process_info);
    }

    else {
        const double basset_force_coeff = 6.0 * mRadius * mRadius * mFluidDensity * std::sqrt(KRATOS_M_PI * mKinematicViscosity);
        basset_force = node.FastGetSolutionStepValue(BASSET_FORCE);
        mOldDaitchePresentCoefficient = 0.0;
        mOldBassetTerm = r_current_process_info[DELTA_TIME] * basset_force_coeff * basset_force;
    }

    // Adding all forces except Basset's
    noalias(non_contact_force) += drag_force
                                + virtual_mass_force
                                + saffman_lift_force
                                + magnus_lift_force
                                + brownian_motion_force
                                + buoyancy
                                + weight;

    double force_reduction_coeff = mRealMass / (mRealMass + added_mass_coefficient);
    array_1d<double, 3> non_contact_force_not_altered = non_contact_force;
    //ApplyNumericalAveragingWithOldForces(non_contact_force, r_current_process_info);
    UpdateNodalValues(non_contact_force_not_altered, non_contact_moment, weight, buoyancy, drag_force, virtual_mass_force, basset_force, saffman_lift_force, magnus_lift_force, force_reduction_coeff, r_current_process_info);
    // The basset force has a different temporal treatment, so first we apply the scheme to the rest of the forces
    // And then we add the Basset force (minus the term proportional to the current acceleration, which is treted inplicityly)
    noalias(non_contact_force) += basset_force;
    non_contact_force *= force_reduction_coeff;

    KRATOS_CATCH( "" )
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
// Here nodal values are modified to record DEM forces that we want to print. In Kratos this is an exception since nodal values are meant to be modified only outside the element. Here it was not possible.
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::UpdateNodalValues(const array_1d<double, 3>& non_contact_force,
                                                              const array_1d<double, 3>& non_contact_moment,
                                                              const array_1d<double, 3>& weight,
                                                              const array_1d<double, 3>& buoyancy,
                                                              const array_1d<double, 3>& drag_force,
                                                              const array_1d<double, 3>& virtual_mass_force,
                                                              const array_1d<double, 3>& basset_force,
                                                              const array_1d<double, 3>& saffman_lift_force,
                                                              const array_1d<double, 3>& magnus_lift_force,
                                                              const double& force_reduction_coeff,
                                                              const ProcessInfo& r_current_process_info)
{
    NodeType& node = GetGeometry()[0];
    noalias(node.FastGetSolutionStepValue(HYDRODYNAMIC_FORCE))       = force_reduction_coeff * (non_contact_force + basset_force - buoyancy - weight);
    noalias(node.FastGetSolutionStepValue(BUOYANCY))                 = buoyancy;
    array_1d<double, 3>& total_force = node.FastGetSolutionStepValue(TOTAL_FORCES);
    total_force *= force_reduction_coeff;

    if (mHasHydroMomentNodalVar){
        noalias(node.FastGetSolutionStepValue(HYDRODYNAMIC_MOMENT))  = non_contact_moment;
    }

    if (mHasDragForceNodalVar){
        noalias(node.FastGetSolutionStepValue(DRAG_FORCE))           = drag_force;
    }

    if (mHasVirtualMassForceNodalVar){ // This only includes the part proportional to the fluid acceleration, since the particle acceleration is treated implicitly
        noalias(node.FastGetSolutionStepValue(VIRTUAL_MASS_FORCE))   = virtual_mass_force;
    }

    if (mHasBassetForceNodalVar & false){
        noalias(node.FastGetSolutionStepValue(BASSET_FORCE))         = basset_force;
        noalias(node.FastGetSolutionStepValue(ADDITIONAL_FORCE_OLD)) = non_contact_force;
        //noalias(node.FastGetSolutionStepValue(VELOCITY_OLD))         = node.FastGetSolutionStepValue(VELOCITY);
    }

    if (mHasLiftForceNodalVar){
        noalias(node.FastGetSolutionStepValue(LIFT_FORCE))           = saffman_lift_force + magnus_lift_force;
    }

    if (mHasDragCoefficientVar){
        double drag_coefficient = ComputeDragCoefficient(r_current_process_info);
        node.FastGetSolutionStepValue(DRAG_COEFFICIENT)              = drag_coefficient;
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ApplyNumericalAveragingWithOldForces(array_1d<double, 3>& non_basset_force, const ProcessInfo& r_current_process_info)
{
    non_basset_force = 0.5 * (3 * non_basset_force - GetGeometry()[0].FastGetSolutionStepValue(ADDITIONAL_FORCE_OLD));
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeBuoyancy(array_1d<double, 3>& buoyancy, const array_1d<double, 3>& gravity, const ProcessInfo& r_current_process_info)
{
    if (mBuoyancyForceType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){ // case of identically null buoyancy
        noalias(buoyancy) = ZeroVector(3);
        return;
    }

    else {
        const double volume = TBaseElement::CalculateVolume();

        if (mBuoyancyForceType == 2 || mDragForceType == 2){ // Maxey-Riley form of buoyancy (minus the fluid acceleration term); Weatherford
            noalias(buoyancy) =  - gravity * mFluidDensity * volume;
        }

        else {
            const array_1d<double, 3>& pressure_grad = GetGeometry()[0].FastGetSolutionStepValue(PRESSURE_GRAD_PROJECTED);
            noalias(buoyancy) = - volume * pressure_grad;
        }
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeDragForce(array_1d<double, 3>& drag_force, const ProcessInfo& r_current_process_info)
{
    if (mDragForceType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){ // case of identically null drag force
        noalias(drag_force) = ZeroVector(3);
        return;
    }

    else { // calculating the 'dimensional' drag coefficient, i.e., the factor by which the slip velocity must be multiplied to yield the drag force
        ProcessInfo const& const_current_process_info = r_current_process_info;
        double drag_coeff = ComputeDragCoefficient(const_current_process_info);

        ApplyDragPorosityModification(drag_coeff);

        noalias(drag_force) = drag_coeff * mSlipVel;
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeVirtualMassForce(double& added_mass_coefficient, array_1d<double, 3>& virtual_mass_force, const ProcessInfo& r_current_process_info)
{
    if (mVirtualMassForceType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){ // case of identically null virtual mass force
        noalias(virtual_mass_force) = ZeroVector(3);
        return;
    }

    else {
        const double volume                     = CalculateVolume();
        const array_1d<double, 3>& fluid_acc    = GetGeometry()[0].FastGetSolutionStepValue(FLUID_ACCEL_PROJECTED);
        const array_1d<double, 3>& particle_acc = 1 / GetMass() * GetForce();
        array_1d<double, 3> slip_acc;

    if (mFluidModelType == 0){ // fluid velocity is modified as a post-process
        noalias(slip_acc) = fluid_acc / mFluidFraction - particle_acc;
    }

    else {
        //noalias(slip_acc) = fluid_acc - particle_acc;
        noalias(slip_acc) = fluid_acc; // the particle acceleration is treated implicitly through the added_mass_coefficient
        double volume = CalculateVolume();
        added_mass_coefficient += 0.5 * mFluidDensity * volume;

        if (mDragForceType == 11) {
            const array_1d<double, 3>& fluid_vel_laplacian_rate = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VEL_LAPL_RATE_PROJECTED);
            noalias(slip_acc) -= 0.1 * mRadius * mRadius * fluid_vel_laplacian_rate; // add Faxen term
        }
    }

    double virtual_mass_coeff = 0.5; // inviscid case

    if (mVirtualMassForceType == 2 || mVirtualMassForceType == 4) { // Zuber (1964) (moderate values of solid fraction)
        virtual_mass_coeff += 1.5 * (1 - mFluidFraction);
    }

    else if (mVirtualMassForceType == 3 || mVirtualMassForceType == 4){ // Odar and Hamilton, 1964
        double acc_number;
        ComputeParticleAccelerationNumber(slip_acc, acc_number);
        virtual_mass_coeff *= 2.1 - 0.132 / (SWIMMING_POW_2(acc_number) + 0.12);
    }
    noalias(virtual_mass_force) = mFluidDensity * volume * (virtual_mass_coeff * slip_acc + fluid_acc); // here we add the part of buoyancy due to the acceleration of the fluid
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::GetDaitcheCoefficient(int order, unsigned int n, unsigned int j)
{
    if (order == 1){
        if (j < n){
            return SphericSwimmingParticle<TBaseElement>::mAjs[j];
        }
        else {
            return SphericSwimmingParticle<TBaseElement>::mBns[n];
        }
    }

    else if (order == 2){
        if (n > 3){
            if (j < n - 1){
                return SphericSwimmingParticle<TBaseElement>::mAjs[j];
            }
            else if (j == n - 1){
                return SphericSwimmingParticle<TBaseElement>::mBns[n];
            }
            else {
                return SphericSwimmingParticle<TBaseElement>::mCns[n];
            }
        }
        else {
            if (n == 2){
                long double sqrt_2_over_15 = std::sqrt(static_cast<long double>(2)) / 15;

                if (j == 0){
                    return 12 * sqrt_2_over_15;
                }
                else if (j == 1){
                    return 16 * sqrt_2_over_15;
                }
                else {
                    return 2 * sqrt_2_over_15;
                }
            }
            else {
                long double sqrt_2_over_5 = std::sqrt(static_cast<long double>(2)) / 5;
                long double sqrt_3_over_5 = std::sqrt(static_cast<long double>(3)) / 5;

                if (j == 0){
                    return 4 * sqrt_2_over_5;
                }
                else if (j == 1){
                    return 14 * sqrt_3_over_5 - 12 * sqrt_2_over_5;
                }
                else if (j == 2){
                    return - 8 * sqrt_3_over_5 + 12 * sqrt_2_over_5;
                }
                else {
                    return 4 * sqrt_3_over_5 - 4 * sqrt_2_over_5;
                }
            }
        }
    }

    else {
        if (n > 6){
            if (j < n - 3){
                return SphericSwimmingParticle<TBaseElement>::mAjs[j];
            }
            else if (j == n - 3){
                return SphericSwimmingParticle<TBaseElement>::mBns[n];
            }
            else if (j == n - 2){
                return SphericSwimmingParticle<TBaseElement>::mCns[n];
            }
            else if (j == n - 1){
                return SphericSwimmingParticle<TBaseElement>::mDns[n];
            }
            else {
                return SphericSwimmingParticle<TBaseElement>::mEns[n];
            }
        }
        else {
            if (n == 3){
                long double sqrt_3_over_105 = std::sqrt(static_cast<long double>(3)) / 105;
                if (j == 0){
                    return 68 * sqrt_3_over_105;
                }
                else if (j == 1){
                    return 90 * sqrt_3_over_105;
                }
                else if (j == 2){
                    return 36 * sqrt_3_over_105;
                }
                else {
                    return 16 * sqrt_3_over_105;
                }
            }
            else if (n == 4){
                long double sqrt_2_over_315 = std::sqrt(static_cast<long double>(2)) / 315;
                long double OneOver315 = 1.0 / 315;

                if (j == 0){
                    return 244 * sqrt_2_over_315;
                }
                else if (j == 1){
                    return 1888 * OneOver315 - 976 * sqrt_2_over_315;
                }
                else if (j == 2){
                    return - 656 * OneOver315 + 1464 * sqrt_2_over_315;
                }
                else if (j == 3){
                    return 1632 - 976 * OneOver315;
                }
                else {
                    return - 292 * OneOver315 + 244 * sqrt_2_over_315;
                }
            }
            else if (n == 5){
                long double sqrt_2_over_315 = std::sqrt(static_cast<long double>(2)) / 315;
                long double sqrt_3_over_105 = std::sqrt(static_cast<long double>(3)) / 105;
                long double sqrt_5_over_63  = std::sqrt(static_cast<long double>(5)) / 63;

                if (j == 0){
                    return 244 * sqrt_2_over_315;
                }
                else if (j == 1){
                    return 362 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 2){
                    return 500 * sqrt_5_over_63 - 1448 * sqrt_3_over_105 + 1464 * sqrt_2_over_315;
                }
                else if (j == 3){
                    return - 870 * sqrt_5_over_63 + 2172 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 4){
                    return 660 * sqrt_5_over_63 - 1448 * sqrt_3_over_105 + 244 * sqrt_2_over_315;
                }
                else {
                    return - 164 * sqrt_5_over_63 + 362 * sqrt_3_over_105;
                }
            }
            else {
                long double sqrt_2_over_315 = std::sqrt(static_cast<long double>(2)) / 315;
                long double sqrt_3_over_105 = std::sqrt(static_cast<long double>(3)) / 105;
                long double sqrt_6_over_105 = std::sqrt(static_cast<long double>(6)) / 105;
                long double OneOver315 = 1.0 / 315;

                if (j == 0){
                    return 244 * sqrt_2_over_315;
                }
                else if (j == 1){
                    return 362 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 2){
                    return 5584 * OneOver315 - 1448 * sqrt_3_over_105 + 1464 * sqrt_2_over_315;
                }
                else if (j == 3){
                    return 1720 * sqrt_6_over_105 - 22336 * OneOver315 + 2172 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 4){
                    return - 3564 * sqrt_6_over_105 + 33504 * OneOver315 - 1448 * sqrt_3_over_105 + 244 * sqrt_2_over_315;
                }
                else if (j == 5){
                    return 2808 * sqrt_6_over_105 - 22336 * OneOver315 + 362 * sqrt_3_over_105;
                }
                else {
                    return - 754 * sqrt_6_over_105 + 5584 * OneOver315;
                }
            }
        }
    }
    return 0.0;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::GetDaitcheCoefficient(int order, unsigned int n, unsigned int j, const double last_h_over_h)
{
    if (order == 1){
        if (j < n){
            return SphericSwimmingParticle<TBaseElement>::mAjs[j];
        }
        else {
            return SphericSwimmingParticle<TBaseElement>::mBns[n];
        }
    }

    else if (order == 2){
        double sqrt_alpha = sqrt(last_h_over_h);
        double sqrt_alpha_cubed = SWIMMING_POW_3(sqrt_alpha);
        double sqrt_alpha_5 = SWIMMING_POW_5(sqrt_alpha);
        long double sqrt_2_over_15 = std::sqrt(static_cast<long double>(2)) / 15;

        if (n > 3){
            if (j < n - 1){
                if (j == n - 2){
                    return mAjs[j] + sqrt_2_over_15 * (2 * sqrt_alpha_5 / (last_h_over_h + 1) - 1.0);
                }
                return SphericSwimmingParticle<TBaseElement>::mAjs[j];
            }
            else if (j == n - 1){
                return SphericSwimmingParticle<TBaseElement>::mBns[n] + sqrt_2_over_15 * (2 * sqrt_alpha_cubed + 5 * sqrt_alpha - 7);
            }
            else {
                return SphericSwimmingParticle<TBaseElement>::mCns[n] + sqrt_2_over_15 * ((8 * last_h_over_h + 10) * sqrt_alpha / (last_h_over_h + 1) - 9);
            }
        }
        else {
            if (n == 2){
                if (j == 0){
                    return 12 * sqrt_2_over_15;
                }
                else if (j == 1){
                    return 16 * sqrt_2_over_15;
                }
                else {
                    return 2 * sqrt_2_over_15;
                }
            }
            else {
                long double sqrt_2_over_5 = std::sqrt(static_cast<long double>(2)) / 5;
                long double sqrt_3_over_5 = std::sqrt(static_cast<long double>(3)) / 5;

                if (j == 0){
                    return 4 * sqrt_2_over_5;
                }
                else if (j == 1){
                    return 14 * sqrt_3_over_5 - 12 * sqrt_2_over_5;
                }
                else if (j == 2){
                    return - 8 * sqrt_3_over_5 + 12 * sqrt_2_over_5;
                }
                else {
                    return 4 * sqrt_3_over_5 - 4 * sqrt_2_over_5;
                }
            }
        }
    }

    else {
        if (n > 6){
            if (j < n - 3){
                return SphericSwimmingParticle<TBaseElement>::mAjs[j];
            }
            else if (j == n - 3){
                return SphericSwimmingParticle<TBaseElement>::mBns[n];
            }
            else if (j == n - 2){
                return SphericSwimmingParticle<TBaseElement>::mCns[n];
            }
            else if (j == n - 1){
                return SphericSwimmingParticle<TBaseElement>::mDns[n];
            }
            else {
                return SphericSwimmingParticle<TBaseElement>::mEns[n];
            }
        }
        else {
            if (n == 3){
                long double sqrt_3_over_105 = std::sqrt(static_cast<long double>(3)) / 105;
                if (j == 0){
                    return 68 * sqrt_3_over_105;
                }
                else if (j == 1){
                    return 90 * sqrt_3_over_105;
                }
                else if (j == 2){
                    return 36 * sqrt_3_over_105;
                }
                else {
                    return 16 * sqrt_3_over_105;
                }
            }
            else if (n == 4){
                long double sqrt_2_over_315 = std::sqrt(static_cast<long double>(2)) / 315;
                long double OneOver315 = 1.0 / 315;

                if (j == 0){
                    return 244 * sqrt_2_over_315;
                }
                else if (j == 1){
                    return 1888 * OneOver315 - 976 * sqrt_2_over_315;
                }
                else if (j == 2){
                    return - 656 * OneOver315 + 1464 * sqrt_2_over_315;
                }
                else if (j == 3){
                    return 1632 - 976 * OneOver315;
                }
                else {
                    return - 292 * OneOver315 + 244 * sqrt_2_over_315;
                }
            }
            else if (n == 5){
                long double sqrt_2_over_315 = std::sqrt(static_cast<long double>(2)) / 315;
                long double sqrt_3_over_105 = std::sqrt(static_cast<long double>(3)) / 105;
                long double sqrt_5_over_63  = std::sqrt(static_cast<long double>(5)) / 63;

                if (j == 0){
                    return 244 * sqrt_2_over_315;
                }
                else if (j == 1){
                    return 362 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 2){
                    return 500 * sqrt_5_over_63 - 1448 * sqrt_3_over_105 + 1464 * sqrt_2_over_315;
                }
                else if (j == 3){
                    return - 870 * sqrt_5_over_63 + 2172 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 4){
                    return 660 * sqrt_5_over_63 - 1448 * sqrt_3_over_105 + 244 * sqrt_2_over_315;
                }
                else {
                    return - 164 * sqrt_5_over_63 + 362 * sqrt_3_over_105;
                }
            }
            else {
                long double sqrt_2_over_315 = std::sqrt(static_cast<long double>(2)) / 315;
                long double sqrt_3_over_105 = std::sqrt(static_cast<long double>(3)) / 105;
                long double sqrt_6_over_105 = std::sqrt(static_cast<long double>(6)) / 105;
                long double OneOver315 = 1.0 / 315;

                if (j == 0){
                    return 244 * sqrt_2_over_315;
                }
                else if (j == 1){
                    return 362 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 2){
                    return 5584 * OneOver315 - 1448 * sqrt_3_over_105 + 1464 * sqrt_2_over_315;
                }
                else if (j == 3){
                    return 1720 * sqrt_6_over_105 - 22336 * OneOver315 + 2172 * sqrt_3_over_105 - 976 * sqrt_2_over_315;
                }
                else if (j == 4){
                    return - 3564 * sqrt_6_over_105 + 33504 * OneOver315 - 1448 * sqrt_3_over_105 + 244 * sqrt_2_over_315;
                }
                else if (j == 5){
                    return 2808 * sqrt_6_over_105 - 22336 * OneOver315 + 362 * sqrt_3_over_105;
                }
                else {
                    return - 754 * sqrt_6_over_105 + 5584 * OneOver315;
                }
            }
        }
    }
    return 0.0;
}
//***************************************************************************************************************************************************
//***************************************************************************************************************************************************
template < class TBaseElement >\
void SphericSwimmingParticle<TBaseElement>:: CalculateFractionalDerivative(array_1d<double, 3>& fractional_derivative, double& present_coefficient, double& delta_time, vector<double>& historic_integrands)
{
    fractional_derivative = ZeroVector(3);
    const int N = historic_integrands.size() - 9;
    const int n = (int)N / 3;
    const int order = 2;
    array_1d<double, 3> integrand;

    for (int j = 0; j < n + 1; j++){
        double coefficient     = GetDaitcheCoefficient(order, n + 1, j + 1);
        double old_coefficient = GetDaitcheCoefficient(order, n, j);
        for (int i_comp = 0; i_comp < 3; i_comp++){
            unsigned int integrand_position = N - 3 * j + i_comp;
            integrand[i_comp] = historic_integrands[integrand_position];
        }
        fractional_derivative += (coefficient - old_coefficient) * integrand;
    }
    const array_1d<double, 3>& fluid_vel = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VEL_PROJECTED);
    for (int i_comp = 0; i_comp < 3; i_comp++){
        integrand[i_comp] = historic_integrands[N + 3 + i_comp];
    }
    present_coefficient = GetDaitcheCoefficient(order, n + 1, 0);
    fractional_derivative += present_coefficient * (fluid_vel - integrand);
}
//***************************************************************************************************************************************************
//***************************************************************************************************************************************************
template < class TBaseElement >\
void SphericSwimmingParticle<TBaseElement>:: CalculateFractionalDerivative(array_1d<double, 3>& fractional_derivative, double& present_coefficient, double& delta_time, vector<double>& historic_integrands, const double last_h_over_h)
{
    fractional_derivative = ZeroVector(3);
    const int N = historic_integrands.size() - 9;
    const int n = (int)N / 3;
    const int order = 2;
    array_1d<double, 3> integrand;

    for (int j = 0; j < n + 1; j++){
        double coefficient     = GetDaitcheCoefficient(order, n + 1, j + 1, last_h_over_h);
        double old_coefficient = GetDaitcheCoefficient(order, n, j, last_h_over_h);
        for (int i_comp = 0; i_comp < 3; i_comp++){
            unsigned int integrand_position = N - 3 * j + i_comp;
            integrand[i_comp] = historic_integrands[integrand_position];
        }
        fractional_derivative += (coefficient - old_coefficient) * integrand;
    }
    const array_1d<double, 3>& fluid_vel = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VEL_PROJECTED);
    for (int i_comp = 0; i_comp < 3; i_comp++){
        integrand[i_comp] = historic_integrands[N + 3 + i_comp];
    }
    present_coefficient = GetDaitcheCoefficient(order, n + 1, 0, last_h_over_h);
    fractional_derivative += present_coefficient * (fluid_vel - integrand);
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >\
void SphericSwimmingParticle<TBaseElement>:: CalculateExplicitFractionalDerivative(array_1d<double, 3>& fractional_derivative, double& present_coefficient, double& delta_time, vector<double>& historic_integrands, const double last_h_over_h)
{
    fractional_derivative = ZeroVector(3);
    const int N = historic_integrands.size() - 9;
    const int n = (int)N / 3;
    const int order = 2;
    array_1d<double, 3> integrand;

    for (int j = 0; j < n + 1; j++){
        double coefficient     = GetDaitcheCoefficient(order, n + 1, j + 1, last_h_over_h);
        for (int i_comp = 0; i_comp < 3; i_comp++){
            unsigned int integrand_position = N - 3 * j + i_comp;
            integrand[i_comp] = historic_integrands[integrand_position];
        }
        fractional_derivative += coefficient * integrand;
    }
    const array_1d<double, 3>& vel = GetGeometry()[0].FastGetSolutionStepValue(VELOCITY);
    const array_1d<double, 3>& fluid_vel = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VEL_PROJECTED);

    present_coefficient = GetDaitcheCoefficient(order, n + 1, 0, last_h_over_h);
    fractional_derivative += present_coefficient * (fluid_vel - vel);
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeBassetForce(double &added_mass_coefficient, array_1d<double, 3>& basset_force, const ProcessInfo& r_current_process_info)
{
    if (mBassetForceType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){ // case of identically null virtual mass force
        noalias(basset_force) = ZeroVector(3);
        return;
    }

    else {
        vector<double>& historic_integrands = GetGeometry()[0].GetValue(BASSET_HISTORIC_INTEGRANDS);
        double delta_time = r_current_process_info[DELTA_TIME];
        double quadrature_delta_time = r_current_process_info[DELTA_TIME_QUADRATURE];
        double current_delta_time = r_current_process_info[TIME] - r_current_process_info[LAST_TIME_APPENDING];
        array_1d<double, 3> fractional_derivative_of_slip_vel;
        double present_coefficient;
        const double basset_force_coeff = 6.0 * mRadius * mRadius * mFluidDensity * std::sqrt(KRATOS_M_PI * mKinematicViscosity);
        const double sqrt_of_quad_h_q = std::sqrt(quadrature_delta_time);
//        const double sqrt_of_current_h = std::sqrt(current_delta_time);
        const double last_h_over_h = current_delta_time / quadrature_delta_time;

        if (fabs(quadrature_delta_time - delta_time) < delta_time){ // they are equal
            CalculateFractionalDerivative(fractional_derivative_of_slip_vel, present_coefficient, quadrature_delta_time, historic_integrands);
        }
        else {
            CalculateExplicitFractionalDerivative(fractional_derivative_of_slip_vel, present_coefficient, quadrature_delta_time, historic_integrands, last_h_over_h);
            array_1d<double, 3> basset_term = fractional_derivative_of_slip_vel;
            const array_1d<double, 3>& vel     = GetGeometry()[0].FastGetSolutionStepValue(VELOCITY);
            const array_1d<double, 3>& old_vel = GetGeometry()[0].FastGetSolutionStepValue(VELOCITY_OLD);
            fractional_derivative_of_slip_vel -= mOldBassetTerm + mOldDaitchePresentCoefficient * (old_vel - vel); // the second term is the part that was treated implicitly in the last step
            mOldBassetTerm = basset_term;
            mOldDaitchePresentCoefficient = present_coefficient;
        }

        added_mass_coefficient += basset_force_coeff * sqrt_of_quad_h_q * present_coefficient;
        noalias(basset_force) = basset_force_coeff * sqrt_of_quad_h_q / delta_time * fractional_derivative_of_slip_vel;
    }
}

//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeSaffmanLiftForce(array_1d<double, 3>& lift_force, const ProcessInfo& r_current_process_info)
{
    if (mSaffmanForceType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){ // case of identically null lift force
        noalias(lift_force) = ZeroVector(3);

        return;
    }

    else if (mSaffmanForceType >= 1){
        const double& shear_rate                       = GetGeometry()[0].FastGetSolutionStepValue(SHEAR_RATE_PROJECTED);
        const array_1d<double, 3>& vorticity           = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VORTICITY_PROJECTED);
        array_1d<double, 3> vort_cross_slip_vel;
        SWIMMING_SET_TO_CROSS_OF_FIRST_TWO_3(mSlipVel, vorticity, vort_cross_slip_vel)
        const double vorticity_norm                    = SWIMMING_MODULUS_3(vorticity);

        double lift_coeff;

        if (mSaffmanForceType == 1){ // El Samni, E.A. (1949), see paper by R. K. Clark (1994)
            lift_coeff = ComputeElSamniLiftCoefficient(shear_rate, vorticity_norm, r_current_process_info);
        }

        else if (mSaffmanForceType == 2){ // Mei, 1992 (Re ~ 0.1 - 100)
            double reynolds;
            double reynolds_shear;
            ComputeParticleReynoldsNumber(reynolds);
            const double norm_of_vort = SWIMMING_MODULUS_3(vorticity);
            ComputeParticleRotationReynoldsNumber(norm_of_vort, reynolds_shear);
            lift_coeff = ComputeMeiLiftCoefficient(reynolds, reynolds_shear);
        }

        else {
            std::cout << "The integer value designating the Saffman lift coefficient calculation model" << std::endl;
            std::cout << " (mSaffmanForceType = " << mSaffmanForceType << "), is not supported" << std::endl << std::flush;
            return;
        }

        noalias(lift_force) = lift_coeff * vort_cross_slip_vel; // the direction is given by the vorticity x (- slip_vel) (Jackson, 2000), which is normalized here
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeMagnusLiftForce(array_1d<double, 3>& lift_force, const ProcessInfo& r_current_process_info)
{
    if (mMagnusForceType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){
        noalias(lift_force) = ZeroVector(3);
        return;
    }

    const array_1d<double, 3> slip_rot = 0.5 * GetGeometry()[0].FastGetSolutionStepValue(FLUID_VORTICITY_PROJECTED) - GetGeometry()[0].FastGetSolutionStepValue(ANGULAR_VELOCITY);
    array_1d<double, 3> slip_rot_cross_slip_vel;
    SWIMMING_SET_TO_CROSS_OF_FIRST_TWO_3(slip_rot, mSlipVel, slip_rot_cross_slip_vel)

    if (mMagnusForceType == 1){ // Rubinow and Keller, 1961 (very small Re)
        lift_force = KRATOS_M_PI * SWIMMING_POW_3(mRadius) *  mFluidDensity * slip_rot_cross_slip_vel;
    }

    else if (mMagnusForceType == 2){ // Oesterle and Bui Dihn, 1998 Re < 140
        const double norm_of_slip_rot = SWIMMING_MODULUS_3(slip_rot);
        double reynolds;
        double rot_reynolds;
        ComputeParticleReynoldsNumber(reynolds);
        ComputeParticleRotationReynoldsNumber(norm_of_slip_rot, rot_reynolds);

        if (reynolds == 0.0 || rot_reynolds == 0.0){
            noalias(lift_force) = ZeroVector(3);
        }

        else {
            const double lift_coeff = 0.45  + (rot_reynolds / reynolds - 0.45) * exp(- 0.05684 * pow(rot_reynolds, 0.4) * pow(reynolds, 0.3));
            noalias(lift_force) = 0.5 *  mFluidDensity * KRATOS_M_PI * SWIMMING_POW_2(mRadius) * lift_coeff * mNormOfSlipVel * slip_rot_cross_slip_vel / norm_of_slip_rot;
        }
    }

    else {
        std::cout << "The integer value designating the magnus lift coefficient calculation model" << std::endl;
        std::cout << " (mMagnusForceType = " << mMagnusForceType << "), is not supported" << std::endl << std::flush;
        return;
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeHydrodynamicTorque(array_1d<double, 3>& hydro_torque, const ProcessInfo& r_current_process_info)
{
    if (mHydrodynamicTorqueType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){
        noalias(hydro_torque) = ZeroVector(3);
        return;
    }

    else if (mHydrodynamicTorqueType == 1){
        const array_1d<double, 3> slip_rot = 0.5 * GetGeometry()[0].FastGetSolutionStepValue(FLUID_VORTICITY_PROJECTED) - GetGeometry()[0].FastGetSolutionStepValue(ANGULAR_VELOCITY);
        const double norm_of_slip_rot = SWIMMING_MODULUS_3(slip_rot);
        double rot_reynolds;
        ComputeParticleRotationReynoldsNumber(norm_of_slip_rot, rot_reynolds);
        double rotational_coeff;

        if (rot_reynolds > 32){ // Rubinow and Keller, 1961 (Re_rot ~ 32 - 1000)
            rotational_coeff = 12.9 / sqrt(rot_reynolds) + 128.4 / rot_reynolds;
        }

        else { // Rubinow and Keller, 1961 (Re_rot < 32)
            rotational_coeff = 64 * KRATOS_M_PI / rot_reynolds;
        }

        noalias(hydro_torque) = 0.5 *  mFluidDensity * SWIMMING_POW_5(mRadius) * rotational_coeff * norm_of_slip_rot * slip_rot;
    }

    else {
        std::cout << "The integer value designating the Hydrodynamic torque calculation model" << std::endl;
        std::cout << " (mHydrodynamicTorqueType = " << mHydrodynamicTorqueType << "), is not supported" << std::endl << std::flush;
        return;
    }

}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeBrownianMotionForce(array_1d<double, 3>& brownian_motion_force, const ProcessInfo& r_current_process_info)
{
    if (mBrownianMotionType == 0 || GetGeometry()[0].IsNot(INSIDE) || GetGeometry()[0].Is(BLOCKED)){
        noalias(brownian_motion_force) = ZeroVector(3);
        return;
    }

    else {
        noalias(brownian_motion_force) = ZeroVector(3);
        /*const double kT = 4.11e-21;
        double current_time = r_current_process_info[TIME] ;
        double delta_t_inv = 1.0 / (current_time - mLastTimeStep);
        mLastTimeStep = current_time;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-0.5, 0.5);
        double coeff = std::sqrt(24 * kT * 2 / KRATOS_M_PI * ComputeStokesDragCoefficient() * delta_t_inv);
        brownian_motion_force[0] = coeff * dis(gen);
        brownian_motion_force[1] = coeff * dis(gen);
        brownian_motion_force[2] = coeff * dis(gen);*/
    }
}

//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeParticleReynoldsNumber(double& reynolds)
{
    reynolds = 2 * mRadius * mNormOfSlipVel / mKinematicViscosity;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeParticleRotationReynoldsNumber(double norm_of_slip_rot, double& reynolds)
{
    reynolds = 4 * SWIMMING_POW_2(mRadius) * norm_of_slip_rot / mKinematicViscosity;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ComputeParticleAccelerationNumber(const array_1d<double, 3>& slip_acc, double& acc_number)
{
    acc_number = SWIMMING_POW_3(mNormOfSlipVel) / fabs(2 * mRadius * SWIMMING_INNER_PRODUCT_3(mSlipVel, slip_acc));
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::AdditionalCalculate(const Variable<double>& rVariable, double& Output, const ProcessInfo& r_current_process_info)
{
    if (rVariable == REYNOLDS_NUMBER){

        if (GetGeometry()[0].IsNot(INSIDE)){
            Output = 0.0;
        }

        else {
            mFluidDensity                           = GetGeometry()[0].FastGetSolutionStepValue(FLUID_DENSITY_PROJECTED);
            mKinematicViscosity                     = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VISCOSITY_PROJECTED);
            mFluidFraction                          = GetGeometry()[0].FastGetSolutionStepValue(FLUID_FRACTION_PROJECTED);
            const array_1d<double, 3>& fluid_vel    = GetGeometry()[0].FastGetSolutionStepValue(FLUID_VEL_PROJECTED);
            const array_1d<double, 3>& particle_vel = GetGeometry()[0].FastGetSolutionStepValue(VELOCITY);

            if (mFluidModelType == 0){ // fluid velocity is modified as a post-process
                noalias(mSlipVel) = fluid_vel / mFluidFraction - particle_vel;
            }

            else {
                noalias(mSlipVel) = fluid_vel - particle_vel;
            }

            mNormOfSlipVel = SWIMMING_MODULUS_3(mSlipVel);
            ComputeParticleReynoldsNumber(Output);
        }
    }

    else if (rVariable == DRAG_COEFFICIENT){
        Output = ComputeDragCoefficient(r_current_process_info);
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeDragCoefficient(const ProcessInfo& r_current_process_info)
{
    double drag_coeff;

    if (mDragForceType == 1 || mDragForceType == 10){
        drag_coeff = ComputeStokesDragCoefficient();
    }

    else if (mDragForceType == 2){ // formulations of Haider (1989) and Chien (1994)
        drag_coeff = ComputeWeatherfordDragCoefficient(r_current_process_info);
    }

    else if (mDragForceType == 3){ // formulation of Ganser (1993)
        drag_coeff = ComputeGanserDragCoefficient(r_current_process_info);
    }

    else if (mDragForceType == 4){ // formulation of Ishii and Zuber (1979)
        drag_coeff = ComputeIshiiDragCoefficient(r_current_process_info);
    }

    else if (mDragForceType == 5){ // Newton regime (Re ~ 1000 - 250000), formulation of Haider and Levenspiel (1989)
        drag_coeff = ComputeNewtonRegimeDragCoefficient();
    }

    else if (mDragForceType == 6){ // Intermediate regime (Re ~ 0.5 - 1000), formulation of Schiller and Naumann (1933)
        drag_coeff = ComputeIntermediateRegimeDragCoefficient();
    }

    else if (mDragForceType == 7){ // All regimes (Re ~ 0 - 250000), formulation of Haider and Levenspiel (1989)
        drag_coeff = ComputeHaiderDragCoefficient();
    }

    else if (mDragForceType == 8){ // Intermediate regimes (Re ~ 0 - 1000), formulation of Beetstra et al. obtained using lattice-Boltzmann (2007)
        drag_coeff = ComputeBeetstraDragCoefficient();
    }

    else if (mDragForceType == 9){ // Coin-shaped Stokesian
        drag_coeff = 2.0 / KRATOS_M_PI * ComputeStokesDragCoefficient();
    }

    else if (mDragForceType == 11){ // Maxey-Riley expression with Faxen correction
        drag_coeff = ComputeStokesDragCoefficient(); // temporary
    }

    else {
        std::string message;
        std::string first_part ("The integer value designating the drag coefficient calculation model.\n");
        std::string second_part ("(mDragForceType) is not supported.");
        message = first_part + second_part;
        KRATOS_THROW_ERROR(std::invalid_argument, message, mDragForceType);
    }

    return drag_coeff;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeStokesDragCoefficient()
{
    double drag_coeff = 6.0 * KRATOS_M_PI * mKinematicViscosity * mFluidDensity * mRadius;

    return drag_coeff;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeWeatherfordDragCoefficient(const ProcessInfo& r_current_process_info)
{
    KRATOS_TRY

    const double particle_density              = GetDensity();
    const array_1d<double, 3>& gravity         = r_current_process_info[GRAVITY];
    const int manually_imposed_drag_law_option = r_current_process_info[MANUALLY_IMPOSED_DRAG_LAW_OPTION];
    const int drag_modifier_type               = r_current_process_info[DRAG_MODIFIER_TYPE];
    const double gel_strength                  = GetGeometry()[0].FastGetSolutionStepValue(YIELD_STRESS);
    const double power_law_n                   = GetGeometry()[0].FastGetSolutionStepValue(POWER_LAW_N);
    const double power_law_K                   = GetGeometry()[0].FastGetSolutionStepValue(POWER_LAW_K);
    const double yield_stress                  = GetGeometry()[0].FastGetSolutionStepValue(YIELD_STRESS);

    int non_newtonian_option = 1;

    if (fabs(power_law_n - 1.0) < 0.00001  ||  fabs(yield_stress) < 0.00001) {
        non_newtonian_option = 0;
    }

    const double initial_drag_force            = r_current_process_info[INIT_DRAG_FORCE];
    const double drag_law_slope                = r_current_process_info[DRAG_LAW_SLOPE];
    const double power_law_tol                 = r_current_process_info[POWER_LAW_TOLERANCE];

    const double area                          = KRATOS_M_PI * SWIMMING_POW_2(mRadius);
    const array_1d<double, 3> weight           = mRealMass * gravity;
    const array_1d<double, 3> buoyancy         = mFluidDensity / particle_density * weight; // hydrostatic case!! (only for Weatherford)

    double shahs_term_vel                      = 0.0;
    double beta                                = 0.0;
    double F0                                  = 0.0;
    double regularization_v                    = 0.02 * mRadius;
    double reynolds;
    double drag_coeff;

    if (!non_newtonian_option){ // Newtonian
        ComputeParticleReynoldsNumber(reynolds);

        if (!non_newtonian_option && reynolds < 0.01){
            reynolds = 0.01;
        }

        CalculateNewtonianDragCoefficient(non_newtonian_option, reynolds, mSphericity, drag_coeff, drag_modifier_type);
        drag_coeff = 0.5 *  mFluidDensity * area * drag_coeff * mNormOfSlipVel;
    }

    else {
        shahs_term_vel = CalculateShahsTerm(power_law_n, power_law_K, power_law_tol, particle_density, mSphericity, drag_modifier_type);

        if (!manually_imposed_drag_law_option){
            F0 = 4.0 * gel_strength * area; //initial value
            beta = (SWIMMING_MODULUS_3(weight) - SWIMMING_MODULUS_3(buoyancy) - F0) / shahs_term_vel; //S
        }

        else {
            F0 = initial_drag_force; //initial value
            beta = drag_law_slope; //slope
        }

        if (mNormOfSlipVel >= regularization_v){
            drag_coeff = (F0 + beta * mNormOfSlipVel) / mNormOfSlipVel;
        }

        else {
            drag_coeff = (F0 + beta * regularization_v) / regularization_v;
        }
    }

    return drag_coeff;

    KRATOS_CATCH("")
}

//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::CalculateNewtonianDragCoefficient(int non_newtonian_option,
                                                                const double reynolds,
                                                                double sphericity,
                                                                double& drag_coeff,
                                                                int drag_modifier_type)
{
    if (reynolds < 1){
        drag_coeff = 24.0; // Reynolds;
    }

    else {

        if (reynolds > 1000){
            drag_coeff = 0.44;
        }

        else{
            drag_coeff = 24.0 / reynolds * (1.0 + 0.15 * pow(reynolds, 0.687));
        }
    }

    if (!non_newtonian_option){ // Newtonian

        if (drag_coeff > 2.0){
            drag_coeff = 2.0; // watch out!
        }
    }

    if (sphericity < 0.9999){
        drag_coeff = CalculateDragCoeffFromSphericity(reynolds, sphericity, drag_modifier_type);
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::CalculateDragCoeffFromSphericity(const double reynolds,
                                                                 const double sphericity,
                                                                 const int drag_modifier_type)
{
    double cdrag = 0.0;

    if (drag_modifier_type == 1){ // visual-Red Book
        double interpolator   = (1 - sphericity) / (1 - 0.806);
        double cdrag_modifier = 1 + 0.97 * interpolator + 0.715 * interpolator * log10(reynolds);

        if (reynolds < 1){
            cdrag_modifier += 0.3 * interpolator * pow(- 1.0 * log10(reynolds), 1.6);
        }

        cdrag = cdrag_modifier * cdrag;
    }

    if (drag_modifier_type == 2){ // Hayder
        cdrag = 24 / reynolds * (1 + exp(2.3288 - 6.4581 * sphericity + 2.4486 * sphericity * sphericity) * pow(reynolds, 0.0964 + 0.5565 * sphericity)) + 73.69 * reynolds * exp(- 5.0748 * sphericity) / (reynolds + 5.378 * exp(6.2122 * sphericity));
    }

    if (drag_modifier_type == 3){ // Chien
        cdrag = 30 / reynolds + 67.289 * exp(- 5.03 * sphericity);
    }

    return cdrag;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::CalculateShahsTerm(double power_law_N,
                                                   double power_law_K,
                                                   double power_law_tol,
                                                   const double& particle_density,
                                                   double sphericity,
                                                   int drag_modifier_type)
{
    if (fabs(power_law_N) < power_law_tol || fabs(power_law_K) < power_law_tol){
        std::cout << "WARNING: Shah's method is being used with Power Law data being zero!!" << std::endl << std::flush;
    }

    double shah_A_i = 1 / (6.9148 * power_law_N * power_law_N - 24.838 * power_law_N + 22.642);
    double shah_B_i = 1 / (-0.5067 * power_law_N * power_law_N + 1.3234 * power_law_N - 0.1744);

    double dimensionless_shah = sqrt(pow(13.08, 2 - power_law_N) * pow(2 * mRadius, power_law_N + 2) * pow( mFluidDensity, power_law_N) * pow(particle_density -  mFluidDensity, 2 - power_law_N) / (pow(2, 2 * (power_law_N - 1)) * power_law_K * power_law_K));
    double reynolds = pow(dimensionless_shah * shah_A_i, shah_B_i);
    double fi_i = CalculateDragCoeffFromSphericity(reynolds, 1.0, drag_modifier_type) / CalculateDragCoeffFromSphericity(reynolds, sphericity, drag_modifier_type);
    dimensionless_shah = sqrt(pow(fi_i, 2 - power_law_N)) * dimensionless_shah;
    reynolds = pow(dimensionless_shah * shah_A_i, shah_B_i);

    double terminal_vel =  pow(pow(2, power_law_N - 1) * power_law_K * reynolds / (pow(2 * mRadius, power_law_N) *  mFluidDensity), 1 / (2 - power_law_N)) ;

    return terminal_vel;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeGanserDragCoefficient(const ProcessInfo& r_current_process_info)
{
    KRATOS_TRY

    const int isometric_shape                = 1; // TEMPORARY!! yes (1) or no (0); shold be given as data
    const double surface_area                = 4 * KRATOS_M_PI * SWIMMING_POW_2(mRadius); // TEMPORARY!! corresponding to a sphere; should be generalized b taking it as a parameter
    const double surface_area_circular_diam  = sqrt(4.0 * surface_area / KRATOS_M_PI);

    double equiv_reynolds;
    double k_1;
    double k_2;
    double drag_coeff;

    ComputeGanserParameters(isometric_shape, surface_area_circular_diam, k_1, k_2);
    ComputeParticleReynoldsNumber(equiv_reynolds);
    equiv_reynolds *= k_1 * k_2;

    // calculating adimensional drag coefficient

    drag_coeff =  k_2 * (24 * (1 + 0.1118 * pow((equiv_reynolds), 0.6567)) / (equiv_reynolds) + 0.4305 / (1 + 3305 / equiv_reynolds));

    // and then the dimensional drag coefficient

    drag_coeff *= 0.5 *  mFluidDensity * surface_area * mNormOfSlipVel;

    return drag_coeff;

    KRATOS_CATCH("")
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeIshiiDragCoefficient(const ProcessInfo& r_current_process_info)
{
    double coeff = 0.45;
    double reynolds;
    ComputeParticleReynoldsNumber(reynolds);

    if (reynolds <= 1000){
        coeff = (24 + 2.4 * pow(reynolds, 0.75)) / reynolds;
    }

    double drag_coeff = 0.5 * coeff * KRATOS_M_PI * SWIMMING_POW_2(mRadius);

    return drag_coeff;
}

//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeNewtonRegimeDragCoefficient()
{
    double drag_coeff  = 0.5 * KRATOS_M_PI * SWIMMING_POW_2(mRadius) * mFluidDensity * mNormOfSlipVel;

    drag_coeff *= 0.44;

    return drag_coeff;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeIntermediateRegimeDragCoefficient()
{
    double reynolds;
    double drag_coeff  = 0.5 * KRATOS_M_PI * SWIMMING_POW_2(mRadius) * mFluidDensity * mNormOfSlipVel;

    ComputeParticleReynoldsNumber(reynolds);

    drag_coeff *= 24 / reynolds * (1 + 0.15 * pow(reynolds, 0.687));

    return drag_coeff;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeHaiderDragCoefficient()
{
    const double sphericity = GetGeometry()[0].FastGetSolutionStepValue(PARTICLE_SPHERICITY);
    double drag_coeff       = 0.5 * KRATOS_M_PI * SWIMMING_POW_2(mRadius) * mFluidDensity * mNormOfSlipVel;

    double A = exp(2.3288 - 6.4581 * sphericity + 2.4486 * sphericity * sphericity);
    double B = 0.0964 + 0.5565 * sphericity;
    double C = exp(4.905  - 13.8944 * sphericity + 18.4222 * sphericity * sphericity - 10.2599 * sphericity * sphericity * sphericity);
    double D = exp(1.4681 + 12.2584 * sphericity - 20.7322 * sphericity * sphericity + 15.8855 * sphericity * sphericity * sphericity);
    double particle_reynolds;
    ComputeParticleReynoldsNumber(particle_reynolds);

    drag_coeff *= (24.0 * (1.0 + A * pow(particle_reynolds, B))) / particle_reynolds + C * particle_reynolds / (particle_reynolds + D);

    return drag_coeff;
}

//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeBeetstraDragCoefficient()
{
    double drag_coeff;
    double particle_reynolds;
    ComputeParticleReynoldsNumber(particle_reynolds);

    if (particle_reynolds < 1.0){
        drag_coeff = ComputeStokesDragCoefficient();
    }

    else {
        double eps = GetGeometry()[0].FastGetSolutionStepValue(FLUID_FRACTION_PROJECTED);

        if (eps > 0.999){
            eps = 0.9;
        }

        const double eps_s = 1.0 - eps;
        particle_reynolds *= eps;

        double A = 180 + 18 * std::pow(eps, 4) / eps_s * (1 + 1.5 * std::sqrt(eps_s));
        double B = 0.31 * (1.0 / eps + 3 * eps_s * eps + 8.4 * std::pow(particle_reynolds, - 0.343)) / (1.0 + std::pow(10.0, 3 * eps_s) * std::pow(particle_reynolds, 2 * eps - 2.5));
        drag_coeff = KRATOS_M_PI_3 * mKinematicViscosity * mFluidDensity * mRadius * (A * eps_s / eps + B * particle_reynolds);
    }

    return drag_coeff;
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
 void SphericSwimmingParticle<TBaseElement>::ComputeGanserParameters(const int isometric_shape, const double dn, double& k_1, double& k_2)
 {
     if (isometric_shape){
         k_1 = 3 / (1 + 2 / sqrt(mSphericity));
     }

     else {
         k_1 = 3 / (0.5 * dn / mRadius + 2 / sqrt(mSphericity));
     }

     k_2 = pow(10.0, 1.8148 * pow(- log10(mSphericity), 0.5743));
 }
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::ApplyDragPorosityModification(double& drag_coeff)
{
    if (mPorosityCorrectionType == 0){
        return;
    }

    else if (mPorosityCorrectionType == 1){ // Richardson and Zaki, 1954 (fluid fraction ~ 0.01 - 0.2)
        double reynolds;
        ComputeParticleReynoldsNumber(reynolds);
        double K;

        if (reynolds > 500){
            K = 2.39;
        }

        else if (reynolds > 1){
            K = 4.45 * pow(reynolds, - 0.1);
        }

        else if (reynolds > 0.2){
            K = 4.35 * pow(reynolds, - 0.03);
        }

        else {
            K = 4.65;
        }

        drag_coeff *= pow(mFluidFraction, 1 - 2 * K);
    }

    else if (mPorosityCorrectionType == 2){

    }

}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
double SphericSwimmingParticle<TBaseElement>::ComputeElSamniLiftCoefficient(const double norm_of_shear_rate,
                                                              const double vorticity_norm,
                                                              const ProcessInfo& r_current_process_info)
{
    if (vorticity_norm > 0.000000000001 && mNormOfSlipVel > 0.000000000001){
         const double yield_stress   = 0.0; // we are considering a Bingham type fluid
         const double power_law_K    = GetGeometry()[0].FastGetSolutionStepValue(POWER_LAW_K);
         const double power_law_n    = GetGeometry()[0].FastGetSolutionStepValue(POWER_LAW_N);
         const double shear_rate_p   = mNormOfSlipVel / mRadius * (4.5 / power_law_n - 3.5); // graphic model by Unhlherr et al. (fit by Wallis, G.B. and Dobson, J.E., 1973)
         double equivalent_viscosity = yield_stress / shear_rate_p + power_law_K * pow(shear_rate_p, power_law_n - 1);
         const double coeff          = std::max(0.09 * mNormOfSlipVel, 5.82 * sqrt(0.5 * mNormOfSlipVel * equivalent_viscosity /  mFluidDensity));
         const double lift_coeff     = 0.5 * KRATOS_M_PI * SWIMMING_POW_2(mRadius) *  mFluidDensity * coeff * mNormOfSlipVel / vorticity_norm;
         return(lift_coeff);
    }

    else {
        return 0.0;
    }
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
 double SphericSwimmingParticle<TBaseElement>::ComputeMeiLiftCoefficient(const double reynolds, const double reynolds_shear)
 {
     if (reynolds != 0.0 && reynolds_shear != 0.0 ){
         double sqrt_beta = sqrt(0.5 * reynolds_shear / reynolds);
         double C;

         if (reynolds < 40){
             C = (1 - 0.3314 * sqrt_beta) * exp(- 0.1 * reynolds) + 0.3314 * sqrt_beta;
         }

         else {
             C = 0.0524 * sqrt_beta * sqrt(reynolds);
         }

         C *= 4.1126 / sqrt(reynolds_shear);

         double lift_coeff = mFluidDensity * KRATOS_M_PI * SWIMMING_POW_3(mRadius) * C;

         return lift_coeff;
     }

     else {
         return 0.0;
     }
 }
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template< class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::Initialize(const ProcessInfo& r_process_info)
{
    TBaseElement::Initialize(r_process_info);
    mHasDragForceNodalVar        = GetGeometry()[0].SolutionStepsDataHas(DRAG_FORCE);
    mHasHydroMomentNodalVar      = GetGeometry()[0].SolutionStepsDataHas(HYDRODYNAMIC_MOMENT);
    mHasVirtualMassForceNodalVar = GetGeometry()[0].SolutionStepsDataHas(VIRTUAL_MASS_FORCE);
    mHasBassetForceNodalVar      = GetGeometry()[0].SolutionStepsDataHas(BASSET_FORCE);
    mHasLiftForceNodalVar        = GetGeometry()[0].SolutionStepsDataHas(LIFT_FORCE);
    mSphericity                  = GetGeometry()[0].SolutionStepsDataHas(PARTICLE_SPHERICITY);
    mHasDragCoefficientVar       = GetGeometry()[0].SolutionStepsDataHas(DRAG_COEFFICIENT);
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template < class TBaseElement >
void SphericSwimmingParticle<TBaseElement>::MemberDeclarationFirstStep(const ProcessInfo& r_process_info)
{
    TBaseElement::MemberDeclarationFirstStep(r_process_info);
    mCouplingType           = r_process_info[COUPLING_TYPE];
    mBuoyancyForceType      = r_process_info[BUOYANCY_FORCE_TYPE];
    mDragForceType          = r_process_info[DRAG_FORCE_TYPE];
    mVirtualMassForceType   = r_process_info[VIRTUAL_MASS_FORCE_TYPE];
    mBassetForceType        = r_process_info[BASSET_FORCE_TYPE];
    mSaffmanForceType       = r_process_info[LIFT_FORCE_TYPE];
    mMagnusForceType        = r_process_info[MAGNUS_FORCE_TYPE];
    mFluidModelType         = r_process_info[FLUID_MODEL_TYPE];
    mPorosityCorrectionType = r_process_info[DRAG_POROSITY_CORRECTION_TYPE];
    mHydrodynamicTorqueType = r_process_info[HYDRO_TORQUE_TYPE];
    mBrownianMotionType     = 0;
    mInitialTime            = r_process_info[TIME];
}
//**************************************************************************************************************************************************
//**************************************************************************************************************************************************
template class SphericSwimmingParticle<SphericParticle>; //Explicit Instantiation
template class SphericSwimmingParticle<NanoParticle>; //Explicit Instantiation
template <typename TBaseElement>
std::vector<double> SphericSwimmingParticle<TBaseElement>::mAjs;
template <typename TBaseElement>
std::vector<double> SphericSwimmingParticle<TBaseElement>::mBns;
template <typename TBaseElement>
std::vector<double> SphericSwimmingParticle<TBaseElement>::mCns;
template <typename TBaseElement>
std::vector<double> SphericSwimmingParticle<TBaseElement>::mDns;
template <typename TBaseElement>
std::vector<double> SphericSwimmingParticle<TBaseElement>::mEns;
}  // namespace Kratos.

