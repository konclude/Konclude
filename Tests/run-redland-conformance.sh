#!/usr/bin/env bash
# Runs the conformance cases that need Redland. The LUBM query contains
# LOAD <file:Tests/...> commands, so the working directory must be the
# repository or source root.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BINARY="${1:-$ROOT/Release/Konclude}"
OUTPUT="${2:-$ROOT/Tests/redland-results/result.owl.xml}"
JOBS="$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)"

mkdir -p "$(dirname "$OUTPUT")"
cd "$ROOT"
"$BINARY" conformance -i Tests/redland-cases -o "$OUTPUT" -j "$JOBS"
