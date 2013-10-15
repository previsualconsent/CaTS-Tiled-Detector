#include "tile-hist.h"
#include "TCanvas.h"

TileHist::TileHist()
{

}

TileHist::TileHist(std::string name, std::string title, int n, int low, int high) : TH1F(name.c_str(), title.c_str(),n,low,high)
{

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
    sprintf(filename,"%s.png",fTitle.Data());
    c->SaveAs(filename);

    delete c;
}
