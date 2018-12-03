void plotFR(TString dir) {
    for(int i = 0; i < 4; i++) {
        for(int doIso = 0; doIso < 2; doIso++) {

            // gROOT->Reset(); // Uh why do we need this? NOTE it causes `dir` to become garbage later
            gStyle->SetOptStat(0);
            gStyle->SetPaintTextFormat("4.2f");

            bool saveDen = true;

            TString sample = "data";
            if(i == 0) sample = "data_el";
            if(i == 1) sample = "data_mu";
            if(i == 2) sample = "qcd_el";
            if(i == 3) sample = "qcd_mu";


            TString postfix = "";
            postfix+="_LooseEMVA";
            if(doIso) postfix+="_IsoTrigs";
            TString var = "";
            var="_cone";


            TFile* f = new TFile(dir+"/rate_histos_"+sample+postfix+".root");

            if (sample.Contains("_el")==0) {
                TH2F* muf = (TH2F*) f->Get("rate"+var+"_histo_mu");
                muf->SetTitle("muon fake rate ("+sample+")");
                muf->GetXaxis()->SetTitle("muon p_{T} [GeV]");
                if (var=="_cone") muf->GetXaxis()->SetTitle("muon p_{T}^{corr.} [GeV]");
                if (var=="_jet") muf->GetXaxis()->SetTitle("jet p_{T} [GeV]");
                muf->GetYaxis()->SetTitle("muon |#eta|");
                muf->GetYaxis()->SetTitleOffset(1.2);
                if (postfix=="_extrPtRel") muf->GetYaxis()->SetTitle("H_{T} [GeV]");
                TCanvas c1("c1","c1",600,600);
                muf->GetZaxis()->SetRangeUser(0,0.5);
                muf->GetZaxis()->SetLabelSize(0.025);
                muf->SetMarkerSize(1.5);
                if (postfix!="_extrPtRel") muf->GetXaxis()->SetNdivisions(6,0);
                muf->Draw("texte,colz");
                c1.RedrawAxis();
                c1.SaveAs("pdfs/mu_fr"+var+"_"+sample+postfix+".pdf");
            }

            if (sample.Contains("_mu")==0) {
                TH2F* elf = (TH2F*) f->Get("rate"+var+"_histo_e");
                elf->SetTitle("electron fake rate ("+sample+")");
                elf->GetXaxis()->SetTitle("electron p_{T} [GeV]");
                if (var=="_cone") elf->GetXaxis()->SetTitle("electron p_{T}^{corr.} [GeV]");
                if (var=="_jet") elf->GetXaxis()->SetTitle("jet p_{T} [GeV]");
                elf->GetYaxis()->SetTitle("electron |#eta|");
                elf->GetYaxis()->SetTitleOffset(1.2);
                if (postfix=="_extrPtRel") elf->GetYaxis()->SetTitle("H_{T} [GeV]");
                TCanvas c2("c2","c2",600,600);
                elf->GetZaxis()->SetRangeUser(0,0.5);
                elf->GetZaxis()->SetLabelSize(0.025);
                elf->SetMarkerSize(1.5);
                if (postfix!="_extrPtRel") elf->GetXaxis()->SetNdivisions(6,0);
                elf->Draw("texte,colz");
                c2.RedrawAxis();
                c2.SaveAs("pdfs/el_fr"+var+"_"+sample+postfix+".pdf");
            }


        }
    }
}
