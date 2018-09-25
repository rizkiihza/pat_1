FROM alpine

RUN apk add --no-cache \
    alpine-sdk \
    libev-dev

ADD . /ever
WORKDIR /ever

EXPOSE 8080

ENTRYPOINT ["make"]
