CC=gcc
CFLAGS=-Wall -g
TARGET=shell

all: $(TARGET)

$(TARGET): shell.c
	$(CC) $(CFLAGS) -o $(TARGET) shell.c

test: $(TARGET)
	chmod +x test_shell.sh
	./test_shell.sh

clean:
	rm -f $(TARGET) *.o test_output/* valgrind.log

