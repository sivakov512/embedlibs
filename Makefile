.PHONY: host-tests-build host-tests host-tests-clean host-lint check-format lint test

CLANG_TIDY ?= clang-tidy
CLANG_FORMAT ?= clang-format

HOST_TESTS_DIR := tests/host
HOST_TESTS_BUILD_DIR := $(HOST_TESTS_DIR)/build

host-tests-build:
	cmake -S ${HOST_TESTS_DIR} -B ${HOST_TESTS_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
	cmake --build ${HOST_TESTS_BUILD_DIR} -j

host-tests: host-tests-build
	ctest --test-dir ${HOST_TESTS_BUILD_DIR} --output-on-failure -V

host-tests-clean:
	rm -rf ${HOST_TESTS_BUILD_DIR}

host-lint: host-tests-build
	find . \
    	\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
    	-o \
    	\( -type d -name include_esp -prune \) \
    	-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_TIDY} -p ${HOST_TESTS_BUILD_DIR} --header-filter=''

check-format:
	find . \
		\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
		-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_FORMAT} --dry-run --Werror

lint: check-format host-lint
test: host-tests
