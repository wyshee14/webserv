# :globe_with_meridians: Webserv

## :dart: Introduction
This is a project to build a robust HTTP server capable of parsing NGINX-style configuration files, handling non-blocking standard I/O for multiple client connections, serving static content, supporting file uploads, handling error codes properly, and executing CGI scripts (e.g., Python or PHP) via sub-processes.

## :memo: Steps
1. Git clone this project 
```
git clone git@github.com:wyshee14/webserv.git
```
2. Go to the directory
```
cd webserv
```
3. Compile this project
```
make
```
4. Execute the program
```
.webserv <configuration_file>
```
(Note: All configuration files are stored in folder config/) \
5. Open your browser based on the port in configuration file
```
eg: http://localhost:8000
``` 
(Note: this web server can only be run on localhost) \
6. Feel free to play around on the website!
