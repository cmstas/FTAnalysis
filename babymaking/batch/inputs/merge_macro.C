void merge_macro(const TString& in, const TString& outpath) {

  //Verbose flag
  bool verbose = 1;

  //Set up chains and iterator
  TChain *chain = new TChain("t");
  chain->SetMaxTreeSize(30000000000LL); //default is 100000000000LL
  if (verbose) std::cout << "Merging files from dir: " << in << std::endl << "Ouputting to: " << outpath << std::endl;
  chain->Add(in + "/*.root");
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  //Next corrupt files
  currentFile = 0;
  fileIter.Reset(); 
  while ( (currentFile = (TFile*)fileIter.Next()) ){
      std::cout << " currentFile->GetTitle(): " << currentFile->GetTitle() << std::endl;
    TFile *file = TFile::Open(currentFile->GetTitle());
    if (file->IsZombie()){
      cout << "Error! " << currentFile->GetTitle() << " is corrupt" << endl;
      return;  
    }
  }
  cout << "No corrupt files, all good."  << endl;

  //Then merge trees
  chain->Merge(outpath, "fast");
  if (verbose) std::cout << "Total events merged: " << chain->GetEntries() << std::endl;

  //Then merge 1D hists
  std::vector <TH1F*> ourHists; 
  currentFile = 0;
  fileIter.Reset(); 
  //file loop
  while ( (currentFile = (TFile*)fileIter.Next()) ){
    TFile *file = TFile::Open(currentFile->GetTitle());
    //Hist loop
    for (auto&& keyAsObj : *file->GetListOfKeys()){
      auto key = (TKey*)keyAsObj;
      if (strncmp(key->GetClassName(), "TH1F", 1000) != 0) continue; 
      TH1F *hist = (TH1F*)key->ReadObj(); 
      //OurHist loop
      bool foundIt = false;
      for (unsigned int i = 0; i < ourHists.size(); i++){
        if (strncmp(ourHists[i]->GetTitle(), hist->GetTitle(), 1000) == 0){ ourHists[i]->Add(hist); foundIt = true; }
      }
      if (!foundIt) ourHists.push_back(hist); 
    }
  }
  cout << ourHists.size() << endl;
  cout << ourHists[0]->GetBinContent(0) << endl; 
  
  TFile *file = new TFile(outpath, "UPDATE"); 
  for (unsigned int i = 0; i < ourHists.size(); i++) ourHists[i]->Write();  
  delete file; 

  if (verbose) cout << "done!" << endl;

}
