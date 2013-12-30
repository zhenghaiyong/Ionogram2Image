GCC = gcc
G++ = g++
RM = rm -f

BINS = ionogram2raw

all: $(BINS) 

%:%.c
	$(GCC) -o $@ -g $<

clean:
	$(RM) $(BINS)
