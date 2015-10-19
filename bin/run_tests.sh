#!/bin/bash

echo "RUNNING TESTS..."

for f in test_*; do ./$f; done
