#!/bin/bash
# Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
#
# This software is the confidential and proprietary information of Roman Gorielov.
# It is furnished under license and may only be used or copied in accordance
# with the terms of such license.
# This software is subject to change without notice and no information
# contained in it should be construed as commitment by Roman Gorielov.

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

resolve_pio() {
    local candidate="${HOME}/.platformio/penv/bin/pio"
    if [[ -x "${candidate}" ]]; then
        printf '%s\n' "${candidate}"
        return 0
    fi

    if command -v pio >/dev/null 2>&1; then
        command -v pio
        return 0
    fi

    return 1
}

if ! PIO="$(resolve_pio)"; then
    echo -e "\033[0;31m[ERROR]\033[0m PlatformIO CLI was not found. Install PlatformIO or add pio to PATH."
    exit 1
fi

PACKAGES_DIR="${ROOT_DIR}/packages"
if [[ ! -d "${PACKAGES_DIR}" ]]; then
    mkdir -p "${PACKAGES_DIR}"
    echo "Created ${PACKAGES_DIR} folder."
fi

packed=0
failed=0

shopt -s nullglob
for lib_dir in "${ROOT_DIR}"/*/; do
    manifest="${lib_dir}library.json"
    if [[ ! -f "${manifest}" ]]; then
        continue
    fi

    lib_name="$(basename "${lib_dir}")"
    echo -e "\033[0;30;47mPacking ${lib_name}...\033[0m"
    if "${PIO}" package pack "${lib_dir}" --output "${PACKAGES_DIR}"; then
        packed=$((packed + 1))
    else
        echo -e "\033[0;31m[FAILED]\033[0m ${lib_name}"
        failed=$((failed + 1))
    fi
    echo
done

echo "========================= [TOTAL] ==========================="
echo "${packed} Packed ${failed} Failed"

if [[ "${failed}" -gt 0 ]]; then
    exit 1
fi
