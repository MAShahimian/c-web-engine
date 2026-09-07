CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = server

SOURCES = server.c http/http.c router/router.c

$(TARGET): $(SOURCES)
	@printf "\n"
	@printf "========================================\n"
	@printf "             C WEB ENGINE\n"
	@printf "      Mohammad Ali Shahimian\n"
	@printf "========================================\n"
	@printf "\n"
	@echo "Building project..."
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)
	@printf "\n"
	@echo "Build completed successfully."
	@printf "\n"

clean:
	rm -f $(TARGET)