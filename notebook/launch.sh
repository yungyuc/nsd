#!/bin/bash

export CFLAGS="-I${YHROOT}/usr/${YHFLAVOR}/include $(python3.7-config --includes) -I$(python3 -c 'import numpy as np; print(np.get_include())') -llapack"

jupyter notebook "$@"
