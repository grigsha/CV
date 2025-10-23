#pragma once

#include <vector>
#include "bmp.h"

const std::vector<std::vector<float>> SHARPENING_MATRIX = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

const std::vector<std::vector<float>> EDGE_MATRIX = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

enum FilterName { CROP, GREYSCALE, NEGATIVE, SHARPENING, EDGEDETECTION, GAUSSIANBLUR, DROP_EFFECT, UNKNOWN_FILTER };
using Matrix = std::vector<std::vector<Pixel>>;

class Filter {
public:
    virtual ~Filter() {
    }
    virtual Matrix Apply(const Matrix &input) const = 0;
};

Matrix ApplyConvolution(const Matrix &input, const std::vector<std::vector<float>> &kernel);

class CropFilter : public Filter {
public:
    CropFilter(int width, int height) : target_width_(width), target_height_(height) {
    }
    Matrix Apply(const Matrix &input) const override;

private:
    int target_width_;
    int target_height_;
};

class GrayscaleFilter : public Filter {
public:
    Matrix Apply(const Matrix &input) const override;
};

class NegativeFilter : public Filter {
public:
    Matrix Apply(const Matrix &input) const override;
};

class SharpenFilter : public Filter {
public:
    SharpenFilter() : kernel_(SHARPENING_MATRIX) {
    }
    Matrix Apply(const Matrix &input) const override;

private:
    std::vector<std::vector<float>> kernel_;
};

class EdgeDetectionFilter : public Filter {
public:
    explicit EdgeDetectionFilter(float threshold) : kernel_(EDGE_MATRIX), threshold_(threshold) {
    }
    Matrix Apply(const Matrix &input) const override;

private:
    std::vector<std::vector<float>> kernel_;
    float threshold_;
};

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(float sigma) : sigma_(sigma) {
    }
    Matrix Apply(const Matrix &input) const override;

private:
    float sigma_;
};

class DropEffectFilter : public Filter {
public:
    explicit DropEffectFilter(float strength, float center_x = -1.0f, float center_y = -1.0f)
        : strength_(strength), centerx_(center_x), centery_(center_y) {
    }
    Matrix Apply(const Matrix &input) const override;

private:
    float strength_;
    float centerx_;
    float centery_;
};
