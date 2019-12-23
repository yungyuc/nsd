# Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
# BSD-style license; see COPYING

# Build modmesh Python extension (even when the timestamp is clean):
#   make
# Build verbosely:
#   make VERBOSE=1
# Build with clang-tidy
#   make USE_CLANG_TIDY=ON

HIDE_SYMBOL ?= OFF
DEBUG_SYMBOL ?= ON
MODMESH_PROFILE ?= OFF
USE_CLANG_TIDY ?= OFF
CMAKE_BUILD_TYPE ?= Release
MODMESH_ROOT ?= $(shell pwd)
CMAKE_ARGS ?=
VERBOSE ?=
ifeq ($(CMAKE_BUILD_TYPE), Debug)
	BUILD_PATH ?= build/dbg37
else
	BUILD_PATH ?= build/dev37
endif

PYTEST ?= $(shell which py.test-3)
ifeq ($(PYTEST),)
	PYTEST := $(shell which pytest)
endif
ifneq ($(VERBOSE),)
	PYTEST_OPTS ?= -v
else
	PYTEST_OPTS ?=
endif

pyextsuffix := $(shell python3-config --extension-suffix)

.PHONY: default
default: buildext

.PHONY: clean
clean:
	make -C $(BUILD_PATH) clean

.PHONY: cmakeclean
cmakeclean:
	rm -rf $(BUILD_PATH)

.PHONY: pytest
pytest: $(MODMESH_ROOT)/modmesh/_modmesh$(pyextsuffix)
	env PYTHONPATH=$(MODMESH_ROOT) $(PYTEST) $(PYTEST_OPTS) tests/

.PHONY: cmake
cmake: $(BUILD_PATH)/Makefile

$(BUILD_PATH)/_modmesh$(pyextsuffix): $(BUILD_PATH)/Makefile
	make -C $(BUILD_PATH) VERBOSE=$(VERBOSE) _modmesh
	touch $@

$(MODMESH_ROOT)/modmesh/_modmesh$(pyextsuffix): $(BUILD_PATH)/_modmesh$(pyextsuffix)
	make -C $(BUILD_PATH) VERBOSE=$(VERBOSE) _modmesh_py
	touch $@

.PHONY: buildext
buildext: $(MODMESH_ROOT)/modmesh/_modmesh$(pyextsuffix)

$(BUILD_PATH)/Makefile: CMakelists.txt Makefile
	mkdir -p $(BUILD_PATH) ; \
	cd $(BUILD_PATH) ; \
	cmake $(MODMESH_ROOT) \
		-DCMAKE_INSTALL_PREFIX=$(MODMESH_ROOT) \
		-DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
		-DHIDE_SYMBOL=$(HIDE_SYMBOL) \
		-DDEBUG_SYMBOL=$(DEBUG_SYMBOL) \
		-DMODMESH_PROFILE=$(MODMESH_PROFILE) \
		-DUSE_CLANG_TIDY=$(USE_CLANG_TIDY) \
		-DLINT_AS_ERRORS=ON \
		$(CMAKE_ARGS)
