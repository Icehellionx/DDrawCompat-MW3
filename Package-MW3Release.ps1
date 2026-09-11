param(
    [Parameter(Mandatory = $true)]
    [string]$DdrawDll,

    [Parameter(Mandatory = $true)]
    [string]$Version,

    [string]$OutputDirectory = (Join-Path $PSScriptRoot 'packages')
)

$ErrorActionPreference = 'Stop'
$dll = (Resolve-Path -LiteralPath $DdrawDll).Path
$stage = Join-Path $env:TEMP ("DDrawCompat-MW3-" + [Guid]::NewGuid().ToString('N'))
$packageRoot = Join-Path $stage ("DDrawCompat-MW3-" + $Version)

try {
    New-Item -ItemType Directory -Path (Join-Path $packageRoot 'profiles\MechWarrior3') -Force | Out-Null
    New-Item -ItemType Directory -Path (Join-Path $packageRoot 'profiles\PiratesMoon') -Force | Out-Null
    Copy-Item -LiteralPath $dll -Destination (Join-Path $packageRoot 'ddraw.dll')
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'profiles\MechWarrior3\DDrawCompat.ini') -Destination (Join-Path $packageRoot 'profiles\MechWarrior3\DDrawCompat.ini')
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'profiles\PiratesMoon\DDrawCompat.ini') -Destination (Join-Path $packageRoot 'profiles\PiratesMoon\DDrawCompat.ini')
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'MW3-FORK.md') -Destination (Join-Path $packageRoot 'README.md')
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'LICENSE.txt') -Destination (Join-Path $packageRoot 'LICENSE.txt')

    $hash = (Get-FileHash -LiteralPath (Join-Path $packageRoot 'ddraw.dll') -Algorithm SHA256).Hash.ToLowerInvariant()
    Set-Content -LiteralPath (Join-Path $packageRoot 'SHA256SUMS.txt') -Value ($hash + '  ddraw.dll') -Encoding ascii

    New-Item -ItemType Directory -Path $OutputDirectory -Force | Out-Null
    $zip = Join-Path $OutputDirectory ("DDrawCompat-MW3-" + $Version + '.zip')
    if (Test-Path -LiteralPath $zip) {
        throw "Refusing to replace an existing release package: $zip"
    }
    Compress-Archive -LiteralPath $packageRoot -DestinationPath $zip -CompressionLevel Optimal
    Get-FileHash -LiteralPath $zip -Algorithm SHA256
}
finally {
    if (Test-Path -LiteralPath $stage) {
        Remove-Item -LiteralPath $stage -Recurse -Force
    }
}
