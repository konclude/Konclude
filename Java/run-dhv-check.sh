#!/usr/bin/env bash
#
# Compiles and runs Java/tools/DhvCheck.java, which checks a class hierarchy against HermiT or
# FaCT++ on a locality module (see the comment in the source for the modes and the background).
#
# The OWL API and HermiT are resolved from the local Maven repository with
# 'mvn dependency:build-classpath' (OWLAPI_VERSION, default 4.5.29, and HERMIT_VERSION, default
# 1.4.3.456; both have to be the OWL API 4 generation), or taken from OWLAPI_CLASSPATH if set.
# FaCT++ is optional: FACTPP_JAR names the jar of its OWL API binding and FACTPP_LIB the directory
# holding its native library (libFaCTPlusPlusJNI.jnilib or .so).
#
# Usage: [HEAP=<Xmx>] ./run-dhv-check.sh module <ontology> <signature file> <module out> <hierarchy out> [factory class]
#        [HEAP=<Xmx>] ./run-dhv-check.sh classify <ontology> <hierarchy out> <factory class>
#        [HEAP=<Xmx>] ./run-dhv-check.sh asserted <ontology> <hierarchy out>

set -u

JAVA_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$JAVA_DIR/owlapi/build/dhv-check"
OWLAPI_VERSION="${OWLAPI_VERSION:-4.5.29}"
HERMIT_VERSION="${HERMIT_VERSION:-1.4.3.456}"
HEAP="${HEAP:-12g}"

if [ $# -lt 3 ]; then
	sed -n '2,15p' "$0" | sed 's/^# \{0,1\}//'
	exit 2
fi
if [ -z "${JAVA_HOME:-}" ]; then
	echo "JAVA_HOME is not set, it is required to compile and run the check." >&2
	exit 2
fi
JAVAC="$JAVA_HOME/bin/javac"
JAVA="$JAVA_HOME/bin/java"

resolve_classpath() {
	if [ -n "${OWLAPI_CLASSPATH:-}" ]; then
		echo "$OWLAPI_CLASSPATH"
		return 0
	fi
	command -v mvn >/dev/null 2>&1 || return 1
	local work
	work="$(mktemp -d)" || return 1
	cat > "$work/pom.xml" <<POM
<project xmlns="http://maven.apache.org/POM/4.0.0">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.konclude</groupId>
  <artifactId>dhv-check-classpath</artifactId>
  <version>1</version>
  <dependencies>
    <dependency>
      <groupId>net.sourceforge.owlapi</groupId>
      <artifactId>owlapi-distribution</artifactId>
      <version>$OWLAPI_VERSION</version>
    </dependency>
    <dependency>
      <groupId>net.sourceforge.owlapi</groupId>
      <artifactId>org.semanticweb.hermit</artifactId>
      <version>$HERMIT_VERSION</version>
      <exclusions>
        <exclusion>
          <groupId>net.sourceforge.owlapi</groupId>
          <artifactId>owlapi-distribution</artifactId>
        </exclusion>
      </exclusions>
    </dependency>
  </dependencies>
</project>
POM
	if (cd "$work" && mvn -q -o dependency:build-classpath -Dmdep.outputFile="$work/cp.txt" >/dev/null 2>&1) \
	   || (cd "$work" && mvn -q dependency:build-classpath -Dmdep.outputFile="$work/cp.txt" >/dev/null 2>&1); then
		cat "$work/cp.txt"
		rm -rf "$work"
		return 0
	fi
	rm -rf "$work"
	return 1
}

CP="$(resolve_classpath)" || {
	echo "The OWL API and HermiT could not be resolved; set OWLAPI_CLASSPATH or install Maven." >&2
	exit 2
}
if [ -n "${FACTPP_JAR:-}" ]; then
	CP="$CP:$FACTPP_JAR"
fi
LIBRARY_PATH_OPTION=""
if [ -n "${FACTPP_LIB:-}" ]; then
	LIBRARY_PATH_OPTION="-Djava.library.path=$FACTPP_LIB"
fi

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
"$JAVAC" -cp "$CP" -d "$BUILD_DIR" "$JAVA_DIR/tools/DhvCheck.java" || exit 1
exec "$JAVA" -Xmx"$HEAP" $LIBRARY_PATH_OPTION -cp "$CP:$BUILD_DIR" DhvCheck "$@"
