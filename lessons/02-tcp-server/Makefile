CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = server

SOURCES = server.c http/http.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)