#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
docusaurus_bin="$(command -v docusaurus || true)"
if [[ -z "$docusaurus_bin" ]]; then
    docusaurus_bin="$script_dir/../node_modules/.bin/docusaurus"
fi
if [[ ! -x "$docusaurus_bin" ]]; then
    echo "找不到 Docusaurus，请先运行 npm install" >&2
    exit 1
fi

# The browser is running on Windows while the server runs inside WSL. Use the
# WSL adapter IPv4 as Docusaurus' host so its automatic browser URL is reachable.
site_ip="${SITE_IP:-}"
if [[ -z "$site_ip" ]]; then
    site_ip="$(ip -4 route get 1.1.1.1 2>/dev/null \
        | awk '{for (i = 1; i <= NF; i++) if ($i == "src") {print $(i + 1); exit}}')"
fi
if [[ -z "$site_ip" ]]; then
    site_ip="$(ip -4 -o addr show scope global 2>/dev/null \
        | awk '$2 != "lo" {split($4, a, "/"); print a[1]; exit}')"
fi
if [[ -z "$site_ip" ]]; then
    site_ip="$(hostname -I 2>/dev/null \
        | tr ' ' '\n' \
        | awk '$0 ~ /^[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+$/ && $0 !~ /^127\\./ && $0 != "10.255.255.254" {print; exit}')"
fi
if [[ -z "$site_ip" ]]; then
    echo "无法检测到 WSL IPv4 地址，请使用 SITE_IP=WSL_IP npm start" >&2
    exit 1
fi

is_ipv4() {
    local value="$1"
    local part
    [[ "$value" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]] || return 1
    IFS='.' read -r -a parts <<< "$value"
    for part in "${parts[@]}"; do
        (( 10#$part <= 255 )) || return 1
    done
}

if ! is_ipv4 "$site_ip" || [[ "$site_ip" == 127.* || "$site_ip" == "10.255.255.254" ]]; then
    echo "检测到的地址不是可访问的 WSL IPv4：$site_ip，请使用 SITE_IP=WSL_IP npm start" >&2
    exit 1
fi
export SITE_IP="$site_ip"

extra_args=("$@")
command=("$docusaurus_bin" start --host "$site_ip" "${extra_args[@]}")
if [[ "${1:-}" == "serve" ]]; then
    extra_args=("${@:2}")
    command=("$docusaurus_bin" serve --host "$site_ip" "${extra_args[@]}")
fi
exec "${command[@]}"
