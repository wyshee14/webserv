int req()
{
    size_t n = amt.find("\r\n");

    if (n == std::string::npos)
    {
        return 0;
    }

    std::string line = amt.substr(0, n);

    amt.erase(0, n + 2);

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

    a = line.substr(0, i);

    b = line.substr(i + 1, j - i - 1);

    c = line.substr(j + 1);

    if (!check(a))
	{
        return 1;
	}

    if (c != "HTTP/1.0" && c != "HTTP/1.1")
    {
        return 1;
    }

    if (b.empty() || b[0] != '/')
    {
        return 1;
    }

    size_t k = b.find('?');

    if (k == std::string::npos)
    {
        p = b;
    }
    else
    {
        p = b.substr(0, k);

        q = b.substr(k + 1);
    }

    r = 0;

    return 0;
}
