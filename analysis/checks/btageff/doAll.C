{
    gROOT->ProcessLine(".L ../../misc/class_files/v8.02/SS.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");

    TChain *ch = new TChain("t");
    ch->Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v0.10_WSFv2/output/TTTTnew.root");

    ScanChain(ch);

}

