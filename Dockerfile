#FROM ubuntu:latest as vaccel-release
#COPY vaccel-Release/ /vaccel
FROM ubuntu:latest as builder

#COPY --from=vaccel-release /vaccel/lib/libvaccel* /usr/local/lib/
#COPY --from=vaccel-release /vaccel/include/. /usr/local/include/
#COPY --from=vaccel-release /vaccel/share/vaccel.pc /usr/local/share/

RUN apt-get update && apt-get install -y build-essential
#RUN apt-get install -y git

RUN apt-get update && apt-get install -y libjansson-dev libjansson4
#RUN git clone https://github.com/nubificus/stdinout.git -b aarch64 && \
#    cd stdinout && \
#    make

COPY src /build
WORKDIR /build
RUN make

#FROM debian:buster-slim

#COPY --from=vaccel-release /vaccel/lib/libvaccel.so /lib/
#COPY --from=vaccel-release /vaccel/lib/libvaccel-vsock.so /lib/
#RUN chmod go+rwX /run/user
#USER $APP_USER

FROM ghcr.io/openfaas/classic-watchdog:0.1.4 as watchdog

FROM ubuntu:20.04

RUN mkdir -p /home/app

COPY --from=watchdog /fwatchdog /usr/bin/fwatchdog
RUN chmod +x /usr/bin/fwatchdog

# Add non root user
RUN adduser app && adduser app app
RUN chown app /home/app
#RUN mkdir -p /run/user
#RUN chown -R app /run/user/

WORKDIR /home/app

USER app


COPY --from=builder /build/test /test
COPY --from=builder /build/libfileread.so /lib
COPY --from=builder /build/libsgemm.so /lib
COPY --from=builder /usr/lib/x86_64-linux-gnu/libjansson.so.4 /lib
#COPY --from=builder /usr/local/lib/libvaccel* /lib/

ENV LD_LIBRARY_PATH=/lib/
#ENV VACCEL_BACKENDS=/lib/libvaccel-vsock.so 
#ENV VACCEL_VSOCK=vsock://2:2048
#ENV VACCEL_DEBUG_LEVEL=4

EXPOSE 8080

HEALTHCHECK --interval=3s CMD [ -e /tmp/.lock ] || exit 1

CMD ["fwatchdog"]
