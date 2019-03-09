SRCS := $(wildcard *.cpp)
PFX_DIR := ../libpfx
INCLUDE_DIR := $(PFX_DIR)
PFX_LIB := $(PFX_DIR)/libpfx.a

include ../common/makefile.inc

all: sample

clean:
	rm -f sample

$(PFX_LIB):
	$(MAKE) -C $(PFX_DIR)

sample: $(SRCS) $(PFX_LIB)
	$(CXX) $(CXXFLAGS) _.cpp -iquote$(INCLUDE_DIR) $(PFX_LIB) -o $@

.PHONY: all clean