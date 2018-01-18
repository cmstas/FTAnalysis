{
    gSystem->Load("CORE/CMS3_CORE.so");
    gSystem->Load("libyaml-cpp.so");

    int error;
    gROOT->ProcessLine(".L Yield.C+", &error);
    if (error != 0) exit(error);

    auto getEnv = [](const char* name, string default_){
        const char* env = gSystem->Getenv(name);
        if (env != nullptr) return string(env);
        else return default_;
    };

    string dataset = getEnv("dataset", "N/A");
    if (dataset == "N/A") exit(0);
    string config_filename = getEnv("config", "config.yaml");
    ScanChain(dataset, config_filename);
}
