#ifndef LHE_H
#define LHE_H

#include <iostream>
#include <array>
#include <vector>

#include "event.h"



struct lhe{
    std::vector<event> events;
    int beam_id1;
    int beam_id2;
    double beam_energy1;
    double beam_energy2;
    double pdfg1;
    double pdfg2;
    double pdfs1;
    double pdfs2;
    int id_weight;
    int nproc;
    double xsection;
    double xsection_unc;
    double xsection_max;
    double pid;

        lhe(){        
        beam_id1        = 0;
        beam_id2        = 0;
        beam_energy1    = 0;
        beam_energy2    = 0;
        pdfg1           = 0;
        pdfg2           = 0;
        pdfs1           = 0;
        pdfs2           = 0;
        id_weight       = 0;
        nproc           = 0;
        xsection        = 0;
        xsection_unc    = 0;
        xsection_max    = 0;
        pid             = 0;
    }

    lhe(std::vector<event> const & events1, int const & beam_id11, int const & beam_id21, double const beam_energy11, double const beam_energy21,
        double const & pdfg11, double const & pdfg21, double const & pdfs11, double const & pdfs21,
        int const & id_weight1, int const & nproc1, double const & xsection1, double const & xsection_unc1,
        double const & xsection_max1, double const & pid1){
        events          = events1;
        beam_id1        = beam_id11;
        beam_id2        = beam_id21;
        beam_energy1    = beam_energy11;
        beam_energy2    = beam_energy21;
        pdfg1           = pdfg11;
        pdfg2           = pdfg21;
        pdfs1           = pdfs11;
        pdfs2           = pdfs21;
        id_weight       = id_weight1;
        nproc           = nproc1;
        xsection        = xsection1;
        xsection_unc    = xsection_unc1;
        xsection_max    = xsection_max1;
        pid             = pid1;
    }

    lhe(int const & beam_id11, int const & beam_id21, double const beam_energy11, double const beam_energy21,
         double const & xsection1, double const & xsection_unc1){
        
        beam_id1        = beam_id11;
        beam_id2        = beam_id21;
        beam_energy1    = beam_energy11;
        beam_energy2    = beam_energy21;
        pdfg1           = 0;
        pdfg2           = 0;
        pdfs1           = 0;
        pdfs2           = 0;
        id_weight       = 0;
        nproc           = 0;
        xsection        = xsection1;
        xsection_unc    = xsection_unc1;
        xsection_max    = xsection1;
        pid             = 0;
    }

    void add_event(event const & e){
        events.push_back(e);
    }

    lhe& operator=(const lhe& other){
            events          = other.events;
            beam_id1        = other.beam_id1;
            beam_id2        = other.beam_id2;
            beam_energy1    = other.beam_energy1;
            beam_energy2    = other.beam_energy2;
            pdfg1           = other.pdfg1;
            pdfg2           = other.pdfg2;
            pdfs1           = other.pdfs1;
            pdfs2           = other.pdfs2;
            id_weight       = other.id_weight;
            nproc           = other.nproc;
            xsection        = other.xsection;
            xsection_unc    = other.xsection_unc;
            xsection_max    = other.xsection_max;
            pid             = other.pid;
        return *this;
    };


};

    std::ostream &operator<<(std::ostream &s, const lhe & l) {
        s   << "<LesHoucheEvents>"<< std::endl;

        s   << "<Header>"<< std::endl;
        s   << "</Header>"<< std::endl;

        s   << "<init>"<< std::endl;
        s   << std::setw(4) << l.beam_id1 << std::setw(4) << l.beam_id2;
        s   << std::setw(15) <<  std::scientific << l.beam_energy1 << std::setw(15) << l.beam_energy2;
        s   << std::setw(2) << std::defaultfloat << l.pdfg1 << std::setw(2) << l.pdfg2;
        s   << std::setw(2) << l.pdfs1 << std::setw(2) << l.pdfs2;
        s   << std::setw(3) << l.id_weight;
        s   << std::setw(2) << l.nproc;
        s   << std::endl;
        s   << std::setw(15) << std::scientific <<  l.xsection << std::setw(15) << l.xsection_unc << std::setw(15) << l.xsection_max;
        s   << std::setw(2)  << std::defaultfloat << l.pid << std::endl; 
        s   << "</init>"<< std::endl;
        
        for (const auto& l : l.events)
        {
            s  <<  l;
        }
        s   << "</LesHoucheEvents>";
        
        return s;
    };




#endif