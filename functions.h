#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <math.h>
#include <vector>

#include "constants.h"
#include "particle.h"
#include "MC_integral.h"
#include "event.h"
#include "lhe.h"



void throw_number_of_arguments_error(const int & N, std::vector<double> & x){
    if (x.size() != N){
    std::cout << "Error: Wrong number of Arguments." << std::endl;
    throw std::runtime_error("Wrong number of Arguments.");
    }
}

/**
 * Function to calculate e+ e- -> Z/gamma -> mu+ mu-
 * For the cross section integrate over cos_theta
 * 
 * @param cos_theta vector to be in accordance with MC_integral, between -1 and 1
 * @return value of the function 
*/
double function_eeZGmumu(std::vector<double> & cos_theta){
    throw_number_of_arguments_error(1,cos_theta);

    double CV       =   -0.5+2*sw2;
    double CA       =   -0.5;
    double kappa    =   sqrt(2)*G_Fermi*pow(MZ,2)/(4*M_PI*alpha_QED);
    double chi1     =   kappa*hat_s * (hat_s-pow(MZ,2))/(pow(hat_s-pow(MZ,2),2)+pow(WZ,2)*pow(MZ,2)) ;
    double chi2     =   pow(kappa,2)*pow(hat_s,2)/(pow(hat_s-pow(MZ,2),2)+pow(WZ,2)*pow(MZ,2));
    double A_0      =   1+2*pow(CV,2)*chi1+pow(pow(CA,2)+pow(CV,2),2)*chi2;
    double A_1      =   4*pow(CA,2)*chi1+8*pow(CA,2)*pow(CV,2)*chi2;
    return natunits*2*M_PI*std::pow(alpha_QED,2)/(4 * hat_s)*(A_0*(1+pow(cos_theta[0],2))+ A_1*cos_theta[0]);
}

/**
 * Calculates the 4 momenta of the 4 particles in e+ e- -> Z/gamma -> mu+ mu-
 * 
 * @param parameter encodes all parameters needed to calculate the momenta etc fo the particles
 *                  here it is parameter[0]=cos_theta, parameter[1]=ECM
 * @return returns a vector of particles (see class particle)
*/
std::vector<particle>  particle_eeZGmumu(std::vector<double> & parameter){
    std::vector<particle> particles;
    double costh = parameter[0];
    double ECM   = parameter[1];

    double phi = random_value(0, 2*M_PI);
    double sinphi = sin(phi);
    double cosphi = cos(phi);
    double sinth = sqrt(1-pow(costh,2));

    int pdg_id = 11;
    int status = -1;
    std::array<int, 2> parents{0,0};
    std::array<double, 4> momentum{0.5*ECM, 0., 0. , 0.5*ECM};
    particle em(pdg_id, status, parents, momentum);                 //e-

    momentum = {0.5*ECM, 0., 0. , -0.5*ECM};
    particle ep(-pdg_id, status, parents, momentum);                //e+

    
    pdg_id = 13;
    status = 1;
    parents = {1,2};
    momentum = {0.5*ECM, 0.5*ECM*sinth*cosphi, 0.5*ECM*sinth*sinphi , 0.5*ECM*costh};
    particle mum(pdg_id, status, parents, momentum);                 //mu-
    mum.set_mass(0.);

    momentum = {0.5*ECM, -0.5*ECM*sinth*cosphi, -0.5*ECM*sinth*sinphi , -0.5*ECM*costh};
    particle mup(-pdg_id, status, parents, momentum);                 //mu+
    mup.set_mass(0.);

    particles.push_back(em);
    particles.push_back(ep);
    particles.push_back(mum);
    particles.push_back(mup);

    return particles;
}

event event_eeZGmumu(std::vector<double> & parameter, int const & id, double const & xsection0,
                     double const & scale0 ,double const & alpha0, double const & alpha_s0){    
    std::vector<particle> particles = particle_eeZGmumu(parameter);
    event event0(id, xsection0, scale0, alpha0, alpha_s0, particles);
    return event0;
    };

lhe lhe_eeZGmumu(double const beam_energy1, double const beam_energy2,double const & xsection, double const & xsection_unc){
    int beam_id1 = 11;
    int beam_id2 = -11;
    lhe lhe1(beam_id1, beam_id2, beam_energy1, beam_energy2, xsection, xsection_unc);    
    return lhe1;
    };



/**
 * Analytic integral of e+ e- -> Z/gamma -> mu+ mu- cross section
 * 
 * @param lim_lower lower limit as vector to be in accordance with MC_integral
 * @param lim_upper upper limit as vector to be in accordance with MC_integral
 * @return value of the integral
*/
double integral_eeZGmumu(std::vector<double> & lim_lower,std::vector<double> & lim_upper){
    throw_number_of_arguments_error(1, lim_lower);
    throw_number_of_arguments_error(1, lim_upper);
    double CV       =   -0.5+2*sw2;
    double CA       =   -0.5;
    double kappa    =   sqrt(2)*G_Fermi*pow(MZ,2)/(4*M_PI*alpha_QED);
    double chi1     =   kappa*hat_s * (hat_s-pow(MZ,2))/(pow(hat_s-pow(MZ,2),2)+pow(WZ,2)*pow(MZ,2)) ;
    double chi2     =   pow(kappa,2)*pow(hat_s,2)/(pow((hat_s-pow(MZ,2)),2)+pow(WZ,2)*pow(MZ,2));
    double A_0      =   1+2*pow(CV,2)*chi1+pow((pow(CA,2)+pow(CV,2)),2)*chi2;
    double A_1      =   4*pow(CA,2)*chi1+8*pow(CA,2)*pow(CV,2)*chi2;

    double tmp_upper= A_0*(lim_upper[0]+pow(lim_upper[0],3)/3)+A_1*0.5*pow(lim_upper[0],2);
    double tmp_lower= A_0*(lim_lower[0]+pow(lim_lower[0],3)/3)+A_1*0.5*pow(lim_lower[0],2);
    return natunits*2*M_PI*std::pow(alpha_QED,2)/(4 * hat_s)*(tmp_upper-tmp_lower);
}




double function2(std::vector<double> & x){
    throw_number_of_arguments_error(1,x);
    return x[0];
}

double integral2(std::vector<double>& lim_lower, std::vector<double>& lim_upper){
    throw_number_of_arguments_error(1, lim_lower);
    throw_number_of_arguments_error(1, lim_upper);

    double tmp=pow(lim_upper[0],2)-pow(lim_lower[0],2);
    return 0.5*tmp;
}




double function3(std::vector<double> & x){
    throw_number_of_arguments_error(2,x);
    return x[0]*x[1];
}

double integral3(std::vector<double>& lim_lower, std::vector<double>& lim_upper){
    throw_number_of_arguments_error(2, lim_lower);
    throw_number_of_arguments_error(2, lim_upper);
    double tmp=(pow(lim_upper[0],2)-pow(lim_lower[0],2))*(pow(lim_upper[1],2)-pow(lim_lower[1],2));
    return 0.25*tmp;
}

#endif