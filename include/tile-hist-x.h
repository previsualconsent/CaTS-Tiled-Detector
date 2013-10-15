#ifndef tile_hist_x_h
#define tile_hist_x_h

#include <string>
#include "tile-hist.h"

class TileHistX : public TileHist
{
   public:
      TileHistX();
      TileHistX(std::string tag, std::string name, std::string title, int num, float x_lim);
      virtual ~TileHistX();
      void fill_xyz(G4ThreeVector pos, float edep);

   private:
      int m_x_lim;
      int m_num;
};

#endif // #ifndef tile_hist_x_h
