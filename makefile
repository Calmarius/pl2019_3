.PHONY: all clean

all:
	$(MAKE) -C libpfx
	$(MAKE) -C common_pfx
	$(MAKE) -C example
	$(MAKE) -C test
	$(MAKE) -C docs

lint:
	$(MAKE) -C libpfx lint
	$(MAKE) -C common_pfx lint
	$(MAKE) -C example lint


clean:
	$(MAKE) -C libpfx clean
	$(MAKE) -C common_pfx clean
	$(MAKE) -C example clean
	$(MAKE) -C test clean
	$(MAKE) -C docs clean

