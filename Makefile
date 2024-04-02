# Variable names
IMAGE_NAME ?= dbtest-c:1.0.0
APP_NAME ?= dbtest
CONTAINER_NAME ?= test-app-c
NETWORK ?= test-network
ENV_FILE ?= .env

CC=gcc
CFLAGS=-I/usr/include/postgresql
LDFLAGS=-lpq

.PHONY: all clean run image push

all: $(APP_NAME)

$(APP_NAME): $(APP_NAME).c
	echo "$(CC) $(CFLAGS) $(APP_NAME).c -o $(APP_NAME) $(LDFLAGS)"
	$(CC) $(CFLAGS) $(APP_NAME).c -o $(APP_NAME) $(LDFLAGS)

clean:
	rm -f $(APP_NAME)

# Run Docker image
run:
	docker run --rm --network=$(NETWORK) --env-file $(ENV_FILE) $(IMAGE_NAME)

# Build Docker image, passing APP_NAME as a build argument
image:
	docker build --build-arg APP_NAME=$(APP_NAME) -t $(IMAGE_NAME) .

# Upload Docker image
push:
	docker push $(IMAGE_NAME)

