#!/bin/bash
# Simple build, no cmake needed.
set -e
cd "$(dirname "$0")"
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o test_engine
echo "Built: ./test_engine"
echo "Run: ./test_engine questions.txt"
