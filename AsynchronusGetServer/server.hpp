/*

    A basic asynchronous server that responds to browser get requests.
    Capable of sending a limited number of binary files and all text
    files with a properly formatted HTTP response.

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint> 
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <fstream>
#include <algorithm>
#include "connection.hpp"

using std::vector;

class Server {
private:
    std::ofstream log_writer;
    boost::asio::io_service m_ioservice;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::list<Connection> m_connections;
    using con_handle_t = std::list<Connection>::iterator;

public:

    Server() : m_ioservice(), m_acceptor(m_ioservice), m_connections(), log_writer() { }

    void write_to_log(string);
    void close_log_writer();
    void handle_read(con_handle_t, boost::system::error_code const&, size_t);
    void do_async_read(con_handle_t con_handle);
    void handle_response(con_handle_t, std::shared_ptr<string>, bool, boost::system::error_code const&);
    void write_response(con_handle_t con_handle);
    void handle_acknowledge(con_handle_t, std::shared_ptr<string>, boost::system::error_code const&);
    void handle_accept(con_handle_t&, boost::system::error_code const&);
    void start_accept();
    void listen(uint16_t);
    void run();
    string parse_get(const char[]);
    std::tuple<string, bool, vector<unsigned char>> formulate_response(string);
};

vector<string> split_string(string, char);

#endif // SERVER_HPP