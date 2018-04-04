#ifndef HIST_UTILS_CPP
#define HIST_UTILS_CPP

#include <string>

#include "TH1F.h"
#include "TH2F.h"

class KinematicMeasurement{
    TH1F pt;
    TH1F eta;
    TH1F phi;

  public:
    KinematicMeasurement(const string& title,
                         int nbins_pt, float pt_max,
                         int nbins_eta, float eta_max,
                         int nbins_phi)
      :pt{(title+"_pt").c_str(),   (title+"_pt").c_str(),  nbins_pt,         0, pt_max},
       eta{(title+"_eta").c_str(), (title+"_eta").c_str(), nbins_eta, -eta_max, eta_max},
       phi{(title+"_phi").c_str(), (title+"_phi").c_str(), nbins_phi, -3.14159, 3.14159} { }

    void save(TFile* dest = nullptr){
        if(dest) dest->cd();

        pt.Write();
        eta.Write();
        phi.Write();
    }

    template<typename T>
    void fill(const T& obj){
        pt.Fill(obj.p4.pt());
        eta.Fill(obj.p4.eta());
        phi.Fill(obj.p4.phi());
    }
};

class EfficiencyMeasurement{
    TH1F num;
    TH1F den;
    TH1F eff;

    void update(){
        eff.Sumw2();
        eff.Divide(&num, &den, 1, 1, "B");
    }

  public:
    EfficiencyMeasurement(const string& title, int nbins, float min, float max)
      :num{(title+"_num").c_str(), (title+"_num").c_str(), nbins, min, max},
       den{(title+"_den").c_str(), (title+"_den").c_str(), nbins, min, max},
       eff{title.c_str(), title.c_str(), nbins, min, max} { }

    TH1F& get_eff(){
        update();
        return eff;
    }

    void save(bool save_all=false, TFile* dest = nullptr){
        if(dest) dest->cd();

        update();
        eff.Write();
        if(save_all){
            num.Write();
            den.Write();
        }
    }

    void fill(const float& x, const bool& pass, float weight=1){
        den.Fill(x, weight);
        if(pass) num.Fill(x, weight);
    }
};

class EfficiencyMeasurement2D{
    TH2F num;
    TH2F den;
    TH2F eff;

    void update(){
        eff.Sumw2();
        eff.Divide(&num, &den, 1, 1, "B");
    }

  public:
    EfficiencyMeasurement2D(const string& title, int nbins_x, float min_x, float max_x,
                                                 int nbins_y, float min_y, float max_y)
      :num{(title+"_num").c_str(), (title+"_num").c_str(), nbins_x, min_x, max_x, nbins_y, min_y, max_y},
       den{(title+"_den").c_str(), (title+"_den").c_str(), nbins_x, min_x, max_x, nbins_y, min_y, max_y},
       eff{title.c_str(), title.c_str(), nbins_x, min_x, max_x, nbins_y, min_y, max_y} { }

    TH2F& get_eff() {
        update();
        return eff;
    }

    void save(bool save_all=false, TFile* dest = nullptr) {
        if (dest) dest->cd();

        update();
        eff.Write();
        if (save_all) {
            num.Write();
            den.Write();
        }
    }

    void fill(const float& x, const float& y, const bool& pass, float weight=1) {
        den.Fill(x, y, weight);
        if (pass) num.Fill(x, y, weight);
    }
};

#endif  // HIST_UTILS_CPP
