CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Wno-unused-parameter -pthread -lpthread
UOBJ = U/mainU2.o U/inputreader.o
QOBJ = Q/mainQ2.o Q/inputreader.o
COBJ = common/utils.o
TARGETS = U2 Q2

all: U2 Q2

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

U2: $(UOBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(UOBJ) -o $@

Q2: $(QOBJ) $(COBJ)
	$(CC) $(CFLAGS) $(COBJ) $(QOBJ) -o $@

.PHONY: clean
clean:
	rm -f $(UOBJ) $(QOBJ) $(COBJ) $(TARGETS)
