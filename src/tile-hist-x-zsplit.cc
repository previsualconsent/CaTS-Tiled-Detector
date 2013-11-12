#include "tile-hist-x-zsplit.h"
#include "TCanvas.h"

TileHistXZSplit::TileHistXZSplit()
{

}

TileHistXZSplit::TileHistXZSplit(TileDetector * detector, std::string name, std::string title, int lowz, int highz) : TileHist(detector,name.c_str(), title.c_str(),"x") 
{
   m_lowz = lowz;
   m_highz = highz;

   GetXaxis()->SetTitle("X pos (cell)");
   GetYaxis()->SetTitle("Energy (GeV)");
}

TileHistXZSplit::~TileHistXZSplit()
{
}


void TileHistXZSplit::fill_xyz(G4ThreeVector pos, float edep)
{
   if(pos.z()<m_highz && pos.z()>= m_lowz)
      Fill(pos.x(),edep);
}
