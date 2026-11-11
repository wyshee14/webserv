
#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <sstream>
#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

int check_file(const std::string &path);
std::string read_file(std::string path);
int remove_file(std::string path);
std::string generate(std::string path, std::string url);
int write_file(std::string path, std::string body);
std::string get_body(std::string reason);
std::string err(int code);
std::string resp(int code, const std::string &reason, const std::string &body, const std::string &location, const std::string &allow);
int have(const std::string &path);
int dir(const std::string &path);
int have_file(const std::string &path);

#endif
