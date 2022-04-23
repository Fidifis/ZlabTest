#!/bin/bash
OUTPUT="$1";
REFERENCE="$2";
DIFF_FILE="$3";

diff -u "$REFERENCE" "$OUTPUT" | tail -n +3 > "$DIFF_FILE";
exit "$?";
