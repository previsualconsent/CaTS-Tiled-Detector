#ifndef tile_hist_h
#define tile_hist_h

#include <string>
#include "TH1F.h"

class TileHist : public TH1F
{
   public:
      TileHist();
      TileHist(std::string name, std::string title, int n, int low, int high);
      void save_plot();
      virtual ~TileHist();

   private:
};

#endif // #ifndef tile_hist_h
