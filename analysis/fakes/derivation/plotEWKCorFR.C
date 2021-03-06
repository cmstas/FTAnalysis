void plotEWKCorFR(TString dir, float elSF_zp,float muSF_zp,float elSF_mt, float muSF_mt, bool useIsoTrig, int year) {

  for(int doMu = 0; doMu < 2; doMu++) {

  // gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPaintTextFormat("4.2f");

  TString suffix = (useIsoTrig ? "_IsoTrigs" : "");

  TString lname = (doMu ? "_mu" : "_e");
  TString lepname = (doMu ? "muon" : "electron");
  TString lepstr = (doMu ? "_mu" : "_el");
  TString ystr = Form("y%i_",year);

  TCanvas c1("c1","c1",600,600);

  TFile *_file_data = TFile::Open(dir+"/"+ystr+"rate_histos_data"+lepstr+"_LooseEMVA"+suffix+".root");
  TFile *_file_wj = TFile::Open(dir+"/"+ystr+"rate_histos_wjets_"+"LooseEMVA"+suffix+".root");
  TFile *_file_dy = TFile::Open(dir+"/"+ystr+"rate_histos_dy_"+"LooseEMVA"+suffix+".root");

  TH2F* den_data = (TH2F*) _file_data->Get("Nl_cone_histo"+lname);
  TH2F* den_wj = (TH2F*) _file_wj->Get("Nl_cone_histo"+lname);
  TH2F* den_dy = (TH2F*) _file_dy->Get("Nl_cone_histo"+lname);
  TH2F* num_data = (TH2F*) _file_data->Get("Nt_histo"+lname);
  TH2F* num_wj = (TH2F*) _file_wj->Get("Nt_histo"+lname);
  TH2F* num_dy = (TH2F*) _file_dy->Get("Nt_histo"+lname);

  TH2F* den_data_zp = (TH2F*) den_data->Clone("den_data_zp");
  TH2F* den_wj_zp   = (TH2F*) den_wj->Clone("den_wj_zp");
  TH2F* den_dy_zp   = (TH2F*) den_dy->Clone("den_dy_zp");
  TH2F* num_data_zp = (TH2F*) num_data->Clone("num_data_zp");
  TH2F* num_wj_zp   = (TH2F*) num_wj->Clone("num_wj_zp");
  TH2F* num_dy_zp   = (TH2F*) num_dy->Clone("num_dy_zp");
  TH2F* num_data_raw = (TH2F*) num_data->Clone("num_data_raw");

  float ewkSF = (doMu ? muSF_mt : elSF_mt);
  den_data->Add(den_wj,-1.*ewkSF);
  den_data->Add(den_dy,-1.*ewkSF);
  num_data->Add(num_wj,-1.*ewkSF);
  num_data->Add(num_dy,-1.*ewkSF);
  for (int binx=1;binx<=num_data->GetNbinsX();++binx) {
    for (int biny=1;biny<=num_data->GetNbinsY();++biny) {
      if (num_data->GetBinContent(binx,biny)<0) num_data->SetBinContent(binx,biny,0.);
    }
  }
  num_data->Divide(num_data, den_data, 1, 1, "B");

  float ewkSF_zp = (doMu ? muSF_zp : elSF_zp);
  den_data_zp->Add(den_wj_zp,-1.*ewkSF_zp);
  den_data_zp->Add(den_dy_zp,-1.*ewkSF_zp);
  num_data_zp->Add(num_wj_zp,-1.*ewkSF_zp);
  num_data_zp->Add(num_dy_zp,-1.*ewkSF_zp);
  for (int binx=1;binx<=num_data_zp->GetNbinsX();++binx) {
    for (int biny=1;biny<=num_data_zp->GetNbinsY();++biny) {
      if (num_data_zp->GetBinContent(binx,biny)<0) {
          num_data_zp->SetBinContent(binx,biny,0.);
      }
    }
  }
  num_data_zp->Divide(num_data_zp, den_data_zp, 1, 1, "B");

  // check for ~0 fake rate due to oversubtraction and take the content from
  // the adjacent bin with same eta and lower pt
  for (int binx=1;binx<=num_data->GetNbinsX();++binx) {
      for (int biny=1;biny<=num_data->GetNbinsY();++biny) {
          float val = num_data->GetBinContent(binx,biny);
          float err = num_data->GetBinError(binx,biny);
          if (val<1.0e-6 or (err/val)>3.0) {
              float pt = num_data->GetXaxis()->GetBinLowEdge(binx);
              float eta = num_data->GetYaxis()->GetBinLowEdge(biny);
              if (pt < 30.) continue; // low pt bins are supposed to be empty...
              float newval = num_data->GetBinContent(binx-1,biny);
              float newerr = num_data->GetBinError(binx-1,biny);
              if (val<1.0e-6) {
                  std::cout << Form("%i isotrig=%i mu=%i pt [%.0f,%.0f] eta [%.1f,%.1f] bin has 0 FR after subtraction, setting to %.3f+-%.3f",
                          year, useIsoTrig, doMu,
                          num_data->GetXaxis()->GetBinLowEdge(binx),
                          num_data->GetXaxis()->GetBinUpEdge(binx),
                          num_data->GetYaxis()->GetBinLowEdge(biny),
                          num_data->GetYaxis()->GetBinUpEdge(biny),
                          newval, newerr
                          ) << std::endl;
              } else {
                  std::cout << Form("%i isotrig=%i mu=%i pt [%.0f,%.0f] eta [%.1f,%.1f] bin has >300 percent error after subtraction, setting to %.3f+-%.3f",
                          year, useIsoTrig, doMu,
                          num_data->GetXaxis()->GetBinLowEdge(binx),
                          num_data->GetXaxis()->GetBinUpEdge(binx),
                          num_data->GetYaxis()->GetBinLowEdge(biny),
                          num_data->GetYaxis()->GetBinUpEdge(biny),
                          newval, newerr
                          ) << std::endl;
              }
              num_data->SetBinContent(binx,biny,newval);
              num_data->SetBinError(binx,biny,newerr);
          }
      }
  }
  // gStyle->SetPaintTextFormat("4.3f");

  num_data->SetTitle("EWK-corrected "+lepname+" fake rate");
  num_data->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  num_data->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  num_data->GetYaxis()->SetTitleOffset(1.2);

  num_data->GetZaxis()->SetRangeUser(0,0.5);
  num_data->GetZaxis()->SetLabelSize(0.025);

  num_data->SetMarkerSize(1.5);

  num_data->Draw("textecolz");

  // std::cout << "Saving pdfs/ewkCorFR_"+lepname+suffix+".pdf" << std::endl;
  c1.SaveAs("pdfs/"+ystr+"ewkCorFR_"+lepname+suffix+".pdf");
  TFile out("pdfs/"+ystr+"ewkCorFR_"+lepname+suffix+".root","RECREATE");
  num_data->Write();
  num_data_zp->SetTitle("Alternative EWK-corrected "+lepname+" fake rate");
  num_data_zp->Write();
  out.Close();

  den_wj->SetTitle("WJets "+lepname+" denominator");
  den_wj->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  den_wj->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  den_wj->GetYaxis()->SetTitleOffset(1.2);
  den_wj->GetZaxis()->SetLabelSize(0.025);
  den_wj->SetMarkerSize(1.5);
  den_wj->Draw("textecolz");
  c1.SaveAs("pdfs/"+ystr+"den_wj_"+lepname+suffix+".pdf");

  den_dy->SetTitle("DY "+lepname+" denominator");
  den_dy->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  den_dy->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  den_dy->GetYaxis()->SetTitleOffset(1.2);
  den_dy->GetZaxis()->SetLabelSize(0.025);
  den_dy->SetMarkerSize(1.5);
  den_dy->Draw("textecolz");
  c1.SaveAs("pdfs/"+ystr+"den_dy_"+lepname+suffix+".pdf");

  num_wj->SetTitle("WJets "+lepname+" numerator");
  num_wj->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  num_wj->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  num_wj->GetYaxis()->SetTitleOffset(1.2);
  num_wj->GetZaxis()->SetLabelSize(0.025);
  num_wj->SetMarkerSize(1.5);
  num_wj->Draw("textecolz");
  c1.SaveAs("pdfs/"+ystr+"num_wj_"+lepname+suffix+".pdf");

  num_dy->SetTitle("DY "+lepname+" numerator");
  num_dy->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  num_dy->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  num_dy->GetYaxis()->SetTitleOffset(1.2);
  num_dy->GetZaxis()->SetLabelSize(0.025);
  num_dy->SetMarkerSize(1.5);
  num_dy->Draw("textecolz");
  c1.SaveAs("pdfs/"+ystr+"num_dy_"+lepname+suffix+".pdf");

  den_data->SetTitle("Data "+lepname+" denominator");
  den_data->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  den_data->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  den_data->GetYaxis()->SetTitleOffset(1.2);
  den_data->GetZaxis()->SetLabelSize(0.025);
  den_data->SetMarkerSize(1.5);
  den_data->Draw("textecolz");
  c1.SaveAs("pdfs/"+ystr+"den_data_"+lepname+suffix+".pdf");

  num_data_raw->SetTitle("Data "+lepname+" numerator");
  num_data_raw->GetXaxis()->SetTitle(""+lepname+" p_{T}^{corr.} [GeV]");
  num_data_raw->GetYaxis()->SetTitle(""+lepname+" |#eta|");
  num_data_raw->GetYaxis()->SetTitleOffset(1.2);
  num_data_raw->GetZaxis()->SetLabelSize(0.025);
  num_data_raw->SetMarkerSize(1.5);
  num_data_raw->Draw("textecolz");
  c1.SaveAs("pdfs/"+ystr+"num_data_"+lepname+suffix+".pdf");

  }

}
