FROM 32bit/ubuntu:16.04

WORKDIR /home/app

# Install required packages
COPY benchmark.sh .
RUN apt-get -y update && apt-get -y install sysbench
RUN sh benchmark.sh | tee docker_results.txt