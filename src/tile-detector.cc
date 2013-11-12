#include "tile-detector.h"

TileDetector::TileDetector()
{

}

TileDetector::TileDetector(std::string detector_name, std::string filename)
{
   m_detector_name = detector_name;
   m_filename = filename;
   m_n_cells = 0;
}

TileDetector::~TileDetector()
{
}

