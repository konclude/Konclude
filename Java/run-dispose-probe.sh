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
# Usage: ONTOLOGY=<file> [CYCLES=<n>] [HEAP=<Xmx>] ./run-dispose-probe.sh [<directory or file of the Konclude shared library>]

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

ONTOLOGY="${ONTOLOGY:-}"
CYCLES="${CYCLES:-4}"
HEAP="${HEAP:-10g}"
if [ -z "$ONTOLOGY" ] || [ ! -f "$ONTOLOGY" ]; then
	echo "ONTOLOGY has to name the ontology file to classify." >&2
	exit 2
fi

PROBE_BUILD_DIR="$JAVA_DIR/build/dispose-probe"
echo "Compiling the probe into '$PROBE_BUILD_DIR'."
rm -rf "$PROBE_BUILD_DIR"
mkdir -p "$PROBE_BUILD_DIR"
if ! find "$JAVA_DIR/src" "$JAVA_DIR/owlapi/src" "$JAVA_DIR/tools" -name '*.java' -print0 \
		| xargs -0 "$JAVAC" -cp "$OWLAPI_CP" -d "$PROBE_BUILD_DIR"; then
	echo "Compilation of the probe failed." >&2
	exit 1
fi

echo "Running $CYCLES create-classify-dispose cycle(s) on '$ONTOLOGY'."
# the process size after every dispose: a reasoner that leaks shows it as a rise from cycle to cycle, see Java/Readme.md
exec "$JAVA" -Xmx"$HEAP" -Djava.library.path="$(native_path "$LIBRARY_DIR")" \
	-cp "$(native_path "$PROBE_BUILD_DIR")$CLASSPATH_SEPARATOR$OWLAPI_CP" DisposeProbe "$ONTOLOGY" "$CYCLES"
