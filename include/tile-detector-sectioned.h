#ifndef tile_detector_sectioned_h
#define tile_detector_sectioned_h

#include "tile-detector.h"

class TileDetectorSectioned : public TileDetector
{
   public:
      TileDetectorSectioned();
      TileDetectorSectioned(std::string detector_name, std::string filename);
      virtual ~TileDetectorSectioned();

      G4ThreeVector find_index(G4ThreeVector pos);

      plot_limits get_limits(std::string dim);

   protected:

   private:
      float m_cellsize;
      float m_cellthick;
      float m_nlayers1;
      float m_absthick1;
      float m_nlayers2;
      float m_absthick2;
      float m_nlayers3;
      float m_absthick3;
      float m_layertotal1;
      float m_layertotal2;
      float m_layertotal3;
      float m_sect_total1;
      float m_sect_total2;
      float m_sect_total3;

      float m_total_thick;
      float m_total_width;
};

#endif // #ifndef tile_detector_sectioned_h
