/*

	Function definitions for Server class

	Author: Jarod Graygo

	Special thanks to GitHub user beached for his barebones ASIO server
	His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#include "server.hpp"

// Simple way to identify each connection
int CONNECTOR_TALLY = 0;

// DEBUGGING OPTION
// #define DEBUG_MODE

// LOGGING OPTION
#define LOG_MODE

// Writes string to log file
void Server::write_to_log(string str) {
	log_writer << str;
}

// Handles closing log_writer at exit
void Server::close_log_writer() {
	log_writer.close();
}

// Handle what happens after asynchronous read
// Reads the request and passes the connection handler with request attached into write_response()
void Server::handle_read(con_handle_t con_handle, boost::system::error_code const& err, size_t bytes_transfered) {
	if (bytes_transfered > 0) {
		std::istream is(&con_handle->read_buffer);
		std::istreambuf_iterator<char> eos;
		string tempreq(std::istreambuf_iterator<char>(is), eos);
		con_handle->request = tempreq;
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
void Server::do_async_read(con_handle_t con_handle) {
	auto handler = boost::bind(&Server::handle_read, this, con_handle, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
	boost::asio::async_read_until(con_handle->socket, con_handle->read_buffer, "\r\n\r\n", handler);
}

// Handle what happens after the response is sent
// Checks if there is still binary data to be sent via isFinished parameter
// If there isn't close the connection and clear the streambuf
void Server::handle_response(con_handle_t con_handle, std::shared_ptr<string> msg_buffer, bool isFinished, boost::system::error_code const& err) {
	if (!err && isFinished) {
		std::cout << "Finished sending response to connection...\n";
		if (con_handle->socket.is_open()) {
			std::cout << "Connection will be shut down...\n";
			con_handle->read_buffer.consume(con_handle->read_buffer.size());
			con_handle->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
			std::cout << "Connection successfully shut down.\n";
		}
	}
	else if (err) {
		std::cerr << "We had an error: " << err.message() << std::endl;
		m_connections.erase(con_handle);
	}
}

// Create and send a proper HTTP response to the request
void Server::write_response(con_handle_t con_handle) {

	string req(con_handle->request);

#ifdef LOG_MODE		
	time_t now = time(0);
	char* dt = new char[26];
	ctime_s(dt, 26, &now);
	string date = dt;
	std::replace(date.begin(), date.end(), ' ', '_');
	std::replace(date.begin(), date.end(), '\n', '\0');
	date = date.substr(0, date.length() - 1);
	write_to_log(con_handle->socket.remote_endpoint().address().to_string());
	write_to_log(" - - [");
	write_to_log(date);
	write_to_log("] \"");
	string temp = split_string(req, '\n')[0];
	string log_req = temp.substr(0, temp.length() - 1);
	write_to_log(log_req);
	write_to_log("\" ");
#endif // LOG_MODE

	string reqfile = parse_get(req.c_str());
	std::tuple<string, bool, vector<unsigned char>> resinfo = formulate_response(reqfile);
	bool isBinary = std::get<1>(resinfo);
	vector<unsigned char> fileBuff = std::get<2>(resinfo);
	size_t binarySize = fileBuff.size();
	auto buff = std::make_shared<string>(std::get<0>(resinfo));

#ifdef DEBUG_MODE
	std::cout << "Request received by connection: " << split_string(req, '\n')[0] << std::endl << std::endl;
	std::cout << "RESPONSE:\n" << *buff << std::endl << std::endl;
#endif // DEBUG_MODE


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
void Server::handle_acknowledge(con_handle_t con_handle, std::shared_ptr<string> msg_buffer, boost::system::error_code const& err) {
	if (err) {
		std::cerr << "We had an error: " << err.message() << std::endl;
		m_connections.erase(con_handle);
	}
}

// Handle what happens after a function is accepted
// This function simply sends an empty acknowledgment message to the client
void Server::handle_accept(con_handle_t& con_handle, boost::system::error_code const& err) {
	if (!err) {

#ifdef DEBUG_MODE
		std::cout << "Connection from: " << con_handle->socket.remote_endpoint().address() << std::endl;
		std::cout << "Sending acknowledgement" << std::endl;
#endif // DEBUG_MODE

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
void Server::start_accept() {
	auto con_handle = m_connections.emplace(m_connections.begin(), m_ioservice);
	auto handler = boost::bind(&Server::handle_accept, this, con_handle, boost::asio::placeholders::error);
	m_acceptor.async_accept(con_handle->socket, handler);
}

// Begin listening for incoming requests on the port provided and call start_accept for each new connection
void Server::listen(uint16_t port) {
	auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
	m_acceptor.open(endpoint.protocol());
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint);
	m_acceptor.listen();
	start_accept();
}

// Begin running the Boost ioservice
void Server::run() {
#ifdef LOG_MODE
	string log_file_name = "log.txt";
	log_writer.open(log_file_name, std::ios::app);
	if (!log_writer) std::cerr << "COULD NOT CREATE LOG" << std::endl;
#endif
	m_ioservice.run();
}

// Extract the requested filename from the request and return it as a string
string Server::parse_get(const char buff[]) {

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

// Grabs the requested file and returns a tuple containing the response as a string (response contains the
// requested file if the file is in a text format), a boolean value indicating whether the file is in binary
// format, and a vector of unsigned chars containing the binary data of the file (if binary, empty otherwise)
std::tuple<string, bool, vector<unsigned char>> Server::formulate_response(string filePath) {

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
			"<p>The requested URL /" + fileName + " was not found on this server.</p>\r\n" +
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
		}

		// Begin creating response
		response = response + "HTTP/1.1 200 OK\r\n" +
			"Date: " + charTime + " EST\r\n"
			"Server : GetServer9000\r\n" +
			"Content - Type : " + content_type + "\r\n" +
			"Connection: Closed\r\n";
		char num_char[10 + sizeof(char)];

		// Convert size to char array
		sprintf_s(num_char, "%d", (int)rS.length() + (int)response.length() + (int)fileBuff.size());
#ifdef LOG_MODE
		write_to_log("200 ");
		write_to_log(num_char);
		log_writer << std::endl;
#endif // LOG_MODE
		if (binaryStatus) response = response + "accept-ranges: bytes\r\nContent-Transfer-Encoding: binary\r\n";
		response = response + "Content - Length : " + num_char + "\r\n\r\n";
		response += rS;

	}
	in.close();

	// Return a tuple containing all necessary information about the response
	return std::make_tuple(response, binaryStatus, fileBuff);
}

//////////////////////// FREE FUNCTION ////////////////////////
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