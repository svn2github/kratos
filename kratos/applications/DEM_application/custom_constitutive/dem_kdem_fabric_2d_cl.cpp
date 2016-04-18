
#include <iostream>
#include "DEM_application.h"
#include "custom_elements/spheric_particle.h"

namespace Kratos {

    DEMContinuumConstitutiveLaw::Pointer DEM_KDEMFabric2D::Clone() const {
        DEMContinuumConstitutiveLaw::Pointer p_clone(new DEM_KDEMFabric2D(*this));
        return p_clone;
    }

    void DEM_KDEMFabric2D::SetConstitutiveLawInProperties(Properties::Pointer pProp) const {
        std::cout << "Assigning DEM_KDEMFabric2D to properties " << pProp->Id() << std::endl;
        pProp->SetValue(DEM_CONTINUUM_CONSTITUTIVE_LAW_POINTER, this->Clone());
    }

    void DEM_KDEMFabric2D::CalculateContactArea(double radius, double other_radius, double& calculation_area) {
        
        KRATOS_TRY
        double radius_sum = radius + other_radius;
        double equiv_radius = radius * other_radius / radius_sum;
        calculation_area = 2.0 * equiv_radius;
        KRATOS_CATCH("")
    }

    void DEM_KDEMFabric2D::ComputeParticleRotationalMoments(SphericContinuumParticle* element,
                                                    SphericContinuumParticle* neighbor,
                                                    double equiv_young,
                                                    double distance,
                                                    double calculation_area,
                                                    double LocalCoordSystem[3][3],
                                                    array_1d<double, 3>& mContactMoment) {
        KRATOS_TRY
        
        DEM_KDEM::ComputeParticleRotationalMoments(element, neighbor, equiv_young, distance, calculation_area, LocalCoordSystem, mContactMoment);
        mContactMoment *= 0.1; // TODO: Hardcoded the reduction of a 90% of the flection in relation to KDEM
        
        KRATOS_CATCH("")
    }
} // namespace Kratos
