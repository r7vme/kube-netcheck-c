FROM alpine:3.8

ADD kube-netcheck.c .

RUN apk add --no-cache build-base && \
    gcc -Wall kube-netcheck.c -o /usr/local/bin/kube-netcheck && \
    apk del build-base

ENTRYPOINT ["/usr/local/bin/kube-netcheck"]
