TEMP_FILE := $(shell mktemp)

test:
	gcc -Iinclude -Llib -std=c99 tests/*.c lib/*.c -o $(TEMP_FILE)
	$(TEMP_FILE)
	rm $(TEMP_FILE)

docs:
	doxygen