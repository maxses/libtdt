#!/bin/bash

set -e -u

files=$(find . -name "*.json")

for f in $files; do
    echo "verify $f"
    cat $f | json_verify
    # jsonschema -i $f schemas/board.json
done

echo "[$0] fin."

#---fin------------------------------------------------------------------------
