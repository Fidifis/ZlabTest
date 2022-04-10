#!/bin/bash
OUTPUT="$1";
REFERENCE="$2";
DIFF_FILE="$3";

diff -y --width=50 "$OUTPUT" "$REFERENCE" > "$DIFF_FILE"'.tmp';
EXIT_CODE="$?";

grep -v "^---" "$DIFF_FILE"'.tmp' > "$DIFF_FILE";
rm "$DIFF_FILE"'.tmp';
exit "$EXIT_CODE";
