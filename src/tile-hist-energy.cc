#include "tile-hist-energy.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TPaveLabel.h"
#include <iostream>

TileHistEnergy::TileHistEnergy()
{

}

TileHistEnergy::TileHistEnergy(std::string tag, std::string name, std::string title, int n_bins, float low_e, float high_e) : TileHist(tag,name.c_str(), title.c_str(),n_bins,low_e,high_e) 
{
   m_low_e = low_e;
   m_high_e = high_e;
   m_total_energy = 0;
   m_n_bins = n_bins;

   GetXaxis()->SetTitle("Energy (MeV)");
   GetYaxis()->SetTitle("NEvents");
}

TileHistEnergy::~TileHistEnergy()
{
}


void TileHistEnergy::fill_xyz(G4ThreeVector pos, float edep)
{
   m_total_energy += edep;
}

void TileHistEnergy::normalize(int nevents) {
   //if(!m_norm) Scale(1.0/Integral());
   m_norm = true;
}

void TileHistEnergy::end_event() 
{

   Fill(m_total_energy);
   m_total_energy=0;
}


void TileHistEnergy::calc_data()
{
   if(!m_calculated)
   {
      m_data["RMS"]=GetRMS();
   }
   m_calculated=true;
}
data_map TileHistEnergy::get_data()
{
   calc_data();
   return m_data;
}
