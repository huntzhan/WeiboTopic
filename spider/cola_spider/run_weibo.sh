#!/bin/bash - 
#===============================================================================
#
#          FILE: run.sh
# 
#         USAGE: ./run.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Dr. Fritz Mehner (fgm), mehner.fritz@fh-swf.de
#  ORGANIZATION: FH Südwestfalen, Iserlohn, Germany
#       CREATED: 07/31/2014 19:50
#      REVISION:  ---
#===============================================================================

TEMP_PYTHONPATH=$PYTHONPATH
PYTHONPATH=$(pwd):$PYTHONPATH
echo $PYTHONPATH
export PYTHONPATH

python $(pwd)/contrib/weibo/__init__.py

unset PYTHONPATH
PYTHONPATH=$TEMP_PYTHONPATH
export PYTHONPATH

# mongo --eval "db.getSiblingDB('admin').shutdownServer()"
