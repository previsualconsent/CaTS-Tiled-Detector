#ifndef tile_hist_ring_h
#define tile_hist_ring_h

#include <string>
#include "tile-hist.h"

class TileHistRing : public TileHist
{
   public:
      TileHistRing();
      TileHistRing(TileDetector * detector, std::string name, std::string title);
      virtual ~TileHistRing();
      void fill_xyz(G4ThreeVector pos, float edep);

      //over-written functins
      void normalize(int nevents);
      void save_plot();

   private:
      int m_ring_lim;
      int m_n_cells;
      float m_total_energy;
};

#endif // #ifndef tile_hist_ring_h
