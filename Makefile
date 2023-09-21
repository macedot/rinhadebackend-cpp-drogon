.DEFAULT_GOAL := build

setup:
	$ pip install conan
	$ sudo apt-get install git gcc g++ build-essential cmake
	$ chmod +x ./scripts/build.sh
PHONY: setup

build:
	$ ./scripts/build.sh
PHONY: build

build-image:
	$ docker build --no-cache -t macedot/rinhadebackend-cpp-drogon --progress=plain -f ./Dockerfile .
PHONY: build-image

start-dev:
	$ docker compose -f docker-compose.dev.yml up -d database
	run-dev
PHONY: start-database

run-dev:
	$ export $(cat .env | xargs) && ./build/rinhadebackend-cpp-drogon
PHONY: run

stop-dev:
	$ docker compose -f docker-compose.dev.yml down
	$ docker volume rm rinhadebackend-cpp-drogon-postgres-dev-data
PHONY: stop-dev

up:
	$ docker compose -f docker-compose.yml up -d
PHONY: start-all

down:
	$ docker compose -f docker-compose.yml down
PHONY: stop-all

# run-container:
# 	$ docker run --rm --name rinhadebackend-cpp-drogon --env-file=.env -p 9998:9998 macedot/rinhadebackend-cpp-drogon
# PHONY: run-container

# stop-container:
# 	$ docker stop rinhadebackend-cpp-drogon
# PHONY: stop-container

# push-image:
# 	$ docker push macedot/rinhadebackend-cpp-drogon
# PHONY: push-image
