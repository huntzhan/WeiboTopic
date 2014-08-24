#! /bin/sh

./run >> run.log &
tail -f run.log