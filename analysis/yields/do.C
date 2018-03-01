{
  gROOT->ProcessLine(".L ../misc/class_files/v8.02/SS.cc+"); 
  gSystem->Load("../../common/CORE/CMS3_CORE.so");
  gROOT->ProcessLine(".L ../../common/CORE/Tools/dorky/dorky.cc+");
  gROOT->ProcessLine(".L yieldMaker.C+"); 
  gROOT->ProcessLine("getyields()"); 
}
