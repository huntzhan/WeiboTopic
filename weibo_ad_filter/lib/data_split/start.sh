#! /bin/sh

./run_filter >> tmp.log &
tail -f tmp.log