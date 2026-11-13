# Sourced by the run-*.sh scripts beside it, after JAVA_DIR is set.
#
# A crash of the virtual machine does not always show in its exit status: a crash while the
# process exits, such as the one of issue #30, where a processor thread of Konclude was still
# running while the static objects of Qt were destroyed, left the status at 0 and nothing but
# the crash report. So every scenario is run with CRASH_REPORT_OPTION, which directs the report
# into CRASH_REPORT_DIR, and crash_report_found is called after every scenario, whatever its
# exit status. It is true if a report appeared, renames the report after the suite and the
# scenario, so that it is neither matched again nor blamed on the next scenario, and prints
# what crashed.

. "$JAVA_DIR/native-paths.sh"

CRASH_REPORT_DIR="$JAVA_DIR/crash-reports"
mkdir -p "$CRASH_REPORT_DIR"
# the JVM is a Windows program under Git Bash, so it gets the path in the form of the platform
CRASH_REPORT_OPTION="-XX:ErrorFile=$(native_path "$CRASH_REPORT_DIR/hs_err_pid%p.log")"

# crash_report_found <suite> <scenario>
crash_report_found() {
	local suite="$1" scenario="$2" report kept found=1
	for report in "$CRASH_REPORT_DIR"/hs_err_pid*.log; do
		[ -e "$report" ] || continue
		kept="$CRASH_REPORT_DIR/$suite-$scenario-$(basename "$report")"
		mv "$report" "$kept"
		echo "The virtual machine crashed in the scenario '$scenario', the crash report is '$kept':" >&2
		# the signal, the problematic frame and the first frames of the native stack
		grep -m 1 -E '^# +(SIG[A-Z]+|EXCEPTION_[A-Z_]+|Internal Error|Out of Memory)' "$kept" >&2
		grep -m 1 -A 1 '^# Problematic frame:' "$kept" | tail -n 1 >&2
		sed -n '/^Native frames:/,/^$/p' "$kept" | head -n 9 >&2
		found=0
	done
	return $found
}
