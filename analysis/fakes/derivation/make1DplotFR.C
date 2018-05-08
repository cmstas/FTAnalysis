// #include "../../software/dataMCplotMaker/dataMCplotMaker.cc"
#include "TFile.h"
#include "TH2.h"
#include "TPaveText.h"
#include "../../misc/common_utils.h"

void make1DplotFR(TString dir, float elSF_zp,float muSF_zp,float elSF_mt, float muSF_mt, bool useIsoTrig){
    bool do_qcd = true;

    for(int doPt = 0; doPt < 2; doPt++) {
        gROOT->Reset();
        gStyle->SetOptStat(0);
        gStyle->SetPaintTextFormat("4.2f");

        TString postfix = "";
        postfix+="_LooseEMVA";
        // postfix+="_PtRatioCor";
        // postfix+="_InvPtRatio";
        // postfix+="_doBonly";
        // postfix+="_doLightonly";
        TString var = "";
        var="_cone";

        TString suffix = (useIsoTrig ? "_IsoTrigs" : "");
        postfix+=suffix;

        TH1D *muf_qcd;
        TH1D *elf_qcd;
        if (do_qcd) {
            TFile* f_qcd_mu = TFile::Open(dir+"/rate_histos_qcd_mu"+postfix+".root");
            TFile* f_qcd_el = TFile::Open(dir+"/rate_histos_qcd_el"+postfix+".root");

            TH2F* mud_qcd = (TH2F*) f_qcd_mu->Get("Nl"+var+"_histo_mu");  
            TH2F* eld_qcd = (TH2F*) f_qcd_el->Get("Nl"+var+"_histo_e");

            TH1D* mud1d_qcd = (doPt ? mud_qcd->ProjectionX() : mud_qcd->ProjectionY());
            TH1D* eld1d_qcd = (doPt ? eld_qcd->ProjectionX() : eld_qcd->ProjectionY());

            TH2F* mun_qcd = 0;
            TH2F* eln_qcd = 0;
            mun_qcd = (TH2F*) f_qcd_mu->Get("Nt_histo_mu");  
            eln_qcd = (TH2F*) f_qcd_el->Get("Nt_histo_e");

            TH1D* mun1d_qcd = (doPt ? mun_qcd->ProjectionX() : mun_qcd->ProjectionY());
            TH1D* eln1d_qcd = (doPt ? eln_qcd->ProjectionX() : eln_qcd->ProjectionY());

            muf_qcd = (TH1D*) mun1d_qcd->Clone("rate_histo_mu");
            muf_qcd->Divide(mun1d_qcd,mud1d_qcd,1,1,"B");
            elf_qcd = (TH1D*) eln1d_qcd->Clone("rate_histo_el");
            elf_qcd->Divide(eln1d_qcd,eld1d_qcd,1,1,"B");
        }

        TFile* f_data_el = TFile::Open(dir+"/rate_histos_data_el"+postfix+".root");
        TFile* f_data_mu = TFile::Open(dir+"/rate_histos_data_mu"+postfix+".root");
        TFile* f_dy = TFile::Open(dir+"/rate_histos_dy"+postfix+".root");
        TFile* f_wj = TFile::Open(dir+"/rate_histos_wjets"+postfix+".root");

        TH2F* mud_data = (TH2F*) f_data_mu->Get("Nl"+var+"_histo_mu");
        TH2F* mud_wj = (TH2F*) f_wj->Get("Nl"+var+"_histo_mu");
        TH2F* mud_dy = (TH2F*) f_dy->Get("Nl"+var+"_histo_mu");
        TH2F* eld_data = (TH2F*) f_data_el->Get("Nl"+var+"_histo_e");
        TH2F* eld_wj = (TH2F*) f_wj->Get("Nl"+var+"_histo_e");
        TH2F* eld_dy = (TH2F*) f_dy->Get("Nl"+var+"_histo_e");

        TH2F* mud_data_zp = (TH2F*) mud_data->Clone("mud_data_zp");
        TH2F* mud_wj_zp   = (TH2F*) mud_wj->Clone("mud_wj_zp");
        TH2F* mud_dy_zp   = (TH2F*) mud_dy->Clone("mud_dy_zp");
        TH2F* eld_data_zp = (TH2F*) eld_data->Clone("eld_data_zp");
        TH2F* eld_wj_zp   = (TH2F*) eld_wj->Clone("eld_wj_zp");
        TH2F* eld_dy_zp   = (TH2F*) eld_dy->Clone("eld_dy_zp");

        mud_data->Add(mud_wj,-1.*muSF_mt);
        mud_data->Add(mud_dy,-1.*muSF_mt);
        eld_data->Add(eld_wj,-1.*elSF_mt);
        eld_data->Add(eld_dy,-1.*elSF_mt);

        TH1D* mud1d_data = (doPt ? mud_data->ProjectionX() : mud_data->ProjectionY());
        TH1D* eld1d_data = (doPt ? eld_data->ProjectionX() : eld_data->ProjectionY());

        mud_data_zp->Add(mud_wj_zp,-1.*muSF_zp);
        mud_data_zp->Add(mud_dy_zp,-1.*muSF_zp);
        eld_data_zp->Add(eld_wj_zp,-1.*elSF_zp);
        eld_data_zp->Add(eld_dy_zp,-1.*elSF_zp);

        TH1D* mud1d_data_zp = (doPt ? mud_data_zp->ProjectionX() : mud_data_zp->ProjectionY());
        TH1D* eld1d_data_zp = (doPt ? eld_data_zp->ProjectionX() : eld_data_zp->ProjectionY());

        TH2F* mun_data = 0;
        TH2F* mun_wj   = 0;
        TH2F* mun_dy   = 0;
        TH2F* eln_data = 0;
        TH2F* eln_wj   = 0;
        TH2F* eln_dy   = 0;
        mun_data = (TH2F*) f_data_mu->Get("Nt_histo_mu");
        mun_wj   = (TH2F*) f_wj->Get("Nt_histo_mu");
        mun_dy   = (TH2F*) f_dy->Get("Nt_histo_mu");
        eln_data = (TH2F*) f_data_el->Get("Nt_histo_e");
        eln_wj   = (TH2F*) f_wj->Get("Nt_histo_e");
        eln_dy   = (TH2F*) f_dy->Get("Nt_histo_e");

        TH2F* mun_data_zp = (TH2F*) mun_data->Clone("mun_data_zp");
        TH2F* mun_wj_zp   = (TH2F*) mun_wj->Clone("mun_wj_zp");
        TH2F* mun_dy_zp   = (TH2F*) mun_dy->Clone("mun_dy_zp");
        TH2F* eln_data_zp = (TH2F*) eln_data->Clone("eln_data_zp");
        TH2F* eln_wj_zp   = (TH2F*) eln_wj->Clone("eln_wj_zp");
        TH2F* eln_dy_zp   = (TH2F*) eln_dy->Clone("eln_dy_zp");

        mun_data->Add(mun_wj,-1.*muSF_mt);
        mun_data->Add(mun_dy,-1.*muSF_mt);
        eln_data->Add(eln_wj,-1.*elSF_mt);
        eln_data->Add(eln_dy,-1.*elSF_mt);

        for (int binx=1;binx<=mun_data->GetNbinsX();++binx) {
            for (int biny=1;biny<=mun_data->GetNbinsY();++biny) {
                if (mun_data->GetBinContent(binx,biny)<0) mun_data->SetBinContent(binx,biny,0.);
                if (eln_data->GetBinContent(binx,biny)<0) eln_data->SetBinContent(binx,biny,0.);
            }
        }

        TH1D* mun1d_data = (doPt ? mun_data->ProjectionX() : mun_data->ProjectionY());
        TH1D* eln1d_data = (doPt ? eln_data->ProjectionX() : eln_data->ProjectionY());

        mun_data_zp->Add(mun_wj_zp,-1.*muSF_zp);
        mun_data_zp->Add(mun_dy_zp,-1.*muSF_zp);
        eln_data_zp->Add(eln_wj_zp,-1.*elSF_zp);
        eln_data_zp->Add(eln_dy_zp,-1.*elSF_zp);

        for (int binx=1;binx<=mun_data_zp->GetNbinsX();++binx) {
            for (int biny=1;biny<=mun_data_zp->GetNbinsY();++biny) {
                if (mun_data_zp->GetBinContent(binx,biny)<0) mun_data_zp->SetBinContent(binx,biny,0.);
                if (eln_data_zp->GetBinContent(binx,biny)<0) eln_data_zp->SetBinContent(binx,biny,0.);
            }
        }

        TH1D* mun1d_data_zp = (doPt ? mun_data_zp->ProjectionX() : mun_data_zp->ProjectionY());
        TH1D* eln1d_data_zp = (doPt ? eln_data_zp->ProjectionX() : eln_data_zp->ProjectionY());

        TH1D *muf_data = (TH1D*) mun1d_data->Clone("rate_histo_mu");
        muf_data->Divide(mun1d_data,mud1d_data,1,1,"B");
        muf_data->SetMarkerStyle(20);
        muf_data->SetMarkerSize(1.5);
        muf_data->SetLineColor(kBlack);
        TH1D *elf_data = (TH1D*) eln1d_data->Clone("rate_histo_el");
        elf_data->Divide(eln1d_data,eld1d_data,1,1,"B");
        elf_data->SetMarkerStyle(20);
        elf_data->SetMarkerSize(1.5);
        elf_data->SetLineColor(kBlack);

        TH1D *muf_data_zp = (TH1D*) mun1d_data_zp->Clone("rate_histo_mu_zp");
        muf_data_zp->Divide(mun1d_data_zp,mud1d_data_zp,1,1,"B");
        TH1D *elf_data_zp = (TH1D*) eln1d_data_zp->Clone("rate_histo_el_zp");
        elf_data_zp->Divide(eln1d_data_zp,eld1d_data_zp,1,1,"B");

        TH1D *muf_data_syst = (TH1D*) muf_data->Clone("rate_histo_mu_syst");
        muf_data_syst->SetFillColor(kGray+1);
        muf_data_syst->SetFillStyle(3644);
        for (int bin=1;bin<=muf_data_syst->GetNbinsX();++bin) {
            muf_data_syst->SetBinError(bin,fabs(muf_data->GetBinContent(bin)-muf_data_zp->GetBinContent(bin)));
        }
        TH1D *elf_data_syst = (TH1D*) elf_data->Clone("rate_histo_el_syst");
        elf_data_syst->SetFillColor(kGray+1);
        elf_data_syst->SetFillStyle(3644);
        for (int bin=1;bin<=elf_data_syst->GetNbinsX();++bin) {
            elf_data_syst->SetBinError(bin,fabs(elf_data->GetBinContent(bin)-elf_data_zp->GetBinContent(bin)));
        }

        TString xname = (doPt ? "p_{T} (GeV)" : "|#eta|");
        if (var=="_cone"&&doPt) xname = "p_{T}^{corr.} (GeV)";

        TCanvas c1("c1","c1",600,600);
        if (do_qcd) {
            muf_qcd->SetTitle("");
            muf_qcd->GetXaxis()->SetTitleSize(1.2*0.035);
            muf_qcd->GetXaxis()->SetTitleFont(42);
            muf_qcd->GetYaxis()->SetTitleSize(1.2*0.035);
            muf_qcd->GetYaxis()->SetTitleFont(42);
            muf_qcd->GetXaxis()->SetLabelSize(1.2*0.035);
            muf_qcd->GetXaxis()->SetLabelFont(42);
            muf_qcd->GetYaxis()->SetLabelSize(1.2*0.035);
            muf_qcd->GetYaxis()->SetLabelFont(42);
            muf_qcd->GetYaxis()->SetTitleOffset(1.20);
            muf_qcd->GetXaxis()->SetTitleOffset(1.10);
            muf_qcd->GetYaxis()->SetNdivisions(505);
            muf_qcd->GetYaxis()->SetTitle("Muon #epsilon_{TL}");
            muf_qcd->GetXaxis()->SetTitle("Muon "+xname);
            if (var=="_jet"&&doPt) muf_qcd->GetXaxis()->SetTitle("jet "+xname);
            muf_qcd->GetYaxis()->SetRangeUser(0,0.6);
            muf_qcd->SetMarkerStyle(21);
            muf_qcd->SetMarkerSize(1.5);
            muf_qcd->SetMarkerColor(kRed);
            muf_qcd->SetLineColor(kRed);
            muf_qcd->Draw("PE");
        }
        muf_data_syst->Draw("E2,same");
        muf_data->Draw("PEsame");
        c1.RedrawAxis();

        TLegend* leg = new TLegend(0.62,0.75,0.89,0.87);
        leg->SetFillColor(kWhite);
        leg->SetLineColor(kWhite);
        leg->SetTextFont(42);
        leg->SetTextSize(1.2*0.035);
        leg->SetTextAlign(12);
        leg->AddEntry(muf_data,"Data","ple");
        if (do_qcd) {
            leg->AddEntry(muf_qcd,"Multijet MC","ple");
        }
        leg->Draw();

        TPaveText* labelcms  = new TPaveText(0.75,0.90,0.89,0.99,"NDCNB");
        labelcms->SetTextAlign(33);
        labelcms->SetTextSize(1.2*0.035);
        labelcms->SetTextFont(42);
        labelcms->SetFillColor(kWhite);
        labelcms->AddText(Form("%.1f fb^{-1} (13 TeV)",getLumi()));
        labelcms->SetBorderSize(0);
        labelcms->SetLineWidth(2);
        labelcms->Draw();

        TLatex latex;
        latex.SetNDC();
        latex.SetTextAngle(0);
        latex.SetTextColor(kBlack);    
        latex.SetTextFont(61);
        latex.SetTextSize(1.2*0.044);
        latex.SetTextAlign(11);
        latex.DrawLatex(0.12, 0.91, "CMS");
        latex.SetTextFont(52);
        latex.SetTextAlign(11);
        latex.SetTextSize(1.2*0.044*0.75);
        latex.DrawLatex(0.24, 0.91, "Supplementary");

        if (doPt) {
            elf_data->SetBinContent(1,0.);
            elf_data->SetBinError(1,0.);
            elf_data_syst->SetBinContent(1,0.);
            elf_data_syst->SetBinError(1,0.);
            elf_qcd->SetBinContent(1,0.);
            elf_qcd->SetBinError(1,0.);
        }

        TCanvas c2("c2","c2",600,600);
        if (do_qcd) {
            elf_qcd->SetTitle("");
            elf_qcd->GetXaxis()->SetTitleSize(1.2*0.035);
            elf_qcd->GetXaxis()->SetTitleFont(42);
            elf_qcd->GetYaxis()->SetTitleSize(1.2*0.035);
            elf_qcd->GetYaxis()->SetTitleFont(42);
            elf_qcd->GetXaxis()->SetLabelSize(1.2*0.035);
            elf_qcd->GetXaxis()->SetLabelFont(42);
            elf_qcd->GetYaxis()->SetLabelSize(1.2*0.035);
            elf_qcd->GetYaxis()->SetLabelFont(42);
            elf_qcd->GetYaxis()->SetTitleOffset(1.20);
            elf_qcd->GetXaxis()->SetTitleOffset(1.10);
            elf_qcd->GetYaxis()->SetNdivisions(505);
            elf_qcd->GetYaxis()->SetTitle("Electron #epsilon_{TL}");
            elf_qcd->GetXaxis()->SetTitle("Electron "+xname);
            if (var=="_jet"&&doPt) elf_qcd->GetXaxis()->SetTitle("jet "+xname);
            elf_qcd->GetYaxis()->SetRangeUser(0,0.6);
            elf_qcd->SetMarkerStyle(21);
            elf_qcd->SetMarkerSize(1.5);
            elf_qcd->SetMarkerColor(kRed);
            elf_qcd->SetLineColor(kRed);
            elf_qcd->Draw("PE");
        }
        elf_data_syst->Draw("E2,same");
        elf_data->Draw("PEsame");
        c2.RedrawAxis();
        leg->Draw();
        labelcms->Draw();
        latex.SetTextFont(61);
        latex.SetTextSize(1.2*0.044);
        latex.SetTextAlign(11);
        latex.DrawLatex(0.12, 0.91, "CMS");
        latex.SetTextFont(52);
        latex.SetTextAlign(11);
        latex.SetTextSize(1.2*0.044*0.75);
        latex.DrawLatex(0.24, 0.91, "Supplementary");

        c1.SaveAs("pdfs/mu_1dfr"+var+postfix+(doPt ? "" : "_eta")+".pdf");
        c2.SaveAs("pdfs/el_1dfr"+var+postfix+(doPt ? "" : "_eta")+".pdf");

    }

}
