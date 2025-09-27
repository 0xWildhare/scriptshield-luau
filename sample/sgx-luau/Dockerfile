# Dockerfile for building and running ScriptShield SGX-Luau with Gramine SGX support
FROM gramineproject/gramine:stable-jammy AS builder

# Install minimal dependencies - keep this layer small
RUN apt-get update && apt-get install -y --no-install-recommends \
  pkg-config \
  libssl-dev \
  build-essential \
  curl \
  ca-certificates \
  libclang-dev \
  libsgx-dcap-quote-verify \
  libsgx-dcap-quote-verify-dev \
  gcc \
  g++ \
  cmake \
  make \
  && rm -rf /var/lib/apt/lists/* \
  && gcc --version && g++ --version

# Set deterministic build environment variables (CRITICAL for consistent mr_enclave)
ENV SOURCE_DATE_EPOCH=1640995200

# Locale and internationalization settings
ENV LC_ALL=C
ENV LC_CTYPE=C
ENV LANG=C
ENV LC_NUMERIC=C
ENV LC_TIME=C
ENV LC_COLLATE=C
ENV LC_MONETARY=C
ENV LC_MESSAGES=C

# Time zone setting
ENV TZ=UTC

# Temporary directories
ENV TMPDIR=/tmp
ENV TEMP=/tmp
ENV TMP=/tmp

# Compiler settings
ENV CC=gcc
ENV CXX=g++

# System settings for determinism
ENV MALLOC_CONF=junk:false
ENV GLIBC_TUNABLES=glibc.malloc.mmp_threshold=131072:glibc.malloc.trim_threshold=131072

# Standard environment variables
ENV HOME=/root
ENV USERPROFILE=/root
ENV USER=root
ENV USERNAME=root
ENV PWD=/workspace
ENV SHELL=/bin/bash

# Terminal and color settings
ENV NO_COLOR=""
ENV COLORTERM=truecolor
ENV TERM=xterm-256color
ENV WT_SESSION=""
ENV ITERM_SESSION_ID=""
ENV VSCODE_PID=""
ENV COLUMNS=80
ENV LINES=24
ENV CLICOLOR=1
ENV LS_COLORS="di=34:ln=35:so=32:pi=33:ex=31:bd=34;46:cd=34;43:su=30;41:sg=30;46:tw=30;42:ow=30;43"

# Generate Gramine SGX key
RUN gramine-sgx-gen-private-key

# Create directories first
WORKDIR /workspace
RUN mkdir -p /root/save && chmod 700 /root/save

# Copy the entire ScriptShield-Luau project
COPY . ./scriptshield-luau

# Build the SGX-Luau enclave with deterministic flags
WORKDIR /workspace/scriptshield-luau

# Build the SGX-Luau enclave
WORKDIR /workspace/scriptshield-luau/sample/sgx-luau
RUN make

# Copy the manifest template to final stage (we'll generate it there)
# No manifest generation here since paths will be different in final container

# Final stage - runtime image
FROM gramineproject/gramine:stable-jammy AS final

# Install minimal runtime dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    openssl \
    ca-certificates \
    gcc-multilib \
    libssl3 \
    curl \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the same deterministic environment variables in final stage
ENV LC_ALL=C
ENV LC_CTYPE=C
ENV LANG=C
ENV LC_NUMERIC=C
ENV LC_TIME=C
ENV LC_COLLATE=C
ENV LC_MONETARY=C
ENV LC_MESSAGES=C
ENV TZ=UTC
ENV TMPDIR=/tmp
ENV TEMP=/tmp
ENV TMP=/tmp
ENV SOURCE_DATE_EPOCH=1640995200
ENV HOME=/root
ENV USER=root
ENV USERNAME=root
ENV SHELL=/bin/bash

# Create necessary directories
WORKDIR /scriptshield-luau
RUN mkdir -p /root/save && chmod 700 /root/save

# Generate Gramine SGX key in final stage
RUN gramine-sgx-gen-private-key

# Copy only what's needed from the builder
COPY --from=builder /workspace/scriptshield-luau/sample/sgx-luau/app /scriptshield-luau/
COPY --from=builder /workspace/scriptshield-luau/sample/sgx-luau/scriptshield-luau.manifest.template /scriptshield-luau/

# Copy the production script for testing
COPY sample/sgx-luau/production-script.lua /production-script.lua

# Copy the start script
COPY sample/sgx-luau/start.sh /scriptshield-luau/start.sh

# Generate the Gramine manifest with correct runtime paths
RUN gramine-manifest \
    -Dentrypoint=/scriptshield-luau/app \
    -Darch_libdir=/lib/x86_64-linux-gnu \
    -Dbinary_dir=/scriptshield-luau \
    scriptshield-luau.manifest.template > scriptshield-luau.manifest

# Sign the manifest
RUN gramine-sgx-sign \
    --manifest scriptshield-luau.manifest \
    --output scriptshield-luau.manifest.sgx

# Ensure all copied files have proper permissions
RUN chmod 755 /scriptshield-luau/app && \
    chmod 644 /scriptshield-luau/scriptshield-luau.manifest.sgx && \
    chmod 644 /scriptshield-luau/scriptshield-luau.sig && \
    chmod 644 /scriptshield-luau/scriptshield-luau.manifest && \
    chmod +x /scriptshield-luau/start.sh && \
    chown -R root:root /scriptshield-luau

# Install socat for any needed socket operations
RUN apt-get update && apt-get install -y socat && rm -rf /var/lib/apt/lists/*

# Use the wrapper script as the entrypoint
ENTRYPOINT ["/scriptshield-luau/start.sh"]

# Empty CMD to prevent arguments from being passed
CMD []