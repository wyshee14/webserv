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
			return generate(p, path);
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

std::string generate(std::string path, std::string url)
{
	DIR *dir;
	struct dirent *entry;
	std::string name;
	std::string href;
	std::string body;

	dir = opendir(path.c_str());
	if (!dir)
	{
		if (errno == EACCES)
			return err(403);
		if (errno == ENOENT)
			return err(404);
		return err(500);
	}

	body = "<html><body>\n";
	body += "<h1>Index of ";
	body += url;
	body += "</h1>\n";

	while ((entry = readdir(dir)) != NULL)
	{
		name = entry->d_name;

		if (name == "." || name == "..")
			continue;

		href = url;

		if (href.empty() || href[href.size() - 1] != '/')
			href += "/";

		href += name;
		body += "<a href=\"";
		body += href;
		body += "\">";
		body += name;
		body += "</a><br>\n";
	}

	closedir(dir);
	body += "</body></html>\n";
	return resp(200, "OK", body, "");
}
