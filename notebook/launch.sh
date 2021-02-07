#!/bin/bash

export CFLAGS="$(python3-config --includes) -I$(python3 -c 'import numpy as np; print(np.get_include())')"

export CFLAGS="-I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers ${CFLAGS}"

export CPATH="/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/Headers"

if [ -n "${YHROOT}" ] ; then
  export CFLAGS="-I${YHROOT}/usr/${YHFLAVOR}/include ${CFLAGS}"
fi

jupyter notebook "$@"
