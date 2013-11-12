#include "tile-hist-x.h"
#include "TCanvas.h"

TileHistX::TileHistX()
{

}

TileHistX::TileHistX(TileDetector * detector, std::string name, std::string title) : TileHist(detector,name.c_str(), title.c_str(),"x") 
{
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
