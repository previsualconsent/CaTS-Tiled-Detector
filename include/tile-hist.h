#ifndef tile_hist_h
#define tile_hist_h

#include <string>
#include "TH1F.h"
#include "G4ThreeVector.hh"

class TileHist : public TH1F
{
   public:
      TileHist();
      TileHist(std::string tag, std::string name, std::string title, int n, int low, int high);
      virtual void save_plot();
      virtual void normalize(int nevents);
      virtual ~TileHist();

      virtual void fill_xyz(G4ThreeVector pos, float edep)=0;

   protected:
      std::string m_tag;
      bool m_norm;
};

#endif // #ifndef tile_hist_h
