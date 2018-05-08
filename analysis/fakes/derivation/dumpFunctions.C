void dumpFunctions(TString dir) {

    for (int doIso=0;doIso<2;doIso++) {

        for (int doMu=0;doMu<2;doMu++) {

            TString suffix = (doIso ? "_IsoTrigs" : "" );

            TString lep = doMu ? "muon" : "electron";
            TString l = doMu ? "mu" : "e";
            TString ll = doMu ? "mu" : "el";

            TFile* f = TFile::Open("ewkCorFR_"+lep+suffix+".root");

            TH2D* centr = (TH2D*) f->Get("Nt_histo_"+l);


            cout << "float "+lep+"FakeRate"+suffix+"(float pt, float eta) {" << endl;
            for (int xbin=1;xbin<=centr->GetNbinsX();xbin++) {
                for (int ybin=1;ybin<=centr->GetNbinsY();ybin++) {
                    if (xbin!=centr->GetNbinsX()) { 
                        cout << "   if (pt>=" << centr->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << centr->GetXaxis()->GetBinUpEdge(xbin) 
                            << " && fabs(eta)>=" << centr->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << centr->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  centr->GetBinContent(xbin,ybin) << ";" << endl;
                    } else {
                        cout << "   if (pt>=" << centr->GetXaxis()->GetBinLowEdge(xbin)
                            << " && fabs(eta)>=" << centr->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << centr->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  centr->GetBinContent(xbin,ybin) << ";" << endl;      
                    }
                }
            }
            cout << "   return 0.;" << endl;
            cout << "}" << endl;

            cout << "float "+lep+"FakeRateError"+suffix+"(float pt, float eta) {" << endl;
            for (int xbin=1;xbin<=centr->GetNbinsX();xbin++) {
                for (int ybin=1;ybin<=centr->GetNbinsY();ybin++) {
                    if (xbin!=centr->GetNbinsX()) { 
                        cout << "   if (pt>=" << centr->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << centr->GetXaxis()->GetBinUpEdge(xbin) 
                            << " && fabs(eta)>=" << centr->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << centr->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  centr->GetBinError(xbin,ybin) << ";" << endl;
                    } else {
                        cout << "   if (pt>=" << centr->GetXaxis()->GetBinLowEdge(xbin)
                            << " && fabs(eta)>=" << centr->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << centr->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  centr->GetBinError(xbin,ybin) << ";" << endl;      
                    }
                }
            }
            cout << "   return 0.;" << endl;
            cout << "}" << endl;


            TH2D* altern = (TH2D*) f->Get("num_data_zp");

            cout << "float "+lep+"AlternativeFakeRate"+suffix+"(float pt, float eta) {" << endl;
            for (int xbin=1;xbin<=altern->GetNbinsX();xbin++) {
                for (int ybin=1;ybin<=altern->GetNbinsY();ybin++) {
                    if (xbin!=altern->GetNbinsX()) { 
                        cout << "   if (pt>=" << altern->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << altern->GetXaxis()->GetBinUpEdge(xbin) 
                            << " && fabs(eta)>=" << altern->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << altern->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  altern->GetBinContent(xbin,ybin) << ";" << endl;
                    } else {
                        cout << "   if (pt>=" << altern->GetXaxis()->GetBinLowEdge(xbin)
                            << " && fabs(eta)>=" << altern->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << altern->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  altern->GetBinContent(xbin,ybin) << ";" << endl;      
                    }
                }
            }
            cout << "   return 0.;" << endl;
            cout << "}" << endl;

            f->Close();

            TFile* fmc = TFile::Open(dir+"/rate_histos_qcd_"+ll+"_LooseEMVA"+suffix+".root");
            TH2D* mc = (TH2D*) fmc->Get("rate_cone_histo_"+l);
            cout << "float "+lep+"QCDMCFakeRate"+suffix+"(float pt, float eta) {" << endl;
            for (int xbin=1;xbin<=mc->GetNbinsX();xbin++) {
                for (int ybin=1;ybin<=mc->GetNbinsY();ybin++) {
                    if (xbin!=mc->GetNbinsX()) { 
                        cout << "   if (pt>=" << mc->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << mc->GetXaxis()->GetBinUpEdge(xbin) 
                            << " && fabs(eta)>=" << mc->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << mc->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  mc->GetBinContent(xbin,ybin) << ";" << endl;
                    } else {
                        cout << "   if (pt>=" << mc->GetXaxis()->GetBinLowEdge(xbin)
                            << " && fabs(eta)>=" << mc->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << mc->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  mc->GetBinContent(xbin,ybin) << ";" << endl;      
                    }
                }
            }
            cout << "   return 0.;" << endl;
            cout << "}" << endl;

            cout << "float "+lep+"QCDMCFakeRateError"+suffix+"(float pt, float eta) {" << endl;
            for (int xbin=1;xbin<=mc->GetNbinsX();xbin++) {
                for (int ybin=1;ybin<=mc->GetNbinsY();ybin++) {
                    if (xbin!=mc->GetNbinsX()) { 
                        cout << "   if (pt>=" << mc->GetXaxis()->GetBinLowEdge(xbin) << " && pt<" << mc->GetXaxis()->GetBinUpEdge(xbin) 
                            << " && fabs(eta)>=" << mc->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << mc->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  mc->GetBinError(xbin,ybin) << ";" << endl;
                    } else {
                        cout << "   if (pt>=" << mc->GetXaxis()->GetBinLowEdge(xbin)
                            << " && fabs(eta)>=" << mc->GetYaxis()->GetBinLowEdge(ybin) << " && fabs(eta)<" << mc->GetYaxis()->GetBinUpEdge(ybin) << " ) return " 
                            <<  mc->GetBinError(xbin,ybin) << ";" << endl;      
                    }
                }
            }
            cout << "   return 0.;" << endl;
            cout << "}" << endl;

            fmc->Close();

        }
    }

}
