#!/bin/bash
OUTPUT="$1";
REFERENCE="$2";
DIFF_FILE="$3";

diff -y --width=50 "$OUTPUT" "$REFERENCE" > "$DIFF_FILE";
exit "$?";
