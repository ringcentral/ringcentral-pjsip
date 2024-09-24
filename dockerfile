FROM ubuntu
LABEL maintainer="Tyler Liu (tyler.liu@ringcentral.com)"
RUN apt-get update && apt-get install -y wget build-essential pkg-config
RUN wget --no-check-certificate https://github.com/pjsip/pjproject/archive/2.10.tar.gz && tar -zxvf 2.10.tar.gz
WORKDIR /pjproject-2.10
RUN ./configure && make dep && make && make install
WORKDIR /ringcentral-pjsip
