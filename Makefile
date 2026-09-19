CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = c-web-engine
TEST_HTTP_PARSER = tests/test_http_parser
TEST_RESPONSE = tests/test_response
TEST_JSON = tests/test_json
TEST_CONTENT = tests/test_content

SOURCES := $(shell find src -name "*.c")

OBJECTS := $(SOURCES:.c=.o)

.PHONY: all clean banner test

all: banner $(TARGET)
	@echo ""
	@echo "========================================"
	@echo "  C Web Engine - Build Successful"
	@echo "  Author: Mohammad Ali Shahimian"
	@echo "  Version: 1.0"
	@echo "========================================"
	@echo ""

banner:
	@echo ""
	@echo "========================================"
	@echo "        C WEB ENGINE"
	@echo "        Version 1.0"
	@echo "        Mohammad Ali Shahimian"
	@echo "========================================"
	@echo ""

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

$(TEST_HTTP_PARSER): tests/test_http_parser.c \
    src/http/http_parser.c \
    src/http/http_parser.h \
    src/http/http_query.c \
    src/http/http_query.h \
    src/http/http_receiver.c \
    src/http/http_receiver.h \
    src/http/http.h \
	src/http/http_error.c \
    src/http/http_error.h
	$(CC) $(CFLAGS) \
		tests/test_http_parser.c \
		src/http/http_parser.c \
		src/http/http_query.c \
		src/http/http_receiver.c \
		src/http/http_error.c \
		-o $(TEST_HTTP_PARSER)

$(TEST_RESPONSE): tests/test_response.c \
    src/http/http_response.c \
    src/http/http_response.h
	$(CC) $(CFLAGS) \
	    tests/test_response.c \
	    src/http/http_response.c \
	    -o $(TEST_RESPONSE)

$(TEST_JSON): tests/test_json.c \
    src/http/http_json.c \
    src/http/http_json.h
	$(CC) $(CFLAGS) \
	    tests/test_json.c \
	    src/http/http_json.c \
	    -o $(TEST_JSON)

$(TEST_CONTENT): tests/test_content.c \
    src/http/http_content.c \
    src/http/http_content.h \
    src/http/http_parser.c \
    src/http/http_parser.h \
    src/http/http_query.c \
    src/http/http_query.h
	$(CC) $(CFLAGS) \
	    tests/test_content.c \
	    src/http/http_content.c \
	    src/http/http_parser.c \
	    src/http/http_query.c \
	    -o $(TEST_CONTENT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_HTTP_PARSER) $(TEST_RESPONSE) $(TEST_JSON) $(TEST_CONTENT)
	
test: $(TARGET) \
      $(TEST_HTTP_PARSER) \
      $(TEST_RESPONSE) \
      $(TEST_JSON) \
      $(TEST_CONTENT)
	  
	@./$(TEST_HTTP_PARSER)
	@./$(TEST_RESPONSE)
	@./$(TEST_JSON)
	@./$(TEST_CONTENT)
	@./tests/test_server.sh