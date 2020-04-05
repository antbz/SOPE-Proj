CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -Wno-unused-parameter
OBJS = simpledu.o sduarg.o exreg.o
XHDRS = simpledu.h sduarg.h exreg.o
EXEC = simpledu

$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c %.h $(XHDRS)
	@$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	@-rm $(OBJS) $(EXEC) 