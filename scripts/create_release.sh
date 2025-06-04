#!/usr/bin/env bash
# Package the component for release on the ESP-IDF component registry.
# Usage: ./scripts/create_release.sh [version]
# If version is 'git', the version will be taken from a git tag.
set -euo pipefail

# Ensure the HF-BNO08x submodule is up to date
git submodule update --init --recursive

VERSION="${1:-git}"

python3 -m idf_component_manager component pack \
    --name hf-bno08x \
    --version "$VERSION" \
    --dest-dir dist

