.PHONY: all clean

all:
	$(MAKE) -C libpfx
	$(MAKE) -C example


clean:
	$(MAKE) -C libpfx clean
	$(MAKE) -C example clean

