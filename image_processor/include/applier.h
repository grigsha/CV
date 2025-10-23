#pragma once

#include "filters.h"
#include "bmp.h"
#include "args.h"

class Applier : public BMP {
public:
    explicit Applier(const std::vector<ArgStructure>& filters);
    void ApplyFilters();

private:
    std::vector<ArgStructure> filters_list_;
    FilterName GetFilterType(const std::string& filter_name);
};
