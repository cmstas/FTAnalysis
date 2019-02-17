import ROOT as r

def make_2018():

    # 2018
    # https://raw.githubusercontent.com/cms-sw/cmssw/CMSSW_10_2_X/SimGeneral/MixingModule/python/mix_2018_25ns_JuneProjectionFull18_PoissonOOTPU_cfi.py
    xs = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99 ]
    ys = [ 4.695341e-10, 1.206213e-06, 1.162593e-06, 6.118058e-06, 1.626767e-05, 3.508135e-05, 7.12608e-05, 0.0001400641, 0.0002663403, 0.0004867473, 0.0008469, 0.001394142, 0.002169081, 0.003198514, 0.004491138, 0.006036423, 0.007806509, 0.00976048, 0.0118498, 0.01402411, 0.01623639, 0.01844593, 0.02061956, 0.02273221, 0.02476554, 0.02670494, 0.02853662, 0.03024538, 0.03181323, 0.03321895, 0.03443884, 0.035448, 0.03622242, 0.03674106, 0.0369877, 0.03695224, 0.03663157, 0.03602986, 0.03515857, 0.03403612, 0.0326868, 0.03113936, 0.02942582, 0.02757999, 0.02563551, 0.02362497, 0.02158003, 0.01953143, 0.01750863, 0.01553934, 0.01364905, 0.01186035, 0.01019246, 0.008660705, 0.007275915, 0.006043917, 0.004965276, 0.004035611, 0.003246373, 0.002585932, 0.002040746, 0.001596402, 0.001238498, 0.0009533139, 0.0007282885, 0.000552306, 0.0004158005, 0.0003107302, 0.0002304612, 0.0001696012, 0.0001238161, 8.96531e-05, 6.438087e-05, 4.585302e-05, 3.23949e-05, 2.271048e-05, 1.580622e-05, 1.09286e-05, 7.512748e-06, 5.140304e-06, 3.505254e-06, 2.386437e-06, 1.625859e-06, 1.111865e-06, 7.663272e-07, 5.350694e-07, 3.808318e-07, 2.781785e-07, 2.098661e-07, 1.642811e-07, 1.312835e-07, 1.081326e-07, 9.141993e-08, 7.890983e-08, 6.91468e-08, 6.119019e-08, 5.443693e-08, 4.85036e-08, 4.31486e-08, 3.822112e-08 ]

    hnumints_true = r.TH1F("hnumints_true","hnumints_true",len(xs),0,len(xs))
    hnumints_baby = r.TH1F("hnumints_baby","hnumints_baby",len(xs),0,len(xs))
    hnumints_cms4 = r.TH1F("hnumints_cms4","hnumints_cms4",len(xs),0,len(xs))
    hnumints_true.Sumw2()
    hnumints_baby.Sumw2()
    hnumints_cms4.Sumw2()

    for x,y in zip(xs,ys):
        hnumints_true.SetBinContent(x+1,y)
        hnumints_true.SetBinError(x+1,0.)

    print "Drawing from baby"
    ch = r.TChain("t")
    ch.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.21/output/year_2018/DY_high.root")
    ch.Draw("trueNumInt[0]>>hnumints_baby","","norme")

    print "Drawing from cms4"
    ch = r.TChain("Events")
    for i in range(1,4)+range(5,8)+range(9,10):
        ch.Add("/hadoop/cms/store/group/snt/run2_mc2018//DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1_MINIAODSIM_CMS4_V10-02-04/merged_ntuple_{}.root".format(i))
    ch.Draw("puInfo_trueNumInteractions[0]>>hnumints_cms4","","norme")


    fout = r.TFile("truenumint_mc2018.root", "RECREATE")
    hnumints_true.Write()
    hnumints_baby.Write()
    hnumints_cms4.Write()
    fout.Write()
    fout.Close()

def make_2017():

    # 2017
    # https://github.com/cms-sw/cmssw/blob/master/SimGeneral/MixingModule/python/mix_2017_25ns_WinterMC_PUScenarioV1_PoissonOOTPU_cfi.py
    xs = [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89]
    ys = [ 3.39597497605e-05, 6.63688402133e-06, 1.39533611284e-05, 3.64963078209e-05, 6.00872171664e-05, 9.33932578027e-05, 0.000120591524486, 0.000128694546198, 0.000361697233219, 0.000361796847553, 0.000702474896113, 0.00133766053707, 0.00237817050805, 0.00389825605651, 0.00594546732588, 0.00856825906255, 0.0116627396044, 0.0148793350787, 0.0179897368379, 0.0208723871946, 0.0232564170641, 0.0249826433945, 0.0262245860346, 0.0272704617569, 0.0283301107549, 0.0294006137386, 0.0303026836965, 0.0309692426278, 0.0308818046328, 0.0310566806228, 0.0309692426278, 0.0310566806228, 0.0310566806228, 0.0310566806228, 0.0307696426944, 0.0300103336052, 0.0288355370103, 0.0273233309106, 0.0264343533951, 0.0255453758796, 0.0235877272306, 0.0215627588047, 0.0195825559393, 0.0177296309658, 0.0160560731931, 0.0146022004183, 0.0134080690078, 0.0129586991411, 0.0125093292745, 0.0124360740539, 0.0123547104433, 0.0123953922486, 0.0124360740539, 0.0124360740539, 0.0123547104433, 0.0124360740539, 0.0123387597772, 0.0122414455005, 0.011705203844, 0.0108187105305, 0.00963985508986, 0.00827210065136, 0.00683770076341, 0.00545237697118, 0.00420456901556, 0.00367513566191, 0.00314570230825, 0.0022917978982, 0.00163221454973, 0.00114065309494, 0.000784838366118, 0.000533204105387, 0.000358474034915, 0.000238881117601, 0.0001984254989, 0.000157969880198, 0.00010375646169, 6.77366175538e-05, 4.39850477645e-05, 2.84298066026e-05, 1.83041729561e-05, 1.17473542058e-05, 7.51982735129e-06, 6.16160108867e-06, 4.80337482605e-06, 3.06235473369e-06, 1.94863396999e-06, 1.23726800704e-06, 7.83538083774e-07, 4.94602064224e-07]

    hnumints_true = r.TH1F("hnumints_true","hnumints_true",len(xs),0,len(xs))
    hnumints_baby = r.TH1F("hnumints_baby","hnumints_baby",len(xs),0,len(xs))
    hnumints_cms4 = r.TH1F("hnumints_cms4","hnumints_cms4",len(xs),0,len(xs))
    hnumints_true.Sumw2()
    hnumints_baby.Sumw2()
    hnumints_cms4.Sumw2()

    for x,y in zip(xs,ys):
        hnumints_true.SetBinContent(x+1,y)
        hnumints_true.SetBinError(x+1,0.)

    print "Drawing from baby"
    ch = r.TChain("t")
    ch.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.24/output/year_2017/DY_high.root")
    ch.Draw("trueNumInt[0]>>hnumints_baby","","norme")

    print "Drawing from cms4"
    ch = r.TChain("Events")
    for i in range(1,15):
        ch.Add("/hadoop/cms/store/group/snt/run2_mc2017//TTJets_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_{}.root".format(i))
        # ch.Add("/hadoop/cms/store/group/snt/run2_mc2017//TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V10-02-05/merged_ntuple_{}.root".format(i))
    ch.Draw("puInfo_trueNumInteractions[0]>>hnumints_cms4","","norme")


    fout = r.TFile("truenumint_mc2017.root", "RECREATE")
    hnumints_true.Write()
    hnumints_baby.Write()
    hnumints_cms4.Write()
    fout.Write()
    fout.Close()


def make_2016():

    # 2016 
    # https://github.com/cms-sw/cmssw/blob/master/SimGeneral/MixingModule/python/mix_2016_25ns_Moriond17MC_PoissonOOTPU_cfi.py
    xs = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74]
    ys = [1.78653e-05 ,2.56602e-05 ,5.27857e-05 ,8.88954e-05 ,0.000109362 ,0.000140973 ,0.000240998 ,0.00071209 ,0.00130121 ,0.00245255 ,0.00502589 ,0.00919534 ,0.0146697 ,0.0204126 ,0.0267586 ,0.0337697 ,0.0401478 ,0.0450159 ,0.0490577 ,0.0524855 ,0.0548159 ,0.0559937 ,0.0554468 ,0.0537687 ,0.0512055 ,0.0476713 ,0.0435312 ,0.0393107 ,0.0349812 ,0.0307413 ,0.0272425 ,0.0237115 ,0.0208329 ,0.0182459 ,0.0160712 ,0.0142498 ,0.012804 ,0.011571 ,0.010547 ,0.00959489 ,0.00891718 ,0.00829292 ,0.0076195 ,0.0069806 ,0.0062025 ,0.00546581 ,0.00484127 ,0.00407168 ,0.00337681 ,0.00269893 ,0.00212473 ,0.00160208 ,0.00117884 ,0.000859662 ,0.000569085 ,0.000365431 ,0.000243565 ,0.00015688 ,9.88128e-05 ,6.53783e-05 ,3.73924e-05 ,2.61382e-05 ,2.0307e-05 ,1.73032e-05 ,1.435e-05 ,1.36486e-05 ,1.35555e-05 ,1.37491e-05 ,1.34255e-05 ,1.33987e-05 ,1.34061e-05 ,1.34211e-05 ,1.34177e-05 ,1.32959e-05 ,1.33287e-05]

    xs.extend([75, 76, 77, 78, 79])
    ys.extend([0.,0.,0.,0.,0.,])

    hnumints_true = r.TH1F("hnumints_true","hnumints_true",len(xs),0,len(xs))
    hnumints_baby = r.TH1F("hnumints_baby","hnumints_baby",len(xs),0,len(xs))
    hnumints_cms4 = r.TH1F("hnumints_cms4","hnumints_cms4",len(xs),0,len(xs))
    hnumints_true.Sumw2()
    hnumints_baby.Sumw2()
    hnumints_cms4.Sumw2()

    for x,y in zip(xs,ys):
        hnumints_true.SetBinContent(x+1,y)
        hnumints_true.SetBinError(x+1,0.)

    print "Drawing from baby"
    ch = r.TChain("t")
    ch.Add("/nfs-7/userdata/namin/tupler_babies/merged/FT/v3.23/output/year_2016/DY_high.root")
    ch.Draw("trueNumInt[0]>>hnumints_baby","","norme")

    print "Drawing from cms4"
    ch = r.TChain("Events")
    for i in [55,44,72,7,106,66,34,158,125]:
        ch.Add("/hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_{}.root".format(i))
    ch.Draw("puInfo_trueNumInteractions[0]>>hnumints_cms4","","norme")


    fout = r.TFile("truenumint_mc2016.root", "RECREATE")
    hnumints_true.Write()
    hnumints_baby.Write()
    hnumints_cms4.Write()
    fout.Write()
    fout.Close()

if __name__ == "__main__":
    # make_2018()
    make_2017()
    # make_2016()

