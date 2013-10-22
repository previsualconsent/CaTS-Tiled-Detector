#include "tile-hist-x.h"
#include "TCanvas.h"

TileHistX::TileHistX()
{

}

TileHistX::TileHistX(std::string tag, std::string name, std::string title, int num, float x_lim) : TileHist(tag,name.c_str(), title.c_str(),num,0,x_lim) 
{
   m_x_lim = x_lim;
   m_num = num;

   GetXaxis()->SetTitle("X pos (cell)");
   GetYaxis()->SetTitle("Energy (GeV)");
}

TileHistX::~TileHistX()
{
}


void TileHistX::fill_xyz(G4ThreeVector pos, float edep)
{
   Fill(pos.x(),edep);
}
