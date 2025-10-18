.PHONY: host-test-build host-test host-test-clean host-lint check-format lint test

CLANG_TIDY ?= clang-tidy
CLANG_TIDY_EXTRAS :=
CLANG_FORMAT ?= clang-format

HOST_TESTS_DIR := tests/host
HOST_TESTS_BUILD_DIR := $(HOST_TESTS_DIR)/build

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
# Add path to SDK on MacOS
  XCRUN := $(shell command -v xcrun 2>/dev/null)
  ifneq ($(XCRUN),)
    SDK := $(shell xcrun --show-sdk-path 2>/dev/null)
    ifneq ($(SDK),)
      CLANG_TIDY_EXTRAS += --extra-arg=-isysroot$(SDK)
    endif
  endif
endif

host-test-build:
	cmake -S ${HOST_TESTS_DIR} -B ${HOST_TESTS_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
	cmake --build ${HOST_TESTS_BUILD_DIR} -j

host-test: host-test-build
	ctest --test-dir ${HOST_TESTS_BUILD_DIR} --output-on-failure -V

host-test-clean:
	rm -rf ${HOST_TESTS_BUILD_DIR}

host-lint: host-test-build
	find . \
    	\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
    	-o \
    	\( -type d -name include_esp -prune \) \
    	-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_TIDY} ${CLANG_TIDY_EXTRAS} -p ${HOST_TESTS_BUILD_DIR}

check-format:
	find . \
		\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
		-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_FORMAT} --dry-run --Werror

lint: check-format host-lint
test: host-test
