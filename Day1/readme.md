# Day 1 - REST Server Basics with cpp-httplib

## Project Setup
1. Create a new directory for your project
2. Download httplib.h from [cpp-httplib GitHub](https://github.com/yhirose/cpp-httplib)

## Objectives:
1. Set up a basic HTTP server
2. Implement GET and POST endpoints
3. Understand HTTP status codes
4. Handle simple request/response cycles

## Files:
- `day1_server.cpp` - Main server implementation
- `CMakeLists.txt` - Build configuration

## Testing Endpoints
Open a new terminal and test with `curl`:
1. Test root endpoint:
```bash
    curl http://localhost:8080
```
2. Test greeting with parameter:
```bash
    curl 'http://localhost:8080/greet?name=Achyut'
```
3. Test POST endpoint:
```bash
    curl -X POST -d "Day 1 practice" http://localhost:8080/echo
```
4. Test invalid endpoint (should return 404):
```bash
    curl http://localhost:8080/invalid
```

## Study Questions
1. What are the key components of an HTTP request?
    - Method (GET, POST, etc.), headers, path, query parameters, and body

2. Why do we use different HTTP methods?
    - GET for retrieving data (safe, idempotent)
    - POST for creating resources (not idempotent)
    - PUT for updating resources (idempotent)
    - DELETE for removing resources (idempotent)

3. What's the purpose of HTTP status codes?
    - They indicate the result of the HTTP request:
        - 2xx: Success
        - 3xx: Redirection
        - 4xx: Client errors
        - 5xx: Server errors

3. How does cpp-httplib handle routing?
    - It provides methods like Get(), Post() that map HTTP methods to path patterns and handler functions

## Code breakdown

Let me break down the syntax and structure of the C++ REST server code using cpp-httplib, explaining each component in detail:

---

### 1. Including the Library

```cpp
#include "httplib.h"
```

This includes the single-header HTTP library that provides both server and client functionality.

---

### 2. Server Instantiation

```cpp
Server server;
```

Creates an instance of the HTTP server that will listen for incoming requests.

---

### 3. Defining Routes (Endpoints)

The basic syntax for defining routes is:

```cpp
server.Method("path", handler_function);
```

Where:

- `Method` can be `Get`, `Post`, `Put`, `Delete`, etc.
- `"path"` is the URL path (e.g., `"/"`, `"/greet"`)
- `handler_function` processes the request and generates a response

---

### 4. Handler Function Syntax

```cpp
[](const Request& req, Response& res) {
    // Handler logic here
}
```

This is a lambda function that takes:

- `Request& req`: Contains all request data (headers, body, params)
- `Response& res`: Used to build the response

---

### 5. Request Object (req)

Key properties/methods:

- `req.path`: The requested URL path
- `req.method`: HTTP method (GET, POST, etc.)
- `req.body`: The request body content
- `req.headers`: Map of request headers
- `req.has_param(key)`: Checks for query parameter
- `req.get_param_value(key)`: Gets query parameter value
- `req.path_params`: For route parameters (like `/users/:id`)

---

### 6. Response Object (res)

Key methods:

- `res.set_content(body, content_type)`: Sets response body and Content-Type
- `res.status = code`: Sets HTTP status code (default is 200)
- `res.set_header(key, value)`: Sets response headers

---

### 7. Error Handling

```cpp
server.set_error_handler([](const Request& req, Response& res) {
    // Custom error handling
});
```

- Called when no route matches the request
- Default sends 404 status

---

### 8. Starting the Server

```cpp
server.listen("host", port);
```

- Binds the server to a specific host and port
- Blocks execution until server stops