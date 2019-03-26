
LIBNAME := libpfx.a

SRCS := $(wildcard impl/*.cpp impl/*.c impl/*.h impl/*.hpp)
include ../common/makefile.inc

.PHONY: all clean

all: $(LIBNAME)

clean:
	rm -f $(LIBNAME)

lint:
	clang-tidy impl/_libpfx.cpp -checks=-*,cppcoreguidelines-*,modernize-*,performance-*

libpfx.a: $(SRCS)
	$(CXX) $(CXXFLAGS)  -c  impl/_libpfx.cpp -o _.o
	$(AR) rcs $(LIBNAME) _.o
	rm *.o


