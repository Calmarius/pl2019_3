PFX_DIR := ../libpfx
PFX_LIB := $(PFX_DIR)/libpfx.a
LIBNAME := libcommon_pfx.a

include ../common/makefile.inc

SRCS := $(wildcard impl/*.cpp impl/*.hpp *.hpp)

$(LIBNAME) : $(SRCS) $(PFX_LIB)
	$(CXX) $(CXXFLAGS) -c  impl/_commonpfx.cpp -o _.o
	$(AR) rcs $(LIBNAME) _.o
	rm *.o

$(PFX_LIB): always_build
	$(MAKE) -C $(PFX_DIR)

clean:
	rm -f $(LIBNAME)

lint:
	clang-tidy impl/_commonpfx.cpp -checks=-*,cppcoreguidelines-*,modernize-*,performance-*

test: $(PFX_LIB)
	$(CXX) $(CXXFLAGS) -DUNITTEST impl/_commonpfx.cpp -o test
	./test
	rm test

.PHONY: clean test always_build
