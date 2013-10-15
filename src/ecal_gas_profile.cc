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
    char histoname[50];
    char histotitle[100];
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
    
    fname = string(argv[2]);
    TileDetector detector(fname);

    float edep_lim_mult = 40.;
    float x_lim = detector.m_num-1;
    float y_lim = detector.m_num-1;
    float z_lim = detector.m_nlayers;
    float ring_lim = (detector.m_num+1)/2;
    float r2_lim = (x_lim*x_lim*detector.m_cellsize*detector.m_cellsize+y_lim*y_lim*detector.m_cellsize*detector.m_cellsize)/8.0;
    //float r_lim = sqrt(r2_lim);
    int n_x = detector.m_num;
    int n_y = detector.m_num;
    int n_z = detector.m_nlayers;
    int n_rings = ring_lim;
    //int n_r = 100;
    int n_r2 = 100;




    for (unsigned int index = 0; index < energies.size(); index++) {
        TileHist * hEdepx;
        TileHist * hEdepz;

        double Ein = energies[index];
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
        
        typedef map<G4ThreeVector,CalorimeterHit*> hitmap;
        hitmap HitCol;

        sprintf(histoname, "EdepZX%.2fGeV", Ein);
        sprintf(histotitle, "Ionization Energy in X , (Ein %.2f GeV)",Ein);
        hEdepx = new TileHist(histoname, histotitle, n_x, 0, x_lim);
        //
        sprintf(histoname, "EdepXY%.2fGeV", Ein);
        sprintf(histotitle, "Ionization Energy in Z , (Ein %.2f GeV)",Ein);
        hEdepz = new TileHist(histoname, histotitle, n_z, 0, z_lim);

        for(Int_t i = 0; i<1000; i++)
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
                G4ThreeVector hitPos;
                G4ThreeVector hitIndex;
                G4double newEdep;
                hitmap::iterator it;
                G4double oldEdep;
                if (Classname == "Calorimeter") {
                    //cout << "There are " << NbHits << " hits in " << (*hciter).first << endl;
                    for (G4int ii = 0; ii < NbHits; ii++) {
                        CalorimeterHit* CalHit = dynamic_cast<CalorimeterHit*> (hits[ii]);

                        hitPos = CalHit->GetPos();
                        hitIndex = detector.find_index(hitPos);
                        newEdep = CalHit->GetEdep();
                        it = HitCol.find(hitIndex);
                        if(it == HitCol.end()) {
                            CalorimeterHit Hit;
                            Hit.SetPos(hitPos);
                            Hit.SetEdep(newEdep);
                            HitCol[hitIndex] = new CalorimeterHit(Hit);
                        } else {
                            oldEdep = HitCol[hitIndex]->GetEdep();
                            HitCol[hitIndex]->SetEdep(newEdep+oldEdep);
                        }
                    }
                }
            }
            G4double edep;
            G4int nceren;
            int nceren_cut = 10;
            int nceren_count = 0;
            for (hitmap::iterator it = HitCol.begin(); 
                    it != HitCol.end();
                    it++) {

                edep = it->second->GetEdep();
                int hitRing = detector.find_ring(it->first,false);
                //fill Edep
                if(edep) hEdepx->Fill(it->first.x(),edep/1e3);
                if(edep) hEdepz->Fill(it->first.z(),edep/1e3);
                delete it->second;
            }
            HitCol.clear();
            Edir->cd();
        }

        hEdepx->GetXaxis()->SetTitle("X pos (cell)");
        hEdepx->GetYaxis()->SetTitle("Energy (GeV)");
        hEdepz->GetXaxis()->SetTitle("Z pos (cm)");
        hEdepz->GetYaxis()->SetTitle("Energy (GeV)");
        hEdepx->Write();
        hEdepz->Write();
      
        gStyle->SetOptStat(1002211);
        char filenamex[128];
        char filenamez[128];
        sprintf(filenamex,"%sx",argv[3]);
        sprintf(filenamez,"%sz",argv[3]);

        //save_plot(hEdepx,"canvas1", filenamex);
        //save_plot(hEdepz,"canvas2", filenamez);
        hEdepx->save_plot();
        hEdepz->save_plot();


        delete  hEdepx;
        delete  hEdepz;
    }
}



