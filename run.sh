#!/bin/bash
g++ -o /tmp/a.out index.cpp $(pkg-config --cflags --libs libpjproject)
