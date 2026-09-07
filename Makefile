CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = c-web-engine

SOURCES = \
	src/server/server.c \
	src/http/http.c \
	src/router/router.c

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean banner

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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)