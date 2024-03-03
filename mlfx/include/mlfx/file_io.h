#pragma once

#include <vector>

bool read_file(const char* filename, std::vector<uint8_t>& vec);

int write_file_binary(const char* src, size_t size, const char* path);
