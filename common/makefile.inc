CXXFLAGS := -Wall -Wextra -fmax-errors=1

ifeq ($(debug), yes)
	CXXFLAGS += -g -O0 -DDEBUG
else
	CXXFLAGS += -O5 -DNDEBUG
endif
