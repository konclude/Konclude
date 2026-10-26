# Redland-only conformance cases

These cases load Turtle/RDF data and therefore need a build with the Redland
RDF libraries (`Konclude.pro`, not `KoncludeWithoutRedland.pro`).

Run them from the repository or source root, so that the `LOAD <file:Tests/...>`
commands in the LUBM query resolve:

```bash
Release/Konclude conformance -i Tests/redland-cases -o Tests/redland-results/result.owl.xml
```

or use the helper, which accepts an alternative binary as its first argument:

```bash
Tests/run-redland-conformance.sh
```
