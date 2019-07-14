#!/bin/bash
runcmd () {
  echo "run command: ${@:2}"
  { time "${@:2}" ; } > $1 2>&1
  echo "done; log file: $(cd "$(dirname $1)" && pwd)/$1"
}
runcmd line1.log echo "first command"
runcmd line2.log echo "second command"
