#! /bin/sh

./run_filter >> run.log &
tail -f run.log