
REM @Set /A "Counter=0" 
REM @git log --oneline > tmpGitLog.txt
REM @FOR /F "delims=" %%A IN (tmpGitLog.txt) DO @SET /A "Counter+=1" 
REM @echo #define KONCLUDE_VERSION_GIT_REVISION_NUMBER %Counter% > revision-git.h

REM @set VersionName=Unknown
REM @git describe --tags --match v*.*.* --abbrev=0 > tmpGitTagName.txt
REM @SET /p VersionName=<tmpGitTagName.txt
REM @echo #define KONCLUDE_VERSION_GIT_TAG_NAME_STRING "%VersionName%" >> revision-git.h

REM @set VersionHash=Unknown
REM @git log --pretty=format:%%h -n 1 > tmpGitHash.txt
REM @set /p VersionHash=<tmpGitHash.txt
REM @echo #define KONCLUDE_VERSION_GIT_REVISION_HASH_STRING "%VersionHash%" >> revision-git.h