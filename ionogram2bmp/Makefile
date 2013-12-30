GCC = gcc
G++ = g++
RM = rm -f

BINS = ionogram2bmp
OBJS = ionogram2bmp.o bmp.o

all: $(BINS)

$(BINS): $(OBJS)
	$(GCC) -o $@ $(OBJS)

%:%.c
	$(GCC) -o $@ -g $<

%.o:%.c
	$(GCC) -c $<

clean:
	$(RM) $(OBJS) $(BINS)
