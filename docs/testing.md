# Testing

C Web Engine uses automated integration tests to verify the behavior of the
running HTTP server.

## Running Tests

Build the project and run the test suite with:

```bash
make test
```

The test runner automatically:

1. Starts the C Web Engine server.
2. Waits for the /health endpoint.
3. Executes HTTP integration tests.
4. Reports passed and failed tests.
5 Stops the server after the test suite finishes.

## Current Test Suite
|Request|	Expected Status|	Purpose
|GET /health|	200 OK|	Server health check
|GET /hello|	200 OK|	Successful route
|GET /about|	200 OK|	Successful route
|GET /does-not-exist|	404 Not Found|	Unknown route
|POST /hello|	405 Method Not Allowed|	Unsupported HTTP method

## Example

A successful test run should look like:

```bash
make test

[PASS] Parse GET /hello HTTP/1.1
[PASS] Reject invalid HTTP request
========================================
       C Web Engine - Server Tests
========================================

Starting server...
Server PID: ****
Waiting for server...
Server is ready.
[PASS] GET /health
[PASS] GET /hello
[PASS] GET /about
[PASS] GET unknown route
[PASS] POST /hello

========================================
Passed: 5
Failed: 0
========================================
All tests passed.

Stopping server...
```

## Test Structure
Current integration tests are located in:

```bash
tests/
└── test_server.sh
```

The test suite communicates with the server through HTTP using curl.

## Future Tests

The test suite will gradually expand as the server architecture evolves.

Planned areas include:

- HTTP request parser unit tests
- HTTP response tests
- Router tests
- HTTP headers
- JSON responses
- REST API endpoints
- Error handling
- Authentication
- Database integration
- Concurrent request handling