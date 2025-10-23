#pragma once

struct BMPHeader;
struct BMPInfo;

void CheckBMPHeaders(const BMPHeader &file_header, const BMPInfo &info_header);
