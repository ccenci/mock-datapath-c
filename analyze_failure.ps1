param(
    [Parameter(Mandatory=$true)][string]$CommitHash
)

$makeOutput = gcc -Wall -Wextra -O2 -std=c99 -o mock_datapath src/datapath.c 2>&1

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build succeeded."
    exit 0
}

$errorOutput = $makeOutput | Out-String

# Match regex for GCC error: file.c:line:col: error:
if ($errorOutput -match '([a-zA-Z0-9_/\.\-]+):(\d+):\d+: error:') {
    $filePath = $matches[1]
    $lineNumber = $matches[2]

    # Run git blame
    $blameOutput = git blame -L "$lineNumber,$lineNumber" --line-porcelain $filePath
    $author = "Unknown"
    foreach ($line in $blameOutput) {
        if ($line -match '^author (.*)') {
            $author = $matches[1]
            break
        }
    }

    Write-Host "Build failed on $CommitHash. Root cause: Syntax/Pointer logic error in $filePath at line $lineNumber."
    Write-Host "Blame: @[$author]. Suggestion: Revert commit $CommitHash."
} else {
    Write-Host "Build failed, but couldn't parse the error line."
    Write-Host $errorOutput
}
