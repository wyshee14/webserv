
#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <vector>
#include <cstddef>
#include <sys/stat.h>
#include "request.hpp"
#include "file.hpp"
#include "util.hpp"

std::string process(request req);

std::string process_get(const std::string &root, const std::string &path, const std::vector<std::string> &index, int autoindex);

std::string process_delete(const std::string &root, const std::string &path);

std::string process_post(const std::string &path, const std::string &dir, const std::string &body);

#endif
