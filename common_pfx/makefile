PFX_DIR := ../libpfx
PFX_LIB := $(PFX_DIR)/libpfx.a
LIBNAME := libcommon_pfx.a

include ../common/makefile.inc

SRCS := $(wildcard impl/*.cpp impl/*.hpp *.hpp)

$(LIBNAME) : $(SRCS) $(PFX_LIB)
	$(CXX) $(CXXFLAGS) -c  impl/_commonpfx.cpp -o _.o
	$(AR) rcs $(LIBNAME) _.o
	rm *.o

$(PFX_LIB):
	$(MAKE) -C $(PFX_DIR)

clean:
	rm -f $(LIBNAME)


.PHONY: $(PFX_LIB) clean
