#include "energy-graph-res.h"
#include "TH1F.h"
#include <algorithm>

EnergyGraphRes::EnergyGraphRes(std::string name) : EnergyGraph("Resolution vs Energy",name +"_res","Res (#sigma/#mu)")
{
   m_energy_of_event = 0;
}


EnergyGraphRes::~EnergyGraphRes()
{
}

void EnergyGraphRes::fill_xyz(G4ThreeVector pos, float edep)
{
   m_energy_of_event += edep;
}

void EnergyGraphRes::end_event()
{
   m_histv.push_back(m_energy_of_event);
   m_energy_of_event = 0;
}

void EnergyGraphRes::end_energy()
{
 float min_value = *std::min_element(m_histv.begin(),m_histv.end());
 float max_value = *std::max_element(m_histv.begin(),m_histv.end());

 char histoname[50];
 char histotitle[100];

 sprintf(histoname, "EnergyNew_%.2fGeV", m_energies.back());
 sprintf(histotitle, "Energy Response, (Ein %.2f GeV)",m_energies.back());
 TH1F * hist = new TH1F(histoname,histotitle,100,min_value,max_value);
 for(std::vector<float>::iterator it= m_histv.begin();
       it!=m_histv.end();
       it++)
    hist->Fill(*it);
 if( hist->Integral() != 0 ) hist->Scale(1.0/hist->Integral());
 hist->Write();

 m_data.push_back(hist->GetRMS()/hist->GetMean());
}
