#ifndef tile_hist_z_h
#define tile_hist_z_h

#include <string>
#include "tile-hist.h"

class TileHistZ : public TileHist
{
   public:
      TileHistZ();
      TileHistZ(std::string tag, std::string name, std::string title, int num, float z_lim);
      virtual ~TileHistZ();
      void fill_xyz(G4ThreeVector pos, float edep);

   private:
      int m_z_lim;
      int m_num;
};

#endif // #ifndef tile_hist_z_h
