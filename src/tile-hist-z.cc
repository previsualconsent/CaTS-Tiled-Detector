#include "tile-hist-z.h"
#include "TCanvas.h"

TileHistZ::TileHistZ()
{

}

TileHistZ::TileHistZ(TileDetector * detector, std::string name, std::string title) : TileHist(detector, name.c_str(), title.c_str(),"z") 
{
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
