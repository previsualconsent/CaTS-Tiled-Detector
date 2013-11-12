#include "tile-hist-ring.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TPaveLabel.h"

TileHistRing::TileHistRing()
{

}

TileHistRing::TileHistRing(TileDetector * detector, std::string name, std::string title) : TileHist(detector,name.c_str(), title.c_str(),"ring") 
{
   m_ring_lim = detector->get_limits("ring").high;
   m_n_cells = detector->get_n_cells();
   m_total_energy = 0;

   GetXaxis()->SetTitle("Ring Number");
   GetYaxis()->SetTitle("Percent Energy (cumulative)");
}

TileHistRing::~TileHistRing()
{
}


void TileHistRing::fill_xyz(G4ThreeVector pos, float edep)
{
   int center = m_n_cells-1;
   int ring = int(std::max(abs(pos.x()-center),abs(pos.y()-center)));
   for(int i = m_ring_lim-1 ; i >=ring; i--)
      Fill(i,edep);

   m_total_energy += edep;
}

void TileHistRing::normalize(int nevents) {
   if(!m_norm) Scale(1.0/double(m_total_energy));
   else std::cerr << "TileHistRing:: SECOND NORMALIZATION!! don't do that\n";
   m_norm = true;
}

double fitting(double *x, double *par) {
   return par[0] - par[1]*std::exp(-par[2]*x[0]);
}
double inv_fitting(double x, double *par) {
   return -std::log((-x + par[0])/par[1])/par[2];
}

void TileHistRing::save_plot() {

   char ctitle[128],cname[128];
   sprintf(cname,"c%s",fName.Data());
   sprintf(ctitle,"c%s",fTitle.Data());
   TCanvas * c = new TCanvas(cname,ctitle);
   c->cd();

   SetAxisRange(0,1,"Y");
   SetStats(false);
   Draw();

   TF1 * fitFcn = new TF1("fitting",fitting,1,m_ring_lim-2,3);
   fitFcn->SetParameter(0,1.0); 
   fitFcn->SetParameter(1,2.0);
   fitFcn->SetParameter(2,.05);
   Fit("fitting","L");

   double par[3];
   fitFcn->GetParameters(par);
   double rms = inv_fitting(0.9,par);


   char label_str[20];
   sprintf(label_str,"90%% contained in %.02f cell square",rms*2+1);
   TPaveLabel *label = new TPaveLabel(10,.2,m_ring_lim-1,.3,label_str);
   label->SetFillColor(42);
   label->Draw();

   c->Update();
   char filename[128];
   sprintf(filename,"%s-%s.png",m_detector_name.c_str(),fName.Data());
   c->SaveAs(filename);

   delete c;

}
