#!/usr/bin/env bash
#
# Classifies an ontology with the Konclude command line and reduces the result to a hash, so
# that two builds, two runs or two configurations can be compared for identical hierarchies
# without keeping the outputs (SNOMED CT's is 175 MB). The hash is the MD5 of the sorted lines
# of the OWL/XML output, which Konclude writes in a layout-dependent order; sorted, the same
# hierarchy always gives the same hash. The phase times of the log are printed with it.
#
# Usage:
#   check-hierarchy.sh hash    <Konclude binary> <ontology> [<expected hash>] [-- <Konclude options>]
#   check-hierarchy.sh compare <output A.owl> <output B.owl>
#
# 'hash' classifies with -w AUTO (and any options after --, e.g. +=Konclude.Calculation.Classification.SaturationSubsumerExtraction=false),
# prints "hash <md5 prefix> preprocessing <ms> precomputation <ms> classification <ms>" and, given an expected hash, exits 1 if
# it differs. KEEP_OUTPUT=<file> keeps the output for a later 'compare'.
#
# 'compare' extracts the SubClassOf pairs and EquivalentClasses groups of two outputs and lists what only one of them has
# (comm on sorted pairs; diff of two such files can run for a very long time).
#
# Reference hashes of the files used in October 2026 (first ten characters), all produced by master since d5ddf4fc:
#   SNOMED CT International 2024-11-26 with additions      62e1eefa88
#   SNOMED CT AU 20260930 baseline (au-baseline.owl)        21f0920c63
#   95k variant module (m95k-variant.owl)                   e84a1bbaec
#   full OWL 2 DL variant (variant.owl, 383 204 classes)     19da30f3a9   (classification ~11-13 min, 40 GB)
#   The hashes compare only if the output was sorted with LC_ALL=C, as this script does; a locale-aware sort gives another hash.

set -u

mode="${1:-}"
case "$mode" in
	hash)
		shift
		binary="${1:?Konclude binary}"; ontology="${2:?ontology}"; shift 2
		expected=""
		if [ $# -gt 0 ] && [ "$1" != "--" ]; then expected="$1"; shift; fi
		[ "${1:-}" = "--" ] && shift
		work="$(mktemp -d)"
		output="${KEEP_OUTPUT:-$work/classified.owl}"
		"$binary" classification -w AUTO "$@" -i "$ontology" -o "$output" > "$work/log.txt" 2>&1
		status=$?
		if [ $status -ne 0 ] || [ ! -s "$output" ]; then
			echo "classification failed (exit $status), log:" >&2
			tail -5 "$work/log.txt" >&2
			rm -rf "$work"
			exit 2
		fi
		hash="$(LC_ALL=C sort "$output" | md5 | cut -c1-10)"
		phases="$(grep -o 'Finished preprocessing in [0-9]* ms\|Finished precomputing in [0-9]* ms\|Finished class classification in [0-9]* ms' "$work/log.txt" \
			| sed 's/Finished preprocessing in/preprocessing/; s/Finished precomputing in/precomputation/; s/Finished class classification in/classification/; s/ ms$//' | tr '\n' ' ')"
		classifier="$(grep -o 'sufficiently saturated' "$work/log.txt" | head -1)"
		echo "hash $hash $phases${classifier:+(saturation extraction) }"
		rm -rf "$work"
		if [ -n "$expected" ] && [ "${expected:0:10}" != "$hash" ]; then
			echo "DIFFERENT: expected ${expected:0:10}" >&2
			exit 1
		fi
		;;
	compare)
		shift
		a="${1:?output A}"; b="${2:?output B}"
		pairs() {
			LC_ALL=C awk '/<SubClassOf>/{getline x; getline y; if (match(x,/IRI="[^"]*"/)) {s=substr(x,RSTART+5,RLENGTH-6); if (match(y,/IRI="[^"]*"/)) print s, substr(y,RSTART+5,RLENGTH-6)}}
			              /<EquivalentClasses>/{eq=""; while ((getline l) > 0 && l !~ /<\/EquivalentClasses>/) { if (match(l,/IRI="[^"]*"/)) eq=eq" "substr(l,RSTART+5,RLENGTH-6) } print "equiv" eq }' "$1" | LC_ALL=C sort -u
		}
		work="$(mktemp -d)"
		pairs "$a" > "$work/a.txt"; pairs "$b" > "$work/b.txt"
		echo "pairs: A $(wc -l < "$work/a.txt" | tr -d ' '), B $(wc -l < "$work/b.txt" | tr -d ' ')"
		echo "== only in A ($(LC_ALL=C comm -23 "$work/a.txt" "$work/b.txt" | wc -l | tr -d ' '))"; LC_ALL=C comm -23 "$work/a.txt" "$work/b.txt" | head -40
		echo "== only in B ($(LC_ALL=C comm -13 "$work/a.txt" "$work/b.txt" | wc -l | tr -d ' '))"; LC_ALL=C comm -13 "$work/a.txt" "$work/b.txt" | head -40
		rm -rf "$work"
		;;
	*)
		sed -n '2,24p' "$0" | sed 's/^# \{0,1\}//'
		exit 2
		;;
esac
