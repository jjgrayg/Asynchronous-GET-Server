#include <boost/asio.hpp> 
#include <boost/bind.hpp>
#include <cstdint> 
#include <iostream>
#include <list>
#include <memory>
#include <vector>
#include <fstream>
#include "string.hpp"

using std::vector;

struct Connection {
	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf read_buffer;
	Connection(boost::asio::io_service& io_service) : socket(io_service), read_buffer() { }
	Connection(boost::asio::io_service& io_service, size_t max_buffer_size) : socket(io_service), read_buffer(max_buffer_size) { }
    std::string request;
};

class Server {
	boost::asio::io_service m_ioservice;
	boost::asio::ip::tcp::acceptor m_acceptor;
	std::list<Connection> m_connections;
	using con_handle_t = std::list<Connection>::iterator;

public:

	Server() : m_ioservice(), m_acceptor(m_ioservice), m_connections() { }

	void handle_read(con_handle_t& con_handle, boost::system::error_code const& err, size_t bytes_transfered) {
		if (bytes_transfered > 0) {
			std::istream is(&con_handle->read_buffer);
            std::istreambuf_iterator<char> eos;
            std::string tempreq(std::istreambuf_iterator<char>(is), eos);
            con_handle->request = tempreq;
			std::string line;
			std::getline(is, line);
			std::cout << "Message Received: " << tempreq << std::endl;
		}

		if (!err) {
            handle_accept(con_handle, err);
			do_async_read(con_handle);
		}
        else if (err == boost::asio::error::eof) {}
		else {
			std::cerr << "We had an error: " << err.message() << std::endl;
			m_connections.erase(con_handle);
		}
	}

	void do_async_read(con_handle_t& con_handle) {
		auto handler = boost::bind(&Server::handle_read, this, con_handle, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_read_until(con_handle->socket, con_handle->read_buffer, "\r\n\r\n", handler);
	}

	void handle_write(con_handle_t& con_handle, std::shared_ptr<std::string> msg_buffer, boost::system::error_code const& err) {
		if (!err) {
			std::cout << "Finished sending message\n";
			if (con_handle->socket.is_open()) {
                //do_async_read(con_handle);
                //if (con_handle->request.size() == 0) {
                //    con_handle->socket.close();
                //}
			}
            //else {
            //    con_handle->socket.close();
            //}
		}
		else {
			std::cerr << "We had an error: " << err.message() << std::endl;
			m_connections.erase(con_handle);
		}
	}

	void handle_accept(con_handle_t& con_handle, boost::system::error_code const& err) {
		if (!err) {
            do_async_read(con_handle);
			std::cout << "Sending message\n";

            String reqfile = parseGet(con_handle->request.c_str());
            std::tuple<String, bool, vector<unsigned char>> resinfo = formulateResponse(reqfile);
            String res = std::get<0>(resinfo);
            bool isBinary = std::get<1>(resinfo);
            vector<unsigned char> fileBuff = std::get<2>(resinfo);
            size_t binarySize = fileBuff.size();

            if (res != "") {
                auto buff = std::make_shared<std::string>(res.getArr());
                auto handler = boost::bind(&Server::handle_write, this, con_handle, buff, boost::asio::placeholders::error);
                boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff), handler);

                std::cout << "Res: " << *buff << std::endl;

                if (isBinary) {
                    char* fileBuffArr = new char[binarySize];
                    int i = 0;
                    for_each(fileBuff.begin(), fileBuff.end(), [&](unsigned char c) {
                        fileBuffArr[i] = c;
                        ++i;
                        });
                    auto buff2 = std::make_shared<std::string>(fileBuffArr);
                    auto handler = boost::bind(&Server::handle_write, this, con_handle, buff2, boost::asio::placeholders::error);
                    boost::asio::async_write(con_handle->socket, boost::asio::buffer(fileBuffArr, binarySize), handler);
                }
            }
		}
		else {
			std::cerr << "We had an error: " << err.message() << std::endl;
			m_connections.erase(con_handle);
		}
		start_accept();
	}

	void start_accept() {
		auto con_handle = m_connections.emplace(m_connections.begin(), m_ioservice);
		auto handler = boost::bind(&Server::handle_accept, this, con_handle, boost::asio::placeholders::error);
		m_acceptor.async_accept(con_handle->socket, handler);
	}

	void listen(uint16_t port) {
		auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
		m_acceptor.open(endpoint.protocol());
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		m_acceptor.bind(endpoint);
		m_acceptor.listen();
		start_accept();
	}

	void run() {
		m_ioservice.run();
	}

    String parseGet(const char buff[]) {

        // Convert buffer to custom string class (CS2 assignment) with built-in split
        String buffStr(buff);
        vector<String> reqLines = buffStr.split('\n');
        String reqFile;

        // Getting requested filename
        vector<String> reqFileLines = reqLines[0].split(' ');
        if (reqFileLines.size() > 1) {
            String fileName = reqFileLines[1].substr(1, reqFileLines[1].length());
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

    std::tuple<String, bool, vector<unsigned char>> formulateResponse(String filePath) {

        // Create input file stream
        std::ifstream in(filePath.getArr());

        // Get current time and store in char array
        auto start = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(start);
        char charTime[256];
        ctime_s(charTime, sizeof(charTime), &time);

        // Initialize necessary vars
        String response;
        vector<unsigned char> fileBuff;
        unsigned long imageSize = 0;
        bool binaryStatus = false;
        vector<String> vec = filePath.split('.');
        String fileExtension;
        if (vec.size() > 1) {
            fileExtension = vec[1];
        }
        String fileName = vec[0];
        std::cout << filePath << std::endl;
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
            String content_type;
            String rS;

            // Determine content type for header
            if (fileExtension == "html") content_type = "text / html";
            else if (fileExtension == "js") content_type = "text / javascript; charset=utf-8";
            else if (fileExtension == "css") content_type = "text / css";
            else if (fileExtension == "png") { content_type = "image / png"; binaryStatus = true; }
            else if (fileExtension == "ico") { content_type = "image / png"; binaryStatus = true; }

            // If the file isn't a binary file then simply read into a string and convert to custom String class
            if (!binaryStatus) {
                std::string contents((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());
                rS = contents.c_str();
            }

            // If the file is binary then read it in as such by opening a new file stream in binary mode
            else {
                std::ifstream bin_file(filePath.getArr(), std::ios::binary | std::ios::in);
                std::vector<unsigned char> v_buf((std::istreambuf_iterator<char>(bin_file)), (std::istreambuf_iterator<char>()));
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
            sprintf_s(num_char, "%d", rS.length() + response.length() + (int)fileBuff.size() + 50);
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
	srv.listen(80);

	srv.run();
	return 0;
}