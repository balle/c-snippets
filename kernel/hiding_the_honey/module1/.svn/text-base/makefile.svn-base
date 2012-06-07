CC 	= gcc
CFLAGS 	= -DDEBUG -I/usr/src/kernel-headers-2.4.18-l3/include -Wall -O2
OBJECTS = main.o syscall_hook.o proc_hook.o

all: $(OBJECTS)
	ld -o getroot.o -r $(OBJECTS) 
clean: 
	rm -f *.o

clean-all: clean all
