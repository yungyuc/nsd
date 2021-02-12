include common.mk

.PHONY: default
default:
	make -C 01_mul
	make -C 02_fma
	make -C 03_omp

.PHONY: hardware
hardware:
ifeq ($(UNAME_S),Darwin)
	sysctl -a | grep machdep.cpu.*features
else ifeq ($(UNAME_S),Linux)
	grep flags /proc/cpuinfo
endif

.PHONY: clean
clean:
	make -C 01_mul clean
	make -C 02_fma clean
	make -C 03_omp clean
