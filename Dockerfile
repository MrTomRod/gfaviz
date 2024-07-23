FROM debian:bookworm as builder
MAINTAINER Thomas Roder

# install build dependencies
RUN apt-get update
RUN apt-get install -y build-essential wget
RUN apt-get install -y qtbase5-dev libqt5svg5-dev
RUN apt-get install -y git

WORKDIR /build

# clone MrTomRod/gfaviz to /tmp/gfaviz
RUN git clone https://github.com/MrTomRod/gfaviz.git
WORKDIR /build/gfaviz
RUN git checkout text-not-path

RUN qmake && make

FROM troder/gfaviz
MAINTAINER Thomas Roder

# copy the built binary from the builder image to /opt/gfaviz/gfaviz-mrtomrod
COPY --from=builder /build/gfaviz/gfaviz /opt/gfaviz/gfaviz-mrtomrod
