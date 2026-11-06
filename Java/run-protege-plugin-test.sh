#!/usr/bin/env bash
#
# Builds the Protege plug-in of Konclude and runs it inside the OSGi framework of a Protege
# installation, see Java/Readme.md.
#
# The plug-in is built with Maven from Java/protege, around the shared library of Konclude,
# which has to be built beforehand with KoncludeLIB.pro. The plug-in is then loaded into the
# Felix framework together with every bundle of the Protege installation, as Protege loads it,
# and driven from inside the framework: once the way Protege drives a reasoner, and once per
# scenario of the OWL API test.
#
# Usage: ./run-protege-plugin-test.sh [<Protege directory>] [<directory or file of the shared library>]
#
# The Protege directory is the one that holds the 'bundles' and 'plugins' directories. Without
# the argument the environment variable PROTEGE_DIR is used, and without that the newest
# /Applications/Protege-5.* is taken.

set -u

JAVA_DIR="$(cd "$(dirname "$0")" && pwd)"
KONCLUDE_DIR="$(dirname "$JAVA_DIR")"
PLUGIN_DIR="$JAVA_DIR/protege"
BUILD_DIR="$PLUGIN_DIR/build"

if [ -z "${JAVA_HOME:-}" ]; then
	echo "JAVA_HOME is not set, it is required to build and run the plug-in." >&2
	exit 2
fi
JAVAC="$JAVA_HOME/bin/javac"
JAVA="$JAVA_HOME/bin/java"
JAR="$JAVA_HOME/bin/jar"
for tool in "$JAVAC" "$JAVA" "$JAR"; do
	if [ ! -x "$tool" ]; then
		echo "'$tool' not found or not executable, is JAVA_HOME correct?" >&2
		exit 2
	fi
done
command -v mvn >/dev/null 2>&1 || {
	echo "Maven is not installed, it is required to build the plug-in." >&2
	exit 2
}

# the Protege installation
if [ $# -ge 1 ]; then
	PROTEGE_DIR="$1"
elif [ -n "${PROTEGE_DIR:-}" ]; then
	:
else
	PROTEGE_DIR="$(ls -d /Applications/Protege-5.* 2>/dev/null | sort -V | tail -n 1)"
fi
if [ -z "$PROTEGE_DIR" ] || [ ! -d "$PROTEGE_DIR/bundles" ]; then
	echo "No Protege installation found, pass its directory as first argument." >&2
	exit 2
fi
PROTEGE_DIR="$(cd "$PROTEGE_DIR" && pwd)"
FELIX_JAR="$PROTEGE_DIR/bundles/org.apache.felix.main.jar"
if [ ! -f "$FELIX_JAR" ]; then
	echo "'$FELIX_JAR' not found, is '$PROTEGE_DIR' a Protege 5 installation?" >&2
	exit 2
fi
echo "Using the Protege installation in '$PROTEGE_DIR'."

# the shared library, the plug-in packages the one of this platform
find_library() {
	local candidate name
	for candidate in "$@"; do
		[ -d "$candidate" ] || continue
		for name in libKonclude.dylib libKonclude.so Konclude.dll; do
			if [ -e "$candidate/$name" ]; then
				echo "$candidate/$name"
				return 0
			fi
		done
	done
	return 1
}
if [ $# -ge 2 ]; then
	if [ -f "$2" ]; then
		LIBRARY="$(cd "$(dirname "$2")" && pwd)/$(basename "$2")"
	elif [ -d "$2" ]; then
		LIBRARY="$(find_library "$(cd "$2" && pwd)")" || {
			echo "No Konclude shared library in '$2'." >&2
			exit 2
		}
	else
		echo "'$2' is neither a file nor a directory." >&2
		exit 2
	fi
else
	LIBRARY="$(find_library "$KONCLUDE_DIR/Release" "$KONCLUDE_DIR/release" "$KONCLUDE_DIR")" || {
		echo "No Konclude shared library found." >&2
		echo "Build it with 'qmake KoncludeLIB.pro && make' and pass its directory as second argument," >&2
		echo "see Java/Readme.md." >&2
		exit 2
	}
fi
echo "Using the Konclude shared library '$LIBRARY'."

case "$(uname -s)-$(uname -m)" in
	Darwin-arm64) LIBRARY_PROPERTY="konclude.library.macos-arm64" ;;
	Linux-x86_64) LIBRARY_PROPERTY="konclude.library.linux-x64" ;;
	Linux-aarch64) LIBRARY_PROPERTY="konclude.library.linux-arm64" ;;
	*)
		echo "The plug-in packages the shared library for macOS on arm64 and Linux on x64 and arm64 only, this is $(uname -s) on $(uname -m)." >&2
		exit 2
		;;
esac

# the plug-in
echo "Building the plug-in with Maven."
(cd "$PLUGIN_DIR" && mvn -q -B package -D"$LIBRARY_PROPERTY"="$LIBRARY") || {
	echo "The Maven build of the plug-in failed." >&2
	exit 1
}
PLUGIN_JAR="$(ls "$PLUGIN_DIR"/target/konclude-protege-plugin-*.jar | grep -v -- '-sources\|-javadoc' | head -n 1)"
if [ -z "$PLUGIN_JAR" ]; then
	echo "The plug-in jar was not found in '$PLUGIN_DIR/target'." >&2
	exit 1
fi
echo "Built '$PLUGIN_JAR'."

# the test, which is compiled against the same dependencies as the plug-in
CLASSPATH_FILE="$BUILD_DIR/classpath.txt"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR/classes"
(cd "$PLUGIN_DIR" && mvn -q -B dependency:build-classpath -Dmdep.outputFile="$CLASSPATH_FILE" >/dev/null) || {
	echo "The dependencies of the plug-in could not be resolved." >&2
	exit 1
}
DEPENDENCIES="$(cat "$CLASSPATH_FILE")"
echo "Compiling the test into '$BUILD_DIR'."
if ! find "$PLUGIN_DIR/test" "$JAVA_DIR/owlapi/src/com/konclude/owlapitest" -name '*.java' -print0 \
		| xargs -0 "$JAVAC" -proc:none -cp "$PLUGIN_JAR:$FELIX_JAR:$DEPENDENCIES" -d "$BUILD_DIR/classes"; then
	echo "Compilation of the test failed." >&2
	exit 1
fi

# the scenarios are run inside the framework, from a bundle that requires the plug-in
TEST_BUNDLE="$BUILD_DIR/konclude-protege-plugin-test.jar"
cat > "$BUILD_DIR/MANIFEST.MF" <<MANIFEST
Manifest-Version: 1.0
Bundle-ManifestVersion: 2
Bundle-SymbolicName: com.konclude.protege.test
Bundle-Version: 0.0.0
Bundle-Name: Konclude Protege Plug-in Test
Require-Bundle: com.konclude.protege, org.protege.editor.owl, org.semanticweb.owl.owlapi
DynamicImport-Package: *
MANIFEST
(cd "$BUILD_DIR/classes" && "$JAR" cfm "$TEST_BUNDLE" "$BUILD_DIR/MANIFEST.MF" \
	com/konclude/protegetest/KoncludeProtegePluginScenarios*.class com/konclude/owlapitest) || {
	echo "The test bundle could not be built." >&2
	exit 1
}

# the logback of Protege is among the bundles and would log at DEBUG level without this
cat > "$BUILD_DIR/logback.xml" <<LOGBACK
<configuration>
	<appender name="console" class="ch.qos.logback.core.ConsoleAppender">
		<encoder><pattern>%-5level %logger{0} - %msg%n</pattern></encoder>
	</appender>
	<logger name="com.konclude" level="INFO"/>
	<root level="WARN"><appender-ref ref="console"/></root>
</configuration>
LOGBACK

. "$JAVA_DIR/jvm-crash-reports.sh"

FAILED_SCENARIOS=""
for scenario in plugin hierarchy expressions expressions-tableau expressions-el individuals properties datatypes inconsistency unsupported lifecycle merges timeout entailment interrupt progress; do
	echo
	echo "--------------------------------------------------------------------------"
	CACHE_DIR="$BUILD_DIR/felix-cache-$scenario"
	rm -rf "$CACHE_DIR"
	"$JAVA" "$CRASH_REPORT_OPTION" -cp "$FELIX_JAR:$BUILD_DIR/classes" -Djava.awt.headless=true \
			-Dlogback.configurationFile="$BUILD_DIR/logback.xml" \
			com.konclude.protegetest.KoncludeProtegePluginTest \
			"$PROTEGE_DIR" "$PLUGIN_JAR" "$TEST_BUNDLE" "$CACHE_DIR" "$scenario"
	status=$?
	# the report is looked for first, so that it is taken care of whatever the status
	if crash_report_found protege-plugin "$scenario" || [ $status -ne 0 ]; then
		FAILED_SCENARIOS="$FAILED_SCENARIOS $scenario"
	fi
	rm -rf "$CACHE_DIR"
done

echo
echo "=========================================================================="
if [ -n "$FAILED_SCENARIOS" ]; then
	echo "FAILED scenarios:$FAILED_SCENARIOS"
	exit 1
fi
echo "All scenarios passed, the Protege plug-in of Konclude works inside the framework of Protege."
echo "Copy '$PLUGIN_JAR' into '$PROTEGE_DIR/plugins' and restart Protege to use it."
