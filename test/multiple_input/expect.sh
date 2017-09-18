#!/bin/bash
# This script runs in the test root after the shell has exited.
# Use it to check any expectations on the state of the filesystem, etc.
# If this script returns a non-zero exit code, the test will fail.
# This script should emit a helpful message to stdout in the case of a failure
# You can delete this file if you do not need it.
