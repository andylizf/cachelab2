FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    clang-format \
    git \
    python3

COPY . /app

WORKDIR /app

CMD ["/bin/bash"]