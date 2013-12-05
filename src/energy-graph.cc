#include "energy-graph.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"

EnergyGraph::EnergyGraph()
{

}

EnergyGraph::EnergyGraph(std::string title, std::string name, std::string ytitle)
{
   m_title = title;
   m_name = name;
   m_ytitle = ytitle;
}

EnergyGraph::~EnergyGraph()
{
   delete m_graph;
}

void EnergyGraph::write() {


   std::string xtitle = "Energy (GeV)";
   TCanvas * c = new TCanvas((m_name+"_c").c_str(),m_title.c_str());
   m_graph= new TGraph(m_energies.size(),&(m_energies.front()),&(m_data.front()));

   m_graph->SetTitle(m_title.c_str());
   m_graph->GetXaxis()->SetTitle(xtitle.c_str());
   m_graph->GetYaxis()->SetTitle(m_ytitle.c_str());

   m_graph->Draw("AL*");

   c->Update();
   c->Write();
   c->SaveAs(m_name.c_str());

   m_graph->Write();

   delete c;
}

void EnergyGraph::start_energy(float Ein)
{
   m_energies.push_back(Ein);
}
