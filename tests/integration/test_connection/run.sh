#!/bin/sh

TEST_PROGRAM=$(mktemp)
# build the test_connection program
gcc -Iinclude -Llib -std=c99 tests/integration/test_connection/c/main.c lib/*.c lib/platform/windows/*.c -lws2_32 -o $TEST_PROGRAM

# execute the main go program in the background
go run tests/integration/test_connection/main.go &
GO_PID=$!
sleep 1
# Run the test_connection program
$TEST_PROGRAM

# get go program exit code
wait $GO_PID
GO_EXIT_CODE=$?

# ensure both programs exited successfully
if [ $GO_EXIT_CODE -ne 0 ]; then
  echo "Go program failed with exit code $GO_EXIT_CODE"
  exit 1
fi
if [ $? -ne 0 ]; then
  echo "Test_connection program failed with exit code $?"
  exit 1
fi

# remove the test_connection program
rm $TEST_PROGRAM