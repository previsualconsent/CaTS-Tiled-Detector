#ifndef energy_graph_h
#define energy_graph_h

#include <vector>
#include "G4ThreeVector.hh"
#include "TGraph.h"
#include <string>

class EnergyGraph 
{
   public:
      EnergyGraph();
      EnergyGraph(std::string title,std::string name, std::string ytitle);
      virtual ~EnergyGraph();

      void write();

      virtual void start_energy(float Ein);
      virtual void end_event()=0;
      virtual void end_energy()=0;
      virtual void fill_xyz(G4ThreeVector pos, float edep)=0;

   protected:
      std::vector<float> m_energies;
      std::vector<float> m_data;
      std::string m_title;
      std::string m_name;
      std::string m_ytitle;

   private:
      TGraph * m_graph;
};

#endif // #ifndef energy_graph_h
