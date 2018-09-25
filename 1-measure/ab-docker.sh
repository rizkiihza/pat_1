#!/bin/sh

docker run --rm --net=host jordi/ab -n 10000 -c 10000 $*
