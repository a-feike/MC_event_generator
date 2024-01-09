#include <iostream>
#include <math.h>
#include <vector>
#include <functional> // Include the necessary header for std::function
//#include "LHAPDF/LHAPDF.h"


#include "functions.h"
#include "MC_integral.h"
#include "constants.h"
#include "particle.h"

//using namespace MC_namespace;



int main(){

    //declare variables
    std::vector<double> lim_lower;
    std::vector<double> lim_upper;
    std::vector<double> result;
    double I_theo;
    double N;
    double max_weight;

    lim_lower    = {-1}; //set lower and upper limits 
    lim_upper    = {1};   
    N=10000; //Monte Carlo runs for integration
    result = MC_integral(N, lim_lower, lim_upper, function_eeZGmumu);  //Monte Carlo integration
    I_theo = integral_eeZGmumu(lim_lower, lim_upper);

    std::cout << "I_MC=" <<result[0] << "+/-" << result[1] <<" with f_max=" << result[2] << " at:" ;
    for (size_t i = 3; i < result.size(); i++)
    {
        std::cout<< " " << result[i];
    }
    std::cout << std::endl;
    
    std::cout << "analytic I=" << I_theo << std::endl;
    std::cout << "ratio I=" << I_theo/result[0] << std::endl;
    std::cout << "------------------------" << std::endl;

    //event generation of N events
    N=5;
    std::cout<<MC_generator(N, lim_lower, lim_upper, result, function_eeZGmumu,event_eeZGmumu , lhe_eeZGmumu);

      
    return 0;

    
}