#include "tile-hist-x-zsplit.h"
#include "TCanvas.h"

TileHistXZSplit::TileHistXZSplit()
{

}

TileHistXZSplit::TileHistXZSplit(std::string tag, std::string name, std::string title, int num, float x_lim,int lowz, int highz) : TileHist(tag,name.c_str(), title.c_str(),num,0,x_lim) 
{
   m_x_lim = x_lim;
   m_num = num;
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
