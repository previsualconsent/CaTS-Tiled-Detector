#ifndef energy_graph_z_template_h
#define energy_graph_z_template_h

#include "energy-graph.h"
#include <string>
#include <vector>

#include "tile-hist-z.h"
#include "tile-detector.h"

class EnergyGraphZTemplate : public EnergyGraph
{
   public:
      EnergyGraphZTemplate(TileDetector * detector);
      virtual ~EnergyGraphZTemplate();

      virtual void fill_xyz(G4ThreeVector index, float edep);
      virtual void end_event();
      virtual void start_energy(float Ein);
      virtual void end_energy();

      void add_zhist();
      class Fit 
      {
         public:
            TileHistZ * hist;
            double operator()(double *x, double *par);
      };
   protected:

   private:
      void new_zhist();

      std::vector<TileHistZ*> m_zhistv;
      TileHistZ * m_zhist_temp;
      TileDetector * m_detector;
      int m_event_n;
};

#endif // #ifndef energy_graph_z_template_h
