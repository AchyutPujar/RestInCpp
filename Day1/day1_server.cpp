#include <iostream>
#include "httplib.h"

int main() {
    // Create HTTP server
    httplib::Server server;

    // Basic GET endpoint
    server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Welcome to Day 1 of REST with C++!", "text/plain");
    });

    // GET with parameters
    server.Get("/greet", [](const httplib::Request& req, httplib::Response& res) {
        // Check for name parameter
        std::string name = req.has_param("name") 
        ? req.get_param_value("name")  // If exists
        : "Guest";                     // Default

        // Set response
        res.set_content("Hello, " + name + "!", "text/plain");
    });

    // POST endpoint
    server.Post("/echo", [](const httplib::Request& req, httplib::Response& res) {
        if (req.body.empty()) {
            res.status = 400; // Bad Request
            res.set_content("Request body is empty", "text/plain");
        } else {
            res.set_content("You sent: " + req.body, "text/plain");
        }
    });

    // 404 Not Found handler
    server.set_error_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Path " + req.path + " not found", "text/plain");
    });

    std::cout << "Server started at http://localhost:8080\n";
    server.listen("localhost", 8080);

    return 0;
}