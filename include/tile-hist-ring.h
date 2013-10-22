#ifndef tile_hist_ring_h
#define tile_hist_ring_h

#include <string>
#include "tile-hist.h"

class TileHistRing : public TileHist
{
   public:
      TileHistRing();
      TileHistRing(std::string tag, std::string name, std::string title, int num, float ring_lim);
      virtual ~TileHistRing();
      void fill_xyz(G4ThreeVector pos, float edep);

      //over-written functins
      void normalize(int nevents);
      void save_plot();

   private:
      int m_ring_lim;
      int m_num;
      float m_total_energy;
};

#endif // #ifndef tile_hist_ring_h
