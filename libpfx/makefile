
LIBNAME := libpfx.a

SRCS := $(wildcard impl/*.cpp impl/*.c impl/*.h impl/*.hpp)
include ../common/makefile.inc

.PHONY: all clean

all: $(LIBNAME)

clean:
	rm -f $(LIBNAME)

libpfx.a: $(SRCS)
	$(CXX) $(CXXFLAGS)  -c  impl/_libpfx.cpp -o _.o
	$(AR) rcs $(LIBNAME) _.o
	rm *.o


