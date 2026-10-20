std::string get_string(size_t n)
{
	std::string s;

	if (n == 0)
		return "0";

	while (n > 0)
	{
		char digit = '0' + n % 10;
		s = digit + s;
		n /= 10;
	}

	return s;
}

std::string get_path(std::string path)
{
	std::string f;

	for (size_t i = 0; i < paths.size(); ++i)
	{
		std::string p = paths[i];

		if (p.empty())
		{
			continue;
		}

		if (p == "/")
		{
			if (f.empty())
				f = p;
			continue;
		}

		if (path == p)
		{
			return p;
		}

		if (path.size() > p.size() && path.compare(0, p.size(), p) == 0 && path[p.size()] == '/')
		{
			if (p.size() > f.size())
				f = p;
		}
	}

	return f;
}

int check_method(std::vector<std::string> m, std::string met)
{
	for (size_t i = 0; i < m.size(); ++i)
	{
		if (m[i] == met)
			return 0;
	}

	return 1;
}