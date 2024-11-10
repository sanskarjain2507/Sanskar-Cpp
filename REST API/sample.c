#include <iostream>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast; 
namespace http = beast::http;    
namespace net = boost::asio;   
using tcp = net::ip::tcp;       

void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    // Here you can implement your API logic based on the incoming request
    // For demonstration purposes, let's just echo back the received message
    res.version(req.version());
    res.result(http::status::ok);
    res.set(http::field::server, "RESTful server");
    res.set(http::field::content_type, "text/plain");
    res.body() = "hello";
    res.prepare_payload();
}

void do_session(tcp::socket& socket) {
    beast::error_code ec;
    beast::flat_buffer buffer;

    http::request<http::string_body> req;
    http::read(socket, buffer, req, ec);
    if(ec == http::error::end_of_stream)
        return;
    if(ec)
        throw beast::system_error{ec};

    http::response<http::string_body> res;
    handle_request(req, res);

    http::write(socket, res, ec);
    if(ec)
        throw beast::system_error{ec};
}

int main() {
    try {
        net::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        for(;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::thread{std::bind(&do_session, std::move(socket))}.detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
