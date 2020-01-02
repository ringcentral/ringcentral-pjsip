FROM ubuntu
LABEL maintainer="Tyler Liu (tyler.liu@ringcentral.com)"
RUN apt-get update && apt-get install -y wget build-essential pkg-config
RUN wget https://www.pjsip.org/release/2.9/pjproject-2.9.tar.bz2 && tar -jxvf pjproject-2.9.tar.bz2
WORKDIR /pjproject-2.9
RUN ./configure && make dep && make && make install
WORKDIR /src/cpp/ringcentral-pjsip
CMD ["./run.sh"]
