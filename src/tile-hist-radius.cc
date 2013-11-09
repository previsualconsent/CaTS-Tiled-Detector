#include "tile-hist-radius.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TPaveLabel.h"
#include <iostream>

TileHistRadius::TileHistRadius()
{

}

TileHistRadius::TileHistRadius(std::string tag, std::string name, std::string title, int n_r, float r_lim,int n_cells) : TileHist(tag,name.c_str(), title.c_str(),n_r,0,r_lim) 
{
   m_r_lim = r_lim;
   m_num = n_r;
   m_n_cells = n_cells;
   m_total_energy = 0;

   GetXaxis()->SetTitle("Radius (cells)");
   GetYaxis()->SetTitle("Percent Energy (cumulative)");
}

TileHistRadius::~TileHistRadius()
{
}


void TileHistRadius::fill_xyz(G4ThreeVector pos, float edep)
{
   int center = (m_n_cells-1)/2;
   float radius  = std::sqrt(std::pow(pos.x()-center,2)+std::pow(pos.y()-center,2));
   for(float i = radius; i < m_r_lim ; i+=m_r_lim/float(m_num))
      Fill(i,edep);

   m_total_energy += edep;
}

void TileHistRadius::normalize(int nevents) {
   if(!m_norm) Scale(1.0/double(m_total_energy));
   else std::cerr << "TileHistRadius:: SECOND NORMALIZATION!! don't do that\n";
   m_norm = true;
}

void TileHistRadius::save_plot() {

   char ctitle[128],cname[128];
   sprintf(cname,"c%s",fName.Data());
   sprintf(ctitle,"c%s",fTitle.Data());
   TCanvas * c = new TCanvas(cname,ctitle);
   c->cd();

   SetAxisRange(0,1,"Y");
   SetStats(false);
   Draw();

   calc_data();

   std::cout << m_data["r90%"] << std::endl;
   
   char label_str[20];
   sprintf(label_str,"90%% contained in %0.2f cell radius",m_data["r90%"]);
   TPaveLabel *label = new TPaveLabel(10,.2,m_r_lim-1,.3,label_str);
   label->SetFillColor(42);
   label->Draw();

   c->Update();
   char filename[128];
   sprintf(filename,"%s_%s.png",m_tag.c_str(),fName.Data());
   c->SaveAs(filename);

   delete c;

}

void TileHistRadius::calc_data()
{
   if(!m_calculated)
   {
      int i=0;

      for(; i < m_num && GetBinContent(i)<.9; i++);
      m_data["r90%"]=float(i)*m_r_lim/float(m_num);
   }
   m_calculated=true;
}
data_map TileHistRadius::get_data()
{
   return m_data;
}
