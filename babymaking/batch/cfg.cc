#include "cfg.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

Config cfg;

template <typename T>
void _set_param(YAML::Node& root, const std::string& varname, T& store, const T& default_) {
    store = root[varname].as<T>(default_);
    std::cout << varname << " = " << store << std::endl;
}

bool read_config(const std::string& config_filename) {
    YAML::Node root = YAML::LoadFile(config_filename);

    if (!root.IsMap()) {
        std::cout << "ERROR: config file " << config_filename << " not found." << std::endl;
        return false;
    }

    std::cout << "CONFIG BEGIN" << std::endl;

    /** jetMinPt: applied as a minimum cut on JEC corrected jet pt
     */
    _set_param(root, "jetMinPt", cfg.jetMinPt, 40.0f);

    /** bjetMinPt: applied as a minimum cut on JEC corrected jet pt for B-Jets
     * see: https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco
     */
    _set_param(root, "bjetMinPt", cfg.bjetMinPt, 25.0f);

    std::cout << "CONFIG END" << std::endl;
    return true;
}
