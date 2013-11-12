#ifndef tile_hist_h
#define tile_hist_h

#include <string>
#include <map>
#include "TH1F.h"
#include "G4ThreeVector.hh"

#include "tile-detector.h"

typedef std::map<std::string,float> data_map;

class TileHist : public TH1F
{
   public:
      TileHist();
      TileHist(TileDetector * detector, std::string name, std::string title, std::string dim);
      virtual ~TileHist();

      virtual void save_plot();
      virtual void normalize(int nevents);
      virtual void fill_xyz(G4ThreeVector pos, float edep)=0;
      virtual void end_event() {return;}

      virtual bool can_get_data(){ return false;}

      std::string get_detector_name(){return m_detector_name;}
      virtual data_map get_data();

   protected:
      std::string m_detector_name;
      bool m_norm;
};

#endif // #ifndef tile_hist_h
