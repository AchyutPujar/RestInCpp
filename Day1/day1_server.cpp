#include <iostream>
#include "httplib.h"
#include <nlohmann/json.hpp>

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

    // POST /sum endpoint (JSON example)
    server.Post("/sum", [](const httplib::Request& req, httplib::Response& res) {
        try {
            // Parse JSON body
            auto json_body = nlohmann::json::parse(req.body);
            
            // Validate required fields
            if (!json_body.contains("a") || !json_body.contains("b") ||
                !json_body["a"].is_number() || !json_body["b"].is_number()) {
                res.status = 400;
                res.set_content(nlohmann::json{{"error", "Invalid input: requires numeric fields 'a' and 'b'"}}.dump(), 
                              "application/json");
                return;
            }

            // Calculate sum
            double result = json_body["a"].get<double>() + json_body["b"].get<double>();
            
            // Return result
            res.set_content(nlohmann::json{{"result", result}}.dump(), "application/json");
            
        } catch (const nlohmann::json::parse_error& e) {
            res.status = 400;
            res.set_content(nlohmann::json{{"error", "Invalid JSON format"}}.dump(), "application/json");
        }
    });

    // GET /users/:id endpoint (path parameter)
    server.Get(R"(/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        auto user_id = req.matches[1].str(); // Extract ID from URL
        
        // In a real app, you would look up the user in a database
        nlohmann::json response = {
            {"id", user_id},
            {"name", "User " + user_id},
            {"email", "user" + user_id + "@pujar.com"}
        };
        
        res.set_content(response.dump(), "application/json");
    });

    // GET /auth endpoint (with auth header)
    server.Get("/auth", [](const httplib::Request& req, httplib::Response& res) {
        // Check for Authorization header
        if (!req.has_header("Authorization")) {
            res.status = 401;
            res.set_content(nlohmann::json{{"error", "Authorization header missing"}}.dump(), 
                          "application/json");
            return;
        }

        auto auth_header = req.get_header_value("Authorization");
        
        // Simple token validation (in real app, use proper JWT validation)
        if (auth_header != "Bearer abc123xyz") {
            res.status = 403;
            res.set_content(nlohmann::json{{"error", "Invalid token"}}.dump(), "application/json");
            return;
        }

        // Return protected data
        nlohmann::json response = {
            {"message", "Authenticated successfully"},
            {"user", {
                {"id", "123"},
                {"role", "admin"}
            }}
        };
        
        res.set_content(response.dump(), "application/json");
    });

    std::cout << "Server started at http://localhost:8080\n";
    server.listen("localhost", 8080);

    return 0;
}