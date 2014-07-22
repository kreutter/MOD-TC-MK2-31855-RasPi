CC = gcc
CFLAG = -Wall -std=c99 -lwiringPi
EXECUTE = mod-tc-read-temp

all: $(EXECUTE)

$(EXECUTE): mod_tc_read_temp.c
	$(CC) $(CFLAG) mod_tc_read_temp.c -o $(EXECUTE)


clean:
	rm -rf *.o $(EXECUTE)
