#ifndef tile_hist_z_h
#define tile_hist_z_h

#include <string>
#include "tile-hist.h"

class TileHistZ : public TileHist
{
   public:
      TileHistZ();
      TileHistZ(TileDetector * detector, std::string name, std::string title);
      virtual ~TileHistZ();
      void fill_xyz(G4ThreeVector pos, float edep);

   private:
};

#endif // #ifndef tile_hist_z_h
