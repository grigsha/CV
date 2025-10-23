#include "../include/bmp.h"
#include "../include/check_bmp.h"
#include <stdexcept>

void CheckBMPHeaders(const BMPHeader &file_header, const BMPInfo &info_header) {
    if (file_header.type[0] != 'B' || file_header.type[1] != 'M') {
        throw std::runtime_error("Invalid BMP type. Expected 'BM'.");
    }
    if (file_header.reserved_first != 0) {
        throw std::runtime_error("Invalid reserved_first in BMPHeader. Expected 0.");
    }
    if (file_header.reserved_second != 0) {
        throw std::runtime_error("Invalid reserved_second in BMPHeader. Expected 0.");
    }
    if (file_header.offset != PixelDataOffset) {
        throw std::runtime_error("Invalid offset in BMPHeader. Expected 54.");
    }
    if (info_header.header_size != HeaderSize) {
        throw std::runtime_error("Invalid header_size in BMPInfo. Expected 40.");
    }
    if (info_header.bit_count != BitCount) {
        throw std::runtime_error("Unsupported bit_count in BMPInfo. Expected 24.");
    }
}
