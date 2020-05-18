.PHONY: all
all: format test build

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: clean
clean:
	rm -rf build