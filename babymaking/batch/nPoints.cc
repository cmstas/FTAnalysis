//sample: 1-ttH
//sample: 2-tHW
//sample: 3-tHq
//sample += 3 for corresponding pseudoscalar
int nPoints_higgs(int sample, int mH){
    if (sample == 1){
        if (mH == 350) return 103778;
        if (mH == 370) return 91175;
        if (mH == 390) return 93998;
        if (mH == 410) return 98687;
        if (mH == 430) return 102421;
        if (mH == 450) return 87667;
        if (mH == 470) return 96639;
        if (mH == 490) return 93382;
        if (mH == 510) return 101248;
        if (mH == 530) return 86954;
        if (mH == 550) return 91983;
        if (mH == 600) return 97030;
        if (mH == 650) return 88140;
        if (mH == 700) return 92904;
        if (mH == 750) return 95115;
        if (mH == 800) return 86535;
        if (mH == 850) return 96488;
        if (mH == 900) return 93990;
    }
    else if (sample == 2){
        if (mH == 350) return 99550;
        if (mH == 370) return 100616;
        if (mH == 390) return 99218;
        if (mH == 410) return 104747;
        if (mH == 430) return 97215;
        if (mH == 450) return 100081;
        if (mH == 470) return 104684;
        if (mH == 490) return 90082;
        if (mH == 510) return 99372;
        if (mH == 530) return 103017;
        if (mH == 550) return 95217;
    }
    else if (sample == 3){
        if (mH == 350) return 95842;
        if (mH == 370) return 104916;
        if (mH == 390) return 97056;
        if (mH == 410) return 105446;
        if (mH == 430) return 100314;
        if (mH == 450) return 98848;
        if (mH == 470) return 95518;
        if (mH == 490) return 107314;
        if (mH == 510) return 98318;
        if (mH == 530) return 94914;
        if (mH == 550) return 93782;
    }
    return -99999999; 
}

//sample += 3 for corresponding pseudoscalar
float xsec_higgs(int sample, int mH){
    if (sample == 1){
        if (mH == 350) return 0.001*24.2;
        if (mH == 370) return 0.001*20.1;
        if (mH == 390) return 0.001*17.8;
        if (mH == 410) return 0.001*15.8;
        if (mH == 430) return 0.001*14.9;
        if (mH == 450) return 0.001*11.9;
        if (mH == 470) return 0.001*11.2;
        if (mH == 490) return 0.001*9.87;
        if (mH == 510) return 0.001*9.75;
        if (mH == 530) return 0.001*7.78;
        if (mH == 550) return 0.001*7.49;
        if (mH == 600) return 0.00578;
        if (mH == 650) return 0.00465;
        if (mH == 700) return 0.00364;
        if (mH == 750) return 0.00299;
        if (mH == 800) return 0.00233;
        if (mH == 850) return 0.00195;
        if (mH == 900) return 0.00160;
    }
    else if (sample == 2){
        if (mH == 350) return 0.001*8.37;
        if (mH == 370) return 0.001*7.61;
        if (mH == 390) return 0.001*7.07;
        if (mH == 410) return 0.001*6.58;
        if (mH == 430) return 0.001*5.72;
        if (mH == 450) return 0.001*5.16;
        if (mH == 470) return 0.001*4.81;
        if (mH == 490) return 0.001*4.34;
        if (mH == 510) return 0.001*4.19;
        if (mH == 530) return 0.001*3.86;
        if (mH == 550) return 0.001*3.41;
    }
    else if (sample == 3){
        if (mH == 350) return 0.001*15.8;
        if (mH == 370) return 0.001*13.5;
        if (mH == 390) return 0.001*11.7;
        if (mH == 410) return 0.001*10.2;
        if (mH == 430) return 0.001*8.82;
        if (mH == 450) return 0.001*7.73;
        if (mH == 470) return 0.001*6.78;
        if (mH == 490) return 0.001*5.93;
        if (mH == 510) return 0.001*5.23;
        if (mH == 530) return 0.001*4.59;
        if (mH == 550) return 0.001*4.11;
    }

    if (sample == 4){
        if (mH == 350) return 0.001*33.7;
        if (mH == 370) return 0.001*28.6;
        if (mH == 390) return 0.001*25.3;
        if (mH == 410) return 0.001*22.0;
        if (mH == 430) return 0.001*19.9;
        if (mH == 450) return 0.001*17.7;
        if (mH == 470) return 0.001*15.1;
        if (mH == 490) return 0.001*14.9;
        if (mH == 510) return 0.001*12.6;
        if (mH == 530) return 0.001*11.5;
        if (mH == 550) return 0.001*10.0;
        if (mH == 600) return 0.00578;
        if (mH == 650) return 0.00465;
        if (mH == 700) return 0.00364;
        if (mH == 750) return 0.00299;
        if (mH == 800) return 0.00233;
        if (mH == 850) return 0.00195;
        if (mH == 900) return 0.00160;
    }
    else if (sample == 5){
        if (mH == 350) return 0.001*7.97;
        if (mH == 370) return 0.001*7.09;
        if (mH == 390) return 0.001*6.88;
        if (mH == 410) return 0.001*5.92;
        if (mH == 430) return 0.001*5.39;
        if (mH == 450) return 0.001*4.97;
        if (mH == 470) return 0.001*4.59;
        if (mH == 490) return 0.001*4.27;
        if (mH == 510) return 0.001*3.87;
        if (mH == 530) return 0.001*3.82;
        if (mH == 550) return 0.001*3.26;
    }
    else if (sample == 6){
        if (mH == 350) return 0.001*9.28;
        if (mH == 370) return 0.001*8.14;
        if (mH == 390) return 0.001*7.19;
        if (mH == 410) return 0.001*6.43;
        if (mH == 430) return 0.001*5.72;
        if (mH == 450) return 0.001*5.07;
        if (mH == 470) return 0.001*4.55;
        if (mH == 490) return 0.001*4.09;
        if (mH == 510) return 0.001*3.67;
        if (mH == 530) return 0.001*3.27;
        if (mH == 550) return 0.001*2.94;
    }
    return -1.0;
}
