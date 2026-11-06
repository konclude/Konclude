#!/usr/bin/env bash
#
# Classifies an ontology repeatedly with the same binary and fails if the runs do not all
# infer the same class hierarchy.
#
# This is a regression test for a defect in which COptimizedKPSetClassSubsumptionClassifierThread
# took the subsumers of a concept from its precomputed saturation and skipped the subsumption
# tests, losing inferences when the extracted set was incomplete. Which concepts lost subsumers
# depended on the memory layout of the process, so the same binary over the same file produced
# different hierarchies from run to run. That is the signature this test looks for: it needs no
# reference reasoner and no expected output, only that the reasoner agrees with itself.
#
# The comparison is not byte wise. Konclude does not emit a canonical document: which member of
# a set of equivalent classes an edge is hung on varies between runs, and so does the order of
# the elements. Both are meaning preserving. Every class is therefore rewritten to the smallest
# member of its equivalence class and the resulting edges are deduplicated and sorted before the
# runs are compared, which leaves exactly the differences that change what was inferred.
#
# Usage: ./run-classification-reproducibility-test.sh [-n <runs>] [-i <ontology>] [-c <config>] [<Konclude binary>]
#
#   -n <runs>      how often to classify, at least 2, 4 by default
#   -i <ontology>  the ontology to classify, Tests/galen.owl.xml by default
#   -c <config>    a configuration file to classify with, none by default
#
# A larger and more redundant ontology exercises the defect far better than the default one.
# SNOMED CT lost between 3 and 42 subsumptions per run before the fix; Tests/galen.owl.xml is
# small enough that it may well never have lost any, so a clean run of the default is weak
# evidence on its own. It is the default because it is the only ontology in the repository.
#
# What this test cannot do: it compares the runs against each other and not against a reference,
# so it only reports a defect that makes the result vary. One that loses the same inferences on
# every run passes it. The defect it was written for did vary, but it varied intermittently -
# runs that lose nothing are common, and several clean runs in a row mean little. Increase -n
# rather than read a single clean result as evidence, and use an ontology on which the reasoner
# does enough work to be worth testing.

set -u

SCRIPTS_DIR="$(cd "$(dirname "$0")" && pwd)"
KONCLUDE_DIR="$(dirname "$SCRIPTS_DIR")"

RUNS=4
ONTOLOGY="$KONCLUDE_DIR/Tests/galen.owl.xml"
CONFIG=""

while getopts ':n:i:c:h' opt; do
	case "$opt" in
		n) RUNS="$OPTARG" ;;
		i) ONTOLOGY="$OPTARG" ;;
		c) CONFIG="$OPTARG" ;;
		h) sed -n '2,35p' "$0" | sed 's/^#//;s/^ //'; exit 0 ;;
		\?) echo "unknown option '-$OPTARG', use -h for the usage." >&2; exit 2 ;;
		:) echo "option '-$OPTARG' needs an argument, use -h for the usage." >&2; exit 2 ;;
	esac
done
shift $((OPTIND - 1))

case "$RUNS" in
	''|*[!0-9]*) echo "the number of runs has to be a number, got '$RUNS'." >&2; exit 2 ;;
esac
if [ "$RUNS" -lt 2 ]; then
	echo "at least 2 runs are needed to compare them, got $RUNS." >&2
	exit 2
fi
if [ ! -r "$ONTOLOGY" ]; then
	echo "'$ONTOLOGY' cannot be read." >&2
	exit 2
fi
CONFIG_ARGUMENTS=()
if [ -n "$CONFIG" ]; then
	if [ ! -r "$CONFIG" ]; then
		echo "'$CONFIG' cannot be read." >&2
		exit 2
	fi
	CONFIG_ARGUMENTS=(-c "$CONFIG")
fi

# locate the binary, the build directories that qmake uses are searched when none is given
find_binary() {
	local candidate
	for candidate in "$@"; do
		if [ -f "$candidate" ] && [ -x "$candidate" ]; then
			echo "$candidate"
			return 0
		fi
	done
	return 1
}

if [ $# -ge 1 ]; then
	KONCLUDE="$1"
	if [ ! -x "$KONCLUDE" ]; then
		echo "'$KONCLUDE' is not an executable." >&2
		exit 2
	fi
else
	KONCLUDE="$(find_binary \
		"$KONCLUDE_DIR/release/Konclude.app/Contents/MacOS/Konclude" \
		"$KONCLUDE_DIR/release/Konclude" \
		"$KONCLUDE_DIR/Konclude" \
		"$SCRIPTS_DIR/Konclude" \
		"$KONCLUDE_DIR/../build/Konclude.app/Contents/MacOS/Konclude" \
		"$KONCLUDE_DIR/../build/Konclude" || true)"
	if [ -z "$KONCLUDE" ]; then
		echo "No Konclude binary found. Build it with Konclude.pro and pass it as argument," >&2
		echo "see the usage with -h." >&2
		exit 2
	fi
fi

WORK_DIR="$(mktemp -d)"
trap 'rm -rf "$WORK_DIR"' EXIT

# Rewrites a classification result to a canonical form: one sorted line per equivalence class,
# one line per subsumption with both ends replaced by the smallest member of their equivalence
# class, self subsumptions dropped and duplicates removed. Reads the file twice, the equivalence
# classes have to be known before the subsumptions can be rewritten.
canonicalise() {
	# bytewise string comparison, so that the members of an equivalence class are ordered the
	# same everywhere; the awk of macOS compares by the collation of the locale otherwise
	LC_ALL=C awk '
		function iri_of_line(   value) {
			if (!match($0, /IRI="[^"]*"/)) {
				return ""
			}
			return substr($0, RSTART + 5, RLENGTH - 6)
		}
		NR == FNR {
			if ($0 ~ /<EquivalentClasses>/) { in_eq = 1; count = 0; next }
			if (in_eq && $0 ~ /<\/EquivalentClasses>/) {
				for (i = 2; i <= count; i++) {
					value = member[i]
					j = i - 1
					while (j >= 1 && member[j] > value) { member[j + 1] = member[j]; j-- }
					member[j + 1] = value
				}
				line = "E"
				for (i = 1; i <= count; i++) {
					representative[member[i]] = member[1]
					line = line "\t" member[i]
				}
				if (count > 0) { canonical[line] = 1 }
				in_eq = 0
				next
			}
			if (in_eq) {
				value = iri_of_line()
				if (value != "") { member[++count] = value }
			}
			next
		}
		$0 ~ /<SubClassOf>/ { in_sub = 1; ends = 0; next }
		in_sub && $0 ~ /<\/SubClassOf>/ {
			if (ends == 2) {
				sub_class = (end[1] in representative) ? representative[end[1]] : end[1]
				super_class = (end[2] in representative) ? representative[end[2]] : end[2]
				if (sub_class != super_class) {
					canonical["S\t" sub_class "\t" super_class] = 1
				}
			}
			in_sub = 0
			next
		}
		in_sub {
			value = iri_of_line()
			if (value != "") { end[++ends] = value }
		}
		END { for (line in canonical) { print line } }
	' "$1" "$1" | LC_ALL=C sort
}

echo "Konclude  : $KONCLUDE"
echo "ontology  : $ONTOLOGY"
if [ -n "$CONFIG" ]; then
	echo "config    : $CONFIG"
fi
echo "runs      : $RUNS"
echo

for run in $(seq 1 "$RUNS"); do
	printf 'classifying, run %d of %d ... ' "$run" "$RUNS"
	if ! "$KONCLUDE" classification -w AUTO "${CONFIG_ARGUMENTS[@]+"${CONFIG_ARGUMENTS[@]}"}" \
			-i "$ONTOLOGY" -o "$WORK_DIR/run$run.xml" \
			> "$WORK_DIR/run$run.log" 2>&1; then
		echo "FAILED"
		echo
		echo "The classification did not succeed:" >&2
		tail -20 "$WORK_DIR/run$run.log" >&2
		exit 1
	fi
	if [ ! -s "$WORK_DIR/run$run.xml" ]; then
		echo "FAILED"
		echo
		echo "The classification produced no result." >&2
		exit 1
	fi
	canonicalise "$WORK_DIR/run$run.xml" > "$WORK_DIR/run$run.canonical"
	echo "$(wc -l < "$WORK_DIR/run$run.canonical" | tr -d ' ') inferences"
done

echo
differing=0
for run in $(seq 2 "$RUNS"); do
	if ! cmp -s "$WORK_DIR/run1.canonical" "$WORK_DIR/run$run.canonical"; then
		differing=$((differing + 1))
		lost="$(LC_ALL=C comm -23 "$WORK_DIR/run1.canonical" "$WORK_DIR/run$run.canonical" | wc -l | tr -d ' ')"
		gained="$(LC_ALL=C comm -13 "$WORK_DIR/run1.canonical" "$WORK_DIR/run$run.canonical" | wc -l | tr -d ' ')"
		echo "run $run differs from run 1: $lost only in run 1, $gained only in run $run" >&2
		LC_ALL=C comm -3 "$WORK_DIR/run1.canonical" "$WORK_DIR/run$run.canonical" | head -10 >&2
	fi
done

if [ "$differing" -ne 0 ]; then
	echo >&2
	echo "FAILED: $differing of $((RUNS - 1)) comparisons disagree with the first run." >&2
	echo "The same binary inferred different class hierarchies from the same ontology." >&2
	exit 1
fi

echo "All $RUNS runs inferred the same class hierarchy."
