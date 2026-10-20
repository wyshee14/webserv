int check_file(std::string path)
{
	struct stat info;

	if (stat(path.c_str(), &info) != 0)
	{
        return 0;
	}

	return S_ISREG(info.st_mode);
}

std::string read_file(std::string path)
{
	int fd;
	char buffer[8192];
	ssize_t count;
	std::string body;

	fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return "";

	while (1)
	{
		count = read(fd, buffer, sizeof(buffer));

		if (count < 0)
		{
			close(fd);
			return "";
		}

		if (count == 0)
			break;

		body.append(buffer, static_cast<size_t>(count));
	}

	close(fd);
	return body;
}

int remove_file(std::string path)
{
	if (std::remove(path.c_str()) == 0)
		return 0;

	return 1;
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

int write_file(std::string path, std::string body)
{
	int fd;
	ssize_t w;
	size_t total;

	fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		return 0;
	}

	total = 0;

	while (total < body.size())
	{
		w = write(fd, body.c_str() + total, body.size() - total);

		if (w <= 0)
		{
			close(fd);
			return 0;
		}

		total += w;
	}

	close(fd);
	return 1;
}