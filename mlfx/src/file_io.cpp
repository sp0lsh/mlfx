
#include "file_io.h"

#include <fstream>

bool read_file(const char* filename, std::vector<uint8_t>& vec) {
  // open the file:
  std::ifstream file(filename, std::ios::binary);

  // Stop eating new lines in binary mode!!!
  file.unsetf(std::ios::skipws);

  // get its size:
  std::streampos fileSize;

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // reserve capacity
  // std::vector<uint8_t> vec;
  vec.reserve(fileSize);

  // read the data:
  vec.insert(vec.begin(), std::istream_iterator<uint8_t>(file),
             std::istream_iterator<uint8_t>());

  return true;
}

int write_file_binary(const char* src, size_t size, const char* path) {
  auto file = std::fstream(path, std::ios::out | std::ios::binary);
  if (!file) {
    return -1;
  }
  file.write(src, size);
  file.close();

  return 0;
}
