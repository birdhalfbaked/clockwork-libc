#!/bin/sh

TESTS=(
  "test_connection"
)

for test in "${TESTS[@]}"; do
  echo "Running $test"
  ./tests/integration/$test/run.sh
  if [ $? -ne 0 ]; then
    echo "$test failed"
    exit 1
  fi
  echo "$test passed"
done