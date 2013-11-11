#ifndef tile_detector_h
#define tile_detector_h

#include "G4ThreeVector.hh"

#include <string>

struct plot_limits
{
   int n_bins;
   float low;
   float high;
};

class TileDetector 
{
   public:
      TileDetector();
      TileDetector(std::string detector_name, std::string filename);
      virtual ~TileDetector();

      virtual G4ThreeVector find_index(G4ThreeVector pos)=0;
      virtual plot_limits get_limits(std::string dim)=0;

   protected:
      std::string m_detector_name;
      std::string m_filename;

   private:

};

#endif // #ifndef tile_detector_h
