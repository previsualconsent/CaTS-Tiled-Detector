#include "tile-hist-z.h"
#include "TCanvas.h"

TileHistZ::TileHistZ()
{

}

TileHistZ::TileHistZ(std::string tag, std::string name, std::string title, int num, float z_lim) : TileHist(tag, name.c_str(), title.c_str(),num,0,z_lim) 
{
   m_z_lim = z_lim;
   m_num = num;
   GetXaxis()->SetTitle("Z pos (layer)");
   GetYaxis()->SetTitle("Energy (GeV)");

}

TileHistZ::~TileHistZ()
{
}


void TileHistZ::fill_xyz(G4ThreeVector pos, float edep)
{
   Fill(pos.z(),edep);
}
