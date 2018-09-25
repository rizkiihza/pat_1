#!/bin/sh

docker run -d --rm -v $PWD/templates:/usr/share/nginx/html:ro -p 8081:80 nginx
