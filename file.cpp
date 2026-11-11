int check_file(const std::string &path)
{
    return (access(path.c_str(), R_OK) == 0);
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

	body = "<html><body>\n<h1>Index of " + url + "</h1>\n";

	while ((entry = readdir(dir)) != NULL)
	{
		name = entry->d_name;

		if (name == "." || name == "..")
			continue;

		body += name;
		body += "<br>\n";
	}

	closedir(dir);
	body += "</body></html>\n";
	return resp(200, "OK", body, "", "");
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

std::string get_body(std::string reason)
{
	std::stringstream body;

	body << "<html><body><h1>" << reason << "</h1></body></html>";

	return body.str();
}

std::string err(int code)
{
	std::string body;
	std::string response;

	if (code == 400)
	{
		body = get_body("400 Bad Request");
		response = resp(400, "Bad Request", body, "", "");
		return response;
	}

	if (code == 403)
	{
		body = get_body("403 Forbidden");
		response = resp(403, "Forbidden", body, "", "");
		return response;
	}

	if (code == 404)
	{
		body = get_body("404 Not Found");
		response = resp(404, "Not Found", body, "", "");
		return response;
	}

	if (code == 413)
	{
		body = get_body("413 Payload Too Large");
		response = resp(413, "Payload Too Large", body, "", "");
		return response;
	}

	if (code == 500)
	{
		body = get_body("500 Internal Server Error");
		response = resp(500, "Internal Server Error", body, "", "");
		return response;
	}

	if (code == 501)
	{
		body = get_body("501 Not Implemented");
		return resp(501, "Not Implemented", body, "", "");
	}

	body = get_body("Error");
	response = resp(code, "Error", body, "", "");
	
	return response;
}

std::string resp(int code, const std::string &reason, const std::string &body,
                 const std::string &location, const std::string &allow)
{
    std::stringstream response;
    response << "HTTP/1.1 " << code << " " << reason << "\r\n"
             << "Content-Length: " << body.size() << "\r\n";
    if (!location.empty())
        response << "Location: " << location << "\r\n";
    if (!allow.empty())
        response << "Allow: " << allow << "\r\n";
    response << "\r\n" << body;
    return response.str();
}

int have(const std::string &path)
{
    struct stat info;

    return (stat(path.c_str(), &info) == 0);
}

int dir(const std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return 0;

    return S_ISDIR(info.st_mode);
}

int have_file(const std::string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return 0;

    return S_ISREG(info.st_mode);
}
