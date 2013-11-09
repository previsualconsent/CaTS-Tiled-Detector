#ifndef tile_hist_energy_h
#define tile_hist_energy_h

#include <string>
#include "tile-hist.h"

class TileHistEnergy : public TileHist
{
   public:
      TileHistEnergy();
      TileHistEnergy(std::string tag, std::string name, std::string title, int n_bins, float low_e, float high_e);
      virtual ~TileHistEnergy();
      void fill_xyz(G4ThreeVector pos, float edep);

      //over-written functins
      void normalize(int nevents);
      void end_event();
      bool can_get_data(){return true;}
      data_map get_data();

   private:
      float m_low_e;
      float m_high_e;
      float m_total_energy;
      int m_n_bins;

      data_map m_data;
      bool m_calculated;

      void calc_data();
};

#endif // #ifndef tile_hist_ring_h
