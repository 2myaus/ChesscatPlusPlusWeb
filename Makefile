CC = em++
CFLAGS = -Wall -Wextra -g --bind --no-entry
LFLAGS = -L . -lchesscat -s LLD_REPORT_UNDEFINED -lembind -s NO_DISABLE_EXCEPTION_CATCHING
FILENAME = webdemo.js

main: chesscatweb.cpp
	$(CC) $(CFLAGS) chesscatweb.cpp $(LFLAGS) -o $(FILENAME)

.PHONY: clean

clean:
	rm -f $(FILENAME)
