#ifndef tile_hist_h
#define tile_hist_h

#include <string>
#include <map>
#include "TH1F.h"
#include "G4ThreeVector.hh"

typedef std::map<std::string,float> data_map;

class TileHist : public TH1F
{
   public:
      TileHist();
      TileHist(std::string tag, std::string name, std::string title, int n, int low, int high);
      virtual ~TileHist();

      virtual void save_plot();
      virtual void normalize(int nevents);
      virtual void fill_xyz(G4ThreeVector pos, float edep)=0;
      virtual void end_event() {return;}

      virtual bool can_get_data(){ return false;}

      std::string get_tag(){return m_tag;}
      virtual data_map get_data();

   protected:
      std::string m_tag;
      bool m_norm;
};

#endif // #ifndef tile_hist_h
