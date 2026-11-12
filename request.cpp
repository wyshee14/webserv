#include "request.hpp"

int request::check_length(const std::string &value)
{
    if (value.empty())
	{
        return 1;
	}

    for (size_t i = 0; i < value.size(); ++i)
    {
        unsigned char c = value[i];

        if (!std::isdigit(c))
		{
            return 1;
		}
    }

    return 0;
}

size_t request::get_length(const std::string &value)
{
    size_t result = 0;

    for (size_t i = 0; i < value.size(); ++i)
    {
        size_t digit = value[i] - '0';

        result = result * 10 + digit;
    }

    return result;
}

std::string request::lower(const std::string &value)
{
    std::string result = value;

    for (size_t i = 0; i < result.size(); ++i)
    {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }

    return result;
}

std::string request::trim(const std::string &value)
{
    size_t begin = 0;
    size_t end = value.size();

    while (begin < end && (value[begin] == ' ' || value[begin] == '\t'))
    {
        begin++;
    }

    while (end > begin && (value[end - 1] == ' ' || value[end - 1] == '\t'))
    {
        end--;
    }

    return value.substr(begin, end - begin);
}

int request::check_size(char c)
{
    return std::isdigit(static_cast<unsigned char>(c)) ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

request::request(size_t max_body_size)
{
    r = 1;
	h = 1;
	b = 1;
	ce = 0;
	keep_alive = 1;
    content_length = 0;
	size = 0;
	data = 0;
    body_size = max_body_size;
}

int request::get_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return c - 'A' + 10;
}

int request::contain(const std::string &value, const std::string &s)
{
	std::string tmp = trim(value);

	if (lower(tmp) == lower(s))
	{
		return 0;
	}

    return 1;
}

int request::check_method(const std::string &method)
{
    return method == "GET" || method == "POST" || method == "DELETE";
}

int request::process_req()
{
    size_t p = amt.find("\r\n");

    if (p == std::string::npos)
    {
        return 0;
    }

    std::string line = amt.substr(0, p);

    amt.erase(0, p + 2);

    size_t i = line.find(' ');

    if (i == std::string::npos || i == 0)
    {
        return 1;
    }

    size_t j = line.find(' ', i + 1);

    if (j == std::string::npos || j == i + 1 || j + 1 >= line.size())
    {
        return 1;
    }

    if (line.find(' ', j + 1) != std::string::npos || line.find(9) != std::string::npos)
    {
        return 1;
    }

    method = line.substr(0, i);

    full = line.substr(i + 1, j - i - 1);

    version = line.substr(j + 1);

    if (!check_method(method))
	{
        return 1;
	}

    if (version != "HTTP/1.1")
    {
        return 1;
    }

    if (full.empty() || full[0] != '/')
    {
        return 1;
    }

    size_t k = full.find('?');

    if (k == std::string::npos)
    {
        part = full;
    }
    else
    {
        part = full.substr(0, k);

        query = full.substr(k + 1);
    }

    r = 0;

    return 0;
}

int request::process_header()
{
    while (1)
    {
        size_t p = amt.find("\r\n");

        if (p == std::string::npos)
        {
            return 0;
        }

        std::string line = amt.substr(0, p);

        amt.erase(0, p + 2);

        if (line.empty())
        {
			if (header.find("host") == header.end())
			{
				return 1;
			}

			if (header.find("content-length") != header.end())
			{
				if (check_length(header["content-length"]))
				{
					return 1;
				}

				content_length = get_length(header["content-length"]);

				if (content_length > body_size)
				{
					return 1;
				}
			}

			if (header.find("connection") != header.end())
			{
			    std::string value = lower(header["connection"]);
			
			    if (value == "close")
			        keep_alive = 0;
			}

            h = 0;
			return 0;
        }

        size_t col = line.find(':');

        if (col == std::string::npos || col == 0)
        {
            return 1;
        }

        std::string name = line.substr(0, col);

        std::string value = line.substr(col + 1);

		std::string n = lower(name);

		std::string v = trim(value);

		for (size_t i = 0; i < v.size(); ++i)
		{
			unsigned char c = v[i];

			if ((c < 32 && c != 9) || c == 127)
			{
				return 1;
			}
		}

		if (header.find(n) != header.end())
		{
			return 1;
		}

		header[n] = v;
    }
}

int request::process_body()
{
	if (body.size() == content_length)
	{
		b = 0;
		return 0;
	}

	if (amt.empty())
	{
		return 0;
	}

	size_t amount = content_length - body.size();

	if (amt.size() < amount)
	{
		amount = amt.size();
	}

	std::string piece = amt.substr(0, amount);

	amt.erase(0, amount);

	body.append(piece);

	if (body.size() == content_length)
	{
		b = 0;
	}

    return 0;
}

int request::feed(const std::string &s)
{
    amt += s;

	if (r)
	{
		if (process_req())
		{
			return 1;
		}

		if (r)
		{
			return 0;
		}
	}

	if (h)
	{
		if (process_header())
		{
			return 1;
		}

		if (h)
			return 0;
	}

	if (content_length == 0)
	{
		b = 0;
		return 0;
	}

	if (b)
	{
		if (process_body())
		{
			return 1;
		}

		if (b)
		{
			return 0;
		}
	}

    return 0;
}

int request::get_keep_alive()
{
    return keep_alive;
}

int request::is_complete()
{
    return (!r && !h && !b);
}

std::string request::get_method()
{
    return method;
}

std::string request::get_path()
{
    return part;
}

std::string request::get_body()
{
    return body;
}
