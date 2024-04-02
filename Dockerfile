# Global argument declaration to set the default executable name
ARG APP_NAME=helloworld

# Build stage
FROM debian:bullseye as builder

# Install build dependencies (gcc, make)
RUN apt-get update && apt-get install -y gcc make libpq-dev

WORKDIR /app
COPY . /app

# Build the application
RUN make

# Runtime stage
FROM debian:bullseye

# Re-declare the ARG in this stage to use it
ARG APP_NAME

# Install runtime library for PostgreSQL
RUN apt-get update && apt-get install -y libpq5 && rm -rf /var/lib/apt/lists/*

# Copy the built executable from the build stage to the runtime stage
COPY --from=builder /app/${APP_NAME} /app/${APP_NAME}

# Use an environment variable to facilitate executing the application
ENV APP_NAME=${APP_NAME}

WORKDIR /app

# Use the environment variable in the CMD instruction to execute the application
CMD ./${APP_NAME}

