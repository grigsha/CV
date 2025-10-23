#include "../include/bmp.h"
#include "../include/check_bmp.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

constexpr size_t AlingmentDivisibility = 4;

void BMP::ReadBMP(const std::string &filename) {
    std::ifstream reader_stream(filename, std::ios::binary);
    if (!reader_stream) {
        throw std::runtime_error("Cannot open file with filename: " + filename);
    }
    reader_stream.read(reinterpret_cast<char *>(&file_header), sizeof(BMPHeader));
    reader_stream.read(reinterpret_cast<char *>(&info_header), sizeof(BMPInfo));
    CheckBMPHeaders(file_header, info_header);
    matrix.resize(std::abs(info_header.height), std::vector<Pixel>(info_header.width));
    size_t row_size = info_header.width * sizeof(Pixel);
    size_t padding = (AlingmentDivisibility - row_size % AlingmentDivisibility) % AlingmentDivisibility;
    for (size_t i = 0; i < info_header.height; i++) {
        reader_stream.read(reinterpret_cast<char *>(matrix[i].data()), static_cast<std::streamsize>(row_size));
        reader_stream.ignore(static_cast<std::streamsize>(padding));
    }
    if (info_header.height > 0) {
        std::reverse(matrix.begin(), matrix.end());
    }
}
void BMP::WriteBMP(const std::string &filename) {
    std::ofstream writer_stream(filename, std::ios::binary);
    if (!writer_stream) {
        throw std::runtime_error("Cannot write to file with filename: " + filename);
    }
    writer_stream.write(reinterpret_cast<char *>(&file_header), sizeof(BMPHeader));
    writer_stream.write(reinterpret_cast<char *>(&info_header), sizeof(BMPInfo));
    if (info_header.height > 0) {
        std::reverse(matrix.begin(), matrix.end());
    }
    size_t row_size = info_header.width * sizeof(Pixel);
    size_t padding = (AlingmentDivisibility - row_size % AlingmentDivisibility) % AlingmentDivisibility;
    std::vector<uint8_t> pad_bytes(padding, 0);
    for (const std::vector<Pixel> &row : matrix) {
        writer_stream.write(reinterpret_cast<const char *>(row.data()), static_cast<std::streamsize>(row_size));
        writer_stream.write(reinterpret_cast<const char *>(pad_bytes.data()), static_cast<std::streamsize>(padding));
    }
}
const BMPHeader &BMP::GetFileHeader() const {
    return file_header;
}
void BMP::SetFileHeader(const BMPHeader &header) {
    file_header = header;
}

const BMPInfo &BMP::GetInfoHeader() const {
    return info_header;
}
void BMP::SetInfoHeader(const BMPInfo &info) {
    info_header = info;
}
const std::vector<std::vector<Pixel>> &BMP::GetMatrix() const {
    return matrix;
}
void BMP::SetMatrix(const std::vector<std::vector<Pixel>> &mat) {
    matrix = mat;
}
int32_t BMP::GetWidth() const {
    return info_header.width;
}
void BMP::SetWidth(int32_t width) {
    info_header.width = width;
}
int32_t BMP::GetHeight() const {
    return info_header.height;
}
void BMP::SetHeight(int32_t height) {
    info_header.height = height;
}
uint16_t BMP::GetBitCount() const {
    return info_header.bit_count;
}
void BMP::SetBitCount(uint16_t bits_count) {
    info_header.bit_count = bits_count;
}
