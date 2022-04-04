/*

    Function definitions for Server class

    Author: Jarod Graygo

    Special thanks to GitHub user beached for his barebones ASIO server
    His base code may be found here: https://gist.github.com/beached/d2383f9b14dafcc0f585

*/

#include "server.h"


// Writes string to log file
void Server::write_to_outputs(string str) {
    writer_mutex_.lock();
    if (logging_)
        if (str.find("DEBUGGING::") == string::npos)
            log_writer_ << str;
    emit ready_write_console(QString::fromStdString("<span><span style=\"color:grey\">[" + get_current_date_and_time() + "]</span> ~ <span style=\"color:green\">SERVER</span>:: " + str + "</span"));
    writer_mutex_.unlock();
}

void Server::close_connection(con_handle_t con_handle) {
    con_handle->read_buffer_.consume(con_handle->read_buffer_.size());
    con_handle->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}

// Handle what happens after asynchronous read
// Reads the request and passes the connection handler with request attached into write_response()
void Server::handle_read(con_handle_t con_handle, boost::system::error_code const& err, size_t bytes_transfered) {
    if (bytes_transfered > 0) {
        std::istream is(&con_handle->read_buffer_);
        std::istreambuf_iterator<char> eos;
        string tempreq(std::istreambuf_iterator<char>(is), eos);
        con_handle->request_ = tempreq;
    }

    if (!err) {
        do_async_read(con_handle);
        write_response(con_handle);
    }
    else if (err == boost::asio::error::eof) {}
    else {
        write_to_outputs("<span style=\"color:red\">!!ERROR!! </span>" + err.message() + '\n');
        if (con_handle != m_connections_.end()) {
            close_connection(con_handle);
            m_connections_.erase(con_handle);
        }
    }
}

// Perform an asynchronous read on the connecction until the end of the request marked by \r\n\r\n
void Server::do_async_read(con_handle_t con_handle) {
    auto handler = boost::bind(&Server::handle_read, this, con_handle, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
    boost::asio::async_read_until(con_handle->socket_, con_handle->read_buffer_, "\r\n\r\n", handler);
}

// Handle what happens after the response is sent
// Checks if there is still binary data to be sent via isFinished parameter
// If there isn't close the connection and clear the streambuf
void Server::handle_response(con_handle_t con_handle, std::shared_ptr<string> msg_buffer, bool isFinished, boost::system::error_code const& err) {
    if (!err && isFinished) {
        if (con_handle->socket_.is_open()) {
            close_connection(con_handle);
        }
    }
    else if (err) {
        write_to_outputs("<span style=\"color:red\">!!ERROR!! </span>" + err.message() + '\n');
        if (con_handle != m_connections_.end()) {
            close_connection(con_handle);
            m_connections_.erase(con_handle);
        }
    }
}

// Create and send a proper HTTP response to the request
void Server::write_response(con_handle_t con_handle) {

    string req(con_handle->request_);

    string reqfile = parse_get(req.c_str());
    std::tuple<string, bool, vector<unsigned char>> resinfo = formulate_response(reqfile);
    bool isBinary = std::get<1>(resinfo);
    vector<unsigned char> fileBuff = std::get<2>(resinfo);
    size_t binarySize = fileBuff.size();
    auto buff = std::make_shared<string>(std::get<0>(resinfo));

    string date = get_current_date_and_time();
    std::replace(date.begin(), date.end(), ' ', '_');
    string temp = split_string(req, '\n')[0];
    string log_req = temp.substr(0, temp.length() - 1);

    string log_entry = con_handle->socket_.remote_endpoint().address().to_string() +
        " - - [" + date + "] \"" +
        log_req + "\" " + split_string(*buff, ' ')[1] +
        " " + std::to_string(binarySize + (*buff).size());
    write_to_outputs(log_entry);


    if (logging_) {
        log_writer_ << std::endl;
    }

    if (debugging_) {
        string buff_str = *buff;
        buff_str = buff_str.substr(0, buff_str.find("\r\n\r\n"));
        write_to_outputs("<span style=\"color:blue\">DEBUGGING:: </span>Request received by connection: <span style='color:blue'><i>" + split_string(req, '\n')[0] + "</i></span");
        write_to_outputs("<span style=\"color:blue\">DEBUGGING:: </span><br>==============================<br><span style='color:purple'>RESPONSE HEADER:</span><br><pre>" + buff_str + "</pre><br>==============================<br>");
    }


    auto handler = boost::bind(&Server::handle_response, this, con_handle, buff, !isBinary, boost::asio::placeholders::error);
    boost::asio::async_write(con_handle->socket_, boost::asio::buffer(*buff), handler);

    if (isBinary) {
        char* fileBuffArr = new char[binarySize];
        size_t i = 0;
        for_each(fileBuff.begin(), fileBuff.end(), [&](unsigned char c) {
            fileBuffArr[i] = c;
            ++i;
            });
        auto buff2 = std::make_shared<string>(fileBuffArr);
        auto handler = boost::bind(&Server::handle_response, this, con_handle, buff2, isBinary, boost::asio::placeholders::error);
        boost::asio::async_write(con_handle->socket_, boost::asio::buffer(fileBuffArr, binarySize), handler);
    }
}

// Handle what happens after the acknowledgement is sent
// This function does nothing unless an error has occurred
void Server::handle_acknowledge(con_handle_t con_handle, std::shared_ptr<string> msg_buffer, boost::system::error_code const& err) {

    if (debugging_) {
        if (!err)
            write_to_outputs("<span style=\"color:blue\">DEBUGGING:: </span>Acknowledgment sent");
    }
    if (err) {
        write_to_outputs("<span style=\"color:red\">!!ERROR!! </span>" + err.message() + '\n');
        if (con_handle != m_connections_.end()) {
            close_connection(con_handle);
            m_connections_.erase(con_handle);
        }
    }
}

// Handle what happens after a function is accepted
// This function simply sends an empty acknowledgment message to the client
void Server::handle_accept(con_handle_t& con_handle, boost::system::error_code const& err) {
    if (!err) {
        if (debugging_)
            write_to_outputs("<span style=\"color:blue\">DEBUGGING:: </span>New connection from: " + con_handle->socket_.remote_endpoint().address().to_string());

        auto buff = std::make_shared<string>("\r\n\r\n");
        auto handler = boost::bind(&Server::handle_acknowledge, this, con_handle, buff, boost::asio::placeholders::error);
        boost::asio::async_write(con_handle->socket_, boost::asio::buffer(*buff), handler);
        do_async_read(con_handle);

    }
    else {
        write_to_outputs("<span style=\"color:red\">!!ERROR!! </span>" + err.message() + '\n');
        if (con_handle != m_connections_.end()) {
            close_connection(con_handle);
            m_connections_.erase(con_handle);
        }
    }
    start_accept();
}

// Add the connection to the list and asynchronously accept it
void Server::start_accept() {
    auto con_handle = m_connections_.emplace(m_connections_.begin(), m_ioservice_);
    auto handler = boost::bind(&Server::handle_accept, this, con_handle, boost::asio::placeholders::error);
    m_acceptor_.async_accept(con_handle->socket_, handler);
}

// Begin running the Boost ioservice and listening for incoming requests on the port provided and call start_accept for each new connection
void Server::start() {
    if (logging_) {
        string log_file_name = "log.txt";
        log_writer_.open(log_file_name, std::ios::app);
        if (!log_writer_) write_to_outputs("<span style=\"color:red\">!!ERROR!! </span>Could not create log.\n");
    }
    write_to_outputs("Starting sever on port: \"" + std::to_string(port_) + "\"");
    auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_);
    m_acceptor_.open(endpoint.protocol());
    m_acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor_.bind(endpoint);
    m_acceptor_.listen();
    start_accept();
    io_service_thread_ = new IOServiceThread(&m_ioservice_);
    io_service_thread_->start();
}

// Stops the server and all its services
void Server::stop() {
    write_to_outputs("Stopping sever...");
    m_ioservice_.stop();
    io_service_thread_->terminate();
    io_service_thread_->wait();
    delete io_service_thread_;
    if (m_acceptor_.is_open())
        m_acceptor_.close();
    m_connections_.clear();
    log_writer_.close();
    write_to_outputs("<span style =\"color:red\"><i>Server stopped!</i></span><br>");
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

    // Get current date and time and store in char array
    auto start = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(start);
    char charTime[256];
    ctime_s(charTime, sizeof(charTime), &time);

    // Initialize necessary vars
    vector<unsigned char> fileBuff;
    bool binaryStatus = false;
    vector<string> vec = split_string(filePath, '.');
    string fileName = vec[0];
    string fileExtension;
    string response;

    if (vec.size() > 1) {
        fileExtension = vec[1];
    }

    // If the file requested is invalid as decided by "parse_get()" return default values
    if (filePath == "invalid") return std::make_tuple(response, binaryStatus, fileBuff);

    // If file can't be found return a 404 response
    if (!in) {
        write_to_outputs("<span style=\"color:red\">!!ERROR!! </span>Could not open \"" + filePath + "\"");
        response = "HTTP/1.1 404 Not Found\r\n"
            "Date: ";
        response.append(charTime)
            .append(" EST\r\n"
                    "Server : Boost-Async-GET-Server\r\n"
                    "Content - Length : 230\r\n"
                    "Connection : Closed\r\n"
                    "Content - Type : text / html; charset = iso - 8859 - 1\r\n\r\n"
                    "<!DOCTYPE HTML>\r\n"
                    "<html>\r\n"
                    "<head>\r\n"
                    "<title>404 Not Found</title>\r\n"
                    "</head>\r\n"
                    "<body>\r\n"
                    "<h1>Not Found</h1>\r\n"
                    "<p>The requested URL /")
            .append(fileName)
            .append(fileExtension == "" ? "" : "." + fileExtension)
            .append(" was not found on this server.</p>\r\n"
                    "</body>\r\n"
                    "</html>");
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
        response = "HTTP/1.1 200 OK\r\n"
                    "Date: ";
        response.append(charTime)
            .append(" EST\r\n"
                    "Server : Boost-Async-GET-Server\r\n"
                    "Content - Type : ")
            .append(content_type)
            .append("\r\n"
                    "Connection: Closed\r\n");

        // Convert size to char array
        char num_char[10 + sizeof(char)];
        sprintf_s(num_char, "%d", (int)rS.length() + (int)response.length() + (int)fileBuff.size());

        if (binaryStatus) response.append("accept-ranges: bytes\r\nContent-Transfer-Encoding: binary\r\n");
        response.append("Content - Length : ")
            .append(num_char)
            .append("\r\n\r\n")
            .append(rS);

    }
    in.close();

    // Return a tuple containing all necessary information about the response
    return std::make_tuple(response, binaryStatus, fileBuff);
}

//////////////////////// FREE FUNCTION ////////////////////////
// Splits a string on the delimiter provided into a vector of strings
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

std::string replace_all(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string get_current_date_and_time() {
    time_t now = time(0);
    char* dt = new char[26];
    ctime_s(dt, 26, &now);
    std::string date = dt;
    std::replace(date.begin(), date.end(), '\n', '\0');
    date = date.substr(0, date.length() - 1);
    return date;
}
