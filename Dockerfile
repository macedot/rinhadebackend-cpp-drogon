FROM debian:12-slim
RUN apt update && apt upgrade -y
SHELL [ "/bin/bash", "-c" ]
ENV SHELL=/bin/bash
WORKDIR /app
COPY /build/rinhadebackend-cpp-drogon .
RUN chmod +x /app/rinhadebackend-cpp-drogon
CMD /app/rinhadebackend-cpp-drogon
