#include "../include/filters.h"
#include <cmath>
#include <algorithm>

constexpr int32_t MaxPixelValue = 255;
constexpr float RCoefficient = 0.299;
constexpr float GCoefficient = 0.587;
constexpr float BCoefficient = 0.114f;
constexpr float CenterDivider = 2.0f;
constexpr float Two = 2.0f;
constexpr float HalfPi = static_cast<float>(M_PI) / CenterDivider;
constexpr float Pi = static_cast<float>(M_PI);

Matrix ApplyConvolution(const Matrix &input, const std::vector<std::vector<float>> &kernel) {
    int32_t height = static_cast<int32_t>(input.size());
    int32_t width = (height > 0) ? static_cast<int32_t>(input[0].size()) : 0;
    Matrix output = input;
    int32_t kernel_h = static_cast<int32_t>(kernel.size());
    int32_t kernel_w = (kernel_h > 0) ? static_cast<int32_t>(kernel[0].size()) : 0;
    int32_t kernel_center_y = kernel_h / 2;
    int32_t kernel_center_x = kernel_w / 2;
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (int z = 0; z < kernel_h; z++) {
                for (int32_t w = 0; w < kernel_w; w++) {
                    int32_t yy = std::min(std::max(y + z - kernel_center_y, 0), height - 1);
                    int32_t xx = std::min(std::max(x + w - kernel_center_x, 0), width - 1);
                    float weight = static_cast<float>(kernel[z][w]);
                    sum_r += static_cast<float>(input[yy][xx].red) * weight;
                    sum_g += static_cast<float>(input[yy][xx].green) * weight;
                    sum_b += static_cast<float>(input[yy][xx].blue) * weight;
                }
            }
            output[y][x].red =
                static_cast<uint8_t>(std::round(std::min(static_cast<float>(MaxPixelValue), std::max(0.0f, sum_r))));
            output[y][x].green =
                static_cast<uint8_t>(std::round(std::min(static_cast<float>(MaxPixelValue), std::max(0.0f, sum_g))));
            output[y][x].blue =
                static_cast<uint8_t>(std::round(std::min(static_cast<float>(MaxPixelValue), std::max(0.0f, sum_b))));
        }
    }
    return output;
}

Matrix CropFilter::Apply(const Matrix &input) const {
    int32_t height = static_cast<int32_t>(input.size());
    int32_t width = (height > 0) ? static_cast<int32_t>(input[0].size()) : 0;
    int32_t crop_height = std::min(target_height_, height);
    int32_t crop_width = std::min(target_width_, width);
    Matrix output(crop_height, std::vector<Pixel>(crop_width));
    for (int32_t y = 0; y < crop_height; y++) {
        for (int32_t x = 0; x < crop_width; x++) {
            output[y][x] = input[y][x];
        }
    }
    return output;
}

Matrix GrayscaleFilter::Apply(const Matrix &input) const {
    int32_t height = static_cast<int32_t>(input.size());
    int32_t width = (height > 0) ? static_cast<int32_t>(input[0].size()) : 0;
    Matrix output = input;
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            float r = static_cast<float>(input[y][x].red) / static_cast<float>(MaxPixelValue);
            float g = static_cast<float>(input[y][x].green) / static_cast<float>(MaxPixelValue);
            float b = static_cast<float>(input[y][x].blue) / static_cast<float>(MaxPixelValue);
            float gray = RCoefficient * r + GCoefficient * g + BCoefficient * b;
            uint8_t gray_byte = static_cast<uint8_t>(std::round(std::min(1.0f, std::max(0.0f, gray)) * MaxPixelValue));
            output[y][x].red = output[y][x].green = output[y][x].blue = gray_byte;
        }
    }
    return output;
}

Matrix NegativeFilter::Apply(const Matrix &input) const {
    int32_t height = static_cast<int32_t>(input.size());
    int32_t width = (height > 0) ? static_cast<int32_t>(input[0].size()) : 0;
    Matrix output = input;
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            output[y][x].red = MaxPixelValue - input[y][x].red;
            output[y][x].green = MaxPixelValue - input[y][x].green;
            output[y][x].blue = MaxPixelValue - input[y][x].blue;
        }
    }
    return output;
}

Matrix SharpenFilter::Apply(const Matrix &input) const {
    return ApplyConvolution(input, kernel_);
}

Matrix EdgeDetectionFilter::Apply(const Matrix &input) const {
    int32_t height = static_cast<int32_t>(input.size());
    int32_t width = (height > 0) ? static_cast<int32_t>(input[0].size()) : 0;
    GrayscaleFilter gs;
    Matrix grey_scale = gs.Apply(input);
    Matrix convolution = ApplyConvolution(grey_scale, kernel_);
    Matrix output = convolution;
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            float value = static_cast<float>(convolution[y][x].red) / static_cast<float>(MaxPixelValue);
            uint8_t color = (value > threshold_) ? MaxPixelValue : 0;
            output[y][x].red = output[y][x].green = output[y][x].blue = color;
        }
    }
    return output;
}

Matrix GaussianBlurFilter::Apply(const Matrix &input) const {
    Matrix output = input;
    int32_t radius = static_cast<int>(std::ceil(3 * sigma_));
    int32_t kernel_size = 2 * radius + 1;
    std::vector<float> kernel(kernel_size);
    float sigma_squared = sigma_ * sigma_;
    float sum_kernel = 0;
    for (int32_t i = -radius; i <= radius; i++) {
        float value = std::exp(-(static_cast<float>(i * i)) / (Two * sigma_squared)) / (std::sqrt(Two * Pi) * sigma_);
        kernel[i + radius] = value;
        sum_kernel += value;
    }
    for (int32_t i = 0; i < kernel_size; i++) {
        kernel[i] /= sum_kernel;
    }

    std::vector<std::vector<float>> horizontal_kernel(1, kernel);
    output = ApplyConvolution(output, horizontal_kernel);
    std::vector<std::vector<float>> vertical_kernel(kernel_size);
    for (int32_t i = 0; i < kernel_size; i++) {
        vertical_kernel[i] = {kernel[i]};
    }
    output = ApplyConvolution(output, vertical_kernel);
    return output;
}

Matrix DropEffectFilter::Apply(const Matrix &input) const {
    int32_t height = static_cast<int32_t>(input.size());
    if (height == 0) {
        return {};
    }
    int32_t width = static_cast<int32_t>(input[0].size());
    Matrix output(height, std::vector<Pixel>(width));
    float center_x = (centerx_ < 0) ? static_cast<float>(width) / Two : centerx_;
    float center_y = (centery_ < 0) ? static_cast<float>(height) / Two : centery_;
    float radius = std::min(center_x, center_y);
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            float delta_x = static_cast<float>(x) - center_x;
            float delta_y = static_cast<float>(y) - center_y;
            float distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);
            if (distance < radius && distance > 0.0f) {
                float distance_norm = distance / radius;
                float distance_new = radius * std::sin(distance_norm * HalfPi * strength_);
                float scale = distance_new / distance;
                float src_x = center_x + delta_x * scale;
                float src_y = center_y + delta_y * scale;
                int32_t sx_nearest = std::clamp(static_cast<int32_t>(std::round(src_x)), 0, width - 1);
                int32_t sy_nearest = std::clamp(static_cast<int32_t>(std::round(src_y)), 0, height - 1);
                output[y][x] = input[sy_nearest][sx_nearest];
            } else {
                output[y][x] = input[y][x];
            }
        }
    }
    return output;
}
