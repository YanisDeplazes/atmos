FROM nginx:1.24.0

# Install build tools and dependencies
RUN apt-get update && \
    apt-get install -y gcc g++ make libpcre3 libpcre3-dev zlib1g zlib1g-dev libssl-dev git curl

# Download and extract Nginx source
WORKDIR /usr/src
RUN curl -O http://nginx.org/download/nginx-1.24.0.tar.gz && \
    tar -zxvf nginx-1.24.0.tar.gz

# Clone the Push Stream module
RUN git clone https://github.com/wandenberg/nginx-push-stream-module.git

# Build the Push Stream module
WORKDIR /usr/src/nginx-1.24.0
RUN ./configure --with-compat --add-dynamic-module=../nginx-push-stream-module && \
    make modules && \
    cp objs/ngx_http_push_stream_module.so /etc/nginx/modules/

# Clean up to reduce image size
RUN apt-get remove -y gcc g++ make && apt-get autoremove -y && apt-get clean

# Copy your main nginx.conf (relative to build context)
COPY ../../config/nginx/nginx.conf /etc/nginx/nginx.conf
