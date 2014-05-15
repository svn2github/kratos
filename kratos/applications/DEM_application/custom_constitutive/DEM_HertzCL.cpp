
#include "DEM_constitutive_law.h"
#include "DEM_HertzCL.h"

namespace Kratos
{

    void CalculateContactForces(double LocalElasticContactForce[3],double indentation,SphericParticle *neighbour_iterator){

        double myYoung = GetYoung();
        double myPoisson = GetPoisson();
        double myLnOfRestitCoeff = GetLnOfRestitCoeff();
        double myTgOfFrictionAngle = GetTgOfFrictionAngle();
        //const double &other_sqrt_of_mass        = neighbour_iterator->GetGeometry()(0)->FastGetSolutionStepValue(SQRT_OF_MASS);
        double other_sqrt_of_mass                 = neighbour_iterator->GetSqrtOfRealMass();
        //const double &other_ln_of_restit_coeff  = neighbour_iterator->GetGeometry()(0)->FastGetSolutionStepValue(LN_OF_RESTITUTION_COEFF);
        //const double &other_tg_of_fri_angle     = neighbour_iterator->GetGeometry()(0)->FastGetSolutionStepValue(PARTICLE_FRICTION);
        const double other_ln_of_restit_coeff     = neighbour_iterator->GetLnOfRestitCoeff();
        const double other_tg_of_fri_angle        = neighbour_iterator->GetTgOfFrictionAngle();
        //const double &mLnOfRestitCoeff          = this->GetGeometry()(0)->FastGetSolutionStepValue(LN_OF_RESTITUTION_COEFF);

        double radius_sum_i                     = 1 / radius_sum;
        double equiv_radius                     = 2 * mRadius * other_radius * radius_sum_i;
        double equiv_area                       = 0.25 * KRATOS_M_PI * equiv_radius * equiv_radius; // 0.25 is becouse we take only the half of the equivalent radius, corresponding to the case of one ball with radius Requivalent and other = radius 0.
        double equiv_mass                       = mSqrtOfRealMass * other_sqrt_of_mass;
        double equiv_ln_of_restit_coeff;
        double aux_norm_to_tang;

        // Globally defined parameters

        
        double equiv_young;
        double equiv_poisson;
        double corrected_area               = equiv_area;

        //const double &other_young       = neighbour_iterator->GetGeometry()(0)->FastGetSolutionStepValue(YOUNG_MODULUS);
        //const double &other_poisson     = neighbour_iterator->GetGeometry()(0)->FastGetSolutionStepValue(POISSON_RATIO);
        const double other_young       = neighbour_iterator->GetYoung();
        const double other_poisson     = neighbour_iterator->GetPoisson();
        
        double effective_radius         = 0.5 * equiv_radius;


                equiv_young                     = myYoung * other_young / (other_young * (1- myPoisson * myPoisson) + myYoung * (1- other_poisson * other_poisson));
                equiv_poisson                   = 2 * myPoisson * other_poisson / (myPoisson + other_poisson);
                equiv_ln_of_restit_coeff        = 0.5 * (myLnOfRestitCoeff + other_ln_of_restit_coeff);
                equiv_tg_of_fri_ang             = 0.5 * (myTgOfFrictionAngle + other_tg_of_fri_angle);

                kn                              = (4/3) * equiv_young * sqrt(effective_radius);
                kt                              = 2.0 * kn * (1 - equiv_poisson * equiv_poisson) / ((2.0 - equiv_poisson) * (1 + equiv_poisson));
                aux_norm_to_tang                = sqrt(kt / kn); 

                LocalElasticContactForce[2] = kn * pow(indentation, 1.5);

                double degradation = 1.0;
                LocalElasticContactForce[0] += - degradation*kt_el * LocalDeltDisp[0];  // 0: first tangential
                LocalElasticContactForce[1] += - degradation*kt_el * LocalDeltDisp[1];  // 1: second tangential  
              
                                         
                 double ShearForceNow = sqrt(LocalElasticContactForce[0] * LocalElasticContactForce[0]
                                    +   LocalElasticContactForce[1] * LocalElasticContactForce[1]); 



      } //Calculate Contact Forces with Hertz CL

} /* namespace Kratos.*/
