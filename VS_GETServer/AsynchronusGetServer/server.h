/*

    A basic asynchronous server that responds to browser get requests.
    Capable of sending a limited number of binary files and all text
    files with a properly formatted HTTP response.

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <fstream>
#include <algorithm>
#include "connection.h"

using std::vector;

class Server {
private:
    bool logging_;
    bool debugging_;
    uint16_t port_;

    std::ofstream log_writer_;
    boost::asio::io_service m_ioservice_;
    boost::asio::ip::tcp::acceptor m_acceptor_;
    std::list<Connection> m_connections_;
    using con_handle_t = std::list<Connection>::iterator;

    string parse_get(const char[]);
    std::tuple<string, bool, vector<unsigned char>> formulate_response(string);

    void write_to_standard_outputs(string);
    void close_connection(con_handle_t);
    void handle_read(con_handle_t, boost::system::error_code const&, size_t);
    void do_async_read(con_handle_t);
    void handle_response(con_handle_t, std::shared_ptr<string>, bool, boost::system::error_code const&);
    void write_response(con_handle_t);
    void handle_acknowledge(con_handle_t, std::shared_ptr<string>, boost::system::error_code const&);
    void handle_accept(con_handle_t&, boost::system::error_code const&);
    void start_accept();

public:

    Server(uint16_t prt = 8080, bool log = true, bool debug = false) : logging_(log), debugging_(debug), port_(prt), log_writer_(), m_ioservice_(), m_acceptor_(m_ioservice_), m_connections_() { }

    void run();

    bool is_running();
};

vector<string> split_string(string, char);

#endif // SERVER_H
