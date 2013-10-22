#include <string>
#include <vector>
#include "G4ThreeVector.hh"
#include "tile-hist.h"

class TileDetector
{
   public:
      TileDetector(std::string detector_name, std::string filename);
      G4ThreeVector find_index(G4ThreeVector pos);
      void setup_plots(double Ein);
      void fill_plots_xyz(G4ThreeVector pos,float edep);
      void end_event();
      void write_plots();
      void clear_plots();

   public:
      float m_num;
      float m_cellsize;
      float m_nlayers;
      float m_absthick;
      float m_szthick;
      float m_layertotal;
      float m_nevents;
      std::string m_detector_name;

      typedef std::vector<TileHist*> v_plots;
      v_plots m_plots;
};
