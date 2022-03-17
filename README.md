# Boost-ASIO-Asynchronous-GET-Server

This is a basic asynchronous server utilizing Boost's ASIO to respond to get requests from a browser.
The code may be compiled using the following command:
	`clang++ main.cpp server.cpp -o server`
The code may also be openned as a Visual Studio project and built from there.

---

All files that can be accessed by the server *must* be kept in a folder called "html" in the same directory as the server executable. The folder structure should look like:

server/
├── server.exe
└── html/
    ├── index.html
    └── *other files*
    
---

The server will respond with either a 200 status, in which case the response contains the attached file, or a 404 meaning the requested file cannot be found.
Every request is logged in a local *log.txt* file that is in the same location as your executable. 
