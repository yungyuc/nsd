#!/bin/bash

export CFLAGS="-I${YHROOT}/usr/${YHFLAVOR}/include $(python3.7-config --include) -I$(python3 -c 'import numpy as np; print(np.get_include())')"

jupyter notebook
