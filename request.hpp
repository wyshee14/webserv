
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <cstddef>
#include <cctype>

class request
{
private:
    int r;
    int h;
    int b;
    int ce;
    int keep_alive;
    size_t content_length;
    size_t size;
    size_t data;
    size_t body_size;
    std::string amt;
    std::string method;
    std::string full;
    std::string version;
    std::string part;
    std::string query;
    std::string body;
    std::map<std::string, std::string> header;
    int check_length(const std::string &value);
    size_t get_length(const std::string &value);
    std::string lower(const std::string &value);
    std::string trim(const std::string &value);
    int check_size(char c);
    int get_value(char c);
    int contain(const std::string &value, const std::string &s);
    int check_method(const std::string &method);
    int process_req();
    int process_header();
    int process_body();

public:
    request(size_t max_body_size);
    int feed(const std::string &s);
    int is_complete();
    std::string get_method();
    std::string take_amt();
    std::string get_path();
    std::string get_body();
};

#endif
