/*
    
    A basic asynchronous server that responds to browser get requests.
    Capable of sending a limited number of binary files and all text
    files with a properly formatted HTTP response.

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#include <boost/asio.hpp> 
#include <boost/bind.hpp>
#include <cstdint> 
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <fstream>

using std::vector; using std::string;

// Splits a string on the detlimiter provided into a vector of strings
vector<string> split_string(string str, char det) {
    string temp;
    bool working = true;
    vector<string> result;
    size_t begin = -1;
    size_t end = str.find(det, begin + 1);
    while (working)
    {
        if (end == (begin + 1)) end = end + 1;
        temp = str.substr(begin + 1, (end - 1) - begin);
        if (begin == -1 && end == -1) temp = str;
        if (temp[0] == det) temp = '\0';
        if (temp[0] != '\0' && temp != " ") result.push_back(temp);
        begin = str.find(det, end - 1);
        if (begin == -1) break;
        end = str.find(det, begin + 1);
        if (end == -1)
        {
            end = str.size();
        }
    }

    return result;
}

// Simple way to identify each connection for debugging
int CONNECTOR_TALLY = 0;

// Connection structure that tracks the socket, streambuf, and request in string form
struct Connection {
    int conn_id = CONNECTOR_TALLY++;
	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf read_buffer;
    string request;
	Connection(boost::asio::io_service& io_service) : socket(io_service), read_buffer() { }
	Connection(boost::asio::io_service& io_service, size_t max_buffer_size) : socket(io_service), read_buffer(max_buffer_size) { }
};

// Server class that handles incoming connections
class Server {
    private:
        boost::asio::io_service m_ioservice;
        boost::asio::ip::tcp::acceptor m_acceptor;
        std::list<Connection> m_connections;
        using con_handle_t = std::list<Connection>::iterator;

    public:

	    Server() : m_ioservice(), m_acceptor(m_ioservice), m_connections() { }

        // Handle what happens after asynchronous read
        // Reads the request and passes the connection handler with request attached into write_response()
	    void handle_read(con_handle_t con_handle, boost::system::error_code const& err, size_t bytes_transfered) { 
		    if (bytes_transfered > 0) {
			    std::istream is(&con_handle->read_buffer);
			    std::istreambuf_iterator<char> eos;
			    string tempreq(std::istreambuf_iterator<char>(is), eos);
			    con_handle->request = tempreq;
			    string line;
			    std::getline(is, line);
		    }

		    if (!err) {
			    do_async_read(con_handle);
                write_response(con_handle);
		    }
            else if (err == boost::asio::error::eof) {}
		    else {
			    std::cerr << "We had an error: " << err.message() << std::endl;
			    m_connections.erase(con_handle);
		    }
	    }

        // Perform an asynchronous read on the connecction until the end of the request marked by \r\n\r\n
	    void do_async_read(con_handle_t con_handle) {
		    auto handler = boost::bind(&Server::handle_read, this, con_handle, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		    boost::asio::async_read_until(con_handle->socket, con_handle->read_buffer, "\r\n\r\n", handler);
	    }

        // Handle what happens after the response is sent
        // Checks if there is still binary data to be sent via isFinished parameter
        // If there isn't close the connection and clear the streambuf
        void handle_response(con_handle_t con_handle, std::shared_ptr<string> msg_buffer, bool isFinished, boost::system::error_code const& err) {
            if (!err && isFinished) {
                std::cout << "Finished sending response to connection #" << con_handle->conn_id << "...\n";
                if (con_handle->socket.is_open()) {
                    std::cout << "Connection #" << con_handle->conn_id << " will be shut down...\n";
                    con_handle->read_buffer.consume(con_handle->read_buffer.size());
                    con_handle->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
                    std::cout << "Connection #" << con_handle->conn_id << " successfully shut down.\n";
                }
            }
            else if (err) {
                std::cerr << "We had an error: " << err.message() << std::endl;
                m_connections.erase(con_handle);
            }
        }

        // Create and send a proper HTTP response to the request
        void write_response(con_handle_t con_handle) {
            string req(con_handle->request.c_str());
            //std::cout << "WRITE_RESPONSE || Message Received by connection #" << con_handle->conn_id << ": " << split_string(con_handle->request, '\n')[0] << std::endl << std::endl;

            string reqfile = parse_get(con_handle->request.c_str());
            std::tuple<string, bool, vector<unsigned char>> resinfo = formulate_response(reqfile);
            bool isBinary = std::get<1>(resinfo);
            vector<unsigned char> fileBuff = std::get<2>(resinfo);
            size_t binarySize = fileBuff.size();

            auto buff = std::make_shared<string>(std::get<0>(resinfo));
            //std::cout << "RESPONSE:\n" << *buff << std::endl << std::endl;;

            auto handler = boost::bind(&Server::handle_response, this, con_handle, buff, !isBinary, boost::asio::placeholders::error);
            boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff), handler);

            if (isBinary) {
                char* fileBuffArr = new char[binarySize];
                size_t i = 0;
                for_each(fileBuff.begin(), fileBuff.end(), [&](unsigned char c) {
                    fileBuffArr[i] = c;
                    ++i;
                    });
                auto buff2 = std::make_shared<string>(fileBuffArr);
                auto handler = boost::bind(&Server::handle_response, this, con_handle, buff2, isBinary, boost::asio::placeholders::error);
                boost::asio::async_write(con_handle->socket, boost::asio::buffer(fileBuffArr, binarySize), handler);
            }
        }

        // Handle what happens after the acknowledgement is sent
        // This function does nothing unless an error has occurred
        void handle_acknowledge(con_handle_t con_handle, std::shared_ptr<string> msg_buffer, boost::system::error_code const& err) {
            if (err) {
                std::cerr << "We had an error: " << err.message() << std::endl;
                m_connections.erase(con_handle);
            }
        }

        // Handle what happens after a function is accepted
        // This function simply sends an empty acknowledgment message to the client
	    void handle_accept(con_handle_t& con_handle, boost::system::error_code const& err) {
		    if (!err) {
                //std::cout << "Connection from: " << con_handle->socket.remote_endpoint().address() << std::endl;
                //std::cout << "Sending acknowledgement" << std::endl;
			    auto buff = std::make_shared<string>("\r\n\r\n");
                auto handler = boost::bind(&Server::handle_acknowledge, this, con_handle, buff, boost::asio::placeholders::error);
                boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff), handler);
                do_async_read(con_handle);

		    }
		    else {
			    std::cerr << "We had an error: " << err.message() << std::endl;
			    m_connections.erase(con_handle);
		    }
		    start_accept();
	    }

        // Add the connection to the list and asynchronously accept it
	    void start_accept() {
		    auto con_handle = m_connections.emplace(m_connections.begin(), m_ioservice);
		    auto handler = boost::bind(&Server::handle_accept, this, con_handle, boost::asio::placeholders::error);
		    m_acceptor.async_accept(con_handle->socket, handler);
	    }

        // Begin listening for incoming requests on the port provided and call start_accept for each new connection
	    void listen(uint16_t port) {
		    auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
		    m_acceptor.open(endpoint.protocol());
		    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		    m_acceptor.bind(endpoint);
		    m_acceptor.listen();
		    start_accept();
	    }

        // Begin running the Boost ioservice
	    void run() {
		    m_ioservice.run();
	    }

        string parse_get(const char buff[]) {

            // Split the char array into parseable lines
            vector<string> reqLines = split_string(buff, '\n');

            string reqFile;

            // Getting requested filename
            vector<string> reqFileLines = split_string(reqLines[0], ' ');
            if (reqFileLines.size() > 1) {
                string fileName = reqFileLines[1].substr(1);
                if (fileName == "") {
                    fileName = "index.html";
                }
                reqFile = "html/" + fileName;
            }
            else {
                reqFile = "invalid";
            }

            return reqFile;
        }

        std::tuple<string, bool, vector<unsigned char>> formulate_response(string filePath) {

            // Create input file stream
            std::ifstream in(filePath.c_str());

            // Get current time and store in char array
            auto start = std::chrono::system_clock::now();
            std::time_t time = std::chrono::system_clock::to_time_t(start);
            char charTime[256];
            ctime_s(charTime, sizeof(charTime), &time);

            // Initialize necessary vars
            string response;
            vector<unsigned char> fileBuff;
            unsigned long imageSize = 0;
            bool binaryStatus = false;
            vector<string> vec = split_string(filePath, '.');
            string fileExtension;
            if (vec.size() > 1) {
                fileExtension = vec[1];
            }
            string fileName = vec[0];

            // If the file requested is invalid as decided by "parse_get()" return default values
            if (filePath == "invalid") return std::make_tuple(response, binaryStatus, fileBuff);

            // If file can't be found return a 404 response
            if (!in) {
                std::cerr << "Could not open " << filePath << std::endl;
                response = response + "HTTP/1.1 404 Not Found\r\n" +
                    "Date: " + charTime + " EST\r\n" +
                    "Server : GetServer9000\r\n" +
                    "Content - Length : 230\r\n" +
                    "Connection : Closed\r\n" +
                    "Content - Type : text / html; charset = iso - 8859 - 1\r\n\r\n" +
                    "<!DOCTYPE HTML>\r\n" +
                    "<html>\r\n" +
                    "<head>\r\n" +
                    "<title>404 Not Found</title>\r\n" +
                    "</head>\r\n" +
                    "<body>\r\n" +
                    "<h1>Not Found</h1>\r\n" +
                    "<p>The requested URL / " + fileName + " was not found on this server.</p>\r\n" +
                    "</body>\r\n" +
                    "</html>";
            }

            // If the file is found then parse
            else {
                string content_type;
                string rS;

                // Determine content type for header
                if (fileExtension == "html") content_type = "text / html";
                else if (fileExtension == "js") content_type = "text / javascript; charset=utf-8";
                else if (fileExtension == "css") content_type = "text / css";
                else if (fileExtension == "png") { content_type = "image / png"; binaryStatus = true; }
                else if (fileExtension == "ico") { content_type = "image / png"; binaryStatus = true; }
                else if (fileExtension == "jpg") { content_type = "image / jpeg"; binaryStatus = true; }
                else if (fileExtension == "gif") { content_type = "image / gif"; binaryStatus = true; }

                // If the file isn't a binary file then simply read into a string and convert to custom String class
                if (!binaryStatus) {
                    string contents((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
                    rS = contents;
                }

                // If the file is binary then read it in as such by opening a new file stream in binary mode
                else {
                    std::ifstream bin_file(filePath.c_str(), std::ios::binary | std::ios::in);
                    vector<unsigned char> v_buf((std::istreambuf_iterator<char>(bin_file)), (std::istreambuf_iterator<char>()));
                    fileBuff = v_buf;
                    bin_file.close();
                }

                // Begin creating response
                response = response + "HTTP/1.1 200 OK\r\n" +
                    "Date: " + charTime + " EST\r\n"
                    "Server : GetServer9000\r\n" +
                    "Content - Type : " + content_type + "\r\n" +
                    "Connection: Closed\r\n";
                char num_char[10 + sizeof(char)];
            
                // Convert size to char array
                sprintf_s(num_char, "%d", rS.length() + (int)response.length() + (int)fileBuff.size());
                if (binaryStatus) response = response + "accept-ranges: bytes\r\nContent-Transfer-Encoding: binary\r\n";
                response = response + "Content - Length : " + num_char + "\r\n\r\n";
                response += rS;

            }
            in.close();

            // Return a tuple containing all necessary information about the response
            return std::make_tuple(response, binaryStatus, fileBuff);
        }
};

int main(int, char**) {
    Server srv;
	srv.listen(8080);

	srv.run();
	return 0;
}