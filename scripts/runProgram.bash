#!/bin/bash
TIME="$1";
FILE="$2";
BIN="$3";
OUTPUT="$4";
ERRORS="$5";
TIME_FILE="$6";

START="$(date +%s%N)";

( timeout "$TIME" "$BIN" < "$FILE" > "$OUTPUT" 2> "$ERRORS" ) 2>> "$ERRORS";
EXIT="$?";

END="$(date +%s%N)";
DIFF=$(((10#$END - 10#$START)/1000000));

echo "$DIFF" > "$TIME_FILE";

exit "$EXIT";
