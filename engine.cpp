std::string handle(request request)
{
	std::string path = request.get_path();

	std::string method = request.get_method();
	
	std::string p = get_path(path);

	if (request.get_error())
	{
		return err(request.err());
	}

	if (p.empty())
	{
        return err(404);
	}

	if (!r.empty())
	{
		return add_loc(r);
	}

	if (check_method(m, method))
	{
		return err(405);
	}

	if (method == "GET")
	{
		return get(base, path, p, index, auto);
	}

	if (method == "DELETE")
	{
		if (path != "/uploads" &&
			!(path.size() > 9 &&
			path.compare(0, 9, "/uploads") == 0 &&
			path[9] == '/'))
		{
			return err(403);
		}

		return delete(base, path, p);
	}

	return err(405);
}

std::string get(std::string base, std::string path, std::string c, std::string i, int auto)
{
	std::string part;
	std::string p;

	if (c == "/")
		part = path;
	else
		part = path.substr(c.size());

	p = base + part;

	if (!have(p))
	{
		return err(404);
	}

	if (dir(p))
	{
		if (!i.empty())
		{
			std::string r = p + "/" + i;

			if (check_file(r))
			{
				std::string body = read_file(r);

				return resp(200, "OK", body, "");
			}
		}

		if (auto)
		{
			return generate(p, path);
		}

		return err(403);
	}

	if (!check_file(p))
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

std::string post(std::string p, std::string d, std::string bo, int u)
{
	std::string file;
	std::string dst;
	size_t i;

	if (!u)
	{
        return err(403);
	}
		
	if (d.empty())
	{
		return err(500);
	}

	if (p.empty())
	{
		return err(400);
	}

	i = p.size();

	while (i > 0 && p[i - 1] != '/')
		--i;

	file = p.substr(i);

	if (file.empty())
	{
		return err(400);
	}

	dst = d;

	if (dst[dst.size() - 1] != '/')
		dst += "/";

	dst += file;

	if (access(d.c_str(), W_OK) != 0)
	{
		return err(403);
	}

	if (!write_file(dst, bo))
	{
		return err(500);
	}

	return resp(201, "Created", "", "");
}

std::string resp(int code, std::string s, std::string body, std::string l)
{
	std::string response;

	response += "HTTP/1.1 ";
	response += get_string(code);
	response += " ";
	response += s;
	response += "\r\n";

	if (!l.empty())
	{
		response += "Location: ";
		response += l;
		response += "\r\n";
	}

	response += "Content-Length: ";
	response += get_string(body.size());
	response += "\r\n";
	response += "\r\n";
	response += body;

	return response;
}

std::string add_loc(std::string l)
{
	return resp(301, "Moved Permanently", "", l);
}

std::string bod(std::string s)
{
	std::string body;

	body += "<html><body><h1>" + s + "</h1></body></html>";

	return body;
}

std::string err(int code)
{
	std::string body;
	std::string response;

	if (code == 400)
	{
		body = bod("400 Bad Request");
		response = resp(400, "Bad Request", body, "");
		return response;
	}

	if (code == 403)
	{
		body = bod("403 Forbidden");
		response = resp(403, "Forbidden", body, "");
		return response;
	}

	if (code == 404)
	{
		body = bod("404 Not Found");
		response = resp(404, "Not Found", body, "");
		return response;
	}

	if (code == 405)
	{
		body = bod("405 Method Not Allowed");
		response = resp(405, "Method Not Allowed", body, "");
		return response;
	}

	if (code == 413)
	{
		body = bod("413 Payload Too Large");
		response = resp(413, "Payload Too Large", body, "");
		return response;
	}

	if (code == 500)
	{
		body = bod("500 Internal Server Error");
		response = resp(500, "Internal Server Error", body, "");
		return response;
	}

	body = bod("Error");
	response = resp(code, "Error", body, "");
	
	return response;
}