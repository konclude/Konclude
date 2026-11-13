#!/usr/bin/env bash
#
# Compiles and runs the headless test of the OWL API wrapper of Konclude.
#
# The shared library of Konclude has to be built beforehand with KoncludeLIB.pro, see
# Java/Readme.md. In contrast to the smoke test this needs the OWL API on the class path,
# which is searched in this order:
#
#   1. the environment variable OWLAPI_CLASSPATH
#   2. the jars in Java/owlapi/lib
#   3. the local Maven repository, resolved with 'mvn dependency:build-classpath'
#
# Usage: ./run-owlapi-test.sh [<directory or file of the Konclude shared library>]

set -u

JAVA_DIR="$(cd "$(dirname "$0")" && pwd)"
KONCLUDE_DIR="$(dirname "$JAVA_DIR")"
BUILD_DIR="$JAVA_DIR/owlapi/build"
LIB_DIR="$JAVA_DIR/owlapi/lib"
OWLAPI_VERSION="${OWLAPI_VERSION:-4.5.29}"

if [ -z "${JAVA_HOME:-}" ]; then
	echo "JAVA_HOME is not set, it is required to compile and run the test." >&2
	exit 2
fi
JAVAC="$JAVA_HOME/bin/javac"
JAVA="$JAVA_HOME/bin/java"
for tool in "$JAVAC" "$JAVA"; do
	if [ ! -x "$tool" ]; then
		echo "'$tool' not found or not executable, is JAVA_HOME correct?" >&2
		exit 2
	fi
done

# locate the shared library, the name depends on the platform
find_library_directory() {
	local candidate pattern
	for candidate in "$@"; do
		[ -d "$candidate" ] || continue
		# every name has to be tried on its own, ls fails as soon as one of several
		# arguments does not exist, and only one of these names exists per platform
		for pattern in 'libKonclude.dylib' 'libKonclude.so' 'Konclude.dll' \
		               'libKonclude.*.dylib' 'libKonclude.so.*'; do
			if ls "$candidate"/$pattern >/dev/null 2>&1; then
				echo "$candidate"
				return 0
			fi
		done
	done
	return 1
}

if [ $# -ge 1 ]; then
	if [ -f "$1" ]; then
		LIBRARY_DIR="$(cd "$(dirname "$1")" && pwd)"
	elif [ -d "$1" ]; then
		LIBRARY_DIR="$(cd "$1" && pwd)"
	else
		echo "'$1' is neither a file nor a directory." >&2
		exit 2
	fi
else
	LIBRARY_DIR="$(find_library_directory "$KONCLUDE_DIR/Release" "$KONCLUDE_DIR/release" "$KONCLUDE_DIR")" || {
		echo "No Konclude shared library found." >&2
		echo "Build it with 'qmake KoncludeLIB.pro && make' and pass its directory as argument," >&2
		echo "see Java/Readme.md." >&2
		exit 2
	}
fi
echo "Using the Konclude shared library from '$LIBRARY_DIR'."

. "$JAVA_DIR/native-paths.sh"

# the OWL API is not a single jar, owlapi-distribution pulls a good fifty dependencies
resolve_owlapi_classpath() {
	if [ -n "${OWLAPI_CLASSPATH:-}" ]; then
		echo "$OWLAPI_CLASSPATH"
		return 0
	fi
	if [ -d "$LIB_DIR" ] && ls "$LIB_DIR"/*.jar >/dev/null 2>&1; then
		local classpath=""
		local jar
		for jar in "$LIB_DIR"/*.jar; do
			classpath="${classpath:+$classpath$CLASSPATH_SEPARATOR}$(native_path "$jar")"
		done
		echo "$classpath"
		return 0
	fi
	command -v mvn >/dev/null 2>&1 || return 1
	local work
	work="$(mktemp -d)" || return 1
	cat > "$work/pom.xml" <<POM
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.konclude</groupId>
  <artifactId>owlapi-classpath</artifactId>
  <version>1</version>
  <dependencies>
    <dependency>
      <groupId>net.sourceforge.owlapi</groupId>
      <artifactId>owlapi-distribution</artifactId>
      <version>$OWLAPI_VERSION</version>
    </dependency>
  </dependencies>
</project>
POM
	# Maven writes the class path with the separator and the paths of the platform
	local output
	output="$(native_path "$work/cp.txt")"
	if (cd "$work" && mvn -q -o dependency:build-classpath -Dmdep.outputFile="$output" >/dev/null 2>&1) \
	   || (cd "$work" && mvn -q dependency:build-classpath -Dmdep.outputFile="$output" >/dev/null 2>&1); then
		cat "$work/cp.txt"
		rm -rf "$work"
		return 0
	fi
	rm -rf "$work"
	return 1
}

OWLAPI_CP="$(resolve_owlapi_classpath)" || {
	echo "The OWL API could not be found." >&2
	echo "Set OWLAPI_CLASSPATH, or put the jars of owlapi-distribution $OWLAPI_VERSION and its" >&2
	echo "dependencies into '$LIB_DIR', or install Maven so that they can be resolved." >&2
	exit 2
}
echo "Using the OWL API from $(echo "$OWLAPI_CP" | tr "$CLASSPATH_SEPARATOR" '\n' | wc -l | tr -d ' ') jar(s)."

echo "Compiling the test into '$BUILD_DIR'."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
if ! find "$JAVA_DIR/src" "$JAVA_DIR/owlapi/src" -name '*.java' -print0 \
		| xargs -0 "$JAVAC" -cp "$OWLAPI_CP" -d "$BUILD_DIR"; then
	echo "Compilation of the test failed." >&2
	exit 1
fi

. "$JAVA_DIR/jvm-crash-reports.sh"

FAILED_SCENARIOS=""
for scenario in hierarchy expressions expressions-tableau expressions-el individuals properties datatypes inconsistency unsupported lifecycle determinism merges completion-successors completion-successors-plain timeout entailment interrupt progress; do
	echo
	echo "--------------------------------------------------------------------------"
	"$JAVA" "$CRASH_REPORT_OPTION" -cp "$OWLAPI_CP$CLASSPATH_SEPARATOR$(native_path "$BUILD_DIR")" \
			-Djava.library.path="$(native_path "$LIBRARY_DIR")" \
			com.konclude.owlapitest.KoncludeOWLAPITest "$scenario"
	status=$?
	# the report is looked for first, so that it is taken care of whatever the status
	if crash_report_found owlapi "$scenario" || [ $status -ne 0 ]; then
		FAILED_SCENARIOS="$FAILED_SCENARIOS $scenario"
	fi
done

echo
echo "=========================================================================="
if [ -n "$FAILED_SCENARIOS" ]; then
	echo "FAILED scenarios:$FAILED_SCENARIOS"
	exit 1
fi
echo "All scenarios passed, the OWL API wrapper of Konclude works against the library."
