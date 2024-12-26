#!/bin/bash

set -e -u

files=$(find . -name "*.json")

for f in $files; do
    echo "Format $f"
    jq . $f --indent 3 > $f.new
    mv $f.new $f
done

echo "[$0] fin."

#---fin------------------------------------------------------------------------
