#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "tile-hist-x.h"
#include "tile-hist-x-zsplit.h"
#include "tile-hist-z.h"
#include "tile-hist-radius.h"
#include "tile-hist-energy.h"

#include "TGraph.h"
#include "TCanvas.h"

#include "tile-detector-plots.h"

TileDetectorPlots::TileDetectorPlots(TileDetector * detector)
{

   m_detector = detector;
   m_nevents = 0;
}

void TileDetectorPlots::setup_plots(double Ein)
{

   m_energies.push_back(Ein);
   m_res.push_back(0);
   m_moliere.push_back(0);
   m_detector->set_current_energy(Ein);
   m_plots.clear();

   char histoname[50];
   char histotitle[100];

   sprintf(histoname, "EdepX_%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in X , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistX(m_detector,histoname, histotitle));
   //
   sprintf(histoname, "EdepZ_%.2fGeV", Ein);
   sprintf(histotitle, "Ionization Energy in Z , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistZ(m_detector,histoname, histotitle));
   //
   sprintf(histoname, "PercRadius%.2fGeV", Ein);
   sprintf(histotitle, "Percent inside Radius , (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistRadius(m_detector,histoname, histotitle));

   sprintf(histoname, "XProf_Z0-5_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 0-5 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector,histoname, histotitle,0,5));

   sprintf(histoname, "XProf_Z5-15_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 5-15 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector,histoname, histotitle,5,15));

   sprintf(histoname, "XProf_Z15-25_%.2fGeV", Ein);
   sprintf(histotitle, "X Profile 15-25 Layers, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistXZSplit(m_detector,histoname, histotitle,15,25));

   sprintf(histoname, "Energy_%.2fGeV", Ein);
   sprintf(histotitle, "Energy Response, (Ein %.2f GeV)",Ein);
   m_plots.push_back(new TileHistEnergy(m_detector,histoname, histotitle));

}

void TileDetectorPlots::fill_plots_xyz(G4ThreeVector pos,float edep)
{
   G4ThreeVector index = m_detector->find_index(pos);
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->fill_xyz(index,edep);
   }
}

void TileDetectorPlots::end_event()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->end_event();
   }
   m_nevents++;
}

void TileDetectorPlots::write_plots()
{
   std::cout << "Writing Plots\n";
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      (*hist)->normalize(m_nevents);
      (*hist)->Write();
      (*hist)->save_plot();

      if((*hist)->can_get_data())
      {
         std::cout << "Getting data\n";
         data_map h_data = (*hist)->get_data();
         if ( h_data.find("Res") != h_data.end() )
         {
            std::cout << "Getting Res\n";
            if(m_res.back() == 0 )
               m_res.back() = h_data["Res"];
         }
         else if(h_data.find("r90%") != h_data.end() )
            if(m_moliere.back() == 0 )
               m_moliere.back() = h_data["r90%"];
      }
   }


}

void TileDetectorPlots::clear_plots()
{
   for(v_plots::iterator hist = m_plots.begin(); hist!=m_plots.end(); hist++)
   {
      delete (*hist);
   }

}

void TileDetectorPlots::write_graphs()
{
   std::string graphname,graphtitle,xtitle,ytitle;
   graphname = m_detector->get_name() + "_res";
   graphtitle = "Resolution vs Energy";
   xtitle = "Energy (GeV)";
   ytitle = "Res (#sigma/#mu)";
   write_graph(m_energies,m_res,graphname,graphtitle,xtitle,ytitle);

   graphname = m_detector->get_name() + "_moliere";
   graphtitle = "Moliere Radius vs Energy";
   xtitle = "Energy (GeV)";
   ytitle = "Moliere Radius (cells)";
   write_graph(m_energies,m_moliere,graphname,graphtitle,xtitle,ytitle);

   
   std::ofstream outstream;
   outstream.open("dataout.csv");

   for(std::vector<float>::iterator it = m_energies.begin();
         it != m_energies.end();
         it++)
      outstream << "," << *it;

   outstream << std::endl << "Resolution";
   for(std::vector<float>::iterator it = m_res.begin();
         it != m_res.end();
         it++)
      outstream << "," << *it;
   
      
   outstream << std::endl << "Moliere";
   for(std::vector<float>::iterator it = m_moliere.begin();
         it != m_moliere.end();
         it++)
      outstream << "," << *it;
}


template <typename T, typename U>
void TileDetectorPlots::write_graph(std::vector<T> vec1, std::vector<U> vec2, std::string graphname, std::string graphtitle,std::string xtitle,std::string ytitle) {

    TCanvas * c = new TCanvas(graphname.c_str(),graphtitle.c_str());
    TGraph* graph= new TGraph(vec1.size(),&(vec1.front()),&(vec2.front()));

    graph->SetTitle(graphtitle.c_str());
    graph->GetXaxis()->SetTitle(xtitle.c_str());
    graph->GetYaxis()->SetTitle(ytitle.c_str());

    graph->Draw("AL*");

    c->Update();
    c->Write();

    delete c;
    delete graph;
}
