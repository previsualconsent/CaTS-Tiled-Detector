#ifndef energy_graph_res_h
#define energy_graph_res_h

#include "energy-graph.h"
#include <string>
#include <vector>

class EnergyGraphRes : public EnergyGraph
{
   public:
      EnergyGraphRes(std::string name);
      virtual ~EnergyGraphRes();

      virtual void fill_xyz(G4ThreeVector pos, float edep);
      virtual void end_event();
      virtual void end_energy();

   protected:

   private:
      float m_energy_of_event;
      std::vector<float> m_histv;
};

#endif // #ifndef energy_graph_res_h
