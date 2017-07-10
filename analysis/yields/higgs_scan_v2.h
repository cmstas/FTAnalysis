TChain *higgs_scan_350_chain = new TChain("t","higgs_scan_m350" );
higgs_scan_350_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan350.root");
higgs_scan_350_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan350.root");
higgs_scan_350_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan350.root");
pair<yields_t, plots_t> results_higgs_scan_350 = run(higgs_scan_350_chain, 0, 0, 0, 0, 1);
delete higgs_scan_350_chain;

TChain *higgs_scan_370_chain = new TChain("t","higgs_scan_m370" );
higgs_scan_370_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan370.root");
higgs_scan_370_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan370.root");
higgs_scan_370_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan370.root");
pair<yields_t, plots_t> results_higgs_scan_370 = run(higgs_scan_370_chain, 0, 0, 0, 0, 1);
delete higgs_scan_370_chain;

TChain *higgs_scan_390_chain = new TChain("t","higgs_scan_m390" );
higgs_scan_390_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan390.root");
higgs_scan_390_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan390.root");
higgs_scan_390_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan390.root");
pair<yields_t, plots_t> results_higgs_scan_390 = run(higgs_scan_390_chain, 0, 0, 0, 0, 1);
delete higgs_scan_390_chain;

TChain *higgs_scan_410_chain = new TChain("t","higgs_scan_m410" );
higgs_scan_410_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan410.root");
higgs_scan_410_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan410.root");
higgs_scan_410_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan410.root");
pair<yields_t, plots_t> results_higgs_scan_410 = run(higgs_scan_410_chain, 0, 0, 0, 0, 1);
delete higgs_scan_410_chain;

TChain *higgs_scan_430_chain = new TChain("t","higgs_scan_m430" );
higgs_scan_430_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan430.root");
higgs_scan_430_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan430.root");
higgs_scan_430_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan430.root");
pair<yields_t, plots_t> results_higgs_scan_430 = run(higgs_scan_430_chain, 0, 0, 0, 0, 1);
delete higgs_scan_430_chain;

TChain *higgs_scan_450_chain = new TChain("t","higgs_scan_m450" );
higgs_scan_450_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan450.root");
higgs_scan_450_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan450.root");
higgs_scan_450_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan450.root");
pair<yields_t, plots_t> results_higgs_scan_450 = run(higgs_scan_450_chain, 0, 0, 0, 0, 1);
delete higgs_scan_450_chain;

TChain *higgs_scan_470_chain = new TChain("t","higgs_scan_m470" );
higgs_scan_470_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan470.root");
higgs_scan_470_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan470.root");
higgs_scan_470_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan470.root");
pair<yields_t, plots_t> results_higgs_scan_470 = run(higgs_scan_470_chain, 0, 0, 0, 0, 1);
delete higgs_scan_470_chain;

TChain *higgs_scan_490_chain = new TChain("t","higgs_scan_m490" );
higgs_scan_490_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan490.root");
higgs_scan_490_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan490.root");
higgs_scan_490_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan490.root");
pair<yields_t, plots_t> results_higgs_scan_490 = run(higgs_scan_490_chain, 0, 0, 0, 0, 1);
delete higgs_scan_490_chain;

TChain *higgs_scan_510_chain = new TChain("t","higgs_scan_m510" );
higgs_scan_510_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan510.root");
higgs_scan_510_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan510.root");
higgs_scan_510_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan510.root");
pair<yields_t, plots_t> results_higgs_scan_510 = run(higgs_scan_510_chain, 0, 0, 0, 0, 1);
delete higgs_scan_510_chain;

TChain *higgs_scan_530_chain = new TChain("t","higgs_scan_m530" );
higgs_scan_530_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan530.root");
higgs_scan_530_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan530.root");
higgs_scan_530_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan530.root");
pair<yields_t, plots_t> results_higgs_scan_530 = run(higgs_scan_530_chain, 0, 0, 0, 0, 1);
delete higgs_scan_530_chain;

TChain *higgs_scan_550_chain = new TChain("t","higgs_scan_m550" );
higgs_scan_550_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//TTH_scan550.root");
higgs_scan_550_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THW_scan550.root");
higgs_scan_550_chain->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/"+tag+"/output//THQ_scan550.root");
pair<yields_t, plots_t> results_higgs_scan_550 = run(higgs_scan_550_chain, 0, 0, 0, 0, 1);
delete higgs_scan_550_chain;

