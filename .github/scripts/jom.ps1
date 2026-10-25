# Loaded with ". .github/scripts/jom.ps1" by the Windows build steps.
#
# jom is Qt's parallel drop-in for nmake. Chocolatey downloads it from download.qt.io, and a time-out
# there failed a build that had nothing wrong with it (run 36446254885). The install is therefore tried
# three times, and when jom is still missing the build goes on with nmake, one translation unit at a
# time: slower, about 30 instead of 10 minutes for Konclude, but not a failure.

function Install-Jom {
	for ($attempt = 1; $attempt -le 3; $attempt++) {
		if (Get-Command jom -ErrorAction SilentlyContinue) {
			return
		}
		if ($attempt -eq 1) {
			choco install jom --no-progress -y
		} else {
			Start-Sleep -Seconds (30 * ($attempt - 1))
			choco install jom --no-progress -y --force
		}
	}
	if (-not (Get-Command jom -ErrorAction SilentlyContinue)) {
		Write-Host "::warning::jom could not be installed, the build falls back to nmake"
	}
	# a failed install is not a failed step: the steps of GitHub Actions exit with the last exit code
	$global:LASTEXITCODE = 0
}

# runs jom with one job per processor, or nmake without jom, with the given targets, and stops the
# step when the build fails
function Invoke-Make {
	param([string[]] $Targets = @())
	if (Get-Command jom -ErrorAction SilentlyContinue) {
		jom -j "$env:NUMBER_OF_PROCESSORS" @Targets
	} else {
		nmake @Targets
	}
	if ($LASTEXITCODE -ne 0) {
		exit $LASTEXITCODE
	}
}
