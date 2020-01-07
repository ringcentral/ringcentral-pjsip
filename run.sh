#!/bin/bash
g++ -o /tmp/a.out $1 $(pkg-config --cflags --libs libpjproject)
export LD_LIBRARY_PATH=/pjproject-2.9/pjsip/lib:/pjproject-2.9/pjlib/lib:/pjproject-2.9/pjmedia/lib:/pjproject-2.9/pjlib-util/lib:/pjproject-2.9/pjnath/lib:/pjproject-2.9/third_party/lib 
/tmp/a.out
