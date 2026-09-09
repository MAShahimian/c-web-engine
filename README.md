# C Web Engine

A lightweight web server built from scratch in **C**, with the goal of understanding the foundations behind modern web frameworks and HTTP servers.

The project is developed incrementally: each feature is implemented, tested, and documented before moving to the next stage.

## Current Architecture

```text
Browser / HTTP Client
        │
        ▼
   C Web Server
        │
        ├── TCP Socket
        ├── HTTP Parser
        ├── HTTP Headers
        ├── Router
        └── HTTP Response
        │
        ▼
      Client
```

## Current Features

### TCP Server

The server currently implements the basic TCP server lifecycle:

```text
socket()
   ↓
bind()
   ↓
listen()
   ↓
accept()
   ↓
recv()
   ↓
process request
   ↓
send response
   ↓
close()
```

The server listens on port `8080` and currently handles connections sequentially.

### HTTP Request Parsing

The HTTP parser currently extracts the basic Request Line:

```http
GET /hello HTTP/1.1
```

into:

```text
Method  → GET
Path    → /hello
Version → HTTP/1.1
```

Basic validation is also performed to reject incomplete Request Lines.

### HTTP Headers

HTTP request headers are parsed and stored in a structured representation.

Example:

```http
GET /hello HTTP/1.1
Host: localhost
User-Agent: curl/8.5.0
Accept: */*
Connection: close
```

The request is represented internally as:

```text
HttpRequest
├── method
├── path
├── version
└── headers[]
    ├── name
    └── value
```

The parser currently supports multiple headers and different spacing after the colon:

```http
Host: localhost
Host:    localhost
Host:localhost
```

### HTTP Header Lookup

A simple API is available for retrieving a header by name:

```c
const char *host = http_get_header(&request, "Host");
```

If the requested header does not exist, the function returns `NULL`.

### HTTP Responses

The server can generate basic HTTP responses containing:

* HTTP status code
* Status text
* Content-Type
* Content-Length
* Connection header
* Response body

Example:

```http
HTTP/1.1 200 OK
Content-Type: text/plain
Content-Length: 3
Connection: close

OK
```

### Router

The server currently contains a dedicated Router module.

Current routes:

```text
GET /hello   → 200 OK
GET /about   → 200 OK
GET /health  → 200 OK
```

Unknown routes return:

```text
404 Not Found
```

Known routes requested with an unsupported HTTP method return:

```text
405 Method Not Allowed
```

For example:

```text
POST /hello → 405
```

## Testing

The project contains both **unit tests** and **integration tests**.

### HTTP Parser Unit Tests

The parser tests currently verify:

* Request Line parsing
* Multiple HTTP headers
* Header names and values
* Different header spacing
* HTTP Header lookup
* Missing Header handling
* Invalid HTTP request rejection

### Server Integration Tests

The integration test starts the server, waits for the `/health` endpoint, executes HTTP requests using `curl`, verifies the returned status codes, and then shuts down the server.

Current integration tests include:

```text
GET /health             → 200
GET /hello              → 200
GET /about              → 200
GET unknown route       → 404
POST /hello             → 405
```

Run the complete test suite with:

```bash
make test
```

A successful run should end with:

```text
Passed: 5
Failed: 0
All tests passed.
```

## Development Environment

The project provides a setup script for Ubuntu/Debian/WSL environments.

Run:

```bash
./scripts/setup.sh
```

The setup script checks for the required development tools:

```text
GCC
GNU Make
curl
ApacheBench
```

ApacheBench is currently installed for future performance and concurrency testing. Benchmarking is intentionally postponed until the core HTTP features are more complete.

## Build

Build the project with:

```bash
make
```

Clean generated files with:

```bash
make clean
```

Run all tests with:

```bash
make test
```

## Development Roadmap

The project is being developed incrementally.

Current progress:

```text
TCP Server                  ✅
HTTP Request Line           ✅
HTTP Response               ✅
HTTP Router                 ✅
HTTP Headers                ✅
Header Lookup API           ✅
Unit Tests                  ✅
Integration Tests           ✅
Development Setup Script   ✅
```

Planned features:

```text
HTTP Header Improvements
        ↓
Query String
        ↓
Request Body
        ↓
JSON
        ↓
REST API
        ↓
Database / PostgreSQL
        ↓
Authentication
        ↓
JWT
        ↓
Middleware
        ↓
Concurrency
        ↓
Performance & Benchmarking
```

The benchmark phase will be performed after the core HTTP and application-layer features are implemented, so that performance measurements represent a meaningful version of the server rather than only experimental endpoints.

## Project Goal

The long-term goal is to build a small but functional web platform from first principles in C and JavaScript while understanding the abstractions commonly provided by modern frameworks.

Conceptually:

```text
C Web Engine        → Modern Framework Concepts

C Router            → MVC Routing
C Middleware        → Request Filters / Middleware
C Authentication    → Security Layer
C Controller        → REST Controller
C Service           → Service Layer
C Repository        → Data Access Layer
PostgreSQL Client   → Database Access
```

The purpose of the project is not to replace modern frameworks, but to understand what happens underneath them.
