#!/bin/bash

BASE_URL="http://127.0.0.1:8080"
SERVER="./c-web-engine"

passed=0
failed=0

echo "========================================"
echo "       C Web Engine - Server Tests"
echo "========================================"
echo ""

echo "Starting server..."

"$SERVER" > /tmp/c-web-engine-test.log 2>&1 &
SERVER_PID=$!

echo "Server PID: $SERVER_PID"

cleanup() {
    echo ""
    echo "Stopping server..."
    kill "$SERVER_PID" 2>/dev/null
    wait "$SERVER_PID" 2>/dev/null
}

trap cleanup EXIT

echo "Waiting for server..."

for i in {1..20}; do
    if curl -s "$BASE_URL/health" > /dev/null 2>&1; then
        echo "Server is ready."
        break
    fi

    sleep 0.1
done

run_test() {
    name="$1"
    expected_status="$2"
    url="$3"
    method="$4"

    if [ "$method" = "GET" ]; then
        response=$(curl -s -o /dev/null -w "%{http_code}" "$url")
    else
        response=$(curl -s -o /dev/null -w "%{http_code}" -X "$method" "$url")
    fi

    if [ "$response" = "$expected_status" ]; then
        echo "[PASS] $name"
        ((passed++))
    else
        echo "[FAIL] $name - expected $expected_status, got $response"
        ((failed++))
    fi
}

run_test "GET /health" "200" "$BASE_URL/health" "GET"
run_test "GET /hello" "200" "$BASE_URL/hello" "GET"
run_test "GET /about" "200" "$BASE_URL/about" "GET"
run_test "GET unknown route" "404" "$BASE_URL/does-not-exist" "GET"
run_test "POST /hello" "405" "$BASE_URL/hello" "POST"

echo ""
echo "========================================"
echo "Passed: $passed"
echo "Failed: $failed"
echo "========================================"

if [ "$failed" -eq 0 ]; then
    echo "All tests passed."
    exit 0
else
    echo "Some tests failed."
    exit 1
fi