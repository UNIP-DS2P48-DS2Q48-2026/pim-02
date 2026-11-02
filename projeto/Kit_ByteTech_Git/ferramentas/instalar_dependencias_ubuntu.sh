#!/usr/bin/env bash
set -euo pipefail
if [[ ! -f /etc/os-release ]]; then echo 'Execute no Ubuntu WSL/Linux.' >&2; exit 1; fi
. /etc/os-release
if [[ "$ID" != "ubuntu" ]]; then echo 'Este instalador foi preparado para Ubuntu.' >&2; exit 1; fi
printf 'Ubuntu %s\n' "$VERSION_ID"
sudo apt-get update
sudo apt-get install -y gcc make libssl-dev unixodbc-dev curl ca-certificates gnupg
if ! dpkg-query -W -f='${Status}' msodbcsql18 2>/dev/null | grep -q 'install ok installed'; then
  TMPDIR_BT=$(mktemp -d)
  trap 'rm -rf "$TMPDIR_BT"' EXIT
  curl -fsSLo "$TMPDIR_BT/packages-microsoft-prod.deb" \
    "https://packages.microsoft.com/config/ubuntu/${VERSION_ID}/packages-microsoft-prod.deb"
  sudo dpkg -i "$TMPDIR_BT/packages-microsoft-prod.deb"
  sudo apt-get update
fi
sudo env ACCEPT_EULA=Y apt-get install -y msodbcsql18 mssql-tools18
printf '\nDependencias instaladas. Configure o Docker Desktop se precisar iniciar SQL Server local.\n'
