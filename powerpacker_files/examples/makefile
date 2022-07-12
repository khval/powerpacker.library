
CFLAGS = -D__USE_INLINE__

objs = init.o

tests = example.elf password.elf
	

all: $(objs)  $(tests)

%.o : %.c
	gcc $(CFLAGS) $(CXXFLAGS) -c  $< -o $@

%.elf : %.c
	gcc $(CFLAGS) $(CXXFLAGS) $(objs)  $< -o $@

clean:
	delete #?(.o|.elf)

