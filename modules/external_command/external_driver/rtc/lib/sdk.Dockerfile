# base image
FROM frolvlad/alpine-glibc

LABEL author=shaohongsheng@baidu.com

ADD libwebsockets.so.14 /usr/glibc-compat/lib
ADD libssl.so.1.0.0 /usr/glibc-compat/lib
ADD libcrypto.so.1.0.0 /usr/glibc-compat/lib
ADD libturbojpeg.so.0 /usr/glibc-compat/lib
ADD libbaidurtc.so /usr/glibc-compat/lib

ADD a.cer   /tmp/bin/
ADD sdkdemo /tmp/bin/

# Build & Run in docker
# docker build -t brtc_sdk_bin:vX -f sdk.Dockerfile .
# docker run -it -v $(pwd)/../sdkdemo:/tmp/sdkdemo brtc_sdk_bin:vX 

WORKDIR /tmp/bin