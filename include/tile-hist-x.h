#ifndef tile_hist_x_h
#define tile_hist_x_h

#include <string>
#include "tile-hist.h"

class TileHistX : public TileHist
{
   public:
      TileHistX();
      TileHistX(TileDetector * detector, std::string name, std::string title);
      virtual ~TileHistX();
      void fill_xyz(G4ThreeVector pos, float edep);

   private:
};

#endif // #ifndef tile_hist_x_h
