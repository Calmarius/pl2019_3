.PHONY: all clean

all:
	$(MAKE) -C libpfx
	$(MAKE) -C example
	$(MAKE) -C test
	$(MAKE) -C docs


clean:
	$(MAKE) -C libpfx clean
	$(MAKE) -C example clean
	$(MAKE) -C test clean
	$(MAKE) -C docs clean

