#include "../include/applier.h"
#include <stdexcept>
#include <iostream>

constexpr float DropRestriction = 2.0f;

FilterName Applier::GetFilterType(const std::string& filter_name) {
    if (filter_name == "-crop") {
        return CROP;
    } else if (filter_name == "-gs") {
        return GREYSCALE;
    } else if (filter_name == "-neg") {
        return NEGATIVE;
    } else if (filter_name == "-sharp") {
        return SHARPENING;
    } else if (filter_name == "-edge") {
        return EDGEDETECTION;
    } else if (filter_name == "-blur") {
        return GAUSSIANBLUR;
    } else if (filter_name == "-drop") {
        return DROP_EFFECT;
    } else {
        return UNKNOWN_FILTER;
    }
}

Applier::Applier(const std::vector<ArgStructure>& filters) : filters_list_(filters){};

void Applier::ApplyFilters() {
    Matrix current_matrix = GetMatrix();
    for (const auto& [filter, parameters] : filters_list_) {
        FilterName name = GetFilterType(filter);
        switch (name) {
            case CROP: {
                if (parameters.size() < 2) {
                    throw std::runtime_error("Crop filter requires width and height parameters.");
                }
                int new_width = std::stoi(parameters[0]);
                int new_height = std::stoi(parameters[1]);
                CropFilter crop(new_width, new_height);
                Matrix cropped = crop.Apply(current_matrix);
                int32_t crop_height = static_cast<int32_t>(cropped.size());
                int32_t crop_width = (crop_height > 0) ? static_cast<int32_t>(cropped[0].size()) : 0;
                current_matrix = cropped;
                SetHeight(crop_height);
                SetWidth(crop_width);
                break;
            }
            case GREYSCALE: {
                GrayscaleFilter grayscale;
                current_matrix = grayscale.Apply(current_matrix);
                break;
            }
            case NEGATIVE: {
                NegativeFilter negative;
                current_matrix = negative.Apply(current_matrix);
                break;
            }
            case SHARPENING: {
                SharpenFilter grayscale;
                current_matrix = grayscale.Apply(current_matrix);
                break;
            }
            case EDGEDETECTION: {
                if (parameters.empty()) {
                    throw std::runtime_error("Edge detection filter requires a threshold parameter.");
                }
                float threshold = std::stof(parameters[0]);
                EdgeDetectionFilter edge(threshold);
                current_matrix = edge.Apply(current_matrix);
                break;
            }
            case GAUSSIANBLUR: {
                if (parameters.empty()) {
                    throw std::runtime_error("Gaussian blur filter requires a sigma parameter.");
                }
                float sigma = std::stof(parameters[0]);
                GaussianBlurFilter blur(sigma);
                current_matrix = blur.Apply(current_matrix);
                break;
            }
            case DROP_EFFECT: {
                if (parameters.empty() || std::stof(parameters[0]) < DropRestriction) {
                    throw std::runtime_error("Drop effect filter requires a strength parameter not less than 2.");
                }
                float strength = std::stof(parameters[0]);
                float center_x = -1.0f;
                float center_y = -1.0f;
                if (parameters.size() >= 3) {
                    center_x = std::stof(parameters[1]);
                    center_y = std::stof(parameters[2]);
                }
                DropEffectFilter drop_effect(strength, center_x, center_y);
                current_matrix = drop_effect.Apply(current_matrix);
                break;
            }
            case UNKNOWN_FILTER:
            default:
                std::cerr << "Unknown filter: " << filter << std::endl;
                break;
        }
    }
    SetMatrix(current_matrix);
}
