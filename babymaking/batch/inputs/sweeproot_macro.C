#include "Riostream.h"
#include "TFile.h"
#include "TObject.h"

int sweeproot_macro(const char *rootFileName){

    TFile *rootFile = new TFile(rootFileName, "read");



    // Check if file handle is bad, Return 0 if good, 1 if bad
    if(!rootFile){
        cout << "result=" << 1 << endl;
        return 1;
    }

    // Check if file isZombie
    bool isZombie = (bool)rootFile->IsZombie();
    bool missingTree = false;

    TObject* tree = rootFile->Get("t");
    if (!tree || tree->IsZombie()) {
        missingTree = true;
    }

    rootFile->Close();

    // Return 0 if good, 1 if bad
    cout << "result=" << (isZombie || missingTree) << endl;
    return (int)(isZombie || missingTree);

}

