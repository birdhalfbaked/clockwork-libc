TEMP_FILE := $(shell mktemp)

CLOCKWORK_LIB := build/libclockwork.a

build-lib:
	echo "wtf"
	@echo "########################################################"
	@echo "Building $(CLOCKWORK_LIB)"
	@echo "########################################################"
	@cd build && gcc -I../include -L../lib -std=c99 -c ../lib/*.c ../lib/platform/windows/*.c -lws2_32
	@ar rcs $(CLOCKWORK_LIB) build/*.o
	@rm build/*.o

test: build test-unit test-integration

test-unit:
	@echo "########################################################"
	@echo "Running unit tests"
	@echo "########################################################"
	@gcc -Iinclude -std=c99 tests/unit/*.c -Lbuild -lclockwork -o $(TEMP_FILE)
	@$(TEMP_FILE)
	@rm $(TEMP_FILE)

test-integration:
	@echo "########################################################"
	@echo "Running integration tests"
	@echo "########################################################"
	@./tests/integration/tests.sh

docs:
	doxygen