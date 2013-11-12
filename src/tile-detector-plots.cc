#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "tile-hist-x.h"
#include "tile-hist-x-zsplit.h"
#include "tile-hist-z.h"
#include "tile-hist-radius.h"
#include "tile-hist-energy.h"

#include "tile-detector-plots.h"

TileDetectorPlots::TileDetectorPlots(TileDetector * detector)
{

   m_detector = detector;
   m_nevents = 0;
}

void TileDetectorPlots::setup_plots(double Ein)
{
   m_plots.clear();

   char histoname[50];
   char histotitle[100];

   sprintf(histoname, "EdepX_%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in X , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistX(m_detector,histoname, histotitle));
   //
   sprintf(histoname, "EdepZ_%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in Z , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistZ(m_detector,histoname, histotitle));
   //
   sprintf(histoname, "PercRadius%.2fGeV", Ein);
   sprintf(histotitle, "Percent inside Radius , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistRadius(m_detector,histoname, histotitle));

   sprintf(histoname, "XProf_Z0-5_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 0-5 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector,histoname, histotitle,0,5));

   sprintf(histoname, "XProf_Z5-15_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 5-15 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector,histoname, histotitle,5,15));

   sprintf(histoname, "XProf_Z15-25_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 15-25 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector,histoname, histotitle,15,25));

   sprintf(histoname, "Energy_%.2fGeV", Ein);
   sprintf(histotitle, "Energy Response, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistEnergy(m_detector,histoname, histotitle));

}

void TileDetectorPlots::fill_plots_xyz(G4ThreeVector pos,float edep)
{
   G4ThreeVector index = m_detector->find_index(pos);
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->fill_xyz(index,edep);
   }
}

void TileDetectorPlots::end_event()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->end_event();
   }
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
