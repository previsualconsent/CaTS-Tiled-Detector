#include <string>
#include <vector>
#include "G4ThreeVector.hh"
#include "tile-hist.h"
#include "tile-detector.h"

class TileDetectorPlots
{
   public:
      TileDetectorPlots(TileDetector * detector);
      void setup_plots(double Ein);
      void fill_plots_xyz(G4ThreeVector pos,float edep);
      void end_event();
      void write_plots();
      void clear_plots();

      void write_graphs();

   protected:

      template <typename T, typename U>
         void write_graph(std::vector<T> vec1, std::vector<U> vec2, std::string graphname, std::string graphtitle,std::string xtitle,std::string ytitle);
      float m_nevents;

      TileDetector * m_detector;

      typedef std::vector<TileHist*> v_plots;
      v_plots m_plots;
      typedef std::vector<TH1F*> v_hists;
      v_hists m_hists;

      std::vector<float> m_energies;
      std::vector<float> m_res;
      std::vector<float> m_moliere;
};
