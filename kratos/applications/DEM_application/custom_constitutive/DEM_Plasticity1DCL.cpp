
#include "DEM_constitutive_law.h"
#include "DEM_Plasticity1DCL.h"

namespace Kratos
{

    void CalculateContactForces(double LocalElasticContactForce[3], double indentation){
  
    void SphericContinuumParticle::PlasticityAndDamage1D(double LocalElasticContactForce[3], double kn_el, double equiv_young, double indentation, double calculation_area, double radius_sum_i, double& failure_criterion_state, double& acumulated_damage, int i_neighbour_count, int mapping_new_cont, int mapping_new_ini, int time_steps){
                     
      //VARIABLES
    
      // a guardar:
  
      double kn_b = kn_el / mN1;
      double kn_c = kn_el / mN2;
      double kn_d = kn_el / mN3;
      double kp_el = mYoungPlastic/equiv_young * kn_el;
      double Yields_el = mPlasticityLimit * calculation_area;
        
      double Ncstr1_el = mC1 * calculation_area;
      double Ncstr2_el = mC2 * calculation_area;
      double Ncstr3_el = mC3 * calculation_area;
      double Ntstr_el  = mTensionLimit * calculation_area;
      double u_max = mHistory[mapping_new_cont][0];
      
      double& fn = LocalElasticContactForce[2]; //[2] 'normal' contact force
                
      
      if( indentation >= 0.0 ) //COMPRESSION
      {
        
          fn = kn_el * indentation;
          
          double u_ela1 = Ncstr1_el/kn_el;;
          double u_ela2 = u_ela1 + (Ncstr2_el-Ncstr1_el)/(kn_b);
          double u_ela3 = u_ela2 + (Ncstr3_el-Ncstr2_el)/(kn_c);

          if ( ( indentation > u_max ) || ( time_steps <= 1) )   //maximum historical intentation OR first step  MSIMSI 0
            
          {

            mHistory[mapping_new_cont][0]  = indentation;             // Guarda el treshold del màxim desplaçament
            
            
            if (indentation > u_ela3) //4rt tram
            {
              
              fn = Ncstr3_el + ( indentation - u_ela3 )*kn_d;
              mHistory[mapping_new_cont][3] = kn_d/kn_el;
              
            }
            else if (indentation > u_ela2) //3r tram
            {
              
              fn = Ncstr2_el + ( indentation - u_ela2 )*kn_c;
              mHistory[mapping_new_cont][3] = kn_c/kn_el;
              
            }
            else
            {    
              if( indentation > u_ela1) //2n tram
              {
                fn = Ncstr1_el + (indentation - u_ela1)*kn_b;
                mHistory[mapping_new_cont][3] = kn_b/kn_el;
              
              }
              
            }
          
          mHistory[mapping_new_cont][1] = fn; //actualitzar la força màxima a compressió.
          
          }
          
          else //Per sota del màxim.
          {

              if(mHistory[mapping_new_cont][1] > 0.0)  //Màxim en compressió. 
              {
 
                  double u_plas;        //MSIMSI 2 akesta operació de saber quant val la u_plastica es fa cada pas de temps i en realitat es fixe sempre.

                  if(Yields_el <= Ncstr1_el) //si el punt de plastificació està en la primera rama elastica.
                  {
                      u_plas = Yields_el/kn_el;

                  }
                  else
                  {  
                    if(Yields_el <= Ncstr2_el) //si està en la segona...
                    {
                        u_plas = u_ela1 + (Yields_el-Ncstr1_el)/(kn_b);

                    }
                     else if(Yields_el <= Ncstr3_el) //si està en la tercera...
                    {
                        u_plas = u_ela2 + (Yields_el-Ncstr2_el)/(kn_c);

                    }
                    
                    else //en la quarta
                    {

                      u_plas = u_ela3 + (Yields_el-Ncstr3_el)/(kn_d);
                    }
                    
                  }

                  
                  if ( u_plas < u_max ) //si nosaltres estem per sota del maxim pero ja estem plastificant 
                  {
                    fn = mHistory[mapping_new_cont][1] - kp_el*(u_max - indentation); // Esta en zona de descarga plastica (pot estar en carga/descarga)
                    mHistory[mapping_new_cont][3] = kp_el/kn_el;
                    
                    
                  }
                  else                                   // Esta en zona descarga elastica, ens despreocupem de la plasticitat
                  {

                    if ( indentation > u_ela3)  //en la 4a ramma
                    {
                      fn = Ncstr3_el + (indentation - u_ela3)*kn_d;
                      
                    }
                    
                    else if ( indentation > u_ela2)  //en la 3a ramma
                    {
                      fn = Ncstr2_el + (indentation - u_ela2)*kn_c;
                      
                    }
                    
                    else
                    {
                      if(indentation > u_ela1)  //en la 2a rama
                      {
                        fn = Ncstr1_el + (indentation-u_ela1)*kn_b;
                      }
            
                    }
            
                  }

              } //si tenim precàrrega en compressió.
              
          }//Per sota del màxim.


      } //Compression
              

      else //tension
      {
        fn = kn_el * indentation; 
        
        double u1 = Ntstr_el / kn_el;

        double u2 = u1*(1+ mDamageMaxDisplacementFactor);
 
          if(fabs(indentation) > u2)                  // FULL DAMAGE 
          {
            mNeighbourFailureId[i_neighbour_count] = 4; //tension failure
            mIniNeighbourFailureId[ mapping_new_ini ] = 4;
            acumulated_damage = 1.0;
            fn = 0.0;
          }
          else
          {
            if (fabs(indentation) > u1)  
            {
              double u_frac = (fabs(indentation) - u1)/(u2 - u1);
              //failure_criterion_state = fabs(indentation)/u2;
              acumulated_damage = u_frac;
              
              
              if (u_frac > mHistory[mapping_new_cont][2])  
              {
                mHistory[mapping_new_cont][2] = u_frac;
              }
              
            }
            
            fn = indentation * kn_el*(1.0 -  mHistory[mapping_new_cont][2]);  // normal adhesive force (gap +)
            
          }
      
        }//Tension



             double degradation = 1.0;

              if(indentation >= 0.0 ) //COMPRESSION
              {
              
                degradation = mHistory[mapping_new_cont][3];
               
              }
              else
              {
               
                degradation = (1.0 -  mHistory[mapping_new_cont][2]);
               
              }


            LocalElasticContactForce[0] += - degradation*kt_el * LocalDeltDisp[0];  // 0: first tangential
            LocalElasticContactForce[1] += - degradation*kt_el * LocalDeltDisp[1];  // 1: second tangential  
              
                                         
            double ShearForceNow = sqrt(LocalElasticContactForce[0] * LocalElasticContactForce[0]
                                    +   LocalElasticContactForce[1] * LocalElasticContactForce[1]); 





                /* Tangential Friction for broken bonds */  //dempack and kdem do the same.
                
                if ( mNeighbourFailureId[i_neighbour_count] != 0 ) //*   //degut als canvis de DEMPACK hi ha hagut una modificació, ara despres de trencar es fa akest maping de maxima tangencial que és correcte!
                {
                   double Frictional_ShearForceMax = equiv_tg_of_fri_ang * LocalElasticContactForce[2];
                
                 if (Frictional_ShearForceMax < 0.0)
                 {
                  Frictional_ShearForceMax = 0.0;
                 }
                  
                  
                  failure_criterion_state = 1.0;
                                                        
                  if( (ShearForceNow >  Frictional_ShearForceMax) && (ShearForceNow != 0.0) ) 
                  {
                   LocalElasticContactForce[0] = (Frictional_ShearForceMax / ShearForceNow) * LocalElasticContactForce[0];
                   LocalElasticContactForce[1] = (Frictional_ShearForceMax / ShearForceNow )* LocalElasticContactForce[1];
                   sliding = true;              
                  }
                  
                }


               /* Viscodamping (applyied locally)*/ 
               
                 //DAMPING:

              double ViscoDampingLocalContactForce[3]    = {0.0};
              
              if  (indentation > 0.0 || (mNeighbourFailureId[i_neighbour_count] == 0) )
              {  
                
                double LocalRelVel[3] = {0.0};
                
                GeometryFunctions::VectorGlobal2Local(OldLocalCoordSystem, RelVel, LocalRelVel);
                
                   
                  equiv_visco_damp_coeff_normal     = mDempack_damping*2.0*sqrt(kn_el/(mSqrtOfRealMass * mSqrtOfRealMass + other_sqrt_of_mass * other_sqrt_of_mass))*equiv_mass;   // := 2d0* sqrt ( kn_el*(m1*m2)/(m1+m2) )
                  equiv_visco_damp_coeff_tangential = equiv_visco_damp_coeff_normal * aux_norm_to_tang; // dempack no l'utilitza...

                  


                CalculateViscoDamping(LocalRelVel,ViscoDampingLocalContactForce,indentation,equiv_visco_damp_coeff_normal,equiv_visco_damp_coeff_tangential,sliding);
                  
              }

    }

} /* namespace Kratos.*/
