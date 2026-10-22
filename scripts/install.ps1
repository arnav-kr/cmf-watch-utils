# Clone CMF Watch Utils, install the workspace and fetch the published firmware dumps.
$ErrorActionPreference = 'Stop'

$RepoUrl = if ($env:CMF_WATCH_UTILS_REPO) { $env:CMF_WATCH_UTILS_REPO } else { 'https://github.com/arnav-kr/cmf-watch-utils.git' }
$Target = if ($args.Count -ge 1) { $args[0] } else { 'cmf-watch-utils' }

function Assert-Command($Name, $Hint) {
    if (-not (Get-Command $Name -ErrorAction SilentlyContinue)) {
        Write-Error "$Name is required. $Hint"
        exit 1
    }
}

Assert-Command 'git' 'Install Git for Windows.'
Assert-Command 'uv' 'Install uv: https://docs.astral.sh/uv/getting-started/installation/'

if (Get-Command 'git-lfs' -ErrorAction SilentlyContinue) { git lfs install }

if (-not (Test-Path (Join-Path $Target '.git'))) {
    git clone $RepoUrl $Target
}

Push-Location $Target
try {
    uv sync

    if (Get-Command 'git-lfs' -ErrorAction SilentlyContinue) {
        git lfs pull --include='dumps/binaries/**'
    }

    Write-Host ''
    Write-Host 'Ready. Useful commands:'
    Write-Host ''
    Write-Host '  uv run fwkit info   dumps/binaries/firmware/JX402_V1.0.0.73.bin'
    Write-Host '  uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin'
    Write-Host '  uv run python tools/cmf_watch.py --help'
    Write-Host '  uv run python tooling/tests/selftest.py dumps/binaries/firmware/JX402_V1.0.0.73.bin'
    Write-Host ''
    Write-Host 'Documentation: https://cmf.arnv.dev/'
}
finally {
    Pop-Location
}
