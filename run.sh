#!/bin/bash
g++ -o /tmp/a.out $1 $(pkg-config --cflags --libs libpjproject)
export LD_LIBRARY_PATH=/pjproject-2.10/pjsip/lib:/pjproject-2.10/pjlib/lib:/pjproject-2.10/pjmedia/lib:/pjproject-2.10/pjlib-util/lib:/pjproject-2.10/pjnath/lib:/pjproject-2.10/third_party/lib 
/tmp/a.out
