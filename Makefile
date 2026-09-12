CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = c-web-engine
TEST_HTTP_PARSER = tests/test_http_parser

SOURCES = \
	src/server/server.c \
	src/http/http_parser.c \
	src/http/http_query.c \
	src/http/http_response.c \
	src/router/router.c
	
OBJECTS = $(SOURCES:.c=.o)

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
	src/http/http.h
	$(CC) $(CFLAGS) \
		tests/test_http_parser.c \
		src/http/http_parser.c \
		src/http/http_query.c \
		-o $(TEST_HTTP_PARSER)
		
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_HTTP_PARSER)
	
test: $(TARGET) $(TEST_HTTP_PARSER)
	@./tests/test_http_parser
	@./tests/test_server.sh