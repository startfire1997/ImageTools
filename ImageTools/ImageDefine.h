#pragma once

#include <filesystem>
#include <vector>

typedef unsigned char PixelData;

enum class IMAGE_FORMAT {
    YUV444,
    YUV420,
    YUV420NV12,
    YUV420NV21
};

struct ImageData {
    int m_fd = -1;
    std::filesystem::path m_filePath = "";
    long m_width = 0;
    long m_height = 0;
    PixelData* m_bufData = nullptr;
};
