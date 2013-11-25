#include "tile-detector-uniform.h"
#include <fstream>
#include <iostream>

TileDetectorUniform::TileDetectorUniform() : TileDetector()
{

}

TileDetectorUniform::TileDetectorUniform(std::string detector_name, std::string filename) : TileDetector(detector_name, filename)
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
      m_nlayers=atof(line.c_str());

      getline(infile, line);
      m_absthick=atof(line.c_str());

      getline(infile, line);
      m_szthick=atof(line.c_str());

      m_layertotal = m_absthick + m_szthick;

      infile.close();

      // Now print out what we got:

      std::cout << "Dimensions are" << std::endl;
      std::cout << "num: "          << m_n_cells << std::endl;
      std::cout << "cellsize: "     << m_cellsize << std::endl;
      std::cout << "nlayers: "      << m_nlayers << std::endl;
      std::cout << "absthick: "     << m_absthick << std::endl;
      std::cout << "szthick: "      << m_szthick << std::endl;
      std::cout << "layertotal: "   << m_layertotal << std::endl;
      std::cout << std::endl;

   } else {
      std::cout << "couldn't Open dimension file! Exiting" << std::endl;
      exit(1);
   }
}
TileDetectorUniform::~TileDetectorUniform()
{
}

G4ThreeVector TileDetectorUniform::find_index(G4ThreeVector pos) 
{
   return G4ThreeVector(
         int(pos.x()/m_cellsize + m_n_cells/2.0),
         int(pos.y()/m_cellsize + m_n_cells/2.0),
         int(pos.z()/m_layertotal + m_nlayers/2.0));
}

plot_limits TileDetectorUniform::get_limits(std::string dim)
{
   plot_limits ret;
   if(dim=="x")
   {
      ret.n_bins=m_n_cells;
      ret.low  = 0;
      ret.high = m_n_cells-1;
   }
   else if (dim == "z")
   {
      ret.n_bins = m_nlayers;
      ret.low = 0;
      ret.high = m_nlayers-1;
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
      ret.low = 3*m_current_energy;
      ret.high = 8*m_current_energy;
   }
   return ret;
}


