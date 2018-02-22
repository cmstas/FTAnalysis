#include "cfg.h"

template <typename T>
void _set_param(YAML::Node& root, const std::string& varname, T& store, const T& default_) {
    store = root[varname].as<T>(default_);
    std::cout << varname << " = " << store << std::endl;
}

void read_config(const std::string& config_filename) {
    YAML::Node root = YAML::LoadFile(config_filename);

    std::cout << "CONFIG BEGIN" << std::endl;
    _set_param(root, "jetMinPt", cfg.jetMinPt, 40.f);
    std::cout << "CONFIG END" << std::endl;
}
