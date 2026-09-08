#!/usr/bin/env bash

set -e

echo "========================================"
echo "      C Web Engine - Environment Setup"
echo "========================================"
echo ""

if ! command -v gcc >/dev/null 2>&1; then
    echo "GCC not found."
    echo "Installing GCC..."

    sudo apt update
    sudo apt install -y gcc
else
    echo "GCC: $(gcc --version | head -n 1)"
fi

if ! command -v make >/dev/null 2>&1; then
    echo "GNU Make not found."
    echo "Installing Make..."

    sudo apt install -y make
else
    echo "Make: $(make --version | head -n 1)"
fi

if ! command -v curl >/dev/null 2>&1; then
    echo "curl not found."
    echo "Installing curl..."

    sudo apt install -y curl
else
    echo "curl: available"
fi

if ! command -v ab >/dev/null 2>&1; then
    echo "ApacheBench not found."
    echo "Installing ApacheBench..."

    sudo apt update
    sudo apt install -y apache2-utils
else
    echo "ApacheBench: available"
fi

echo ""
echo "========================================"
echo "Environment is ready."
echo "========================================"
echo ""
echo "Next step:"
echo "    make test"
echo ""