# Variables
EXECUTABLE=main
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic
ZIP_NAME=xlogin01.zip
SRC_DIR=.
DEST_DIR=../tests
TESTDIR=$(DEST_DIR)/testdir
SCRIPT=$(DEST_DIR)/is_it_ok.sh
OBJ_FILES=main.o lexical_analyser.o newstring.o syntactic_analysis.o

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
	$(SCRIPT) $(DEST_DIR)/$(ZIP_NAME) $(TESTDIR)

# Clean all build files
clean:
	rm -f $(DEST_DIR)/$(ZIP_NAME)
	rm -rf $(TESTDIR)
	rm -f $(OBJ_FILES) $(EXECUTABLE)