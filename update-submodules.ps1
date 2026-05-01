$ErrorActionPreference = "Stop"

$repositories = @(
    "core",
    "libs/libDns",
    "libs/libPipeHandler",
    "libs/libSocketHandler",
    "libs/libSocks5",
    "thirdParty/donut"
)

$root = $PSScriptRoot
if ([string]::IsNullOrWhiteSpace($root)) {
    $root = (Get-Location).Path
}

foreach ($repository in $repositories) {
    $path = Join-Path $root $repository
    if (-not (Test-Path -Path $path -PathType Container)) {
        throw "Repository path not found: $path"
    }

    Write-Host "[*] Updating $repository"
    Push-Location $path
    try {
        $safeDirectory = (Get-Location).Path
        git config --global --add safe.directory $safeDirectory
        git fetch
        git rebase origin/master
    }
    finally {
        Pop-Location
    }
}

Write-Host "[+] Done"