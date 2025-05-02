#include <iostream>
#include <vector>
#include <algorithm>
#include "httplib.h"
#include <nlohmann/json.hpp>

// In-memory database
std::vector<nlohmann::json> books = {
    {{"id", "1"}, {"title", "The C++ Programming Language"}, {"author", "Bjarne Stroustrup"}, {"year", 2013}, {"available", true}},
    {{"id", "2"}, {"title", "Effective Modern C++"}, {"author", "Scott Meyers"}, {"year", 2014}, {"available", false}}
};

// Helper functions
nlohmann::json find_book(const std::string& id) {
    auto it = std::find_if(books.begin(), books.end(), 
        [&id](const nlohmann::json& book) { return book["id"] == id; });
    return it != books.end() ? *it : nlohmann::json(nullptr);
}

void set_common_headers(httplib::Response& res) {
    res.set_header("Content-Type", "application/json");
    res.set_header("Cache-Control", "no-cache");
    res.set_header("X-API-Version", "1.0");
    res.set_header("Access-Control-Allow-Origin", "*");
}

int main() {
    httplib::Server svr;

    // CORS and OPTIONS handling
    svr.set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        if (req.method == "OPTIONS") {
            set_common_headers(res);
            res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS, HEAD");
            res.set_header("Access-Control-Allow-Headers", "Content-Type, X-Request-ID");
            res.status = 204; // No Content
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });

    // GET all books
    svr.Get("/books", [](const httplib::Request& req, httplib::Response& res) {
        set_common_headers(res);
        
        // Add pagination metadata
        nlohmann::json response = {
            {"data", books},
            {"meta", {
                {"total", books.size()},
                {"page", 1},
                {"per_page", books.size()}
            }}
        };
        
        res.set_content(response.dump(), "application/json");
    });

    // GET single book
    svr.Get(R"(/books/(\w+))", [](const httplib::Request& req, httplib::Response& res) {
        set_common_headers(res);
        std::string id = req.matches[1];
        nlohmann::json book = find_book(id);

        if (book.is_null()) {
            res.status = 404;
            res.set_content(nlohmann::json{{"error", "Book not found"}}.dump(), "application/json");
            return;
        }

        res.set_content(book.dump(), "application/json");
    });

    std::cout << "Server running at http://localhost:8080\n";
    svr.listen("localhost", 8080);
}