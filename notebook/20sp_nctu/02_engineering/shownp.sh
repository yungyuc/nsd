#!/bin/bash
if [[ "$(uname)" == "Darwin" ]] ; then
  NP=${NP:-$(sysctl -n hw.ncpu)}
elif [[ "$(uname)" == "Linux" ]] ; then
  NP=${NP:-$(cat /proc/cpuinfo | grep processor | wc -l)}
else
  NP=${NP:-1}
fi
echo "NP may be set to $NP"
