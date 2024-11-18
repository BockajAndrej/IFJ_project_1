# Variables
CC=gcc

# Main
EXECUTABLE=main
CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic
OBJ_FILES=main.o stack.o expression.o lexical_analyser.o newstring.o syntactic_analysis.o ADT.o

# TESTS (General)
DEST_DIR=../tests
TESTDIR=$(DEST_DIR)/testdir
TEST_SCRIPT=$(DEST_DIR)/is_it_ok.sh

# UNIT-TESTS
TEST_UNIT_CFLAGS = -I../tests/Unity/src/ -I./
# Dependent files (if something can not recognice add there that c file)
TEST_UNIT_SOURCES = ./stack.c ./newstring.c ./lexical_analyser.c ./syntactic_analysis.c ../tests/Unity/src/unity.c
TEST_UNIT_SCRIPT=$(DEST_DIR)/uni_tests.c

# ZIP
ZIP_NAME=xlogin01.zip
SRC_DIR=.

# Default target
all: $(EXECUTABLE)

# Build the executable from object files
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ_FILES)

# Rule for compiling .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create a zip file and move it to destination
zip:
	zip -r $(ZIP_NAME) $(SRC_DIR)
	mv $(ZIP_NAME) $(DEST_DIR)

# Run tests
test: zip
	mkdir -p $(TESTDIR)
	$(TEST_SCRIPT) $(DEST_DIR)/$(ZIP_NAME) $(TESTDIR)

# Run unit tests
uni_tests:
	$(CC) $(TEST_UNIT_CFLAGS) -o uni_test $(TEST_UNIT_SCRIPT) $(TEST_UNIT_SOURCES)

# Clean all build files
clean:
	rm -f $(DEST_DIR)/$(ZIP_NAME)
	rm -rf $(TESTDIR)
	rm -f $(OBJ_FILES) $(EXECUTABLE)
	rm -f uni_test