#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "tile-hist-x.h"
#include "tile-hist-x-zsplit.h"
#include "tile-hist-z.h"
#include "tile-hist-radius.h"
#include "tile-hist-energy.h"

#include "tile-detector-plots.h"

TileDetectorPlots::TileDetectorPlots(std::string detector_name, std::string filename)
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

void TileDetectorPlots::setup_plots(double Ein)
{
   m_plots.clear();

   float x_lim = m_num-1;
   //float y_lim = m_num-1;
   float z_lim = m_nlayers;
   //float ring_lim = (m_num+1)/2;
   float r_lim = std::sqrt((2*x_lim*x_lim*m_cellsize*m_cellsize)/8.0);
   int n_x = m_num;
   //int n_y = m_num;
   int n_z = m_nlayers;
   //int n_rings = ring_lim;
   int n_r = 100;

   char histoname[50];
   char histotitle[100];

   sprintf(histoname, "EdepX_%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in X , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistX(m_detector_name,histoname, histotitle, n_x, x_lim));
   //
   sprintf(histoname, "EdepZ_%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in Z , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistZ(m_detector_name,histoname, histotitle, n_z, z_lim));
   //
   sprintf(histoname, "PercRadius%.2fGeV", Ein);
   sprintf(histotitle, "Percent inside Radius , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistRadius(m_detector_name,histoname, histotitle, n_r, r_lim,m_num));

   sprintf(histoname, "XProf_Z0-5_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 0-5 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector_name,histoname, histotitle, n_x, x_lim,0,5));

   sprintf(histoname, "XProf_Z5-15_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 5-15 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector_name,histoname, histotitle, n_x, x_lim,5,15));

   sprintf(histoname, "XProf_Z15-25_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 15-25 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector_name,histoname, histotitle, n_x, x_lim,15,25));

   sprintf(histoname, "Energy_%.2fGeV", Ein);
   sprintf(histotitle, "Energy Response, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistEnergy(m_detector_name,histoname, histotitle, 100, 0,5000));

}

void TileDetectorPlots::fill_plots_xyz(G4ThreeVector pos,float edep)
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->fill_xyz(find_index(pos),edep);
   }
}
G4ThreeVector TileDetectorPlots::find_index(G4ThreeVector pos) 
{
   return G4ThreeVector(
         int(pos.x()/m_cellsize + m_num/2.0),
         int(pos.y()/m_cellsize + m_num/2.0),
         int(pos.z()/m_layertotal + m_nlayers/2.0));
}


void TileDetectorPlots::end_event()
{
   m_nevents++;
}

void TileDetectorPlots::write_plots()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      std::cout << "Writing Plots\n";
      (*hist)->normalize(m_nevents);
      (*hist)->Write();
      (*hist)->save_plot();
   }

}

void TileDetectorPlots::clear_plots()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      delete (*hist);
   }

}
