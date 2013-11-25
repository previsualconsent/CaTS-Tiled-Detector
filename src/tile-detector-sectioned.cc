#include "tile-detector-sectioned.h"
#include <fstream>
#include <iostream>

TileDetectorSectioned::TileDetectorSectioned() : TileDetector()
{

}

TileDetectorSectioned::TileDetectorSectioned(std::string detector_name, std::string filename) : TileDetector(detector_name, filename)
{

   std::ifstream infile;
   std::string line;

   infile.open(m_filename.c_str());
   if (infile.is_open()) {
      getline(infile, line);
      m_n_cells=atof(line.c_str());

      getline(infile, line);
      m_cellsize=atof(line.c_str());

      getline(infile, line);
      m_cellthick=atof(line.c_str());

      getline(infile, line);
      m_nlayers1=atof(line.c_str());

      getline(infile, line);
      m_absthick1=atof(line.c_str());

      getline(infile, line);
      m_nlayers2=atof(line.c_str());

      getline(infile, line);
      m_absthick2=atof(line.c_str());

      getline(infile, line);
      m_nlayers3=atof(line.c_str());

      getline(infile, line);
      m_absthick3=atof(line.c_str());

      m_layertotal1 = m_absthick1 + m_cellthick;
      m_layertotal2 = m_absthick2 + m_cellthick;
      m_layertotal3 = m_absthick3 + m_cellthick;

      m_sect_total1 = m_layertotal1*m_nlayers1;
      m_sect_total2 = m_layertotal2*m_nlayers2;
      m_sect_total3 = m_layertotal3*m_nlayers3;

      m_total_thick = m_sect_total1 + m_sect_total2 + m_sect_total3;
      m_total_width = m_n_cells*m_cellsize;

      infile.close();
      std::cout << "m_layertotal1: " << m_layertotal1 << std::endl;
      std::cout << "m_layertotal2: " << m_layertotal2 << std::endl;
      std::cout << "m_layertotal3: " << m_layertotal3 << std::endl;

      std::cout << "m_sect_total1: " << m_sect_total1 << std::endl;
      std::cout << "m_sect_total2: " << m_sect_total2 << std::endl;
      std::cout << "m_sect_total3: " << m_sect_total3 << std::endl;

      std::cout << "m_total_thick: " << m_total_thick << std::endl;
      std::cout << "m_total_width: " << m_total_width << std::endl;


   } else {
      std::cout << "couldn't Open dimension file! Exiting" << std::endl;
      exit(1);
   }
}
TileDetectorSectioned::~TileDetectorSectioned()
{
}

G4ThreeVector TileDetectorSectioned::find_index(G4ThreeVector pos) 
{
   pos += G4ThreeVector(0,0,m_total_thick/2.0);

   int z = 0;
   if(pos.z() < m_sect_total1)
   {
      z = pos.z()/m_layertotal1;
   }
   else if(m_sect_total1 <= pos.z() && pos.z() < m_sect_total1 + m_sect_total2)
   {
      z = m_nlayers1+(pos.z()-m_sect_total1)/m_layertotal2;
   }
   else if(m_sect_total1 + m_sect_total2 <= pos.z() && pos.z() < m_sect_total1 + m_sect_total2 + m_sect_total3)
   {
      z = m_nlayers1+m_nlayers2+(pos.z()-m_sect_total1-m_sect_total2)/m_layertotal3;
   }
   else
      std::cout << "not on the detector: " << pos.z() << std::endl;

   return G4ThreeVector(
         int(pos.x()/m_cellsize),
         int(pos.y()/m_cellsize),
         z);
}

plot_limits TileDetectorSectioned::get_limits(std::string dim)
{
   plot_limits ret;
   if(dim=="x")
   {
      ret.n_bins=m_n_cells;
      ret.low  = (1-m_n_cells)/2;
      ret.high = (m_n_cells-1)/2;
   }
   else if (dim == "z")
   {
      ret.n_bins = m_nlayers1+m_nlayers2+m_nlayers3;
      ret.low = 0;
      ret.high = ret.n_bins;
   }
   else if (dim == "r")
   {
      ret.n_bins = 100;
      ret.low = 0;
      ret.high = std::sqrt((2*(m_n_cells-1)*(m_n_cells-1)*m_cellsize*m_cellsize)/8.0);
   }
   else if (dim =="energy")
   {
      ret.n_bins = 100;
      ret.low = 4*m_current_energy;
      ret.high = 8*m_current_energy;
   }
   return ret;
}


