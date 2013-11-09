#include "tile-hist.h"
#include "TCanvas.h"
#include <iostream>

TileHist::TileHist()
{

}

TileHist::TileHist(std::string tag, std::string name, std::string title, int n, int low, int high) :
   TH1F(name.c_str(), title.c_str(),n,low,high)
{
   m_tag = tag;
   m_norm = false;
}

TileHist::~TileHist()
{
}

void TileHist::save_plot() {

    char ctitle[128],cname[128];
    sprintf(cname,"c%s",fName.Data());
    sprintf(ctitle,"c%s",fTitle.Data());
    TCanvas * c = new TCanvas(cname,ctitle);
    c->cd();
    Draw();
    c->Update();
    char filename[128];

    sprintf(filename,"%s_%s.png",m_tag.c_str(),fName.Data());
    std::cout << "debug filename: " << filename << std::endl;
    c->SaveAs(filename);

    delete c;
}

void TileHist::normalize(int nevents) {
   if(!m_norm) Scale(1.0/double(nevents));
   else std::cerr << "TileHist::SECOND NORMALIZATION!! don't do that\n";
   m_norm = true;
}

data_map TileHist::get_data()
{
   data_map empty;
   return empty;
}
