CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -I/usr/include/cmocka
LIBS = `pkg-config --libs gtk+-3.0` -lcmocka
SRC = $(wildcard src/*.c)
TEST_SRC = $(wildcard test/*_test.c)
INC = -Iinclude
TARGET = tabula
TEST_TARGET = test_runner

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $(SRC) $(LIBS)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(INC) -o $(TEST_TARGET) $(TEST_SRC) $(LIBS)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
