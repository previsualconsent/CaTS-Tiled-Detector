#include "tile-hist-energy.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TPaveLabel.h"
#include <iostream>

TileHistEnergy::TileHistEnergy()
{

}

TileHistEnergy::TileHistEnergy(TileDetector * detector, std::string name, std::string title) : TileHist(detector,name.c_str(), title.c_str(),"energy") 
{
   m_total_energy = 0;

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
   if(!m_norm && Integral()!=0) Scale(1.0/Integral());
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
