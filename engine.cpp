std::string process(request request)
{
    std::string path = request.get_path();
    std::string method = request.get_method();

    if (request.get_error())
        return err(request.err());

    const LocationConfig *loc = s.findLocation(path);

    if (loc == NULL)
        return err(404);

    if (!loc->getRedirectTarget().empty())
        return add_loc(loc->getRedirectTarget());

	if (check_method(loc->getAllowedMethods(), method))
	{
		std::string body = get_body("405 Method Not Allowed");

		return resp(405, "Method Not Allowed", body, "", get_methods(loc->getAllowedMethods()));
	}

    if (method == "GET")
    {
        return process_get(loc->getRoot(), path, loc->getPath(), loc->getIndex(), loc->getAutoindex());
    }

    if (method == "DELETE")
    {
        return process_delete(loc->getRoot(), path, loc->getPath());
    }

    if (method == "POST")
    {
        return process_post(path, loc->getUploadDirectory(), request.get_body());
    }

    return err(501);
}

std::string process_get(const std::string &root, const std::string &path, const std::string &loc, const std::vector<std::string> &index, int autoindex)
{
    std::string part;
    std::string file;

    if (loc == "/")
        part = path;
    else
        part = path.substr(loc.size());

    file = root + part;

    if (!have(file))
        return err(404);

    if (dir(file))
    {
        for (size_t i = 0; i < index.size(); i++)
        {
            std::string indexpath = file;

            if (!indexpath.empty() && indexpath[indexpath.size() - 1] != '/')
            {
                indexpath += "/";
            }

            indexpath += index[i];

            if (!have(indexpath))
                continue;

            if (!regular_file(indexpath))
                continue;

            if (!check_file(indexpath))
                return err(403);

            std::string body = read_file(indexpath);

            if (body.empty())
            {
                struct stat st;

                if (stat(indexpath.c_str(), &st) != 0 || st.st_size != 0)
                {
                    return err(500);
                }
            }

            return resp(200, "OK", body, "", "");
        }

        if (autoindex)
            return generate(file, path);

        return err(403);
    }

    if (!check_file(file))
        return err(403);

    std::string body = read_file(file);

    if (body.empty())
    {
        struct stat st;

        if (stat(file.c_str(), &st) != 0 ||
            st.st_size != 0)
        {
            return err(500);
        }
    }

    return resp(200, "OK", body, "", "");
}

std::string process_delete(const std::string &root, const std::string &path, const std::string &loc)
{
    std::string part;
    std::string file;

    if (loc == "/")
        part = path;
    else
        part = path.substr(loc.size());

    file = root + part;

    if (!have(file))
        return err(404);

    if (dir(file))
        return err(403);

    if (remove_file(file))
        return err(500);

    return resp(204, "No Content", "", "", "");
}

std::string process_post(const std::string &path, const std::string &dir, const std::string &body)
{
    std::string file;
    std::string dst;
    size_t i;

    if (dir.empty())
        return err(500);

    if (path.empty())
        return err(400);

    i = path.size();

    while (i > 0 && path[i - 1] != '/')
        --i;

    file = path.substr(i);

    if (file.empty())
        return err(400);

    dst = dir;

    if (dst[dst.size() - 1] != '/')
        dst += "/";

    dst += file;

    if (!write_file(dst, body))
        return err(500);

    return resp(201, "Created", "", "", "");
}
