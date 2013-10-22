#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "tile-hist-x.h"
#include "tile-hist-z.h"
#include "tile-hist-ring.h"

#include "tile-detector.h"

TileDetector::TileDetector(std::string detector_name, std::string filename)
{

   m_detector_name = detector_name;
   m_nevents = 0;

   std::ifstream infile;
   std::string line;

   infile.open(filename.c_str());
   if (infile.is_open()) {
      getline(infile, line);
      m_num=atof(line.c_str());

      getline(infile, line);
      m_cellsize=atof(line.c_str());

      getline(infile, line);
      m_nlayers=atof(line.c_str());

      getline(infile, line);
      m_absthick=atof(line.c_str());

      getline(infile, line);
      m_szthick=atof(line.c_str());

      m_layertotal = m_absthick + m_szthick;

      infile.close();

      // Now print out what we got:

      std::cout << "Dimensions are" << std::endl;
      std::cout << "num: "          << m_num << std::endl;
      std::cout << "cellsize: "     << m_cellsize << std::endl;
      std::cout << "nlayers: "      << m_nlayers << std::endl;
      std::cout << "absthick: "     << m_absthick << std::endl;
      std::cout << "szthick: "      << m_szthick << std::endl;
      std::cout << "layertotal: "   << m_layertotal << std::endl;
      std::cout << std::endl;

   } else {
      std::cout << "couldn't Open dimension file! Exiting" << std::endl;
      exit(1);
   }

}

void TileDetector::setup_plots(double Ein)
{
   m_plots.clear();

   float x_lim = m_num-1;
   //float y_lim = m_num-1;
   float z_lim = m_nlayers;
   float ring_lim = (m_num+1)/2;
   int n_x = m_num;
   //int n_y = m_num;
   int n_z = m_nlayers;
   int n_rings = ring_lim;

   char histoname[50];
   char histotitle[100];

   sprintf(histoname, "EdepX%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in X , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistX(m_detector_name,histoname, histotitle, n_x, x_lim));
   //
   sprintf(histoname, "EdepZ%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in Z , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistZ(m_detector_name,histoname, histotitle, n_z, z_lim));
   //
   sprintf(histoname, "PercRing%.2fGeV", Ein);
   sprintf(histotitle, "Percent Per Ring , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistRing(m_detector_name,histoname, histotitle, n_rings, ring_lim));

}

void TileDetector::fill_plots_xyz(G4ThreeVector pos,float edep)
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->fill_xyz(find_index(pos),edep);
   }
}
G4ThreeVector TileDetector::find_index(G4ThreeVector pos) 
{
   return G4ThreeVector(
         int(pos.x()/m_cellsize + m_num/2.0),
         int(pos.y()/m_cellsize + m_num/2.0),
         int(pos.z()/m_layertotal + m_nlayers/2.0));
}


void TileDetector::end_event()
{
   m_nevents++;
}

void TileDetector::write_plots()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      std::cout << "Writing Plots\n";
      (*hist)->normalize(m_nevents);
      (*hist)->Write();
      (*hist)->save_plot();
   }

}

void TileDetector::clear_plots()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      delete (*hist);
   }

}
