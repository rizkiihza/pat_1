docker run --rm -dit --name tecmint-web -p 8080:80 --name my-apache -v $PWD/templates:/usr/local/apache2/htdocs/ httpd:2.4
