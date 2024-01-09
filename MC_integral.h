#ifndef MC_INTEGRAL_H
#define MC_INTEGRAL_H

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <functional>

#include "functions.h"
#include "lhe.h"

/**
 * Generates a random double value within a specific range.
 * 
 * @param lim_lower The lower limit of the random value range.
 * @param lim_upper The upper limit of the random value range.
 * 
 * @return A random double value within the specified range.
*/
double random_value(const double& lim_lower, const double& lim_upper)
{
    // Use a random_device to generate a seed for the random number engine
    std::random_device rd;

    // Use the seed to initialize a Mersenne Twister engine
    std::mt19937 gen(rd());

    // Use a uniform real distribution to generate a random number between 0 and 1
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double randomValue = distribution(gen);

    // Generate a random number between lim_lower and lim_upper
    randomValue = (lim_upper - lim_lower) * randomValue + lim_lower;

    return randomValue;
}




/**
 * Calculates the integral of arbitrary dimension using the Monte Carlo technique.
 * 
 * @param N Number of random values created to evaluate the integral.
 * @param lim_lower Lower limit(s) given as a vector.
 * @param lim_upper Upper limit(s) given as a vector.
 * @param function Function of which the integral should be evaluated.
 * 
 * @return [integral value, standard deviation, maximal function value, maximal arguments]
*/
std::vector<double> MC_integral(const int& N, const std::vector<double>& lim_lower, const std::vector<double>& lim_upper, 
                                    std::function<double(std::vector<double>&)> func)
{
    std::vector<double> randomValues;                           // store random values
    int size =   lim_lower.size();                              // dimension of the integral
    double delta                        = 1;                    //prod (higher_lim-lower_lim)
    double weight_sum                   = 0;                    // will store the sum of all weights
    double squared_weight_sum           = 0;                    // will store the sum of all squared weights
    double weight                       = 0;                    // stores individual weight
    double value                        = 0;                    // value of the function at one random point
    double max_value                    = 0;                    // maximal value of the function
    std::vector<double> max_arguments;                          // stores arguments that maximize the function
    std::vector<double> result;                                 // stores the result

    //check if upper and lower limit have same dimension
    if (lim_upper.size() != size){ 
        std::cout << "Error: Vectors lim_lower and lim_upper don't have the same size." << std::endl;
        throw std::runtime_error("Vectors lim_lower and lim_upper don't have the same size.");
    }

    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < size; j++) // create multidim. random value point
        {
            randomValues.push_back(random_value(lim_lower[j],lim_upper[j]));
            delta = delta*(lim_upper[j]-lim_lower[j]);
        }   

        value   = func(randomValues);                               // assign value
        weight  = delta*value;                                      // assign weight
        weight_sum         = weight_sum + weight;                   // add weight to the sum
        squared_weight_sum = squared_weight_sum+ pow(weight,2);     // add squared weight to the sum

        if (value > max_value)                                      //check if function reaches a maximum here by comparing with previous maximum
        {
            max_arguments   =  randomValues;                        // if yes assign new max_value and new max_arguments
            max_value       =  func(max_arguments);
        }  

        randomValues.clear();                                       // reset temporary variables 
        delta=1;                
    }
    
    result.push_back(weight_sum/N);                                 //value of the integral (eq 2.4 in 1412.4677)
    double V        =   squared_weight_sum/N-pow(result[0],2);      // Variance (eq. 2.5 in 1412.4677)
    result.push_back(sqrt(V/N));                                    // standard deviation (eq. 2.6 in 1412.4677)
    result.push_back(max_value);

    for (size_t k = 0; k < size; k++)
    {
        result.push_back(max_arguments[k]);
    }
    
    // result = [integral value, standard deviation, maximal function value, maximal arguments]
    return result;        


}


lhe MC_generator(const int& N, const std::vector<double>& lim_lower, const std::vector<double>& lim_upper, std::vector<double> result,
                                    std::function<double(std::vector<double>&)> func, 
                                    std::function<event(std::vector<double>&, int const&, double const&,double const& ,double const&, double const&)> func_event, 
                                    std::function<lhe(double const&, double const&, double const&, double const&)> func_lhe)
{
    std::vector<double> randomValues;                           // store random values
    double randValue;                                           // random value for unweighting
    double prob;                                                // probability for unweighting
    int size =   lim_lower.size();                              // dimension of the integral
    double delta                        = 1;                    //prod (higher_lim-lower_lim)
    double weight                       = 0;                    // stores individual weight
    double value                        = 0;                    // value of the function at one random point
    
    double xsection                     = result[0];
    double xsection_unc                 = result[1];
    double max_weight                   = result[2];

    event event1;
    lhe lhe1;
    lhe1 = func_lhe(E_CM/2,E_CM/2,xsection, xsection_unc);


     
    //check if upper and lower limit have same dimension
    if (lim_upper.size() != size){ 
        std::cout << "Error: Vectors lim_lower and lim_upper don't have the same size." << std::endl;
        throw std::runtime_error("Vectors lim_lower and lim_upper don't have the same size.");
    }

    int i = 0; // event number counter
    while (i<N)
    {
        for (size_t j = 0; j < size; j++) // create multidim. random value point
        {
            randomValues.push_back(random_value(lim_lower[j],lim_upper[j]));
            delta = delta*(lim_upper[j]-lim_lower[j]);
        }   

        value   = func(randomValues);                               // assign value
        weight  = delta*value;                                      // assign weight
        prob    = value/max_weight;

        randValue=random_value(0,1);

        if (randValue < prob)                                      //check if function reaches a maximum here by comparing with previous maximum
        {
            std::vector<double> parameter = randomValues;
            parameter.push_back(E_CM);
            int id =1;
            double scale = 1000;
            double alpha = alpha_QED;
            event1 = func_event(parameter, id, xsection, scale, alpha, alpha_s);
            lhe1.add_event(event1);
            i=i+1;            
        }  


        randomValues.clear();                                       // reset temporary variables 
        delta=1;                
    }

    return lhe1;
         
};


/*
double MC_generator(int &N, double & lim_lower, double & lim_upper,double (*func)(double&), double &max_weight)
{
    double weight               = 0;
    double prob                 = 0;
    int count                   = 0;
    double randomValue          = 0; 
    double randomValue2         = 0; 

    while (count<N)
    {
        randomValue =   random_value(lim_lower,lim_upper);
        weight      =   (lim_upper-lim_lower)*func(randomValue);
        prob        =   weight/max_weight;

        randomValue2=   random_value(0,1);   

        if (randomValue2 < prob)
        {
            count=count+1;
        }
    }

}
*/


/*
std::array<double,4> MC_integral(int & N, double & lim_lower, double & lim_upper, double (*func)(double&)){
    double weight_sum           = 0;
    double squared_weight_sum   = 0;
    double weight               = 0;
    double max_value            = 0;
    double max_argument         = 0;
    double randomValue          = 0; 
    std::array<double, 4> result;


    for (size_t i = 0; i < N; i++)
    {
        randomValue =   random_value(lim_lower,lim_upper);
        weight      =   (lim_upper-lim_lower)*func(randomValue);

        if (weight > max_value)
        {
            max_argument    =   randomValue;
            max_value       =   func(max_argument);
        }      
        
        weight_sum         = weight_sum + weight;
        squared_weight_sum = squared_weight_sum+ pow(weight,2);
    }
    result[0]       =   weight_sum/N;                                   //value of the integral (eq 2.4 in 1412.4677)
    double V        =   squared_weight_sum/N-pow(result[0],2);          // Variance (eq. 2.5 in 1412.4677)
    result[1]       =   sqrt(V/N);                                      // standard deviation (eq. 2.6 in 1412.4677)
    result[2]       =   max_value;
    result[3]       =   max_argument;

    return result;

}
*/


#endif