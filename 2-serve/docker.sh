#!/bin/sh

docker build -t ever .
docker run -d --rm -p 8080:8080 ever
