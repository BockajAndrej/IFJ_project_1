CFLAGS=-std=c11 -Wall -Wextra -Werror -pedantic
ZIP_NAME = xlogin01.zip
SRC_DIR = .
DEST_DIR = ../tests
TESTDIR = $(DEST_DIR)/testdir
SCRIPT = $(DEST_DIR)/is_it_ok.sh

all: main 

zip:
	zip -r $(ZIP_NAME) $(SRC_DIR)
	mv $(ZIP_NAME) $(DEST_DIR)

test: zip
	mkdir -p $(TESTDIR)
	$(SCRIPT) $(DEST_DIR)/$(ZIP_NAME) $(TESTDIR)

main: main.c
	gcc $(CFLAGS) main.c -o main -lm

clean:
	rm -f $(DEST_DIR)/$(ZIP_NAME)
	rm -rf $(TESTDIR)
	rm -rf *.o main