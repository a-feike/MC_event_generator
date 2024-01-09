#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <array>
#include <iomanip>
#include <math.h>

//namespace MC_namespace{


    class particle{
        public:
        
            int                         PDG_ID;
            int                         status;  //incoming -1, outgoing 1, intermediate 2
            std::array<int, 2>          parents;
            std::array<double, 2>       color_flow;
            std::array<double, 4>       momentum;
            double                      mass;
            double                      distance;
            double                      helicity;

        
            double mass_calc(std::array<double, 4> & momentum){
                return sqrt(pow(momentum[0],2)-pow(momentum[1],2)-pow(momentum[2],2)-pow(momentum[3],2));
            }

            particle(){
                PDG_ID      =   0;
                status      =   0;
                parents     =   {0,0};
                momentum    =   {0,0,0,0};
                color_flow  =   {0,0};
                mass        =   0;
                distance    =   0;
                helicity    =   0;                
            }

            particle(const int & pdg_id, const int & status0, const std::array<int, 2> & parents0, std::array<double, 4> & momentum0){
                PDG_ID      =   pdg_id;
                status      =   status0;
                parents     =   parents0;
                momentum    =   momentum0;
                color_flow  =   {0,0};
                mass        =   mass_calc(momentum0);
                distance    =   0;
                helicity    =   0;
            }

            particle(const int & pdg_id, 
                    const int & status0,
                    const std::array<double, 2> & color_flow0,
                    const std::array<int, 2> & parents0, 
                    std::array<double, 4> & momentum0,
                    const double & mass0,  
                    const double & distance0,  
                    const double & helicity0){
                PDG_ID      =   pdg_id;
                status      =   status0;
                parents     =   parents0;
                color_flow  =   color_flow0;
                momentum    =   momentum0;
                mass        =   mass0;
                distance    =   distance0;
                helicity    =   helicity0;

                if (1-mass/mass_calc(momentum0)>0.05)
                {
                    std::cout << "Mass is not equal to 4 momentum squared" << std::endl;
                }
                 
                
            }


            void set_pdg_id(const int & pdg_id){PDG_ID=pdg_id;}

            void set_status(const int & status0){status=status0;}

            void set_mass(const double & mass0){mass=mass0;}

            void set_distance(const double & distance0){distance=distance0;}

            void set_helicity(const double & helicity0){helicity=helicity0;}

            void set_parents(const std::array<int, 2> & parents0){parents=parents0;}

            void set_parents(const int & p1, const int & p2){parents={p1,p2};}

            void set_momentum(const std::array<double, 4> & momentum0){momentum=momentum0;};

            void set_momentum(const double & m1,const double & m2, const double & m3, const double & m4){momentum={m1,m2,m3,m4};};

            void set_color_flow(const  std::array<double, 2> & color_flow0){color_flow=color_flow0;};

            void set_color_flow(const double & c1,const double & c2){color_flow={c1,c2};};


        particle& operator=(const particle& other){
            if (this != &other) { // Check for self-assignment
                PDG_ID = other.PDG_ID;
                status = other.status;
                parents = other.parents;
                color_flow = other.color_flow;
                momentum = other.momentum;
                mass = other.mass;
                distance = other.distance;
                helicity = other.helicity;
            }
            return *this;
        };

             
    };
    
    std::ostream &operator<<(std::ostream &s, const particle & p) {
        s   << std::setw(7) <<p.PDG_ID;
        s   << ' ' << std::setw(2) << p.status;
        s   << ' ' <<  std::setw(4) <<  p.parents[0] << ' ' <<  std::setw(4) << p.parents[1];
        s   << ' ' <<  std::setw(3) <<  std::fixed << std::defaultfloat<<  p.color_flow[0] << ' '  <<  std::setw(3) << p.color_flow[1];
        s   << ' ' <<  std::setw(15)<< std::scientific << p.momentum[0] << ' '<<  std::setw(15) << p.momentum[1] << ' '<<  std::setw(15) << p.momentum[2] << ' ' <<  std::setw(15)<< p.momentum[3];
        s   << ' ' << std::setw(15) <<  p.mass;
        s   << ' ' <<  std::setw(15) <<  p.distance;
        s   << ' ' <<  std::setw(15) << p.helicity;
        s   << std::endl;
        return s;
    };

//}
#endif