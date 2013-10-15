#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "tile-detector.h"

TileDetector::TileDetector(std::string filename)
{
   std::ifstream infile;
   std::string line;

   infile.open(filename.c_str());
   if (infile.is_open()) {
      getline(infile, line);
      m_num=atof(line.c_str());

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
      std::cout << "num: "          << m_num << std::endl;
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

G4ThreeVector TileDetector::find_index(G4ThreeVector pos) 
{
   return G4ThreeVector(
         int(pos.x()/m_cellsize + m_num/2.0),
         int(pos.y()/m_cellsize + m_num/2.0),
         int(pos.z()/m_layertotal + m_nlayers/2.0));
}

int TileDetector::find_ring(G4ThreeVector pos,bool to_index) {
    if(to_index) pos = find_index(pos);
    int center = (m_num-1)/2;
    return int(std::max(abs(pos.x()-center),abs(pos.y()-center)));
}
