# Redland-only conformance cases

These cases need a build with the Redland/Rasqal libraries (`Konclude.pro`, not
`KoncludeWithoutRedland.pro`): the Turtle/RDF datasets need the Redland RDF
parser, and the queries of the `sparql` suite use SPARQL forms (`OPTIONAL`,
`UNION`, `FILTER`, `ORDER BY`, class variables) that only the Rasqal query
parser handles.

Run them from the repository or source root, so that the `LOAD <file:Tests/...>`
commands in the LUBM query resolve:

```bash
Release/Konclude conformance -i Tests/redland-cases -o Tests/redland-results/result.owl.xml
```

or use the helper, which accepts an alternative binary as its first argument:

```bash
Tests/run-redland-conformance.sh
```
