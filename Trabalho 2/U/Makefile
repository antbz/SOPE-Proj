CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-unused-parameter -pthread -lpthread
OBJS = mainU1.o inputreader.o requesthandler.o ../common/utils.o
XHDRS = mainU1.h inputreader.h requesthandler.h ../common/utils.h
EXEC = U1

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	-rm $(OBJS) $(EXEC)