#!/bin/bash
OUTPUT="$1";
REFERENCE="$2";
DIFF_FILE="$3";

diff -u "$REFERENCE" "$OUTPUT" > "$DIFF_FILE"'.tmp';
EXIT="$?";
cat "$DIFF_FILE"'.tmp' | tail -n +3 > "$DIFF_FILE"
rm "$DIFF_FILE"'.tmp';
exit "$EXIT";
