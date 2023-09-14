FROM debian:12-slim
RUN apt update && apt upgrade -y
SHELL [ "/bin/bash", "-c" ]
ENV SHELL=/bin/bash
WORKDIR /app
COPY /build/backend-cockfighting-api .
RUN chmod +x /app/backend-cockfighting-api
CMD /app/backend-cockfighting-api
