//Program that analyzes the  hits produced with CaTS
//

// Include files
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH1D.h"
#include <TCanvas.h>
#include "TStyle.h"
#include "TFitResultPtr.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TVectorF.h"
#include "TLegend.h"

#include "Cintex/Cintex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Event.hh"
#include "CalorimeterHit.hh"

#include "tile-detector.h"
#include "tile-hist.h"
#include "tile-hist-x.h"
#include "tile-hist-z.h"

using namespace std;


template <typename T, typename U>
T GetMax(T a, U b) {
    return (a>b)?a:b;
}

void write_2plots(TH1F * h1,string h1name,TH1F * h2,string h2name,string name, string title,string xtitle) {
    TCanvas * c = new TCanvas(name.c_str(),title.c_str());

    c->cd();
    float max1 = h1->GetAt(h1->GetMaximumBin());
    float max2 = h2->GetAt(h2->GetMaximumBin());

    h1->SetMaximum(1.1*GetMax(max1,max2));
    h1->SetTitle(title.c_str());
    h1->SetXTitle(xtitle.c_str());
    h1->SetLineColor(kBlue);
    h1->Draw();

    h2->SetLineColor(kRed);
    h2->Draw("same");

    TLegend * leg = new TLegend(.5,.7,.85,.6);
    leg->AddEntry(h1,h1name.c_str(),"l");
    leg->AddEntry(h2,h2name.c_str(),"l");
    leg->Draw();

    c->Update();
    c->Write();

    delete c;
}


template <typename T, typename U>
void write_graph(vector<T> vec1, vector<U> vec2, string graphname, string graphtitle,string xtitle,string ytitle) {

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

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    return split(s, delim, elems);
}

int main(int argc, char** argv) {
    TSystem ts;
    gSystem->Load("libCintex");
    gSystem->Load("libClassesDict");
    //  ROOT::Cintex::Cintex::SetDebug(2);
    ROOT::Cintex::Cintex::Enable();
    if (argc < 2) {
        cout << "Missing list of input files!" << endl;
        return 1;
    }
    else if (argc < 3) {
        cout << "Missing list of dimensions!" << endl;
        return 1;
    }
    vector<string> fnames;
    vector<double> energies;
    string tags[4] = {"Material",
        "Physics List",
        "Particle",
        "Energy"};
    char canvname[50];

    char canvtitle[100];

    typedef vector<TH1F*> vec_h;
    typedef vector<vec_h> mat_h;
    typedef vector<TH2F*> vec_2h;
    typedef vector<vec_2h> mat_2h;
    TFile* outfile;


    //BEGIN INITIALIZATION


    string fname(argv[1]);
    string tagvalue[4];
    ifstream infile;
    string line;
    infile.open(fname.c_str());
    if (infile.is_open()) {
        while (infile.good()) {
            getline(infile, line);
            cout << "Filename: " << line << endl;
            vector<string> x = split(line, '_');
            if (x.size() >= 4) {
                for (unsigned int i = 0; i < x.size() - 1; i++) {
                    tagvalue[i] = x[i];
                }
                vector<string> y = split(x[3], 'G');
                for (unsigned int i = 0; i < y.size() - 1; i++) {
                    tagvalue[i + 3] = y[i];
                }
                double evalue = double(atof(tagvalue[3].c_str()));
                string material = tagvalue[0];
                string physlist = tagvalue[1];
                string particle = tagvalue[2];
                string energy = tagvalue[3];
                fnames.push_back(line);
                energies.push_back(evalue);
            }
        }
        infile.close();
        //
        // Now print out what we got:
        //
        cout << "Read data from file " << fname << endl;
        cout << "Nr. of files: " << fnames.size() << endl;
        cout << endl;
        cout << "energies (GeV): ";
        cout << endl;
        for (unsigned int i = 0; i < energies.size(); i++) {
            cout << ", " << energies[i];
        }
        cout << endl;
        string histofile = tagvalue[0] + "_" + tagvalue[1] + "_"+tagvalue[2] + "_profilehistos.root";
        cout << "histo out file:  " << histofile << endl;
        outfile = new TFile(histofile.c_str(), "RECREATE");
        // create a subdirectory "top" in this file

    } else {
        cout << "couldn't Open infile! Exiting" << endl;
        exit(1);
    }
    

    string dim_filename(argv[2]);
    string detector_name(argv[3]);
    TileDetector detector(detector_name,dim_filename);

    for (unsigned int index = 0; index < energies.size(); index++) {
        double Ein = energies[index];

        detector.setup_plots(Ein);
        char Edirname[50];
        sprintf(Edirname, "Energy%.2fGeV", Ein);
        TDirectory * Edir = outfile->mkdir(Edirname);
        Edir->cd();

        TFile f(fnames[index].c_str());
        //f.GetListOfKeys()->Print();
        Event *event = new Event();
        TTree *T = (TTree*) f.Get("T");
        T->SetBranchAddress("event.", &event);
        Int_t nevent = T->GetEntries();
        cout << endl << "Nr. of Events: " << nevent << endl;
        //loop over events
        

        for(Int_t i = 0; i<nevent; i++)
        {
            T->GetEntry(i);
            map<G4String, vector<G4VHit*> >* hcmap = event->GetHCMap();

            if(!(i % 1000))
               cout << "Event " << i << ": There are " << hcmap->size() << "Hit Collections" << endl;
            map<G4String, vector<G4VHit*> >::iterator hciter;

            for (hciter = hcmap->begin(); hciter != hcmap->end(); hciter++) {
                vector<G4VHit*> hits = (*hciter).second;
                G4int NbHits = hits.size();
                vector<string> y = split((*hciter).first, '_');
                string Classname = y[1];

                if (Classname == "Calorimeter") {
                    //cout << "There are " << NbHits << " hits in " << (*hciter).first << endl;
                    for (G4int ii = 0; ii < NbHits; ii++) {
                        CalorimeterHit* CalHit = dynamic_cast<CalorimeterHit*> (hits[ii]);

                        detector.fill_plots_xyz(CalHit->GetPos(),CalHit->GetEdep());
                    }
                }
            }
            detector.end_event();
            Edir->cd();
        }

      
        gStyle->SetOptStat(1002211);

        detector.write_plots();
        detector.clear_plots();
        
    }
}



