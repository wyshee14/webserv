std::string get(std::string b, std::string path, std::string c, std::string i, int a)
{
	std::string part;
	std::string p;

	if (c == "/")
		part = path;
	else
		part = path.substr(c.size());

	p = b + part;

	if (!have(p))
	{
		return err(404);
	}

	if (dir(p))
	{
		if (!i.empty())
		{
			std::string r = p + "/" + i;

			if (regular_file(r))
			{
				std::string body = read_file(r);

				return resp(200, "OK", body, "");
			}
		}

		if (a)
		{
			std::string body = generate(p, path);

			if (body == "")
			{
				return err(403);
			}

			return resp(200, "OK", body, "");
		}

		return err(403);
	}

	if (!regular_file(p))
	{
		return err(403);
	}

	if (access(p.c_str(), R_OK) != 0)
	{
		return err(403);
	}

	std::string body = read_file(p);

	return resp(200, "OK", body, "");
}

std::string delete(std::string b, std::string path, std::string c)
{
	std::string part;
	std::string p;

	if (c == "/")
		part = path;
	else
		part = path.substr(c.size());

	p = b + part;

	if (!have(p))
	{
		return err(404);
	}

	if (dir(p))
	{
		return err(403);
	}

	if (remove_file(p))
	{
		return err(500);
    }

	return resp(204, "No Content", "", "");
}
