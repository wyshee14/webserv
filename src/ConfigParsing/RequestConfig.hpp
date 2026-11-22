#ifndef REQUESTCONFIG_HPP
# define REQUESTCONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

class ServerConfig;

class RequestConfig
{
	private:
		std::string _path;
		const ServerConfig *_serverConfig;	// non-owning, points into Webserv::_servers
		std::string _root;
        std::vector<std::string> _indexFiles;
        std::map<int, std::string> _errorPage;
        bool _autoindex;
        size_t _clientMaxBodySize;
		bool _hasLocation;

		void setConfig();

	public:
		RequestConfig(const std::string &requestPath, const ServerConfig &serverConfig);
		RequestConfig(const RequestConfig &copy);
		RequestConfig &operator=(const RequestConfig &other);
		~RequestConfig();

		// Getter
		const std::string &getPath() const;
		const std::string &getRoot() const;
        const std::vector<std::string> &getIndex() const;
        const std::map<int, std::string> &getErrorPage() const;
        const bool &getAutoindex() const;
        const size_t &getClientMaxBodySize() const;
		const bool hasLocation() const;
};

// <-----TO BE REMOVED (FOR DEBUGGING PURPOSE ONLY)----->
void printRequestConfig(const RequestConfig &config);

#endif
