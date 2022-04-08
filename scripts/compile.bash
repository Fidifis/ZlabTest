#!/bin/bash
ARGS="$1";
BIN="$2";
SOURCE="$3";
ERRORS="$4";

g++ "$ARGS" -o "$BIN" $SOURCE 2> "$ERRORS";

exit "$?";
