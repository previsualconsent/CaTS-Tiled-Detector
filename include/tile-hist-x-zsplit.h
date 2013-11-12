#ifndef tile_hist_x_zsplit_h
#define tile_hist_x_zsplit_h

#include <string>
#include "tile-hist.h"

class TileHistXZSplit : public TileHist
{
   public:
      TileHistXZSplit();
      TileHistXZSplit(TileDetector * detector, std::string name, std::string title,int lowz, int highz);
      virtual ~TileHistXZSplit();
      void fill_xyz(G4ThreeVector pos, float edep);

   private:
      int m_lowz;
      int m_highz;
};

#endif // #ifndef tile_hist_x_zsplit_h
