SRCS := $(wildcard *.cpp)

PFX_DIR := ../libpfx
PFX_INCLUDE := $(PFX_DIR)
PFX_LIB := $(PFX_DIR)/libpfx.a

CPFX_DIR := ../common_pfx
CPFX_INCLUDE := $(CPFX_DIR)
CPFX_LIB := $(CPFX_DIR)/libcommon_pfx.a

include ../common/makefile.inc

all: sample

clean:
	rm -f sample

$(PFX_LIB):
	$(MAKE) -C $(PFX_DIR)

$(CPFX_LIB):
	$(MAKE) -C $(CPFX_DIR)

sample: $(SRCS) $(PFX_LIB) $(CPFX_LIB)
	$(CXX) $(CXXFLAGS) _example.cpp -iquote$(PFX_INCLUDE) -iquote$(CPFX_INCLUDE) $(PFX_LIB) $(CPFX_LIB) -o $@

.PHONY: all clean $(PFX_LIB) $(CPFX_LIB)
