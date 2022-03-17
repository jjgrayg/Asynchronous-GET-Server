/*

    Connection struct for holding all the required information for processing and responding
	to requests

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp> 
#include <boost/bind.hpp>

using std::string;

// Connection structure that tracks the socket, streambuf, and request in string form
struct Connection {
	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf read_buffer;
	string request;
	Connection(boost::asio::io_service& io_service) : socket(io_service), read_buffer() { }
	Connection(boost::asio::io_service& io_service, size_t max_buffer_size) : socket(io_service), read_buffer(max_buffer_size) { }
};

#endif // CONNECTION_HPP