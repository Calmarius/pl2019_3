SRCS := $(wildcard *.cpp)
PFX_DIR := ../libpfx
INCLUDE_DIR := $(PFX_DIR)
PFX_LIB := $(PFX_DIR)/libpfx.a
EXE_NAME := functest

include ../common/makefile.inc

all: $(EXE_NAME)

clean:
	rm -f $(EXE_NAME)

$(PFX_LIB):
	$(MAKE) -C $(PFX_DIR)

$(EXE_NAME): $(SRCS) $(PFX_LIB)
	$(CXX) $(CXXFLAGS) _.cpp -iquote$(INCLUDE_DIR) $(PFX_LIB) -o $@
	valgrind --leak-check=full --error-exitcode=42 ./$(EXE_NAME)

.PHONY: all clean $(PFX_LIB)
