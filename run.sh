#!/bin/bash

export LD_LIBRARY_PATH=./cpprestsdk/Release/build/Binaries:$LD_LIBRARY_PATH

./build/release/multipart_parser_test
