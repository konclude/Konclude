# Sourced by the run-*.sh scripts beside it.
#
# Under the Git Bash of Windows the scripts see paths as /d/a/..., whereas java, javac and Maven
# are Windows programs, which take D:\a\... and separate the entries of a class path with ';'.
# Git Bash translates an argument that looks like a single Unix path, but not one inside a list
# that already holds Windows paths, such as the class path Maven writes, so every path that goes
# into such an argument or into an option is handed over with native_path and every class path
# is joined with CLASSPATH_SEPARATOR.

case "$(uname -s)" in
	MINGW*|MSYS*|CYGWIN*)
		CLASSPATH_SEPARATOR=';'
		native_path() { cygpath -w "$1"; }
		;;
	*)
		CLASSPATH_SEPARATOR=':'
		native_path() { printf '%s\n' "$1"; }
		;;
esac
