#!/bin/bash

find logr/include examples tests benchmarks -name "*.?pp" -print0 | xargs -0 clang-format -i
