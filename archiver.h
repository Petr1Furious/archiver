#include <fstream>
#include <cstdint>
#include <vector>

#ifndef ARCHIVER_ARCHIVER_H
#define ARCHIVER_ARCHIVER_H

void Archive(const std::string_view& archive_name, const std::vector<std::string_view>& filenames);

void Unarchive(const std::string_view& archive_name);

#endif  // ARCHIVER_ARCHIVER_H
