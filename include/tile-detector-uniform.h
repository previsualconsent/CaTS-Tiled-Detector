#ifndef tile_detector_uniform_h
#define tile_detector_uniform_h

#include "tile-detector.h"

class TileDetectorUniform : public TileDetector
{
   public:
      TileDetectorUniform();
      TileDetectorUniform(std::string detector_name, std::string filename);
      virtual ~TileDetectorUniform();

      G4ThreeVector find_index(G4ThreeVector pos);

      plot_limits get_limits(std::string dim);

   protected:

   private:
      float m_cellsize;
      float m_nlayers;
      float m_absthick;
      float m_szthick;
      float m_layertotal;
      float m_energy_low;
      float m_energy_high;
};

#endif // #ifndef tile_detector_uniform_h
