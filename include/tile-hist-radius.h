#ifndef tile_hist_radius_h
#define tile_hist_radius_h

#include <string>
#include "tile-hist.h"

class TileHistRadius : public TileHist
{
   public:
      TileHistRadius();
      TileHistRadius(TileDetector * detector, std::string name, std::string title);
      virtual ~TileHistRadius();
      void fill_xyz(G4ThreeVector pos, float edep);

      //over-written functins
      void normalize(int nevents);
      void save_plot();
      bool can_get_data(){return true;}
      data_map get_data();

   private:
      float m_r_lim;
      float m_n_cells;
      float m_total_energy;
      int m_num;

      data_map m_data;
      bool m_calculated;

      void calc_data();
};

#endif // #ifndef tile_hist_ring_h
