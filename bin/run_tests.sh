#!/bin/bash

echo "RUNNING TESTS..."

for f in test_*; do echo "Running test ${f}"; ./$f; done
