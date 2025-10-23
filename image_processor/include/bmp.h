#pragma once

#include <vector>
#include <string>
#include <cstdint>

constexpr uint32_t PixelDataOffset = 54;
constexpr uint32_t HeaderSize = 40;
constexpr uint16_t BitCount = 24;
constexpr int32_t XPerMeter = 11811;
constexpr int32_t YPerMeter = 11811;

struct BMPHeader {
    char type[2] = {'B', 'M'};
    uint32_t file_size = 0;
    uint16_t reserved_first = 0;
    uint16_t reserved_second = 0;
    uint32_t offset = PixelDataOffset;
} __attribute__((__packed__));

struct BMPInfo {
    uint32_t header_size = HeaderSize;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t planes = 1;
    uint16_t bit_count = BitCount;
    uint32_t compression = 0;
    uint32_t size_image = 0;
    int32_t x_permeter = XPerMeter;
    int32_t y_permeter = YPerMeter;
    uint32_t colors_used = 0;
    uint32_t important_colors = 0;
} __attribute__((__packed__));

struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

class BMP {
public:
    void ReadBMP(const std::string &filename);
    void WriteBMP(const std::string &filename);
    const BMPHeader &GetFileHeader() const;
    void SetFileHeader(const BMPHeader &header);
    const BMPInfo &GetInfoHeader() const;
    void SetInfoHeader(const BMPInfo &info);
    const std::vector<std::vector<Pixel>> &GetMatrix() const;
    void SetMatrix(const std::vector<std::vector<Pixel>> &mat);
    int32_t GetWidth() const;
    void SetWidth(int32_t width);
    int32_t GetHeight() const;
    void SetHeight(int32_t height);
    uint16_t GetBitCount() const;
    void SetBitCount(uint16_t bits_count);

public:
    BMPHeader file_header;
    BMPInfo info_header;
    std::vector<std::vector<Pixel>> matrix;
};
