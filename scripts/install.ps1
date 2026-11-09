param(
    [string]$Target = 'cmf-watch-utils',
    [switch]$Firmware
)
$ErrorActionPreference = 'Stop'

function Invoke-Checked($Command, [string[]]$Arguments) {
    & $Command @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Command exited with status $LASTEXITCODE." }
}

foreach ($Command in 'git', 'uv') {
    if (-not (Get-Command $Command -ErrorAction SilentlyContinue)) {
        throw "Install $Command before running this script."
    }
}

if ($Firmware) {
    git lfs version 2>$null | Out-Null
    if ($LASTEXITCODE -ne 0) { throw 'Install Git LFS to use -Firmware.' }
}

$RepoUrl = if ($env:CMF_WATCH_UTILS_REPO) { $env:CMF_WATCH_UTILS_REPO } else { 'https://github.com/arnav-kr/cmf-watch-utils.git' }
if (-not (Test-Path (Join-Path $Target '.git'))) {
    $PreviousSkipSmudge = $env:GIT_LFS_SKIP_SMUDGE
    try {
        $env:GIT_LFS_SKIP_SMUDGE = '1'
        Invoke-Checked git @('clone', $RepoUrl, $Target)
    }
    finally {
        $env:GIT_LFS_SKIP_SMUDGE = $PreviousSkipSmudge
    }
}

Push-Location $Target
try {
    Invoke-Checked uv @('sync', '--locked')
    if ($Firmware) {
        Invoke-Checked git @('lfs', 'install', '--local')
        Invoke-Checked git @('lfs', 'pull', '--include=dumps/binaries/firmware/JX402_V1.0.0.73.bin', '--exclude=')
    }

    Write-Host "Workspace installed at $($PWD.Path)"
    Write-Host 'Open that directory and run: uv run cmf-watch-utils emu profiles'
    Write-Host 'Setup guide: https://cmf.arnv.dev/getting-started/setup/'
}
finally {
    Pop-Location
}
