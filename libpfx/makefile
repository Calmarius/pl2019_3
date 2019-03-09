
SRCS := $(wildcard *.cpp *.c *.h *.hpp)
CFLAGS := -Wall -Wextra -fmax-errors=1
LIBNAME := libpfx.a

ifeq ($(debug), yes)
	CFLAGS += -g -O0 -DDEBUG
else
	CFLAGS += -O5 -DNDEBUG
endif

.PHONY: all clean

all: $(LIBNAME)

clean:
	-rm $(LIBNAME)

libpfx.a: $(SRCS)
	g++ $(CFLAGS)  -c  _.cpp -o _.o
	ar rcs $(LIBNAME) _.o
	rm _.o


