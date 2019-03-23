docs:
	doxygen
	@echo "*** Doxygen warnings:"
	cat doxygen_warnings.txt
	test ! -s doxygen_warnings.txt

clean:
	rm -rf html/
	rm -f doxygen_warnings.txt