#!/usr/bin/env bash
#
# Measures how the classification time of Konclude scales with the number of processing units,
# for choosing -w on a given machine, or konclude.processorCount for the Protege plug-in.
#
# Every combination of ontology, processing unit count and variant is classified once per round.
# The order of the combinations is rotated and reversed from round to round, so that none of them
# always runs first or always after a particular other one: on a machine whose state drifts,
# a fixed order shows differences that are not there. Compare medians over several rounds, not
# single runs; on a 16 core Mac the same binary spread by about 4 s around 36 s on SNOMED CT.
#
# Usage: ./scaling-benchmark.sh [options] <Konclude binary> <ontology> [<ontology> ...]
#
#   -w <counts>        the processing unit counts to compare, separated by spaces, a number or
#                      AUTO each, "1 2 4 8 16 32 64 AUTO" without the ones above the number of
#                      processors by default
#   -r <rounds>        how often each combination is classified, 3 by default
#   -V <name>=<prefix> a variant, whose command prefix goes in front of the Konclude command,
#                      e.g. -V thp='env MALLOC_CONF=thp:always' for transparent huge pages in
#                      jemalloc, or -V node0='numactl --cpunodebind=0 --membind=0' for one NUMA
#                      node; repeatable, a variant 'base' without prefix is always run
#   -a <argument>      an argument appended to every classification, e.g.
#                      -a +=Konclude.Calculation.Memory.RecycledPoolLimit=200000; repeatable
#   -t <seconds>       the time limit of a run, 7200 by default, needs the timeout command
#   -o <directory>     where the results go, ./scaling-<date>-<time> by default
#   -n                 no warm-up; by default each ontology is classified once before the
#                      measured runs, so that the first measured run does not also read the
#                      file from disk
#   -k                 keep the classification results, which are otherwise deleted after they
#                      have been hashed; the results of SNOMED CT take about 100 MB each
#
# The results directory holds system.txt (processors, NUMA nodes, memory, page size, transparent
# huge pages, kernel, binary), results.csv (one line per run), summary.txt and a log per run.
#
# Each run records the elapsed, user and system time, the peak resident memory, the three phases
# that Konclude logs (preprocessing, precomputation, class classification), the available memory
# and the swap in use before it started, and a hash of the inferred class hierarchy in a
# canonical form, see run-classification-reproducibility-test.sh. The summary reports how many
# different hierarchies each ontology gave. On an ontology whose classification is known to vary
# between runs (issue #28) a count above 1 is expected; on SNOMED CT it is not.
#
# Swap distorts the times: a run that pushes the machine into swap takes longer for reasons that
# have nothing to do with the processing units. The swap in use before each run is in the results,
# and the script warns when it grows. The peak memory of a run on SNOMED CT is 13 to 17 GB.
#
# Needs bash 4 or later, GNU time on Linux (package 'time', /usr/bin/time -v) or the time of
# macOS (/usr/bin/time -l), and awk, sort and sha256sum or shasum.
#
# Example, on a Linux machine with 64 processors, with the binary of an unpacked release package
# (Binaries/Konclude, not the Konclude script beside it, which only works from the package
# directory), SNOMED CT and the variant with universal restrictions, once more with transparent
# huge pages in jemalloc and once more on one NUMA node:
#
#   ./scaling-benchmark.sh -w "8 16 32 64" -r 3 \
#       -V thp='env MALLOC_CONF=thp:always' -V node0='numactl --cpunodebind=0 --membind=0' \
#       ~/Konclude-v0.7.0-1138-Linux-x64-GCC-Static-Qt5.15.17/Binaries/Konclude \
#       snomed-with-additions.owl snomed-variant.owl
#
# That is 3 rounds of 2 ontologies x 3 variants x 4 counts, 72 runs; at about a minute for
# SNOMED CT and a quarter of an hour for the variant, around ten hours. Start with -r 1 and
# fewer counts to see how long a run takes, and leave out the variants that do not apply:
# node0 only on a machine with more than one NUMA node (numactl --hardware).

set -u

SCRIPTS_DIR="$(cd "$(dirname "$0")" && pwd)"

processor_count() {
	getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 1
}
PROCESSORS="$(processor_count)"

COUNTS=""
ROUNDS=3
VARIANT_NAMES=(base)
VARIANT_PREFIXES=("")
EXTRA_ARGUMENTS=()
TIME_LIMIT=7200
OUTPUT_DIR=""
WARM_UP=1
KEEP=0

while getopts ':w:r:V:a:t:o:nkh' opt; do
	case "$opt" in
		w) COUNTS="$OPTARG" ;;
		r) ROUNDS="$OPTARG" ;;
		V)
			case "$OPTARG" in
				*=*) VARIANT_NAMES+=("${OPTARG%%=*}"); VARIANT_PREFIXES+=("${OPTARG#*=}") ;;
				*) echo "a variant has to be given as <name>=<prefix>, got '$OPTARG'." >&2; exit 2 ;;
			esac
			;;
		a) EXTRA_ARGUMENTS+=("$OPTARG") ;;
		t) TIME_LIMIT="$OPTARG" ;;
		o) OUTPUT_DIR="$OPTARG" ;;
		n) WARM_UP=0 ;;
		k) KEEP=1 ;;
		h) sed -n '2,62p' "$0" | sed 's/^#//;s/^ //'; exit 0 ;;
		\?) echo "unknown option '-$OPTARG', use -h for the usage." >&2; exit 2 ;;
		:) echo "option '-$OPTARG' needs an argument, use -h for the usage." >&2; exit 2 ;;
	esac
done
shift $((OPTIND - 1))

if [ "${BASH_VERSINFO[0]}" -lt 4 ]; then
	echo "bash 4 or later is needed, this is $BASH_VERSION." >&2
	exit 2
fi
if [ $# -lt 2 ]; then
	echo "a Konclude binary and at least one ontology are needed, use -h for the usage." >&2
	exit 2
fi
KONCLUDE="$1"
shift
ONTOLOGIES=("$@")
if [ ! -x "$KONCLUDE" ]; then
	echo "'$KONCLUDE' is not an executable." >&2
	exit 2
fi
for ontology in "${ONTOLOGIES[@]}"; do
	if [ ! -r "$ontology" ]; then
		echo "'$ontology' cannot be read." >&2
		exit 2
	fi
done
case "$ROUNDS" in
	''|*[!0-9]*|0) echo "the number of rounds has to be a positive number, got '$ROUNDS'." >&2; exit 2 ;;
esac
case "$TIME_LIMIT" in
	''|*[!0-9]*) echo "the time limit has to be a number of seconds, got '$TIME_LIMIT'." >&2; exit 2 ;;
esac

if [ -z "$COUNTS" ]; then
	for count in 1 2 4 8 16 32 64; do
		if [ "$count" -le "$PROCESSORS" ]; then
			COUNTS="$COUNTS $count"
		fi
	done
	COUNTS="${COUNTS# } AUTO"
fi
read -r -a COUNT_LIST <<< "$COUNTS"
for count in "${COUNT_LIST[@]}"; do
	case "$count" in
		AUTO) ;;
		''|*[!0-9]*|0) echo "a processing unit count has to be AUTO or a positive number, got '$count'." >&2; exit 2 ;;
		*)
			if [ "$count" -gt "$PROCESSORS" ]; then
				echo "Note: $count processing units are more than the $PROCESSORS processors of this machine."
			fi
			;;
	esac
done

# the timing tool, GNU time on Linux and the BSD time of macOS report differently
case "$(uname -s)" in
	Darwin) TIME_COMMAND=(/usr/bin/time -l) ;;
	*)
		if ! /usr/bin/time -v true >/dev/null 2>&1; then
			echo "GNU time is needed for the peak memory, install it with 'sudo apt-get install time'." >&2
			exit 2
		fi
		TIME_COMMAND=(/usr/bin/time -v)
		;;
esac
TIMEOUT_COMMAND=()
if command -v timeout >/dev/null 2>&1; then
	TIMEOUT_COMMAND=(timeout "$TIME_LIMIT")
else
	echo "Note: there is no timeout command, so the runs have no time limit."
fi
if command -v sha256sum >/dev/null 2>&1; then
	HASH_COMMAND=(sha256sum)
else
	HASH_COMMAND=(shasum -a 256)
fi

if [ -z "$OUTPUT_DIR" ]; then
	OUTPUT_DIR="./scaling-$(date +%Y-%m-%d-%H%M%S)"
fi
mkdir -p "$OUTPUT_DIR/logs" || exit 2
OUTPUT_DIR="$(cd "$OUTPUT_DIR" && pwd)"
RESULTS="$OUTPUT_DIR/results.csv"

now() {
	if [ -n "${EPOCHREALTIME:-}" ]; then
		echo "$EPOCHREALTIME"
	else
		date +%s.%N 2>/dev/null | grep -v N || date +%s
	fi
}

# available memory and swap in use, in MB
memory_state() {
	if [ -r /proc/meminfo ]; then
		awk '/^MemAvailable:/ {available = $2} /^SwapTotal:/ {total = $2} /^SwapFree:/ {free = $2}
			END {printf "%d %d\n", available / 1024, (total - free) / 1024}' /proc/meminfo
	else
		local page free inactive swap
		page="$(sysctl -n hw.pagesize)"
		free="$(vm_stat | awk '/Pages free/ {gsub("\\.", "", $3); print $3}')"
		inactive="$(vm_stat | awk '/Pages inactive/ {gsub("\\.", "", $3); print $3}')"
		swap="$(sysctl -n vm.swapusage | awk '{for (i = 1; i <= NF; i++) if ($i == "used") {v = $(i + 2); sub("M", "", v); print int(v)}}')"
		echo "$(( (free + inactive) * page / 1048576 )) ${swap:-0}"
	fi
}

# the peak resident memory in MB and the user and system seconds from the output of time
time_figures() {
	awk -v os="$(uname -s)" '
		os == "Darwin" && /maximum resident set size/ { rss = $1 / 1048576 }
		os == "Darwin" && / real .* user .* sys/ { user = $3; sys = $5 }
		os != "Darwin" && /Maximum resident set size/ { rss = $NF / 1024 }
		os != "Darwin" && /User time \(seconds\)/ { user = $NF }
		os != "Darwin" && /System time \(seconds\)/ { sys = $NF }
		END { printf "%d %s %s\n", rss, (user == "" ? "NA" : user), (sys == "" ? "NA" : sys) }
	' "$1"
}

# the milliseconds of a phase that Konclude logs, 'Finished <phase> in <n> ms'
phase_ms() {
	local value
	value="$(grep -o "Finished $2 in [0-9]* ms" "$1" | tail -n 1 | grep -o "[0-9][0-9]*")"
	echo "${value:-NA}"
}

# the canonical form of a classification result, as in run-classification-reproducibility-test.sh:
# one sorted line per equivalence class, one line per subsumption between the smallest members of
# the two equivalence classes, which leaves exactly the differences that change what was inferred
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

# the machine, for reading the results later
{
	echo "date        : $(date)"
	echo "host        : $(hostname)"
	echo "kernel      : $(uname -srvm)"
	echo "processors  : $PROCESSORS online"
	echo "page size   : $(getconf PAGESIZE 2>/dev/null || getconf PAGE_SIZE 2>/dev/null)"
	echo "binary      : $KONCLUDE"
	echo "counts      : ${COUNT_LIST[*]}"
	echo "rounds      : $ROUNDS"
	for i in "${!VARIANT_NAMES[@]}"; do
		echo "variant     : ${VARIANT_NAMES[$i]} = '${VARIANT_PREFIXES[$i]}'"
	done
	echo "arguments   : ${EXTRA_ARGUMENTS[*]:-none}"
	for ontology in "${ONTOLOGIES[@]}"; do
		echo "ontology    : $ontology ($(wc -c < "$ontology" | tr -d ' ') bytes)"
	done
	echo "MALLOC_CONF : ${MALLOC_CONF:-unset}"
	if [ -r /sys/kernel/mm/transparent_hugepage/enabled ]; then
		echo "THP enabled : $(cat /sys/kernel/mm/transparent_hugepage/enabled)"
		echo "THP defrag  : $(cat /sys/kernel/mm/transparent_hugepage/defrag)"
	fi
	echo
	if command -v lscpu >/dev/null 2>&1; then lscpu; echo; fi
	if command -v numactl >/dev/null 2>&1; then numactl --hardware; echo; fi
	if command -v free >/dev/null 2>&1; then free -g; echo; fi
	if [ "$(uname -s)" = Darwin ]; then
		sysctl -n machdep.cpu.brand_string hw.memsize hw.perflevel0.physicalcpu hw.perflevel1.physicalcpu 2>/dev/null
		echo
	fi
} > "$OUTPUT_DIR/system.txt"

# the combinations, which every round runs in a rotated order
COMBINATIONS=()
for ontology_index in "${!ONTOLOGIES[@]}"; do
	for variant_index in "${!VARIANT_NAMES[@]}"; do
		for count in "${COUNT_LIST[@]}"; do
			COMBINATIONS+=("$ontology_index $variant_index $count")
		done
	done
done
COMBINATION_COUNT=${#COMBINATIONS[@]}

echo "round,ontology,variant,count,status,elapsed_s,user_s,system_s,peak_rss_mb,preprocessing_ms,precomputing_ms,classification_ms,available_mb_before,swap_mb_before,inferences,hierarchy_sha256" > "$RESULTS"

echo "Konclude   : $KONCLUDE"
echo "results    : $OUTPUT_DIR"
echo "runs       : $ROUNDS rounds of $COMBINATION_COUNT combinations"
echo

# runs one classification; the arguments are the log name, the variant index, the count and the
# ontology; prints 'status elapsed' and leaves the log, the output of time and the result behind
classify() {
	local name="$1" variant_index="$2" count="$3" ontology="$4"
	local prefix=()
	read -r -a prefix <<< "${VARIANT_PREFIXES[$variant_index]}"
	local start end status
	start="$(now)"
	# time runs timeout, which runs a shell that sends the output of Konclude to the log and
	# becomes it, so that the report of time, on its standard error, stays apart from the log;
	# time reports the resources of the processes it waited for, Konclude among them
	"${TIME_COMMAND[@]}" "${TIMEOUT_COMMAND[@]+"${TIMEOUT_COMMAND[@]}"}" \
		sh -c 'log="$1"; shift; exec "$@" > "$log" 2>&1' sh "$OUTPUT_DIR/logs/$name.log" \
		"${prefix[@]+"${prefix[@]}"}" "$KONCLUDE" classification -w "$count" -i "$ontology" \
		-o "$OUTPUT_DIR/logs/$name.xml" "${EXTRA_ARGUMENTS[@]+"${EXTRA_ARGUMENTS[@]}"}" \
		2> "$OUTPUT_DIR/logs/$name.time"
	status=$?
	end="$(now)"
	echo "$status $(awk -v s="$start" -v e="$end" 'BEGIN {printf "%.2f", e - s}')"
}

if [ "$WARM_UP" -eq 1 ]; then
	for ontology_index in "${!ONTOLOGIES[@]}"; do
		printf 'warming up with %s ... ' "$(basename "${ONTOLOGIES[$ontology_index]}")"
		read -r status elapsed <<< "$(classify "warm-up-$ontology_index" 0 AUTO "${ONTOLOGIES[$ontology_index]}")"
		rm -f "$OUTPUT_DIR/logs/warm-up-$ontology_index.xml"
		echo "status $status, $elapsed s"
	done
	echo
fi

previous_swap=""
for round in $(seq 1 "$ROUNDS"); do
	# rotate by a share of the list each round and reverse every second round
	offset=$(( (round - 1) * COMBINATION_COUNT / ROUNDS ))
	order=()
	for i in $(seq 0 $((COMBINATION_COUNT - 1))); do
		order+=($(( (i + offset) % COMBINATION_COUNT )))
	done
	if [ $((round % 2)) -eq 0 ]; then
		reversed=()
		for (( i = ${#order[@]} - 1; i >= 0; i-- )); do reversed+=("${order[$i]}"); done
		order=("${reversed[@]}")
	fi
	for index in "${order[@]}"; do
		read -r ontology_index variant_index count <<< "${COMBINATIONS[$index]}"
		ontology="${ONTOLOGIES[$ontology_index]}"
		ontology_name="$(basename "$ontology")"
		variant="${VARIANT_NAMES[$variant_index]}"
		name="round$round-$ontology_index-$variant-w$count"
		read -r available swap <<< "$(memory_state)"
		if [ -n "$previous_swap" ] && [ "$swap" -gt $((previous_swap + 256)) ]; then
			echo "Warning: the swap in use grew from $previous_swap MB to $swap MB, the times that follow may be distorted."
		fi
		previous_swap="$swap"
		printf 'round %d/%d  %-40s %-8s -w %-5s ... ' "$round" "$ROUNDS" "$ontology_name" "$variant" "$count"
		read -r status elapsed <<< "$(classify "$name" "$variant_index" "$count" "$ontology")"
		read -r rss user system <<< "$(time_figures "$OUTPUT_DIR/logs/$name.time")"
		preprocessing="$(phase_ms "$OUTPUT_DIR/logs/$name.log" preprocessing)"
		precomputing="$(phase_ms "$OUTPUT_DIR/logs/$name.log" precomputing)"
		classification="$(phase_ms "$OUTPUT_DIR/logs/$name.log" "class classification")"
		inferences="NA"
		hash="NA"
		if [ "$status" -eq 0 ] && [ -s "$OUTPUT_DIR/logs/$name.xml" ]; then
			canonicalise "$OUTPUT_DIR/logs/$name.xml" > "$OUTPUT_DIR/logs/$name.canonical"
			inferences="$(wc -l < "$OUTPUT_DIR/logs/$name.canonical" | tr -d ' ')"
			hash="$("${HASH_COMMAND[@]}" < "$OUTPUT_DIR/logs/$name.canonical" | cut -c1-16)"
			rm -f "$OUTPUT_DIR/logs/$name.canonical"
		fi
		if [ "$KEEP" -eq 0 ]; then
			rm -f "$OUTPUT_DIR/logs/$name.xml"
		fi
		echo "$round,$ontology_name,$variant,$count,$status,$elapsed,$user,$system,$rss,$preprocessing,$precomputing,$classification,$available,$swap,$inferences,$hash" >> "$RESULTS"
		if [ "$status" -eq 0 ]; then
			echo "$elapsed s, classification $classification ms, peak $rss MB"
		elif [ "$status" -eq 124 ]; then
			echo "stopped after the time limit of $TIME_LIMIT s"
		else
			echo "FAILED with status $status, see logs/$name.log"
		fi
	done
done

# medians over the rounds of each combination, and the number of different hierarchies
summarise() {
	echo "Medians over $ROUNDS rounds (minimum-maximum), successful runs only. Times in seconds,"
	echo "the classification phase as Konclude logs it, peak memory in GB."
	echo
	printf '%-40s %-8s %-6s %4s %22s %22s %14s\n' ontology variant count runs elapsed classification "peak memory"
	tail -n +2 "$RESULTS" | awk -F, '$5 == 0 {print $2 "," $3 "," $4}' | LC_ALL=C sort -u -t, -k1,1 -k2,2 -k3,3n | while IFS=, read -r ontology variant count; do
		# figures <column> <divisor> <decimals>
		figures() {
			awk -F, -v o="$ontology" -v v="$variant" -v c="$count" -v column="$1" -v scale="$2" \
				'$2 == o && $3 == v && $4 == c && $5 == 0 && $column != "NA" {printf "%.4f\n", $column / scale}' "$RESULTS" \
				| sort -n | awk -v d="$3" '{value[NR] = $1} END {
					if (NR == 0) { print "n/a"; exit }
					median = (NR % 2) ? value[(NR + 1) / 2] : (value[NR / 2] + value[NR / 2 + 1]) / 2
					printf "%." d "f (%." d "f-%." d "f)\n", median, value[1], value[NR] }'
		}
		runs="$(awk -F, -v o="$ontology" -v v="$variant" -v c="$count" '$2 == o && $3 == v && $4 == c && $5 == 0' "$RESULTS" | wc -l | tr -d ' ')"
		printf '%-40s %-8s %-6s %4s %22s %22s %14s\n' "$ontology" "$variant" "$count" "$runs" \
			"$(figures 6 1 1)" "$(figures 12 1000 2)" "$(figures 9 1024 1)"
	done
	echo
	for ontology in "${ONTOLOGIES[@]}"; do
		ontology_name="$(basename "$ontology")"
		hashes="$(awk -F, -v o="$ontology_name" '$2 == o && $16 != "NA" {print $16}' "$RESULTS" | sort -u | wc -l | tr -d ' ')"
		failed="$(awk -F, -v o="$ontology_name" '$2 == o && $5 != 0' "$RESULTS" | wc -l | tr -d ' ')"
		echo "$ontology_name: $hashes different class hierarchies, $failed failed or stopped runs"
	done
}
summarise | tee "$OUTPUT_DIR/summary.txt"
