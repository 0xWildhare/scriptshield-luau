#!/bin/bash
set -e

echo "ScriptShield-Luau SGX Starting..."
echo "Environment: $SCRIPTSHIELD_ENV"
echo "SGX Mode: $GRAMINE_SGX"

# Check if running in SGX mode
if [[ "$GRAMINE_SGX" == "1" || "$SGX" == "1" ]]; then
    echo "Running ScriptShield-Luau with SGX protection"

    # Extract mr_enclave value for testing
    if [ -f "/scriptshield-luau/scriptshield-luau.sig" ]; then
        echo "=== MR_ENCLAVE TEST ==="
        echo -n "MR_ENCLAVE: "
        # Extract mr_enclave from the signature file
        gramine-sgx-get-token --output /tmp/token.dat --sig /scriptshield-luau/scriptshield-luau.sig 2>/dev/null || true
        python3 -c "
import sys
try:
    with open('/scriptshield-luau/scriptshield-luau.sig', 'rb') as f:
        data = f.read()
        # MR_ENCLAVE is at offset 960 (0x3C0) in the sigstruct, 32 bytes
        mr_enclave = data[960:992]
        print(mr_enclave.hex().upper())
except Exception as e:
    print('Error extracting MR_ENCLAVE:', str(e))
" 2>/dev/null || echo "Could not extract MR_ENCLAVE"
        echo "======================="
    fi

    # Run with Gramine SGX
    exec gramine-sgx /scriptshield-luau/scriptshield-luau
else
    echo "Running ScriptShield-Luau in simulation mode"
    # Run directly for testing
    exec /scriptshield-luau/app
fi