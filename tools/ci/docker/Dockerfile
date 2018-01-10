FROM ubuntu:18.04

# Suppress "debconf: unable to initialize frontend: Dialog" messages
ENV DEBIAN_FRONTEND=noninteractive
# suppress "Warning: apt-key output should not be parsed (stdout is not a terminal)"
ENV APT_KEY_DONT_WARN_ON_DANGEROUS_USAGE=noninteractive

RUN \
  apt-get update && \
  \
  # Add 3rd party repositories
  apt-get install -y \
    apt-transport-https \
    ca-certificates \
    gnupg \
    software-properties-common \
    wget && \
  # cmake. https://apt.kitware.com/
  wget --quiet -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | apt-key add - && \
  apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
  # gcc. https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test
  add-apt-repository ppa:ubuntu-toolchain-r/test && \
  # clang tools. https://apt.llvm.org/
  wget --quiet -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
  apt-add-repository 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-10 main' && \
  \
  apt-get update && \
  \
  # install packages
  apt-get install -y \
    build-essential \
    python3-pip \
    psmisc \
    cmake \
    socat \
    gcc-9 \
    g++-9 \
    clang-10 \
    clang-format-10 \
    clang-tidy-10 \
    # install tools for remote compilation
    openssh-server \
    rsync \
    gdb \
    gdbserver \
    supervisor && \
  pip3 install conan && \
  # install the latest ninja
  wget -qO- https://github.com/ninja-build/ninja/releases/latest/download/ninja-linux.zip | gunzip > /usr/bin/ninja && \
  chmod +x /usr/bin/ninja && \
  \
  # use gcc-9 by default
  update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9 && \
  \
  # configure ssh server
  mkdir /var/run/sshd && \
  echo 'root:root' | chpasswd && \
  sed -i -E 's/#\s*PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config && \
  sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd && \
  # cleanup
  rm -rf /var/lib/apt/lists/*

COPY sshd.conf /etc/supervisor/conf.d/
COPY build.sh /
COPY entrypoint.sh /
RUN chmod +x *.sh
EXPOSE 22
ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash"]
