# Notice

The ontology files under `ontologies/` are derived from the W3C OWL 2 test
cases published by the W3C OWL Working Group:

- <https://www.w3.org/2009/11/owl-test/approved/all.rdf>
- <https://www.w3.org/TR/owl2-test/>

The W3C test suite is made available by the W3C for testing implementations.
The files here are a format conversion (RDF/XML or functional syntax to
OWL/XML) of the premise ontologies; the test identifiers and expected answers
come from the export's metadata. See the copyright notice in the original
export for the full terms.

The control ontology and this notice are part of the Konclude test harness.
Known divergences are recorded in `Tests/known-issues.owl.xml` (with
`kt:knownFailure` links in the control ontology), not in separate lists.
Approved tests whose RDF premises the RDF-to-OWL mapping rejects are not
imported into the corpus.

`WebOnt-miscellaneous-001`, `WebOnt-miscellaneous-002` and `WebOnt-imports-011`
import further W3C ontologies, which are published as RDF/XML only. The
conformance run is offline and also covers the build without the RDF/XML
parser (`KoncludeWithoutRedland.pro`), so the axioms of the import closure are
inlined into those three premise files instead of being imported; a regeneration
has to apply the same inlining (see the note in each file's header).
