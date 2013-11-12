#ifndef tile_hist_energy_h
#define tile_hist_energy_h

#include <string>
#include "tile-hist.h"

class TileHistEnergy : public TileHist
{
   public:
      TileHistEnergy();
      TileHistEnergy(TileDetector * detector, std::string name, std::string title);
      virtual ~TileHistEnergy();
      void fill_xyz(G4ThreeVector pos, float edep);

      //over-written functins
      void normalize(int nevents);
      void end_event();
      bool can_get_data(){return true;}
      data_map get_data();

   private:
      float m_total_energy;

      data_map m_data;
      bool m_calculated;

      void calc_data();
};

#endif // #ifndef tile_hist_ring_h
