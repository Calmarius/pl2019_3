
SRCS := $(wildcard *.cpp *.c *.h *.hpp)
CXXFLAGS := -Wall -Wextra -fmax-errors=1
LIBNAME := libpfx.a

ifeq ($(debug), yes)
	CXXFLAGS += -g -O0 -DDEBUG
else
	CXXFLAGS += -O5 -DNDEBUG
endif

.PHONY: all clean

all: $(LIBNAME)

clean:
	rm -f $(LIBNAME)

libpfx.a: $(SRCS)
	$(CXX) $(CXXFLAGS)  -c  _.cpp -o _.o
	$(AR) rcs $(LIBNAME) _.o
	rm *.o


