#include "energy-graph-z-template.h"
#include "TH1F.h"
#include "TF1.h"
#include <algorithm>
#include <iostream>
EnergyGraphZTemplate::EnergyGraphZTemplate(TileDetector * detector) : EnergyGraph("Longitudinal Template Fit Resolution",detector->get_name() +"_ztemplate","Res (#sigma/#mu)")
{
   m_event_n = 0;
   m_detector = detector;

}


EnergyGraphZTemplate::~EnergyGraphZTemplate()
{
}

void EnergyGraphZTemplate::start_energy(float Ein)
{
   m_energies.push_back(Ein);

   char name[128];
   char title[128];
   sprintf(name,"EdepZNew_%.2fGeV",m_energies.back());
   sprintf(title,"Ionization Energy in Z , (Ein %.2f GeV)",m_energies.back());
   m_zhist_temp = new TileHistZ(m_detector, name, title);

   new_zhist();
}
void EnergyGraphZTemplate::new_zhist()
{
   char name[128];
   sprintf(name,"EdepZ_%.2fGeV_%05d",m_energies.back(),m_event_n);
   m_zhistv.push_back(new TileHistZ(m_detector, name, name));
   m_event_n++;
}

void EnergyGraphZTemplate::fill_xyz(G4ThreeVector index, float edep)
{
   m_zhistv.back()->fill_xyz(index,edep);
   m_zhist_temp->fill_xyz(index,edep);
}

void EnergyGraphZTemplate::end_event()
{
   new_zhist();
}

void EnergyGraphZTemplate::end_energy() {

   delete m_zhistv.back();
   m_zhistv.pop_back();

   m_zhist_temp->normalize(m_event_n);

   plot_limits lim = m_detector->get_limits("z");
   Fit fit;
   fit.hist = m_zhist_temp;

   TF1 * zfit = new TF1("zfit",fit,double(lim.low),double(lim.high),1);
   zfit->SetParameter(0,1);

   int i =0;
   std::vector<double> amplitudes;
   for(std::vector<TileHistZ*>::iterator hist=m_zhistv.begin();
         hist!=m_zhistv.end();
         hist++)
   {
       
      (*hist)->Fit("zfit","LQ");
      amplitudes.push_back(zfit->GetParameter(0));
      if(i<10)
      {
         (*hist)->Write();
         (*hist)->save_plot();
         i++;
      }
      delete *hist;
   }
   float min_value = *std::min_element(amplitudes.begin(),amplitudes.end());
   float max_value = *std::max_element(amplitudes.begin(),amplitudes.end());
   char histoname[50];
   char histotitle[100];

   sprintf(histoname, "ZTemplate_%.2fGeV", m_energies.back());
   sprintf(histotitle, "Z Template Amplitude, (Ein %.2f GeV)",m_energies.back());
   TH1F * hist = new TH1F(histoname,histotitle,100,min_value,max_value);
   for(std::vector<double>::iterator it= amplitudes.begin();
         it!=amplitudes.end();
         it++)
      hist->Fill(*it);
   if( hist->Integral() != 0 ) hist->Scale(1.0/hist->Integral());
   hist->Write();

   m_data.push_back(hist->GetRMS()/hist->GetMean());

   amplitudes.clear();
   m_zhistv.clear();
   delete m_zhist_temp;
}

double EnergyGraphZTemplate::Fit::operator()(double *x, double *par)
{
   //std::cout << "debug " << round(x[0]) << ' ' << hist->GetBinContent(round(x[0])) << std::endl;
   return hist->GetBinContent(round(x[0]))*par[0];
}
