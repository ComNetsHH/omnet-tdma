#!/usr/bin/env zsh

rm test_exec
g++ -std=c++0x ./test/main.cc -I ./test -lgtest -lgtest_main -pthread -o test_exec
./test_exec --gtest_output="xml:doc/test_report.xml"