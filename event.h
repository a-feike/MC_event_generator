#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <array>
#include <vector>

#include "particle.h"



class event{
    public:
        int num_of_particles;
        int proc_ID;
        double xsection;
        double scale;
        double alpha;
        double alpha_s;
        std::vector<particle> particles;

        event()
        {
            proc_ID=0;
            xsection=0;
            scale= 0;
            alpha = 0;
            alpha_s= 0;
        }

        event(int const & id, double const & xsection0, double const & scale0 ,double const & alpha0, double const & alpha_s0){
            num_of_particles=0;
            proc_ID = id;
            xsection = xsection0;
            scale = scale0;
            alpha = alpha0;
            alpha_s = alpha_s0;
        }

        event(int const & id, double const & xsection0, double const & scale0 ,double const & alpha0, 
            double const & alpha_s0, std::vector<particle> const & particles0){            
            proc_ID = id;
            xsection = xsection0;
            scale = scale0;
            alpha = alpha0;
            alpha_s = alpha_s0;
            particles = particles0;
            num_of_particles = particles.size();
        }

        void add_particle(particle const & p){
            particles.push_back(p);
            num_of_particles = particles.size();
        }

    event& operator=(const event& other){
        if (this != &other) { // Check for self-assignment
            num_of_particles = other.num_of_particles;
            proc_ID = other.proc_ID;
            xsection = other.xsection;
            scale = other.scale;
            alpha = other.alpha;
            alpha_s = other.alpha_s;
            particles = other.particles;
        }
        return *this;
    };

/*
    std::vector<event> operator=(std::vector<event>& other){
            std::vector<event> e;
            for (auto e_other : other)
            {
                e.push_back(e_other);
            }
 
        return e;
    };
*/

};

    std::ostream &operator<<(std::ostream &s, const event & e) {
        s   << "<event>"<< std::endl;
        s   << std::setw(2) << e.num_of_particles;
        s   << std::setw(7) << e.proc_ID;
        s   << std::setw(15) << std::scientific << e.xsection;
        s   << std::setw(15) << e.scale;
        s   << std::setw(15) << e.alpha;
        s   << std::setw(15) << e.alpha_s << std::endl;
        for (const auto& p : e.particles)
        {
            s  <<  p;
        }
        s   << "</event>" << std::endl;
        
        return s;
    };






#endif