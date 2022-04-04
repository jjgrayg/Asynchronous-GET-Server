/*

    Connection struct for holding all the required information for processing and responding
    to requests

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using std::string;

// Connection structure that tracks the socket, streambuf, and request in string form
struct Connection {
    Connection(boost::asio::io_service& io_service) : socket_(io_service), read_buffer_() { }
    Connection(boost::asio::io_service& io_service, size_t max_buffer_size) : socket_(io_service), read_buffer_(max_buffer_size) { }
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf read_buffer_;
    string request_;
};

#endif // CONNECTION_H
