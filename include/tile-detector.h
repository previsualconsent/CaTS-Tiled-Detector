#include <string>
#include <vector>
#include "G4ThreeVector.hh"
#include "tile-hist-x.h"
#include "tile-hist-z.h"

class TileDetector
{
   public:
      TileDetector(std::string detector_name, std::string filename);
      G4ThreeVector find_index(G4ThreeVector pos);

/* |--------------|
 * |00 01 02 03 04|
 * |  |--------|  |
 * |10|11 12 13|14|
 * |  |  |--|  |  |
 * |20|21|22|23|24|
 * |  |  |--|  |  |
 * |30|31 32 33|34|
 * |  |--------|  |
 * |40 41 42 43 44|
 * |--------------|
 *
 * ring 0 - 22
 * ring 1 - 11,12,13,21,23,31,32,33
 * ring 2 - 00,01,02,03,04,10,14,20,24,30,34,40,41,42,43,44
 * etc..
 */
      int find_ring(G4ThreeVector pos,bool to_index);
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
