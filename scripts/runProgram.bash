#!/bin/bash
TIME="$1";
FILE="$2";
BIN="$3";
OUTPUT="$4";
ERRORS="$5";
TIME_FILE="$6";

( '/usr/bin/time' -o "$TIME_FILE" -f '%E' timeout "$TIME" cat "$FILE" | "$BIN" > "$OUTPUT" 2> "$ERRORS" ) 2>> "$ERRORS";
exit "$?";
