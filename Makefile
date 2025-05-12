.PHONY: all test clean editor client common server build

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

server:
	mkdir -p build/
	cmake -S . -B build/ \
		-DCMAKE_BUILD_TYPE=Debug \
		-DTALLER_SERVER=ON \
		-DTALLER_CLIENT=OFF \
		-DTALLER_EDITOR=OFF \
		-DTALLER_TESTS=OFF
	cmake --build build/ --target taller_server

run-server:
	./build-server/taller_server

run-tests: compile-debug
	./build/taller_tests

all: clean run-tests

clean:
	rm -Rf build/
