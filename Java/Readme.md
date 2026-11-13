# THE JAVA SIDE OF THE JNI BRIDGE

Konclude contains a JNI bridge in `Source/Control/Interface/JNI`, which is compiled into a
shared library by `KoncludeLIB.pro` and which exports 90 native entry points. This directory
contains the Java counterpart of that bridge, which has never been part of this repository,
in three layers:

- `Java/src` declares the native methods and drives them from a smoke test. It has no
  dependency beyond the JDK, which is what the CI compiles and runs.
- `Java/owlapi/src` translates an OWL API ontology into the bridge and implements the OWL API
  `OWLReasoner` interface on top of it, with a headless test of its own. It needs the OWL API
  on the class path.
- `Java/protege` packages the two as a plug-in of Protege 5.6, an OSGi bundle that registers
  the reasoner with the reasoner menu and carries the shared library, see THE PROTEGE PLUG-IN
  below.


## BUILDING AND RUNNING

The shared library is built with `KoncludeLIB.pro` and requires `JAVA_HOME` to be set, since
the JNI headers of the JDK are included:

```
export JAVA_HOME=/path/to/jdk
qmake KoncludeLIB.pro
make
```

Afterwards the two tests are compiled and run with

```
Java/run-jni-smoke-test.sh
Java/run-owlapi-test.sh
```

which search the library in `Release` and `release` and accept the directory or the file of
the library as first argument.

Each scenario runs in a virtual machine of its own, and fails if that exits with a status other
than 0 or leaves a crash report. The second is needed because a crash while the process exits
can leave the status at 0 (issue #30). Every scenario writes its report into
`Java/crash-reports`, and the script renames it to `<suite>-<scenario>-hs_err_pid<pid>.log` and
prints the signal and the top native frames, see `Java/jvm-crash-reports.sh`. The same applies
to `run-protege-plugin-test.sh` below.

`run-owlapi-test.sh` also needs the OWL API. Note that `owlapi-distribution` is not a single
jar, it pulls in about fifty dependencies, so the script looks for the class path in

1. the environment variable `OWLAPI_CLASSPATH`,
2. the jars in `Java/owlapi/lib`,
3. the local Maven repository, resolved with `mvn dependency:build-classpath`.

The wrapper is written against OWL API 4.5.29, which is the version that Protege 5.6 ships,
see `OWLAPI_VERSION` in the script.

A reasoner that is created without a loading configuration of its own, which is what
`KoncludeReasonerFactory` does, is initialised with
`KoncludeReasoner.DEFAULT_LOADING_CONFIGURATION`. That differs from the library's own default
in one line, `Konclude.Calculation.ProcessorCount=AUTO`: the library, like the command line
without `-w AUTO`, would otherwise reason on a single processing unit, which classifies
SNOMED CT in 156 s instead of 35 s. A configuration of a caller's own replaces these
arguments instead of extending them, so it has to start from that constant.


## SCENARIOS

`run-jni-smoke-test.sh`, which checks that the bridge itself can be driven from Java:

| scenario | what it checks |
| --- | --- |
| `subclass` | `SubClassOf(A, B)` is built through the bridge and `A` is reported as sub class of `B` |
| `hierarchy` | for `A < B < C` the answers are `[A, B]` for `direct=false` and `[B]` for `direct=true` |
| `lifecycles` | three reasoners are initialised, used and closed in one virtual machine |
| `missing-processor` | a configuration string that drops `-JNICommandProcessorLoader` is reported as a `KoncludeReasonerException` instead of terminating the virtual machine |

`run-owlapi-test.sh`, which checks the wrapper:

| scenario | what it checks |
| --- | --- |
| `hierarchy` | the class hierarchy of a small family ontology, direct and indirect, and the top and the bottom node |
| `expressions-tableau` | the `expressions` checks with the decision of the sub classes from the saturation switched off, so that the tableau path stays tested, see SUB CLASSES OF AN EXPRESSION FROM THE SATURATION |
| `expressions-el` | equivalent and super classes of `hasChild some Person` and `Man and hasChild some Person` over an EL ontology without individuals, whose saturation keeps reduced labels that the tableau must not take as expanded (issue #47); and, without the domain and range of `hasChild`, the equivalent, direct super and direct sub classes of the conjunction asked after `hasChild some Person` (issue #50) |
| `expressions` | the questions about anonymous class expressions, as the DL query tab of Protege asks them: equivalent, sub and super classes, satisfiability and instances of `ObjectSomeValuesFrom`, `ObjectIntersectionOf`, `ObjectUnionOf` and `ObjectComplementOf` expressions, and a fresh entity inside one |
| `individuals` | the types, the instances, the same individuals, the grouping by sameAs and the object property values |
| `properties` | the object property hierarchy, sub, super and equivalent, direct and indirect |
| `datatypes` | a sub data property axiom and a datatype restriction, the two constructs whose entry points were mixed up |
| `inconsistency` | an inconsistent ontology is reported as inconsistent |
| `unsupported` | every question that the bridge cannot answer throws an `UnsupportedOperationException` |
| `lifecycle` | three reasoners in one virtual machine, and `flush` after a change of the ontology |
| `merges` | an ABox that forces two individuals to be merged, which the precomputation used to hang on |
| `timeout` | a call that overruns the configured time out is reported as a `TimeOutException` |
| `entailment` | `isEntailed` for the axiom types the wrapper reduces to its queries, 27 axioms, and the exception for the others |
| `interrupt` | an idle interrupt changes nothing, a call caught in flight ends in a `ReasonerInterruptedException`, the answers afterwards are complete |
| `progress` | a precomputation reports its tasks in order, the saturation and the classification each with progress within range, to the monitor |

All scenarios pass on macOS on arm64 with Liberica JDK 17 and Qt 5.15.


## THE PROTEGE PLUG-IN

`Java/protege` is a Maven project that builds the plug-in as an OSGi bundle, the way the
FaCT++ plug-in is built: `pom.xml` compiles `Java/src` and `Java/owlapi/src` into it beside
`KoncludeProtegeReasonerInfo`, which extends Protege's `AbstractProtegeOWLReasonerInfo` and
hands out `KoncludeReasonerFactory`, and `plugin.xml` registers that class with the extension
point `org.protege.editor.owl.inference_reasonerfactory`, which is what puts 'Konclude' into
the reasoner menu. The bundle requires the OWL API and Protege editor bundles that Protege
ships, so nothing but the classes of this directory and the shared library is packaged.

The shared library travels inside the bundle under `lib/native/<platform>/` and is named by
the `Bundle-NativeCode` header of the manifest. The OSGi framework unpacks the entry that
matches the running platform and answers `System.loadLibrary("Konclude")` with it, so
`KoncludeReasoner.loadNativeLibrary` does not know it is in a plug-in. The header ends in `*`,
which makes the native code optional: on a platform the bundle has no library for it still
resolves, the reasoner is still listed, and starting it fails with an `UnsatisfiedLinkError`
that `KoncludeProtegeReasonerInfo.initialise` has already written into the log of Protege.

Every native call that takes 50 ms or longer is written to the log at INFO with its duration,
and for a question about a class expression with the expression, for instance 'Konclude:
getSubClasses of ObjectComplementOf(<http://snomed.info/id/84753008>) took 63184 ms'; the
questions about named classes take a fraction of a millisecond and stay at DEBUG. A slow
class selection in Protege is thereby attributed at once: the log of 2026-09-23 showed one
such call per selection and the call itself taking the time, which ruled out the views.

The library has to be one that depends on nothing but the system, since Protege has no Qt to
offer: build `KoncludeLIB.pro` against a static Qt, with `QT-=gui`, `CONFIG-=static staticlib`
and `CONFIG+=shared dll`, and `otool -L`, `ldd` or `dumpbin /dependents` shows no Qt
afterwards, see 'A LIBRARY WITHOUT QT DEPENDENCIES' below. The release workflow builds such a
library for macOS on arm64, Linux on x64 and arm64 and Windows on x64 and packages all four
into one plug-in, which the draft release carries beside the packages of the command line program.

The properties `konclude.library.macos-arm64`, `konclude.library.linux-x64`,
`konclude.library.linux-arm64` and `konclude.library.windows-x64` each add the library of that
platform through a profile of the pom, and the build fails if the file is missing. On a Mac with
an ARM processor the library for macOS is always packaged, from `Release/libKonclude.dylib` of
an in-tree build unless the property names another file; elsewhere a build carries only the
libraries whose property is given:

```
cd Java/protege
mvn package -Dkonclude.library.macos-arm64=/path/to/libKonclude.dylib
cp target/konclude-protege-plugin-*.jar /Applications/Protege-5.6.9/plugins/
```

A change of the plug-in needs a restart of Protege. The plug-in is compiled against Protege
5.6.6, the last release on Maven Central, and runs in 5.6.9; the byte code targets Java 11,
which is what the Protege distributions bundle.

The reasoner uses one processing unit per logical processor the system reports (`AUTO`). The
system property `konclude.processorCount`, or else the environment variable
`KONCLUDE_PROCESSOR_COUNT`, sets another number. In Protege that is a line
`append=-Dkonclude.processorCount=8` in `jvm.conf`: `run.sh` reads `~/.Protege/conf/jvm.conf`
if it exists and the `conf/jvm.conf` of the installation otherwise. Setting it
matters inside a container or under `taskset`: on Linux `AUTO` is Qt 5.15's
`idealThreadCount()`, which is `sysconf(_SC_NPROCESSORS_ONLN)`, the processors online on the
machine, and ignores both a CPU limit (`docker --cpus`) and a CPU set (`--cpuset-cpus`). Fewer
can also be better beside other work, and on a machine with many processors more units help
only as far as an ontology can be classified in parallel. A value that is neither `AUTO` nor a
positive number is reported in the log and replaced by `AUTO`. Protege's log shows the count in
effect when the plug-in starts: 'the shared library of Konclude is loaded, processor count 8'.
The same applies to any program that uses `KoncludeReasoner`.

Other settings of the reasoner go into the system property `konclude.configuration`, or else
the environment variable `KONCLUDE_CONFIGURATION`, as `+=Konclude.<Key>=<Value>` separated by
spaces or commas, and are appended to the loading configuration, after the processor count, so
that a `+=Konclude.Calculation.ProcessorCount=` among them wins. In `jvm.conf` they have to be
separated by commas, since `run.sh` splits the options at spaces:

```
append=-Dkonclude.configuration=+=Konclude.Calculation.Memory.RecycledPoolLimit=200000,+=Konclude.Calculation.Memory.TaskProcessorFreePoolReserveLimit=5000
```

Only such settings are taken; any other argument, such as a loader that would start a server
with an open port inside Protege, is dropped with a warning in the log. The settings in effect
are logged beside the processor count. The keys and their defaults are those of
`Source/Control/Command/CReasonerConfigurationGroup.cpp`; on the command line a trailing
`+=Konclude.<Key>=<Value>` does the same, which is how a setting is tried out before it goes
into `jvm.conf`.

Once the class hierarchy is computed, the wrapper reads the whole class taxonomy from the
library in one call and answers the direct sub and super classes and the equivalent classes of a
named class from it, without a call through the bridge each time. A program that walks the whole
hierarchy, such as the classification service of the SNOMED CT toolkit, otherwise makes about two
million such calls for SNOMED CT. Expressions, `direct=false` and the instances still go through
the bridge, and the taxonomy is read again after every `flush()` that has changes to apply.
`-Dkonclude.taxonomyCache=false` switches it off, for instance to compare the answers.

`run-protege-plugin-test.sh [<Protege directory>] [<library>]` builds the plug-in and then
runs it inside the OSGi framework of the given Protege installation rather than on a class
path: it starts Felix from Protege's `bundles` directory, installs every bundle in there,
starts the plug-in, which is what makes Felix unpack the library, and drives it from a test
bundle that requires the plug-in, so that every class is loaded through the bundles as it is
in Protege. Its `plugin` scenario does what Protege does with a reasoner, creating and
precomputing on a classification thread and asking from two other threads, and checks the
tasks reported to the progress monitor and a buffered change; the other scenarios are those
of `run-owlapi-test.sh`, run through the bundles. The script packages the library of the
platform it runs on, macOS on arm64 or Linux on x64 or arm64. All pass against Protege 5.6.9 on
macOS on arm64 and on Debian 13 on arm64, there with a library that uses the Qt 5.15 of the
system, which is fine for the test but not for a plug-in to ship.

What the plug-in cannot do yet: the inferred axioms that Protege's export and its displayed
inferences ask for through the queries listed under WHAT THE BRIDGE DOES NOT ANSWER, the
disjoint classes, the property domains and ranges and the data property hierarchy among
them, end in an `UnsupportedOperationException`; the 'Disjoint classes' displayed inference
is best switched off in the reasoner preferences. `interrupt` releases the caller of a
running calculation but cannot stop the calculation itself, so cancelling in the progress
window returns at once while Konclude works on in the background until it is done.

The bar of the progress window fills twice, once while the saturation initialises its nodes
and once while the classifier tests, see PROGRESS below; the consistency check between the
two and a realization without individuals are reported as busy. On macOS a busy task looks
like one that was only started: the Aqua look and feel of the
JDK (Liberica 17 was tried) draws an indeterminate bar as a flat grey track while the
component believes it is animating.


## THE CONTRACT OF THE NATIVE SIDE

The native side looks up the following Java members by name, see `CJNIHandler` and
`CJNIHandlingClassData`, so they must not be renamed:

- a field `mNativeDataPointer` of type `long` in `KoncludeReasonerBridge`,
  `AxiomExpressionBuildingBridge`, `QueryingBridge`, `ObjectSetCallbackListener` and
  `SetOfObjectSetCallbackListener`, in which the addresses of the corresponding native
  objects are stored,
- a parameterless constructor in all of these classes except `KoncludeReasonerBridge`,
- the methods `startObjectSet()`, `visitObject(Object)` and `endObjectSet()` in
  `ObjectSetCallbackListener` and `SetOfObjectSetCallbackListener`,
- the class `KoncludeReasonerException`, which the native side uses to report problems and
  which therefore has to be on the class path of the caller.

Each native method of the axiom building and the querying bridge takes the
`KoncludeReasonerBridge` as its first argument, followed by the arguments of the axiom,
expression or query. Expressions are identified by their address, which is returned as a
`long` and passed back to compose larger expressions. Axioms are told to the ontology while
they are built, and the ontology is installed by `finalizeAxiomExpressionVisitingBridge`.

The object that is handed over while an entity is built is remembered by the native side and
reported back by the queries, which is how the entities of a client, the `OWLClass` instances
of the OWL API for instance, find their way back into the query results.

All 90 entry points are declared, in `KoncludeReasonerBridge` (6),
`AxiomExpressionBuildingBridge` (66) and `QueryingBridge` (18). The declarations were checked
against the generated headers by compiling them with `javac -h` and comparing the result with
`Source/Control/Interface/JNI/com_konclude_jnibridge_*.h`, which is worth repeating after a
change of either side.

The n-ary constructs take their arguments from an argument list, which the native side keeps
on a stack, so nested lists are allowed. The native side does not check that stack: `addArgument`
and every `...FromArgumentList` method terminate the virtual machine if no list has been
pushed, so a caller has to pair them itself.


## KONCLUDE'S LOG

Konclude's log reached nobody inside another program: the library registers no observer
with its logger unless the loading configuration asks for one, such as
`-CoutLogObserverLoader`, which prints to standard output. So a warning that the ontology is
not in OWL 2 DL (#32) was lost in Protege.

Since then the library keeps its log messages in a queue, `CJNILogMessageQueue`, and the
wrapper takes them after every call into the library and logs them with SLF4J, which Protege
and most OWL API programs use.
- **Logger names:** the logger is `com.konclude.native` followed by Konclude's domain, for
  instance `com.konclude.native.Reasoner.Preprocess.NonSimpleRoleRestrictionCheck`.
- **Levels:** Konclude's warnings are logged as WARN and its errors as ERROR, so Protege shows
  them in its log window (Window > Show log...) and in `~/.Protege/logs/protege.log`. Konclude's
  information messages are logged as DEBUG. A classification writes a few hundred of them,
  mostly about its internal commands, and Protege's root logger is at info.
- **Showing the information messages:** add a logger to Protege's `conf/logback.xml`:

  ```
  <logger name="com.konclude.native" level="debug"/>
  ```

- **One queue per process:** the queue is shared by all library instances of the process,
  like the logger, and holds up to 10 000 messages. A full queue drops the oldest messages and
  reports how many it dropped.

## WHAT THE BRIDGE DOES NOT ANSWER

The querying bridge provides 12 queries about a named entity and 5 about a class expression,
which do not cover the `OWLReasoner` interface. `KoncludeReasoner` throws an
`UnsupportedOperationException` for a question that it cannot answer, rather than an empty
node set, because an empty node set is a valid answer and a caller cannot tell the two apart.

The native side has no query for the disjointness of classes and properties, the domains and
the ranges of properties, the inverse properties, the data property hierarchy, the data
property values, the different individuals and the entailment check.


## QUESTIONS ABOUT A CLASS EXPRESSION

The 12 queries about a named entity identify it by its IRI, so a question about an anonymous
class expression, the sub classes of an `ObjectSomeValuesFrom` for instance, could not be asked
through them, which is what every question of the DL query tab of Protege is, and what its
inferred disjoint classes rest on, the sub classes of an `ObjectComplementOf`.

For these the querying bridge binds a second `AxiomExpressionBuildingBridge` to itself with
`initOWLClassExpressionBuilder`. Its `buildOWL...` methods build into a revision of the
installed ontology that is never installed, so the installed ontology is not written to, and
the expressions that only a query mentions are kept in a hash of the builder's own, as the
OWLlink interface does for its complex queries, otherwise the first query would detach a copy
of the expression hash of the whole ontology from the copy-on-write copy the revision holds.
The address the builder returns is handed to `checkIsOWLClassExpressionSatisfiable` and to the
`queryOWLClassExpression...` queries for the sub, super and equivalent classes and the
instances.

On the native side these are answered by the `CComplex...AnsweringQuery` classes of the OWLlink
interface, which rebuild the expression in a testing ontology of their own and answer with the
names of the entities, so `CJNIQueryProcessor` resolves the names in the installed ontology
and reports the entities through the same callbacks as the named queries. The first such
question pays for setting up that answering machinery. A question the native side does not
answer is reported by a `KoncludeReasonerException`.

An entity that the ontology does not mention is never sent to the native side, which would
answer as if the expression were unsatisfiable and would remember the entity in the revision
of the builder. `KoncludeReasoner` applies the `FreshEntityPolicy` instead, to a class
expression as soon as one of its entities is fresh: with `DISALLOW` a `FreshEntitiesException`,
with `ALLOW` the trivial answer, an empty node set.


## DEFECTS THAT WERE FIXED ON THE NATIVE SIDE

The bridge had never been driven from Java before, so several of its entry points had never
run. Driving them against the library brought out the following, all of which are fixed:

- Two of the 84 entry points were not exported with C linkage at all, so calling them ended
  in an `UnsatisfiedLinkError`. At the end of
  `com_konclude_jnibridge_AxiomExpressionBuildingBridge.cpp` the names of two definitions were
  mixed up: the body of the datatype restriction sat in a second overload of
  `buildOWLDataUnionOfFromArgumentList` and the body of the sub data property axiom sat in
  `buildOWLDatatypeRestrictionFromArgumentList`. Since both differed from the header
  declaration in their parameters, the compiler gave them C++ linkage and the names that JNI
  looks up stayed undefined. `SubDataPropertyOf` axioms and `DatatypeRestriction` data ranges
  are usable since the two definitions were renamed.

- `CJNIQueryProcessor` handed the direct flag to
  `CSubSuperPropertiesResultVisitCallbackQuery` in the place of its `dataProperty` parameter,
  which takes `(subProperties, superProperties, direct, dataProperty)`. The direct flag was
  therefore always false, and `direct = true` asked the data property hierarchy for an object
  property, which answers with nothing at all.

- `queryOWLSubObjectProperties`, `queryOWLSuperObjectProperties` and
  `queryOWLEquivalentObjectProperties` reported only the properties whose role happened to be
  its own inverse, in practice only `owl:topObjectProperty` and `owl:bottomObjectProperty`.
  The role of a property is resolved to its expression through
  `getRoleObjectPropertyTermMappingHash`, and whenever an inverse role was created for a
  role, `CInverseObjectPropertyOfExpression(entry of the role)` was registered under **that
  role** instead of under the inverse role it denotes. It therefore replaced the named
  property of the role, the `dynamic_cast` to `CObjectPropertyExpression` in
  `CEntityExpressionSetResultVisitingCallback::visitRoleAssociatedEntityExpression` failed,
  and the property was silently dropped from the answer. The same mistake sat in both
  directions of the mapping and at three places, twice in
  `CConcreteOntologyInverseRoleBuilder::createDelayedInverseRoles` and once in
  `CSubroleTransformationPreProcess`. All three now register the inverse expression under the
  inverse role.

  The expression to role direction of that hash is read throughout the answering engine, so
  the five OWLlink and SPARQL tests of the CI were compared before and after the change. They
  answer the same, which is what would be expected: the engine unwraps an
  `ObjectInverseOf` itself before it consults the hash, see
  `COptimizedComplexExpressionAnsweringHandler`, so the wrong entries were never read there.
  Note that Konclude is a parallel reasoner and its answers come back in a different order on
  every run, so those responses have to be compared as sets and without the response times,
  not byte for byte.

- `queryOWLSameIndividuals` reported nothing. Its query took the concept realization instead
  of the same realization, and it did not override `getDynamicRealizationRequirement`, so the
  same realization was never computed for it. Both now follow `CSameIndividualsQuery`.

- The generated header declared `queryOWLSameIndividuals` with a
  `SetOfObjectSetCallbackListener` while its implementation has always looked up `visitObject`
  on `ObjectSetCallbackListener`. Calling a method id of one class on an instance of another
  is undefined, and it terminated the virtual machine with a `SIGSEGV` as soon as the query
  reported anything, which only became visible once the query answered at all. The query
  answers with one flat set, like the other `equivalent` queries, so the declaration was
  corrected to `ObjectSetCallbackListener`.

- Every complex query with `direct = true`, the direct sub or super classes or the direct
  instances of a class expression, never returned, over OWLlink as well. In
  `COptimizedComplexExpressionAnsweringHandler` the direct flag was handed on as 'sub class
  realization required', and that step could never run: the loop of
  `continueCalculationCreation` did not look at its queue, so a concept item queued for it
  alone was never processed; had it run, the completion message it sent was of the type of the
  plain realization, whose handling only queues the instances step, so the step would not have
  been finished either; and it was ordered after the instances step, whose completion queues
  no further step. The queue is now part of the loop, the item is taken from it like the items
  of the other steps, the step sends its own completion type and comes before the instances
  step, and it is only asked for when the instances are, which is the one place that needs the
  direct sub classes realized, to subtract their instances.

- Konclude's complex queries reported a class that is equivalent to the asked expression among
  its sub and its super classes, and their direct answers were not minimal, a class below
  another class of the answer was reported as direct as well
  ([rhausam/Konclude#12](https://github.com/rhausam/Konclude/issues/12)). `CJNIQueryProcessor`
  worked around both: it asked for the equivalent classes first and answered an expression
  that is equivalent to a class from the class hierarchy of that class, and it took the direct
  sub or super classes of any other expression from the complete answer, those none of whose
  parents, or children, is in it.

  The engine's direct sub classes are minimal since #50, see below. Since #12 the engine also
  answers strictly: the sub and super classes of a concept item still include the equivalent
  node, which is how the equivalence is found, and the answer of a sub or super classes query
  starts from that node's children or parents instead of from the node. Which node that is,
  the equivalent classes step decides without the sub classes: a node N is equivalent to the
  expression X exactly when N is the only direct super class of X and N is subsumed by X, so
  one decision settles it, by the saturation or by one subsumption test. The super and the
  equivalent classes of an expression therefore no longer wait for the search of its sub
  classes, which on SNOMED CT took 15 to 20 s for an expression whose only direct super class
  is `owl:Thing`. The bridge asks
  for the direct or the complete answer and passes it on. Over OWLlink, `GetSubClasses` and
  `GetSuperClasses` of an expression no longer list its equivalent class either. An
  unsatisfiable expression now has the super classes of `owl:Nothing`, where the bridge had
  answered from the first class of its equivalents, `Impossible` in the family ontology, whose
  named super classes come out empty.

  The answering engine lost part of an expression once a sub expression of it had been asked
  about before, over OWLlink as well
  ([rhausam/Konclude#13](https://github.com/rhausam/Konclude/issues/13)): after the
  equivalent classes of `hasChild some Person` the equivalent classes of
  `Man and (hasChild some Person)` came back empty instead of `Father`, and its super classes
  lacked `Man`. The satisfiability test of the expression gives its known super classes from
  the model, all of them; `searchSuperSubSuperClassesResult` then took the super classes of
  the earlier item that subsumes the expression, `Parent` and its ancestors, and replaced the
  known set with them instead of adding them to it. They are added now. The `expressions`
  scenario asks in that order on purpose and checks the three answers.

  In an ontology without individuals whose classes are all EL, the answers missed classes
  that an absorbed definition gives
  ([rhausam/Konclude#47](https://github.com/rhausam/Konclude/issues/47)): with
  `Parent ≡ Person and (hasChild some Person)` and `hasChild` with the domain `Person`,
  `hasChild some Person` was not equivalent to `Parent`. The absorption turns the definition
  into `Person ⊑ ∀hasChild⁻.T` and `T and Person → Parent`, so a `Person` successor sends `T`
  back to its predecessor. For such an ontology the saturation keeps a reduced label of each
  node, without the ALL and most of the implication concepts it applied; the tableau set up
  the successor of `hasChild some Person` from that label and took the concepts as expanded,
  so `∀hasChild⁻.T` never came up. A saturation node now records whether its label is reduced
  and the tableau does not take a reduced label over. With an inverse property, an individual
  or a negation in the ontology the labels are complete, which is why the `expressions`
  scenario did not show it; the `expressions-el` scenario does.

  The equivalent classes of an expression depended on what had been asked before
  ([rhausam/Konclude#50](https://github.com/rhausam/Konclude/issues/50)): without the domain
  and the range of `hasChild`, `Man and (hasChild some Person)` was equivalent to `Father`
  when asked first, and had no equivalent class after questions about `hasChild some Person`.
  The engine reports an equivalent class when the direct super and the direct sub classes of
  the expression are the same single node. The sub classes are searched top down from a set
  of candidates, stopping at every subsumed node, and the search reaches a node over several
  paths: for `hasChild some Person` it found `Parent` through `Person`, `Father` through `Man`
  and `owl:Nothing` through `Woman`, and kept all three as direct. The later expression took
  those as its candidates, found `Father` and `owl:Nothing`, and so had two direct sub
  classes. Two changes: the direct sub classes are reduced to the topmost of the found nodes,
  the bottom node dropped next to any other; and the candidates taken from an earlier
  subsuming expression are its topmost sub classes, not its most specific ones, which would
  miss the sub classes below the others. The candidates now start lower than before, and on
  SNOMED CT two conjunctions asked after their existential part took 0.4 s and 1 ms instead
  of 12.5 and 11.5 s, with the same answers. The `expressions-el` scenario asks in that
  order over the ontology without the domain and the range.


- A named unsatisfiable class was answered from a node of its own
  ([rhausam/Konclude#55](https://github.com/rhausam/Konclude/issues/55)): `Impossible` had no
  super classes and only itself as equivalent class, while `owl:Nothing` listed `Impossible`
  among its equivalent classes and had the leaves as its direct super classes. The classifier
  that works from the saturation added an unsatisfiable class to the bottom node without
  mapping the class to that node, as the other classifiers do, and `CTaxonomy::getHierarchyNode`
  creates a node for a class it does not find, so the first query about the class made one
  without parents. The mapping is updated now; the `hierarchy` scenario checks `Impossible`.

- The bottom node was a direct child of the top node in every taxonomy, so `owl:Nothing`
  and every unsatisfiable class were reported as direct sub classes of `owl:Thing` beside the
  real roots, and `owl:Thing` as a direct super class of `owl:Nothing` beside the leaves.
  Protege paints the members of the bottom node under `owl:Thing` for that, which is how it
  showed with the pizza ontology, where `CheeseyVegetableTopping` and `IceCream` appeared
  under `owl:Thing` as well as under `owl:Nothing`; HermiT and FaCT++ answer `[DomainConcept,
  ValuePartition]` for the direct sub classes of `owl:Thing`, Konclude answered those plus
  the two and `Nothing`. The same held for `owl:bottomObjectProperty` under
  `owl:topObjectProperty`. The OWLlink route did not show it because the client's local
  hierarchy cache rebuilds the direct relations itself.

  The cause is in the engine, not in the bridge: `CTaxonomy` and
  `CParentChildPredecessorHierarchy` make the bottom node a child of the top node when they
  are created, which is the right hierarchy as long as nothing else exists, and the three
  classifiers that finish a hierarchy, `COptimizedKPSetClassSubsumptionClassifierThread`,
  `COptimizedClassExtractedSaturationSubsumptionClassifierThread` and
  `COptimizedKPSetRoleSubsumptionClassifierThread`, make every leaf a parent of the bottom
  node without removing that first link. They now call `removeBypassedTopBottomLink`, which
  drops the direct link once the bottom node has another parent; the top node stays a
  predecessor of it, so the indirect answers do not change. The incremental classifier copies
  the links of the previous taxonomy and inherits the fix. The `hierarchy` and `properties`
  scenarios check the direct sub classes of `owl:Thing` and the direct super classes of
  `owl:Nothing` since.

  That check brought out a second defect of the same kind in
  `COptimizedClassExtractedSaturationSubsumptionClassifierThread`, the classifier that
  handles an ontology whose saturation suffices, which SNOMED CT and the family ontology of
  the test are. It decides which nodes are leaves, and so parents of the bottom node, by a
  flag that is set on every class that was made a direct parent of another; its path for a
  class with several direct parents, `makeParentAddPredeccessors`, linked the parents without
  setting the flag. A class whose children all have more than one parent, `Man` with its only
  child `Father`, that is `Man and Parent`, was therefore taken for a leaf and reported as a
  direct super class of `owl:Nothing`. HermiT answers `[Father, Grandparent, Mother]` for the
  family ontology, Konclude answered those plus `Man` and `Woman`. The flag is set on that
  path now, as it is in the other two classifiers.

- The reasoner manager read the requirements of a preparation after it had given them
  back. `CReasonerManagerThread::continueRequirementProcessing` calls the callback of a
  finished preparation and only then deletes its `CRequirementPreparingData`, whose
  `COntologyRequirementPreparingData` destructor asked every requirement it had been handed
  `isDynamicRequirement()` to find the ones it owns. The callback is what lets the command
  finish, which releases the caller that owns the other requirements: `prepareOntologyQuerying`
  of the JNI bridge deletes its `CPreprocessKnowledgeBaseRequirementsForQueryCommand` the
  moment it is processed, and the command deletes its requirements. Whenever that caller got
  to run before the reasoner manager's thread reached the destructor, the virtual call went
  through a freed object. On Linux glibc writes its safe-linking value into a freed chunk,
  the chunk's own address shifted right by twelve bits, which the JVM's crash report showed
  as the object's vtable pointer in `__dynamic_cast`; on macOS the freed memory keeps its
  contents, so nothing was ever seen there. It hit the Ubuntu JNI job of the CI twice in
  about fifteen runs (runs 35781295520 and 35803550885, both in the smoke test, `hierarchy`
  and `lifecycles`, both within 20 ms of loading the library) and never in hundreds of runs
  on the Mac. AddressSanitizer in a Docker container on 2 CPUs did not see it in 600 runs
  either; six JVMs sharing one and a half CPUs made it appear twice in 600, with the three
  stacks that name the object, the freeing thread and the reading thread. The preparing data
  now records the dynamic requirements when they are added, while they are certainly alive,
  and deletes only those, so it never touches a requirement it does not own.

  With that fixed the same stress run crashed once more in `__dynamic_cast`, this time with
  a native stack, since the sanitised library keeps its symbols:
  `CReasonerManagerThread::initiateQueryReasoning`, line 888, which is the other half of the
  same mistake and the one the CI runs most likely hit, since both died in a query.
  The function answers a query in one of six blocks, one per kind of query, each of which
  calls the callback that reports the answer and deletes its reasoning data, and none of
  which returned: for a taxonomy query, which the sub class queries of the bridge are, the
  block after it asked `dynamic_cast<CClassificationPremisingQuery*>(query)` of a query that
  the callback had handed back to the JNI thread, which deletes its queries as soon as they
  are answered. AddressSanitizer cannot see that read, it happens inside `__dynamic_cast` of
  the C++ runtime, which is not instrumented; it showed as a plain crash of the JVM. Every
  block that has answered its query now returns. On the way AddressSanitizer also found
  `COccurrenceUnsatisfiableCacheEntry` freeing an array made with `new[]` by `delete`, which
  is fixed as well. The ASan build is worth keeping in mind
  for the next crash of this kind: `ubuntu:24.04` with `qtbase5-dev` and OpenJDK 17, qmake
  with `QMAKE_CXXFLAGS+=-fsanitize=address -fno-omit-frame-pointer` and the matching
  `QMAKE_LFLAGS`, `LD_PRELOAD` of `libasan.so` for the JVM and
  `ASAN_OPTIONS=handle_segv=0:...:allow_user_segv_handler=1` so that the JVM keeps its own
  signal handlers.


## THE PRECOMPUTATION THAT DID NOT FINISH

Konclude used to stay in its precomputation for an ontology whose ABox forces two **named**
individuals to be merged. The smallest case is

```
FunctionalObjectProperty(r)   ObjectPropertyAssertion(r x y1)   ObjectPropertyAssertion(r x y2)
```

and `InverseFunctionalObjectProperty` and `ObjectMaxCardinality 1` behave the same way, so it
is the merge that matters and not the axiom that forces it. Every question that needs the
classification or the realisation hung, `GetSameIndividuals`, `GetSubClasses`, `GetTypes` and
`GetInstances` alike, while `IsKBSatisfiable` answered normally. It was not a loop: the
process sat at 0% CPU with every thread idle in its event loop, and the log stopped after
`Precomputing ontology '...'`.

It was not a defect of the bridge. The released Konclude binary did the same over OWLlink,
with no Java involved, which is how it was separated from the JNI work.


### WHAT IT WAS

`CTotallyOntologyPrecomputationItem` has two flags that sound alike.
`isFullCompletionGraphConstruction` says that the consistency check is *meant* to build the
whole completion graph, which is what makes a separate precomputation of the individuals
unnecessary. `isFullCompletionGraphConstructed` says that it actually *did*.

`createIndividualPrecomputationCheck` skipped the precomputation of the insufficiently
saturated individuals whenever the first flag was set. But
`createConsistencePrecomputationCheck` only builds the graph when
`isForceCompletionGraphConstruction` is set; otherwise it is free to decide the consistency
from the saturation of the all assertion individual instead, and for these ontologies it did,
logging `Trivial consistency detected with merged individual`. The graph was then never
built, so nothing precomputed the merged individuals and nothing set
`hasAllIncompletelyHandledIndividualsRetrieved`, which is what the individual step waits for
before it reports itself finished. With no work scheduled and no callback outstanding the
event queues ran empty and the reasoner sat there.

Instrumenting `createNextTest` showed the stranded state directly:

```
consCheckCreated=1 consChecked=1 allRetrieved=0 allIndiPrecompCreated=0 indiPrecompChecked=0
```

and the working case, with the completion graph forced, differed only in `allRetrieved=1`.

The fix is in `createIndividualPrecomputationCheck`: once the consistency step has finished
without having constructed the graph, the promise behind the first flag has not been kept, so
the individuals are precomputed after all. `isFullCompletionGraphConstructed` already existed
for exactly this distinction and had no reader until now.

### WHAT IT WAS CHECKED AGAINST

The five OWLlink and SPARQL tests of the CI answer the same before and after, and so does the
classification of SNOMED CT (the November 2024 OWL/XML release, 302 MB, 225 000 SubClassOf
axioms), on an M3 Mac with 48 GB:

| | master | with the fix |
| --- | --- | --- |
| wall time | 45.0 s | 44.1 s |
| peak resident set size | 14.5 GB | 12.9 GB |
| preprocessing / precomputing / classification | 7.3 / 23.9 / 7.1 s | 7.9 / 23.1 / 6.3 s |
| inferred subsumptions | 602 853 | 602 871 |

Do not read those differences as an improvement, they are inside the run to run variation
described below. Note also that SNOMED CT has no individuals at all, so it never reaches the
code that this fix changes. It says that the classification is not disturbed, not that the
fix works.

**The trigger is a matter of size, and not in the direction one would expect.** With N
subjects that each have two values of one functional property, so N forced merges:

| forced merges | 1 | 10 | 100 | 1000 | 3000 | 10000 | 20000 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| master | hangs | hangs | hangs | hangs | ok | ok | ok |
| with the fix | ok | ok | ok | ok | ok | ok | ok |

Somewhere between 1000 and 3000 individuals Konclude chooses another strategy and master
stops hanging by itself, so it is the small and the medium ABox that is affected. At 1000
forced merges the fix answers correctly, `y0_1` with `y0_2` and `y999_1` with `y999_2`.


### COMPARING TWO RUNS OF KONCLUDE

Konclude does not answer in the same order twice: synsets, bindings and XML attributes come
back differently arranged on every run, and a response carries its `response-time`. Responses
therefore have to be compared as sets and without the timings rather than byte for byte.

A classification result also has to be compared as a *closure* and not as a set of edges.
Which member of a set of equivalent classes carries the parent edges used to be whichever
one the classifier merged first, so two runs could describe the same hierarchy with
different edges: about 1 900 of the lines of the written hierarchy of the SNOMED CT variant
with universal restrictions differed between runs for that reason alone. Since the taxonomy
orders the equivalent classes of every node by IRI when the classification completes, with
`owl:Thing` and `owl:Nothing` kept in front of any class equivalent to them, the member with
the smallest IRI carries the edges and is the one the writer, the OWL API wrapper and the
OWLlink server report as the representative, so two runs that inferred the same hierarchy
now write the same file. A closure that does not contract the equivalence cycles first still
loses the ancestors of one member of every such set, which looks exactly like a missing
inference.

The answers themselves used to vary as well, by about 90 of the 602 000 inferred subsumptions
per pair of runs. That was the defect described under THE CLASSIFICATION THAT LOST
SUBSUMPTIONS below and it is fixed; four runs now produce byte identical hierarchies.

Beyond EL the hierarchies still vary between runs (issue #28). On the invalid first build of
the variant, with `max 1` on non-simple roles, two runs differed in about 2 200 lines and
1 150 classes had different ancestors, false equivalences included; on the OWL 2 DL-valid
rebuild two runs differ in one direct parent. That one is a genuine miss and it has a
six-second reproduction on the command line, no plug-in involved: on a module of the variant,
`IsClassSatisfiable(473062003 and not 429087003)` is answered true when it is the first
question, which loses the subsumption, and false when `473062003 and not 266987004` was asked
before, and false when it is the first question with
`Konclude.Calculation.Optimization.BranchTriggering=false`. The OR rule with branch triggering
delays a disjunction until a concept-role trigger fires; for the disjunction that the negated
definition of 429087003 unfolds to the trigger never fires, the disjunction is left
unprocessed and the node completes as satisfiable. Only that switch, of forty tried, changes
the answer. The pattern is a class with `max 1 r` inside its role group tested against a
definition with `only r` inside its group. Files, requests and the analysis are in
`~/Documents/konclude-benchmarks/2026-09-24-order-dependent-entailment/`.


### WHAT IS LEFT IN THE WRAPPER

`KoncludeReasoner.MergeSafety` and `KoncludeIndividualMergeInspector` were written while the
defect was open, to find such an ABox and build the completion graph for it. They are kept
for a shared library that predates the fix and default to `OFF`.

| policy | what it does |
| --- | --- |
| `OFF` | leave the loading configuration alone, the default |
| `DETECT` | look for a forced merge and build the completion graph if one is found |
| `ALWAYS` | always build the completion graph, safe and slower |

The time out of the OWL API is independent of all this and worth having in its own right. If
`OWLReasonerConfiguration.getTimeOut()` is set, every native call runs on a watch dog thread
and a call that overruns is reported as the OWL API `TimeOutException` instead of blocking
the caller for ever:

```java
OWLReasoner reasoner = new KoncludeReasonerFactory()
        .createReasoner(ontology, new SimpleConfiguration(60000L));
```

Two things to know. The stuck call **keeps running**, the bridge has no way to cancel it and
`interrupt()` only releases the callers that wait for the reasoner's thread, each with a
`ReasonerInterruptedException`, while the calculation runs on; its thread is a daemon so it does not keep the
virtual machine alive, but it holds its library instance and its share of the machine until
the process ends, so another reasoner should not be started beside it. The reasoner refuses
to be used after a time out, with an `IllegalStateException`, rather than queueing further
calls behind the one that is stuck. The thread the call runs on is the reasoner's own in any
case, see ONE THREAD PER REASONER below, the time out only limits the wait for it.

The `merges` scenario keeps the fix honest, on the default configuration, and the `timeout`
scenario covers the watch dog.


### ONE THREAD PER REASONER

`CJNIHandler` keeps the `JNIEnv` and the method ids of the thread that initialised the
library instance, so every call of one instance has to happen on the same thread. Mixing
threads crashes the virtual machine rather than raising an exception. `KoncludeReasoner`
therefore owns a thread per reasoner and runs the initialisation, every query and the closing
of its instance on it, whichever thread asks, see `guard`. Protege is the caller this is for:
it creates the reasoner and precomputes on its classification thread and then asks from the
event thread and from the threads of its views. The price is a hand over per call, which is
small against the 0.12 ms a query costs on SNOMED CT. The time out, if one is configured, is
the limit on waiting for that thread.


## THE CLASSIFICATION THAT LOST SUBSUMPTIONS

**Resolved.** The cause was found on 2026-10-03 and fixed in PR #82 (issue #34): when a
saturation node's label was copied a second time, the merge of its own entries with the
shared ones let the older shared entries overwrite the newer own ones, dropping every
implication that had registered a wait on a trigger in between. The classifier below was
never where the subsumptions were lost; it only stopped hiding the loss. Since PR #83 it
trusts the saturation again by default, and `TrustSaturationSubsumerCompleteness=false`
keeps the subsumption tests as a check. The sections that follow are kept as the record of
how the defect appeared from the classifier's side.

Classifying SNOMED CT lost inferences. Repeated runs of the same binary over the same file
lost between 3 and 42 of the 56 subsumptions that were seen to vary, over 46 concepts, and no
run ever reported one that does not hold. It is a silent defect: a lost subsumption is simply
absent from the result and nothing reports it.

`COptimizedKPSetClassSubsumptionClassifierThread` takes the subsumers of a concept from its
precomputed saturation and, when the saturation reports itself as sufficient, complete and
free of possible subsumers, marks the result as derived so that no subsumption test is ever
run for that concept. When the extracted set is nevertheless incomplete, the missing
subsumptions can no longer be found.

Which concepts lose subsumers depends on the memory layout of the process and not on the
ontology. It happens single threaded as well, and recording anything about the decision, even
into a preallocated array, is enough to make the losses disappear, which is why the defect
resists being observed from inside. The conditions under which the sufficiency flags are
wrong were therefore not determined, and the flags are no longer trusted as a reason to skip
the tests.

Six runs with the shortcut and six without, on the same file, before the fix:

```
shortcut disabled : 0  0  0  0  0  0
unmodified        : 0 15 27 21 32 12     lost subsumptions per run
```

`Konclude.Calculation.Classification.TrustSaturationSubsumerCompleteness` selects between the
two. It was off by default from PR #7 to PR #83 and is on again since the cause was fixed.


### THE DEFECT IS LATENT AND IS BEING MASKED

Setting that option on this branch does not bring the losses back. Twelve runs of SNOMED CT
with the shortcut restored lose nothing at all and produce the same hierarchy every time, and
all 56 subsumptions are present. The option is a faithful revert - with it set, the condition
reads exactly as it did before - so the old code path really does run. Something else here
stops the defect from showing.

It is not another fix. Bisecting the branch, six runs each, counting the losses against the 56
subsumptions that ELK confirms:

```
master                                    : 11  1  0  1  5 27     6 distinct hierarchies
f47587cf, the JNI entry points            :  0  0  0  0  0  0     1
1198ba29, the inverse role registration   :  0  0  0  0  0  0     1
master + the query callback hunk alone    :  0  0  0  0  0  0     1
```

The last line is the point. It is master with nothing but the thirteen lines that
`f47587cf` added to `CEntityExpressionSetResultVisitingCallback::visitRoleAssociatedEntityExpression`,
which unwrap an inverse property expression. That method is reached only from the property
queries, `CSubSuperPropertiesResultVisitCallbackQuery` and
`CEquivalentPropertiesResultVisitCallbackQuery`; classes go through
`visitConceptAssociatedEntityExpression`. The code cannot execute while a class hierarchy is
computed and cannot add or remove a subsumption, and adding it removes the losses completely.

So the defect answers to the layout of the binary and not to the computation, which is the same
thing that made it resist being observed: recording anything about the decision, even into a
preallocated array, was already enough to make it disappear. It is untouched in the classifier
and merely sits outside the window in which it shows.

That is what makes not trusting the flags the only fix here. Everything else on this branch
moves the defect rather than removing it, and the next change to any part of the binary can
move it back. It also means the numbers above cannot be reproduced on this branch by setting
the option - master is needed for that - and that several clean runs are never evidence that
the defect is gone.

`Scripts/run-classification-reproducibility-test.sh` classifies an ontology repeatedly and
fails if the runs do not all infer the same hierarchy, which is the signature the defect left.
It compares the runs against each other rather than against a reference, so it reports a defect
that makes the result vary and not one that loses the same inferences every time. The losses
were intermittent - runs that lose nothing were common - so a clean run of a few iterations
means little, and the ontology in the repository is small enough that it may never have lost
any. Point it at something substantial with `-i` and raise `-n`.

It does catch this defect where the defect is reachable. On master, six runs of SNOMED CT:

```
classifying, run 1 of 6 ... 602868 inferences
classifying, run 2 of 6 ... 602855 inferences
classifying, run 3 of 6 ... 602851 inferences
classifying, run 4 of 6 ... 602859 inferences
classifying, run 5 of 6 ... 602855 inferences
classifying, run 6 of 6 ... 602852 inferences

run 2 differs from run 1: 47 only in run 1, 34 only in run 2
	S	http://snomed.info/id/111411000119103	http://snomed.info/id/104931000119100
	S	http://snomed.info/id/129151000119102	http://snomed.info/id/104931000119100
S	http://snomed.info/id/129151000119102	http://snomed.info/id/38481006
	S	http://snomed.info/id/129161000119100	http://snomed.info/id/104931000119100
S	http://snomed.info/id/129161000119100	http://snomed.info/id/38481006
[...]
run 5 differs from run 1: 40 only in run 1, 27 only in run 5

FAILED: 5 of 5 comparisons disagree with the first run.
The same binary inferred different class hierarchies from the same ontology.
```

The counts differ in both directions and some runs report more than the 602 853 of the fixed
build, which is not unsoundness. The test compares the direct hierarchy, so losing a
subsumption from the closure promotes edges that were redundant to direct ones and those appear
as entries the other run does not have. Checked against ELK, no run ever reported a subsumption
that does not hold; the defect only ever loses.


### WHAT IT COSTS

The table this section first carried said "nothing measurable", 15.6 s against 15.8 s for
SNOMED CT. That comparison was void: the option had never been registered in the configuration
group, Konclude rejected it with "Configuration ... not supported", and both arms ran with the
tests. Measured with the option registered (PR #83), medians of 6 runs over 3 random-UUID copies
of one build, alternating arms, Mac M3, `-w AUTO`:

| classification | tests kept | subsumers trusted |
| --- | --- | --- |
| SNOMED CT International (374 710 classes) | 17.9 s | 6.7 s |
| 95k module | 6.0 s | 1.6 s |
| 95k variant beyond EL | 324 s | 312 s |

On the 12-core Linux VM the 95k module classifies in 18.6 s against 3.7 s at `-w AUTO` and in
107 s against 10–13 s at `-w 1`. Peak memory and the hierarchies are the same in both arms. The
variant gains little because its classes are decided by tableau tests, which the shortcut does
not touch.


### WHAT IT WAS CHECKED AGAINST

With the fix, four runs produce byte identical hierarchies for SNOMED CT, and the result
agrees with ELK exactly: 7 557 791 ancestor pairs over 369 185 classes, none missing on
either side.

ELK is not a complete reference for SNOMED CT though. It warns 72 217 times that it supports
`DataHasValue` only partially and ignores the 10 `SubDataPropertyOf` axioms, so the agreement
says nothing about the datatype part of the ontology. That part was checked separately
against HermiT, which is complete for OWL 2 DL, on a locality based module around the
`DataHasValue` axioms: 1 162 classes, `SRI(D)`, classified by this same classifier, 11 332
ancestor pairs, none missing on either side over three runs.

The other classifier, `COptimizedClassExtractedSaturationSubsumptionClassifierThread`, does
not have this defect. It is only chosen when no insufficient node occurred anywhere in the
saturation at all, which is a far stronger condition than the per concept flags, and it was
checked against HermiT on GALEN (2 748 classes, `SHIF`, six runs, 30 755 pairs) and on forty
namespace disjoint copies of it (109 920 classes, four runs, 1 230 200 pairs), with nothing
missing or extra in any run.


### THE PRE CHECK ON THE OTHER CLASSIFIER

That condition is a property of the saturation as a whole. It does not establish that the
saturation produced data for each individual class concept, and the extracting classifier is
the one place that does not check. `CPrecomputedSaturationSubsumerExtractor::getConceptFlags`
returns false when it cannot reach the saturation data for a concept, which is exactly when
`extractSubsumers` would report no subsumers at all; the KPSet and the SubClass classifier
both test that return value, the extracting classifier discards it. A concept in that state
would be classified with no subsumers and nothing would report it, which is the same silent
shape as the defect above.

`CConfigDependedSubsumptionClassifierFactory` therefore verifies, before choosing the
extracting classifier, that every active class concept has saturation data, and otherwise
classifies with subsumption tests instead and logs which concept was missing it. The fallback
is the ordinary one: the SubClass classifier when the ontology is deterministic, KPSet
otherwise.

`Konclude.Calculation.Classification.RequireSaturationDataForAllClassConcepts` removes the
pre check. It is on by default.

The check has never been observed to fire. It was exercised by forcing it to report missing
data, which selects the fallback and produces the same hierarchy for full GALEN, 457 090
ancestor pairs with nothing missing or extra, at 4.12 s against 3.50 s. Left to decide for
itself it costs nothing measurable, on the same binary with the setting on and off,
alternating over four rounds on full GALEN:

```
pre check on  : 3.32  3.33  3.16  3.50 s
pre check off : 3.37  3.37  3.39  3.37 s
```

The scan is a single pass over the active class concepts with one pointer chase each, and it
is smaller than the spread between runs.


### WHAT THE CAUSE IS NOT

The account above says the defect answers to the layout of the binary and that it happens
single threaded as well. Both were probed directly on a build of `d981a6b3`, the commit before
the fix, and neither holds in the form stated.

Address layout is not it. Launched through `posix_spawn` with `_POSIX_SPAWN_DISABLE_ASLR`, so
that the binary loads at the same address in every run, verified by comparing the `__TEXT`
range, six runs of SNOMED CT still disagree. Randomised addresses therefore do not select which
concepts lose subsumers, whatever else the allocation layout does.

`-w 1` does not make Konclude single threaded, but it serialises more than an earlier version of
this paragraph said. The process runs about 20 threads, and sampling during such a classification
finds two of them inside saturation and tableau code at the same moment. The setting is honoured
where it applies: `CConfigDependedCalculationEnvironmentFactory` gives a processor count of one a
single `CSingleThreadTaskProcessorUnit`, which is scheduler, completor and processor in one
thread, so every tableau and saturation task runs on that one thread, and the reasoner manager
sizes Qt's global pool from the same count (`Konclude.Calculation.AdaptThreadPoolSizeProcessorCount`,
on by default), plus the one slot it keeps blocked.

The second thread the sampling caught is the precomputation thread, `CTotallyPrecomputationThread`.
Right after it hands the saturation construction job to the task unit, it runs
`addIdentifiedRemainingConsistencyRequiredConcepts` and `addRequiredSaturationIndividuals`, which
reach `markSaturationProcessingItems`. Those walk the saturation items and set their
processing-marked flags, which only the precomputation thread reads, while the saturation reads
other fields of the same items. Its later steps, `saturateRemainingRequiredItems` and
`extractCommonDisjunctConceptsFromPrecomputedSaturation`, run only once `isSaturationComputationRunning`
is cleared, i.e. when no saturation job of the ontology is running. So under `-w 1` the
precomputation thread does not change saturation data that a running saturation task uses.

What does run beside the task thread are the coordinating threads (command handling, preprocessing,
precomputation, classification) and the cache threads. The caches take their writes as events and
apply them on their own threads, so which cache entries a tableau task finds depends on timing.
The saturation reads no such cache while it runs: it uses only the backend association cache,
written at its end for individuals, and the completion graph cached by the consistency test. All
of that is upstream Konclude, older than this fork, and is tracked as rhausam/Konclude#35.

An earlier version of this paragraph blamed `CConcurrentTaskScheduler::run` for dispatching
through `QtConcurrent`; that class is used only by the answerer's variable mapping composition and
the backend cache, not by the saturation. Independently of `-w`, the realizer and the Redland
parsers dispatch through `QtConcurrent` on Qt's global pool.

What else was excluded, each on SNOMED CT with the runs still disagreeing afterwards:

```
uninitialised heap values : MallocPreScribble=1 MallocScribble=1
Qt hash iteration order   : QT_HASH_SEED=0
decisions on elapsed time : the kernel contains none
memory driven caching     : the memory level of CSignatureSatisfiableExpanderCache put out of reach
a shared saturation budget: every Saturation...ProcessLimit and the checking depth raised
parallel classification   : MaximumParallelSubsumptionCalculationCount and both classifier counts at 1
```

The last one also deadlocks: with the subsumption calculation count at 1, the third run of
SNOMED CT sat at 0 % CPU for 58 minutes with four threads waiting on locks or semaphores, which
is the shape of the thread pool starvation fixed in 658c7bb2.

Parallel tasks are not it either. With `-w 1` every tableau and saturation task runs on one
thread, and the saturation reads nothing that another thread feeds while it runs (see above).
Yet on `d981a6b3`, 12 of 120 runs of the 95k module (see below) at `-w 1` lost subsumptions,
on a 12-core Linux machine, in the same family as with `-w AUTO`: defined classes whose
definitions need triggers back-propagated from a role group, such as 285841000119104 under
285831000119108. What differs from run to run is the input: preprocessing assigns concept tags in
a run-dependent order and builds a structurally different, equivalent encoding of the absorbed
definitions every time. So the saturation misses a subsumer for some encodings, not because of a
race. The investigation is recorded in rhausam/Konclude#34. Making preprocessing deterministic
(PR #79 for the absorber's trigger pairing, PR #81 for the order in which the builder creates the
concepts) was what made the deterministic reproducer below possible, and the reproducer led to
the cause, in the saturation's label copy (PR #82).

### A SMALLER REPRODUCER

The defect needs scale, and STAR modules of SNOMED CT extracted with the OWL API bracket it.
The signature is the 63 classes that were seen in differing subsumptions plus a number of
random ones:

```
  63 signature classes ->    928 classes,   1 872 axioms, tiny    : 8 runs agree
2000 signature classes -> 51 330 classes, 102 810 axioms,  4.8 GB : 8 runs agree
8000 signature classes -> 95 073 classes, 190 161 axioms, 11.2 GB : 3 of 7 runs disagree
25000 signature classes -> 146 245 classes, 292 333 axioms, 15.5 GB : 5 of 5 disagree
full ontology          -> 374 710 classes, 750 302 axioms, 14.0 GB : 5 of 5 disagree
```

So a quarter of the axioms still reproduces it, and the threshold lies between 51 000 and
95 000 classes. Lowering the expander cache thresholds to 1 MB on the stable module does not
make it appear, so the scale is not acting through that cache.

A race detector is the instrument this wants next, and the reason for keeping a smaller
reproducer: ThreadSanitizer multiplies the memory of a run by five to ten, which the full
ontology cannot afford on a 48 GB machine and the 95 073 class module still cannot. It becomes
affordable on a machine with 128 GB, and the module is the input that makes that run worth
starting.

### THE DETERMINISTIC REPRODUCER

Since PR #81 every run builds the same encoding of an ontology, and two testing switches, read
from the environment, choose a different one on purpose:

- `KONCLUDE_BUILDORDER_SEED=<n>` permutes the order in which the builder walks the expressions,
  so every run with the same seed gets the same encoding and runs with different seeds different
  ones;
- `KONCLUDE_TRIGGER_TIEBREAK=asc` pairs the absorber's triggers in ascending instead of
  descending tag order, the pairing the slow memory layouts used to draw before PR #79.

On `d981a6b3` with the code of PR #79 and PR #81 ported, the 95k module at `-w 1` with
`KONCLUDE_TRIGGER_TIEBREAK=asc` and seed 2 loses 90791000119104 ⊑ 771000119108 on every run,
seed 8 loses that and 90771000119100 ⊑ 96441000119101, seed 12 loses 128001000119105 ⊑
71701000119105; the default encoding and 36 seeds in descending order lose nothing. A numbered
trace of the label copies and of the implication concluding 771000119108 at the node of
90791000119104 and its parent then showed the loss (rhausam/Konclude#34). With PR #82 the same
three runs are correct, as are 95 of 95 completed runs of `d981a6b3` with only that fix at
`-w 1`, and on master every run of this matrix is correct with the subsumption tests on or off.

The OWL API test has a scenario for this since 2026-10-03, `determinism`: the same generated
ontology is classified with one processor, with the automatic count and once more with the
automatic count, and the three hierarchies, every class with its equivalents and direct sub
classes, have to be identical. It is the regression check for #28, #34, #79 and #81 that the
suite can afford; the reproducer above needs the 95k module and about half a minute per run.

### A LIBRARY WITHOUT QT DEPENDENCIES

Konclude is written in Qt rather than merely using it for input and output: `QString` appears
in 1642 of the 5215 source files, `QList` in 572, `QSet` in 352 and `QHash` in 348, so the
containers, strings, threads and the XML parser of the reasoner are Qt. Only `QTcpServer`,
which the OWLlink and SPARQL servers need, is confined to a single file, so a library for a
plug-in still needs QtCore, QtXml and QtConcurrent. Built against the Qt of a package manager,
the library carries them as dependencies and runs only where that Qt is installed at the same
path, which is what makes shipping it awkward.

Built against a static Qt it carries none of them. The release workflow already builds such a
Qt for the macOS and Linux packages, and the same one serves here:

```
qtbase 5.15.17, configured as in .github/workflows/build-release-with-redland.yml:
  ./configure -static -release -opensource -confirm-license -prefix <prefix> -platform macx-clang \
    -no-gui -no-widgets -no-opengl -no-icu -no-dbus -no-feature-gssapi -no-zstd -qt-pcre \
    -no-sql-mysql -no-sql-psql -no-sql-odbc -nomake examples -nomake tests \
    QMAKE_APPLE_DEVICE_ARCHS=arm64

<prefix>/bin/qmake -o Makefile KoncludeLIB.pro -after "QT-=gui" \
    "CONFIG-=static staticlib" "CONFIG+=shared dll"
make -j16
```

The two `CONFIG` arguments are needed. A static Qt propagates `CONFIG += static` into the
project, and a library project then builds `libKonclude.a`, an archive that `System.loadLibrary`
cannot load. `QT-=gui` drops a dependency on QtGui that the reasoner does not use at all.

The result on macOS is 23.5 MB and depends on the system frameworks only:

```
SystemConfiguration, DiskArbitration, IOKit, AppKit, Security, ApplicationServices,
CoreServices, CoreFoundation, Foundation, CFNetwork, libSystem, libc++, libz, libobjc
```

`otool -L` reports no Qt library at all, against five for a library built with the Qt of
Homebrew. Both `Java/run-jni-smoke-test.sh` and `Java/run-owlapi-test.sh` pass against it, so a
plug-in can ship a single self-contained library the way the FaCT++ plug-in does.

The release workflow builds the library in the same way on all four platforms, in a build
directory of its own, since `KoncludeLIB.pro` compiles the sources with the JNI interface into
the same object directory as `Konclude.pro`:

- Linux x64 and arm64 take the static Qt of the Linux package of their architecture. Qt's mkspec compiles its static
  libraries with `-fPIC` already (`QMAKE_CFLAGS_STATIC_LIB`), so no separate Qt is needed for
  a shared object. The library is linked with `-static-libstdc++ -static-libgcc` like the
  package, and with `-Wl,--exclude-libs,ALL`, so that the C++ runtime and Qt inside it do not
  interpose on the symbols of the virtual machine. `ldd` shows `libz`, `libm` and `libc` only.
- Windows x64 needs a Qt of its own, since the package is linked against the Qt DLLs, which
  `System.loadLibrary` would not find beside the library in the directory OSGi extracts it
  to. The workflow builds qtbase with MSVC 2019 and `-static -static-runtime`, so that the
  DLL imports neither Qt nor the C runtime of MSVC, and caches it.
- The OSGi entry for Windows names both `Win32` and `"Windows 10"`, since Felix 7.0.5, which
  Protege 5.6 ships, normalises 'Windows 10' to `windows10` and every other Windows, 11 and
  the server editions among them, to `win32`.
- The OSGi entries for 64 bit ARM, macOS and Linux, name the processor `aarch64` only, which
  is what Java reports on both. Felix normalises `arm64` to `arm_le`, the 32 bit ARM, so an
  entry with it would offer the library to a 32 bit Java that cannot load it.
- jemalloc, which the Linux packages link statically, fixes the page size when it is built,
  and refuses to run on a kernel with larger pages. The arm64 build assumes 64 KB pages, so
  that it runs on kernels with 4, 16 and 64 KB pages alike.

The library is tested before it is packaged: the smoke test and the OWL API test on all four
platforms, and the plug-in test in the OSGi framework of Protege on macOS. On Windows the
scripts run in Git Bash and hand java, javac and Maven the paths in Windows form and class
paths joined with ';', see `Java/native-paths.sh`.

### A SECOND WAY TO LOSE A TOLD PARENT

Found while measuring #90 with `Konclude.Calculation.Classification.SaturationSubsumerExtraction=false`,
which makes the classifier read the subsumers from the tableau's labels instead of from the
saturation (issue #95). Each run of AU then lost 16 to 26 told `SubClassOf` pairs, different ones
each time, and the taxonomy's equivalence step, which declares two classes equivalent when a
subsumer has the same number of subsumers as the class, turned the losses into false
equivalences of sub classes with their parent.

The cause is one character in `tryInitalizingFromSaturatedData`: `if (satIndiNode = baseSatIndiNode)`,
an assignment where a comparison was meant. A tableau node initialised from the saturation
copies the label of the node at the end of the saturation's substitute chain (a class whose
saturation equals its parent's shares the parent's node, so chains of single-parent primitives
form chains of substitutes). The branch for "no substitution" adds the node's own concept without
unfolding it, the copied label already holding its consequences; the branch for a substitute
chain has to unfold it so that the concepts of the intermediate substitutes, the told parent
among them, enter the label. The assignment made the first branch unconditional, and the
intermediate concepts arrived only when the saturation expansion cache had been filled by an
earlier test, hence the variation. The default classification never showed it because the
saturation subsumer extraction adds the intermediate substitutes' concepts itself when it walks
the chain. Fixed in PR #99; three forced AU runs then produce the default hierarchy, a detector
comparing every class's told parents with its final subsumers finds none missing, and the
default mode is unchanged.

Two guards in the KPSet classifier now catch this class of defect instead of amplifying it.
A told message marks a class derived (no test of its own) only if the subsumers it carries
include the class's told parents; otherwise the class keeps its test and a warning names
the missing parent. And two classes with the same number of subsumers are declared
equivalent only if the subsumer also lists the class; otherwise the subsumption stays
strict and a warning is logged. With the #95 defect reinstated for a check, the forced AU
classification logs the missing parents, produces no false equivalence any more, and its
remaining losses are the root labels' own; with the fix in place both guards are silent on
AU and International and the hierarchies are unchanged.

## CLASSIFYING SNOMED CT THROUGH THE WRAPPER

SNOMED CT with additions, 750 302 axioms over 374 710 classes in 297 MB of OWL 2 XML, on an M3
with 48 GB and the statically linked library:

```
 6.6 s  the OWL API loads the ontology
10.0 s  the wrapper translates it into the library
35.4 s  precomputeInferences(CLASS_HIERARCHY)
 0.12 ms  every query afterwards
41.8 s  walking all 374 710 classes, 619 472 direct super class nodes
```

13 to 16 GB for the process as a whole, about 1 GB of it the Java heap. The result agrees
exactly with the command line: both infer 619 472 direct super class nodes for this file, and
four runs of the same binary produce the same hierarchy, since the classification no longer
skips the subsumption tests. The classification time depends strongly on how much memory is
free: the same run took 148 s on a machine that was paging.

The comparison that matters for a plug-in is with the OWLlink route that Protege uses today,
where the ontology is rendered into a 349 MB `Tell` request, which costs about 30 s and a copy
in the heap of the editor, and the reasoner is a second process holding the ontology again:

```
                    OWLlink                         wrapper
into the reasoner   349 MB request, about 30 s      10 s, no serialisation
load and classify   62 to 76 s                      about 45 s
memory              20 to 34 GB and 4.2 GB in       13 to 16 GB in one process
                    Protege, two processes
per query           0.024 ms from the local cache,  0.12 ms, always current
                    0.9 ms when it has to be asked
```

So the wrapper is worth having for a large ontology: about half the memory and about a third
less time. What it gives up is the isolation of a separate process. Two defects that crashed
the OWLlink server were found and fixed on 2026-09-20; in a plug-in the same defects take the
editor down with them.

### PRECOMPUTEINFERENCES TRIGGERS THE WORK

Konclude computes on demand, so `precomputeInferences` used to return at once and the
classification happened in whichever query arrived first. For an editor that is the worst
arrangement: the progress dialogue completes immediately and the window then stops responding
for the length of the classification with nothing to indicate why.

It now asks for something that cannot be answered without the inference type, the direct sub
classes of `owl:Thing` for `CLASS_HIERARCHY` and the direct instances of `owl:Thing` for
`CLASS_ASSERTIONS`, so the cost falls where the caller expects it. `isPrecomputed` reports
what was computed, `getPrecomputableInferenceTypes` names those two, and both are forgotten
when the ontology is translated again. The remaining inference types have no query of their
own in the bridge and are ignored, which the OWL API allows.

### THE LOADING CONFIGURATION SETS THE PROCESSING UNITS

The library defaults to a single processing unit, as the command line does without `-w AUTO`,
and nothing in the OWL API suggests that a caller should think about it.
`DEFAULT_LOADING_CONFIGURATION` therefore sets

```
Konclude.Calculation.ProcessorCount=AUTO
```

Two runs of SNOMED CT one after the other on the same machine, only that setting differing:

```
AUTO : precomputeInferences  35.4 s
1    : precomputeInferences 192.5 s
```

Note that until the plug-in stage the constant was only documentation: `KoncludeReasonerFactory`
handed an empty configuration to the library, which then applied its own default without
the line above, so a reasoner that was not given a configuration explicitly ran on one
processing unit. An empty configuration now means `DEFAULT_LOADING_CONFIGURATION`, see
`chooseLoadingConfiguration`, so the factory and the plug-in get `AUTO` without saying so.

## THE DRUG MODEL'S DATA VALUES

SNOMED CT models drug strengths and ingredient counts with `DataHasValue` restrictions on
decimal and integer literals (`999000041000168106 value 10.0`): 72 218 of them in the
International Edition, 1 026 888 in the Australian Edition with AMT. Until October 2026 the
classifier could not trust the saturation of any class whose definition reaches such a
literal, so every one of them went through the tableau at each classification: 16 855
classes of International (4.6 %) and 169 448 of AU (30 %). Two defects, both in the
preprocessing rather than in the reasoning (issue #90):

- The saturation refused every datatype value space that carries concept triggers unless it
  was the string space (`tryHandleDatatypeValueSpaceTriggers`), so a node for a plain,
  valid literal such as `8.0^^xsd:decimal` was flagged insufficient as soon as the ontology
  had absorbed any data restriction into triggers. On AU all 3 387 insufficiency flags came
  from that one line. The saturation now fires, for a node whose literal value is known,
  exactly the triggers the tableau fires for a node fixed to one value: those of the whole
  value space, those of the value's types (rational, decimal, integer; strings without a
  language tag), the complete and partial triggers at the value, the inclusive minimum and
  maximum triggers at it, the minimum triggers of the entries below it and the maximum
  triggers of the entries above it. A node whose datatype alone is known is accepted when
  the value space has no value-dependent triggers. `CDatatypeValueSpaceCompareTriggers`
  counts its minimum and maximum triggers, so an ontology with value triggers only, like
  SNOMED CT, costs one map lookup per literal node. Other value spaces (boolean, double,
  float, date time, ...) are still refused.

- The absorber's test for the *complete* absorption of an `EquivalentClasses` axiom had no
  case for data literals, only the partial and the assuring tests had one. A definition with
  a `DataHasValue`, which is every defined medicinal product, was therefore only absorbed
  into an equivalence *candidate*, which the saturation cannot resolve (a candidate is
  problematic unless the class itself is derived), and the classifier tested each such
  class with the tableau. A data literal is now completely absorbable whenever an exact
  value trigger can be built for it, and the complete absorption builds that trigger, the
  same complete value trigger the assuring triggers use. It is registered for the whole
  ontology rather than scoped by the back-propagation concept: it is one entry of the value
  space's trigger map and fires only on nodes whose value is exactly the literal. The
  tableau fires complete triggers only for a node fixed to one value and the saturation
  only for a node carrying that literal, so adding the defined class on it is sound.

### WHAT IT GAINS, AND WHAT THE SECOND HALF COST AT FIRST

Same Mac, same files, command line with `-w AUTO`. The first measurement of the second
half reported the classification phase alone, which hid that it made the saturation of AU
three times slower; all phases are given here (one run each, wall clock where measured):

```
                                   master    saturation    + complete absorption   + ranked
                                               fix (#93)         (#94, reverted)   (#98)
International  preprocessing        6.0 s       6.1 s             6.2 s             6.5 s
               precomputation      13.5 s      13.7 s            13.5 s            13.8 s
               classification       6.3 s       6.0 s             5.1 s             5.1 s
               untrusted classes  16 976      12 716               255               255
Australian     preprocessing       13.1 s      13.8 s            13.3 s            16.6 s
               precomputation      18.8 s      20.9 s            64.0 s            20.7 s
               classification     15.4 s       8.7 s             6.5 s             4.1 s
               untrusted classes 169 448      16 559                 0                 0
               wall clock           57 s        54 s              93 s              54 s
```

The complete absorption of #94 cost 43 s of AU's saturation because of the order of the
triggers in its implication chains. The absorber orders a chain's triggers with the
*highest* complexity first (`CConceptTriggerLinker::operator<=`) and lowers the complexity
of triggers that many implications use, so that chains start with their most selective
trigger and share their prefixes; the exact value trigger of #94 had complexity 2, above an
atom's 1, copied from the assuring triggers. Inside a role group the strength value
therefore came first, and a value such as `10.0` is shared by hundreds of ingredient
combinations, so every role-group node with that value registered one waiting implication
per combination: the concept insertions into the labels went from 128 to 289 million, the
reapply (waiting implication) insertions from 64 to 225 million, spread over some 31 000
additional trigger tags, the largest label from 3 399 to 12 534 entries. PR #97 made the
complete absorption a switch, off, and #98 ranks the exact value trigger at complexity 0,
below the class restrictions, so that the chain starts with the ingredient; the fan-out is
gone (precomputation 20.7 s) and the switch is on again by default.

What remains of the cost is in the absorber itself: with the value last the chains share
fewer prefixes, so building the 170 000 complete chains of AU takes about 3 s more
preprocessing, which on AU is what the classification saves, hence the equal wall clock.
On International the saving stays (about 1 s). What the complete absorption buys besides
is a saturation with no insufficient node on AU, so the factory selects the
saturation-extraction classifier and the classification runs no tableau test at all, and
answers about data-restricted classes come from the saturation too.
`Konclude.Calculation.Preprocessing.CompleteDataLiteralAbsorption=false` restores the
candidates.

### WHAT IT WAS CHECKED AGAINST

The hierarchies of AU, International and the 95k variant module are byte identical to the
ones master produced before the change. Since the trusted path now answers the datatype
part of the ontology without the tableau, that part was also checked against reasoners
that are complete for OWL 2 DL: a STAR locality module of International around 40 randomly
sampled classes defined with a `DataHasValue` (1 058 classes, 2 108 axioms) classifies
identically with Konclude, HermiT 1.4.3 and FaCT++ 1.7.0 (8 845 ancestor pairs and
equivalences, none differing; HermiT 139 s, FaCT++ 5 s, Konclude 2 ms). A module around
300 sampled classes (5 601 classes) was too much for HermiT, which had not finished after
14 minutes at its 14 GB heap. The checker, `Java/tools/DhvCheck.java`, run by
`run-dhv-check.sh`, extracts the module with the OWL API, classifies it with any reasoner
factory and writes the hierarchy as sorted "sub super" and "equiv a b" lines; Konclude's
command line output is read back in its `asserted` mode. HermiT 1.4.3 and the FaCT++
binding need the OWL API 4.5 class path, not 5.1; the script resolves it from Maven and
takes FaCT++'s jar and native library from `FACTPP_JAR` and `FACTPP_LIB`:

```
./run-dhv-check.sh module snomed.owl sampled-classes.txt module.owl hermit.txt
FACTPP_JAR=.../factplusplus-1.7.0-SNAPSHOT.jar FACTPP_LIB=.../FaCT++.JNI \
  ./run-dhv-check.sh classify module.owl factpp.txt uk.ac.manchester.cs.factplusplus.owlapiv3.FaCTPlusPlusReasonerFactory
Konclude classification -i module.owl -o konclude-module.owl
./run-dhv-check.sh asserted konclude-module.owl konclude.txt
diff hermit.txt konclude.txt
```

## THE ORDER OF A DEFINITION'S TRIGGERS

The absorber turns a definition `C = A and B and some r.X and ...` into a chain of binary
implications over triggers, one per conjunct, so that the saturation and the tableau derive
`C` once a node carries all of them. The chain has an order, and the order decides how much
work every node does: a node that carries the chain's first trigger is unfolded into the
implication waiting for the second, whether or not it will ever carry the rest. Until #111
the chain started with the most complex trigger, which for a nested role group is also the
most specific one, and that served SNOMED CT well. It does not serve an ontology whose
definitions share a few conjuncts. The SDD drug ontology (582 633 classes, 93 % of them
defined, each with about twelve existential conjuncts) has four boolean flags such as
`is inert = false` in over 70 % of its definitions; the absorber merged them into one
trigger, and every node carrying the flags was unfolded into one implication per definition
sharing them. The label of a class then holds one implication for every twelve definitions
in the ontology, the saturation's memory grows with the square of the ontology, and three
nested modules of 5, 10 and 20 % of SDD needed 4.4, 21 and 70 GiB, the full ontology 240 GB
on the VM. The SNOMED drug model's data values (above) are the same mechanism on a smaller
scale.

Since #111 a chain starts with the rarest conjunct of its definition. A pass over the
ontology counts in how many conjunctions each conjunct occurs (definitions, conjunctions and
negated disjunctions, nested ones included); every trigger linker carries that count, a
merged pair the smaller of its two, a partial trigger of a partially absorbable definition
the count of its conjunct (it matches at least as many nodes), and the triggers of a filler
the count of the conjunct they belong to. The chain is sorted by that count ascending, then
by complexity as before; triggers without a count come last. A node is then unfolded into
the chains of the few definitions sharing its rare conjuncts, and the common ones are checked
last, where the chain has already narrowed to those definitions. The option
`Konclude.Calculation.Preprocessing.FrequencyOrderedTriggerChains` (default true) switches
back to the complexity order.

Two defects surfaced on the way and are fixed with it. The absorber reuses a pair of
triggers whose implication exists by assigning the stored linker over the current one, and
the assignment copied the linker's data pointer, which the sorted insertion dereferences: a
reused trigger was ordered by the stored linker's fields, not its own. And the merges change
triggers after their insertion into the sorted list, so the list is sorted once more before
the chain is built. Both held with the complexity order too, where they moved a few chains;
with the frequency in the order they moved thousands.

Measured on the Mac M3 with the command line (precomputation + classification, peak
footprint), the hierarchies identical to master's in every run:

| | master (#110) | rarest first |
|---|---|---|
| SDD 10 % module (60 427 classes) | 13.1 + 31.8 s, 21.0 GiB | 1.4 + 4.6 s, 3.7 GiB |
| SDD 20 % module (118 353 classes) | 63.6 + 170.8 s, 69.7 GiB | 3.3 + 18.6 s, 7.0 GiB |
| SNOMED CT AU | 16.9 + 3.4 s, 19.5 GiB | 13.9 + 3.2 s, 16.0 GiB |
| SNOMED CT International | 3.7 + 3.5 s, 6.3 GiB | 3.6 + 3.6 s, 6.1 GiB |
| 95k variant module | 6.7 + 273-285 s, 14.5 GiB | 8.6 + 269 s, 14.7 GiB |
| full OWL 2 DL variant | 18.3 + 684-713 s, 35.1 GiB | 20.4 + 638 s, 27.0 GiB |

An estimate of the number of nodes satisfying a conjunct from the told hierarchy (descendant
counts, occurrences weighted by the defining class's descendants and summed over the filler's
descendants) was built and measured against the plain count; it gave the same figures within
noise and costs a pass of its own, so the count stayed. The measurements, the modules and the
traces are in `~/Documents/konclude-benchmarks/2026-10-09-sdd-modules/`.

## THE LABEL SETS OF THE SATURATION

A profile of the remodelled SNOMED CT AU with #111 (macOS `sample`, one run, the shares are
of the sampled time) put the precomputation on one thread for 90 % of its 21 s, all of it in
the saturation, and about a third of the saturation in the hash of a node's label set: the
map from a concept tag to the concept's saturation descriptor and the implications waiting
on it (`CReapplyConceptSaturationLabelSet`). It was a `CPROCESSHASH`, Qt 4's chained
`QHash` over the task's memory pools: every entry a node of its own from the pool, a bucket
array of a prime size that is reallocated and relinked as the label grows, a modulo per
lookup, two dependent memory reads per hit, and a copy of a label (every saturation node
starts as a copy of its parent's) duplicated the nodes one by one.

Since #112 the label set uses `CReapplyConceptSaturationLabelHash`: open addressing with
linear probing over one array of entries, a power-of-two capacity at most three quarters
full, and Fibonacci hashing of the tag. A hit reads one cache line, an insertion allocates
nothing, a copy is one `memcpy`. The pools cannot take memory back (their release is a
no-op), so an outgrown array is not left behind: it goes to a free list in the process
context, one per capacity, and serves the next label that grows to that size. Without the
free lists the dead arrays cost more page faults than the hash saved on AU, and the footprint
grew by a fifth; with them the label sets take less memory than the chained hash, which kept
its outgrown bucket arrays and 40 bytes per node.

Measured on the Mac M3 Max with the command line (precomputation, peak footprint), the
hierarchies identical to master's in every run, two or three runs of each build in
alternation:

| | master (#111) | flat label hash |
|---|---|---|
| SNOMED CT AU 2026-09-29 release | 13.9-14.1 s, 16.1 GiB | 12.3-12.6 s, 14.9 GiB |
| SNOMED CT International 2026-07 | 3.5 s, 6.1 GiB | 3.2-3.3 s, 5.7 GiB |
| remodelled AU v5 (OWL 2 DL) | 22.8 s, 27.1 GiB | 20.1 s, 25.6 GiB |
| SDD 5 % module (30 699 classes) | 1.47 s, 3.7 GiB | 1.32 s, 3.6 GiB |
| SDD 20 % module (118 353 classes) | 3.1-3.5 s, 7.0-7.1 GiB | 2.95 s, 6.8 GiB |
| 95k module of the DL variant | 8.4 s, 15.1 GiB | 7.6 s, 14.2 GiB |
| full DL variant (SRIF(D), 40 min on the VM) | 20.4-20.7 s, 27.0 GiB | 18.0-18.2 s, 26.1 GiB |

So about a tenth of the precomputation and a twentieth of the footprint. The classification
phase is untouched by it: the two places that iterate a label set during classification are
order-insensitive checks, and the full variant's classification, which spreads between 620
and 930 s from run to run with either build, showed no systematic difference. The rest of the
saturation's time is in the rules themselves, which the profile spread over the AND rule
(a third), the implications, the successors and the copying of labels, with no single
operation left that stands out like the hash did.

## THE TESTING ORDER OF THE CLASSIFICATION

Before the classifier runs any subsumption test, it builds the testing order: for every class it
reads the subsumers the saturation found (`CPrecomputedSaturationSubsumerExtractor::extractSubsumers`
walks the class's saturation node and its label), records them in the class's testing item, links
each class under its subsumers and queues the classes without subsumers first. Profiles of the
remodelled SNOMED CT AU and of SDD 2.9.5 showed this as the first thing the classification does and
as serial: the classifier thread walked the saturation data of every class while all worker threads
waited on the task semaphore. Unsampled it took 2.1-2.7 s for the remodelled AU (557 805 classes),
3.6 s for SDD 2.9.5 (582 633 classes) and 0.75 s for a 118 354-class SDD module; the profile's
larger share was inflated by the sampler, which slows a single busy thread disproportionately.

Since #113 the extraction runs on all threads (`QtConcurrent::blockingMap` over the classes, as the
realizer already does for its instance initialisation): a class writes only into its own testing
item, the saturation data and the concept reference linking are read-only at that point, and the
equivalence-candidate check (`hasSubsumerConceptItem` on the fresh subsumer set) runs in the same
pass for the completely saturated classes. Everything that touches another class's item or the
ontology item, that is the successor links, the counters, the test lists and the fallback to the
build data for incompletely saturated classes, follows sequentially in the original class order, so
the testing order and therefore the tests are exactly those of the sequential build. The individual
dependence tracking writes into one shared vector, so the classes whose saturation nodes are
connected to nominals are extracted a second time sequentially with the tracker; on ontologies
without individuals that costs nothing. The option
`Konclude.Calculation.Classification.ConcurrentSaturationSubsumerExtraction` (default `true`) selects
the sequential build for comparison, and the reasoner manager logs the build's duration ("Testing
order of N classes created from the saturation data in X ms, subsumer extraction Y ms").

Measured on the M3 Max (16 cores) with the same binary, option on against off, hierarchies identical:

| ontology | order build off -> on | classification off -> on |
|---|---|---|
| remodelled AU v5, 557 805 classes | 2.1-2.7 s -> 0.43-0.45 s | 34.7-36.3 s -> 33.6 s |
| SDD 2.9.5, 582 633 classes | 3.6 s -> 0.6 s | 62.6 s -> 59.7 s |
| SDD 20 % module, 118 354 classes | 0.75 s -> 0.13-0.16 s | 19.7 s -> 18.7-19.7 s |

The build itself is five to six times faster; the classification gains the two to three seconds it
had spent waiting, which is 3-8 % on the ontologies where classification matters and nothing on the
SNOMED CT releases, whose classification takes a few seconds in total. The remaining serial start-up
cost is the interpretation of the first test results on the classifier thread.

## THE BRANCH TRIGGERS OF THE PREPROCESSING

For every conjunction, equivalence and disjunction the preprocessing extracts branch triggers
(`CBranchTriggerPreProcess::createBranchingTriggers`): it flattens the (negated) definition into its
disjuncts, and for each atomic disjunct or each universal, existential or cardinality restriction it
records the concept, or the role's domain trigger concept, that should trigger the branching. A
profile of the SDD 2.9.5 preprocessing (44 s, one thread) showed this extraction at 30-40 % of the
window and half of that in malloc and free: the two work lists were `QList<QPair<CConcept*,bool>>`,
which heap-allocates every element because the pair is larger than a pointer, each concept built and
destroyed its own `QSet` of trigger keys, and the role-domain lookup used `operator[]` on the TBox's
hash, which inserted a null entry for every role that had no trigger concept.

Since this change the work lists and the key list are three `QVector` members of the preprocess that
are cleared and reused for every concept, the lists are walked by index instead of `takeFirst`, the
duplicate check is a linear scan of the keys found so far (a set is built only once a concept has
more than 32 triggers), and the lookup without creation uses `value()`. The order in which the
disjuncts are visited and the triggers are linked is unchanged, so the triggers and the hierarchies
are identical.

Measured on the M3 Max, master against the branch, two passes each, hierarchies identical:

| ontology | preprocessing before -> after |
|---|---|
| SDD 2.9.5, 582 633 classes | 38.1-38.7 s -> 24.3-24.9 s |
| SNOMED CT AU baseline | 11.3-11.5 s -> 11.3-11.4 s |
| remodelled AU v5 | 10.2-11.1 s -> 10.1-11.4 s |

SDD, whose definitions are long chains of conjuncts, gains 14 s (36 % of its preprocessing); the
SNOMED CT releases, whose definitions are short, were not spending time here. The next largest parts
of the SDD preprocessing are the trigger absorber (30 % of the window) and the XML parse before it.

## ENTAILMENT CHECKING, INTERRUPTION AND PROGRESS

The first two were the gaps between the wrapper and ELK 0.6.0 on the methods Protege calls, measured
on 2026-09-22 against the `ElkReasoner` classes of ELK 0.4.3 and 0.6.0 and the bytecode of
every Protege bundle: ELK answers `isEntailed` for some axiom types and stops a calculation
on `interrupt`, the wrapper did neither. Everything else Protege asks for is either answered
by both or by neither, and what neither answers, the data properties, disjointness, domains,
ranges and inverses, Protege tolerates: `DisplayedInferencePreferences.executeTask` catches
the `UnsupportedOperationException` and switches that optional inference off, which is how
ELK has always been usable there.

### ENTAILMENT

The bridge has no entailment query, but every axiom type that ELK 0.6.0 checks can be decided
with the queries it has, now that anonymous class expressions are answered. A class inclusion
`C SubClassOf D` holds when `C and not D` is unsatisfiable, and the other types reduce to that
or to the instance, property and individual queries:

```
SubClassOf, EquivalentClasses, DisjointClasses   satisfiability of an intersection
ObjectPropertyDomain, ObjectPropertyRange        'p some Thing' below the domain, 'p some not C' unsatisfiable
SubObjectPropertyOf, EquivalentObjectProperties  the property hierarchy, named properties only
ClassAssertion, ObjectPropertyAssertion          the instances and the property values
SameIndividual                                   the same individuals
DifferentIndividuals                             satisfiability of an intersection of two nominals
Declaration                                      always entailed
```

`isEntailmentCheckingSupported` names exactly these, everything else ends in an
`UnsupportedEntailmentTypeException`, as does an inverse property expression or an anonymous
individual inside one of them. An inconsistent ontology ends in an
`InconsistentOntologyException`. The `entailment` scenario of the test checks 27 axioms
against the family ontology, including expressions on both sides of an inclusion.

### INTERRUPTION

`interrupt()` cancels the calls that wait for the reasoner's thread, each of which ends in a
`ReasonerInterruptedException`. The calculation itself cannot be abandoned, the native side
has no way to stop one, so it runs on to its end on the reasoner's thread and a call made in
the meantime queues behind it. For Protege that is the difference between a cancel button
that returns and one that waits for the classification of a large ontology to finish. The
`interrupt` scenario of the test checks that an idle interrupt changes nothing, that a call
caught in flight ends in that exception and no other, and that the reasoner answers correctly
afterwards.

### PROGRESS

Protege's progress window paints a task that is only started as a bar that stays at zero,
switches to an indeterminate bar when the reasoner reports the task as busy, and fills the
bar from `reasonerTaskProgressChanged(value, max)`. The numbers come from the native side:
`queryOWLReasoningProgress` is a 19th entry point of the querying bridge that reads what the
command line prints with `-a`, the counters of `CClassificationManager::getClassificationProgress`
and `CRealizationManager::getRealizationProgress` and the task counts of the calculation
environment, through the managers the commander registers in its configuration, and one
count that `-a` does not print, the saturation's. It reads counters only, so unlike every
other native call it is made from a thread of its own, the `ProgressReporter` of
`KoncludeReasoner`, every 250 ms while `precomputeInferences` waits for the query that
triggers the work on the reasoner's thread.

The saturation is one calculation task, so the task counts that `-a` shows for a tableau
calculation stay at zero, and its node counter was a local of the algorithm's loop. The
count now lives on the ontology: `CPrecomputation`, which every `CConcreteOntology` owns,
carries two atomic counters, the saturation nodes that have been initialised and the nodes
that exist, and `CCalculationTableauApproximationSaturationTaskHandleAlgorithm` writes both
at the one place it marks a node initialised, from the processing data box of its task,
whose ontology is the one being precomputed. The node vector is allocated for every node up
front, so the total is known from the first report on, 504 683 for SNOMED CT, and the
initialised count climbs almost linearly with the time, which is what a bar wants. The
counters are only ever incremented, and a new revision of the ontology starts with a fresh
`CPrecomputation`.

What the monitor is told for SNOMED CT (375k logical axioms), measured on 2026-09-22 from
the moment the reasoner was created, which itself took 10 s of translation, on a machine
with 2 GB free (Protege held 14 GB), so slower than the 19 and 16 s of the busy-only run:

```
 0 s - 21 s   Precomputing   saturation nodes initialised / nodes, 4 225 / 504 683 up to 504 681 / 504 683
21 s - 24 s   Precomputing   the consistency check, the count sits at 504 681 / 504 683
24 s - 43 s   Classifying    tests done / tests to do, 12 266 / 419 244 up to 443 022 / 445 061
43 s - 48 s   Realizing      busy, no individuals, so no count
```

The wrapper reports the first phase as the task `Precomputing` with the saturation count,
and starts the task `Classifying` the moment the classifier's total appears, with its tests
done against those to do; that total grows a little while the tests run, which Protege
takes as a new maximum. Two tasks one after the other is how ELK reports its stages, and
Protege keeps the window open across the switch. An ontology that the saturation classifies
on its own never reaches the second task. The `progress` scenario of the test checks the
order of the tasks, that every task is stopped, that both tasks got a report, and that
every report keeps `0 < value <= max` with a value that never falls.

## SUB CLASSES OF AN EXPRESSION FROM THE SATURATION

A query about a class expression, the sub classes of `not C` that Protege's 'Disjoint classes'
display asks for every class it shows, or the `p some C` of the DL query tab, used to take
0.7 s on a 41 000 class subset of SNOMED CT and 2.4 s on a 148 000 class one, where FaCT++
takes 30 ms. Issue rhausam/Konclude#19 records the analysis. The search itself was already
what FaCT++ and HermiT do, top down from the direct super classes of the expression into the
children of every node that is not subsumed, but every visited node was a tableau job, about
30 000 per query, because a node not subsumed by the expression can still have subsumed
children. FaCT++ decides almost every visit from its cached pseudo models instead.

`CSaturationSubsumptionDecider` in `Source/Reasoner/Answerer` does the same from the
saturation that the precomputation builds for every named class and that the classifier reads
its subsumers from. `COptimizedComplexExpressionAnsweringHandler` asks it before it creates a
sub class test, and creates the test only for an `UNDECIDED` answer, so a verdict is never a
guess: whenever the saturation of a node is marked insufficient, critical or unprocessed, or
the expression or the label leaves the fragment below, the tableau decides as before.
`Konclude.Answering.SaturationBasedSubClassDecision=false` switches it off, which the
`expressions-tableau` scenario uses to keep the tableau path tested.

For a positive expression `E` the structure of `E` is matched against the label of the node:
a named class is entailed when it is in the label, a conjunction when all its operands are, an
existential restriction when a successor under the role, which the saturation links under
every super role, satisfies the filler, and not entailed when none does and the role is not
involved in chains or transitivity, which the preprocessing compiles away. Universal and
cardinality restrictions, nominals and data ranges are only recognised when the label carries
the very concept.

For a negated expression `not C`, with `C` a conjunction of named classes, the question is
whether the node's class and `C` have a common instance. The labels of both are merged and
the merge is closed under the rules the saturation applies at a node: a conjunction adds its
operands, an implication whose triggers are all present adds its conclusion, the first operand
of the implication concept, and a named class that is derived brings its saturation along. A
concept that ends up positive and negated is a clash, which is how an absorbed disjointness
surfaces, and proves the subsumption. Otherwise the closure with the successors of both parts
is a model, and the subsumption does not hold, unless a universal restriction of one part,
including the automaton transitions of the role chains, reaches a successor of the other part
that lacks its operand, a functional role has successors on both sides, or a label holds a
construct only the tableau handles, a disjunction, a cardinality restriction, a nominal, data
or a branch trigger. The side of `C` is built once per query and shared, and a node is decided
by one scan of its own label without copying it; the closure only runs when an implication
would fire, which is rare.

Two details of the saturation matter for the soundness and cost a day each to find: the
operator codes of the basic constructors come in sign pairs that the negation flag of a label
entry flips, an existential restriction is stored as a negated universal one, so the effective
operator has to be taken before a label concept is classified; and a trigger of an implication
counts when its concept is in the label positively, whatever the negation flag of the trigger's
operand says, see `insertConceptReapplicationReturnTriggered`.

What it does on the 41 149 class subset, one query per class, mean over 200:

```
                              before   after
not C                         685 ms    97 ms
p some C                      400 ms    30 ms
```

and on the 148 000 class subset `not C` went from 2.4 s to 0.5 s. Of the 24 000 nodes a query
visits on the smaller subset, 134 stay undecided, all because of equivalence candidate concepts
that the decider does not interpret. The remaining cost is a few microseconds per visited node,
mostly in the search's own bookkeeping of the candidates.

On the full SNOMED CT with additions, 374 710 classes, one class selection in Protege with the
'Disjoint classes' display on, which asks `not C`, takes 2.7 to 7.4 s against 16 s with the
decision off. The remaining cost there is the nodes that carry data values, which the merge
leaves to the tableau.

Two things went wrong on that ontology before they were found on it, both races between the
decider on the handler's thread and the tableau tasks of the same query, both fatal only under
the burst of tasks the decider produces, since the tasks it leaves are all of one kind. The
successor data getter of a saturation node, `getLinkedRoleSuccessorData`, ignores its create
flag and reaches into the node's hash with the subscript operator, which detaches a shared hash
into a copy allocated in the node's memory context; the decider reads through the constant
lookup instead. And the lookups of the datatype value space triggering map, which the ontology
shares with the testing ontologies of the queries, called the non constant `lowerBound`, which
detaches the map, from every task that initialises a node with a data value; two tasks
detaching it at once corrupted it, and the tasks then crashed the virtual machine in the value
space triggering, on up to fourteen threads at once. They call the constant overload now. The
first was found while looking for the memory growth described next, the second showed as a
crash in the first query on a fresh start, in about every second run.

The growth itself was older than the decider and is not a leak but a reserve. Each task
processing thread keeps the memory pools of the tasks it completes in a free list, to reuse
them for the tasks it creates itself, and only returns pools to the system once the list holds
more than a fixed number of them. That number was 80 000 pools of about 50 KB, 5 GB per
thread, and the tests of a query are created on the answering thread, whose pools flow into
those lists and are never drawn from them again. So every test a query ran added its pools to
some thread's reserve, and a Protege session on the full ontology climbed by 15 to 20 GB per
`not C` query with the decision off, and by about a gigabyte with it on, until all sixteen
threads held their 5 GB and the process levelled off near 100 GB, most of it compressed. The
limit is 1 000 pools per thread now, 50 MB, and is settable with
`Konclude.Calculation.Memory.TaskProcessorFreePoolReserveLimit`. On the 41 000 class subset
with the decision off, 20 selections went from 3 to 25 GB before and stay at 3.7 GB now, and
each query got faster, since the pools it needs come from the free lists instead of from
fresh allocations. On the full ontology with the decision off, 10 selections went from
21 to 99 GB before and from 25 to 29 GB now, and with the decision on, 30 selections went from
22 to 56 GB before and stay between 20 and 22 GB now. The limit does not show in the
classification of the full ontology: five runs each with 1 000 and with 80 000 pools on the
same binary spread from 44.6 to 48.7 s regardless of the value.

The pools that leave a thread's reserve now go to a store shared by the whole process,
`CMemoryPoolRecycler`, from which every pool provider serves its next acquisitions before it
allocates, so the pools the answering thread takes for its tests are the ones the processing
threads gave back a moment earlier. Without it each provider handed released pools straight
to the allocator, which keeps them in the cache of the releasing thread while the acquiring
thread takes fresh pages from the system; Protege after fifteen selections held 33 GB of such
empty blocks, most of them swapped out. The store keeps up to 20 000 pools, a gigabyte, and is
settable with `Konclude.Calculation.Memory.RecycledPoolLimit`.

The statistics line the handler writes with extended logging on, 'Saturation decided ... merge
blocking label concept codes', names the operator codes of the label concepts that kept a
merge undecided and, under negative keys, the other reasons: -100 the class has no saturation
node, -300 its saturation is insufficient, -400 unprocessed, -500 critical, -600 not
completed, -1100 cardinality or nominal flags, -1500 a data value, -1600 an integrated
nominal, -1700 no label, -1000 a functional role with successors on both sides, -2500 and
-3000 universal restrictions reaching successors and self loops, -3500 a derived successor;
and for the query side, whose merged label could not be built at all, -4600 no node, -4700
unreliable, -4800 flags, -4900 a data value or nominal, -5000 no label. On the full SNOMED CT
with additions the 17 000 nodes a `not C` query leaves to the tableau are all -300, the
classes whose definitions carry data values and the classes referencing them, and a query for
a class that carries a universal restriction inside a role group fails with -4700, upon which
every node goes to the tableau, 375 000 jobs and a minute per selection.

### COMPLETING A SATURATION WITH THE TABLEAU, ONCE

A class whose saturation is not reliable, flagged insufficient or critical, or restricted by a
cardinality or a nominal, no longer sends a tableau test for every query that visits it.
The decider answers NEEDS_COMPLETION instead, the handler runs one satisfiability test of the
class conjoined with everything above it in the class hierarchy, and the tableau reports the
label of the root node of the completion graph, each concept with its polarity and whether it
was derived without a choice, through the message adapter that already carries the subsumers.
That completed label stands in for the saturation from then on, on the query side as well as
for the candidate nodes, and is kept for the life of the ontology. The test is on the class
with its ancestors because a model need not name every subsumer at its root, a functional
role can imply one the saturation never derives, and the consequences of those subsumers have
to be in the label; the named subsumers themselves are read from the class hierarchy, which
the classification made complete.

Three rules keep it sound. An entry that rests on a choice holds in the model the tableau
found, not in every model, so it can show a conjunction satisfiable but never unsatisfiable:
a clash on such an entry, or with a query whose label has one, is left undecided. The
successors of a completed label are known by their roles only, so a universal restriction
that could reach one of them, on either side, is left undecided, and so are a functional role
and a data role with successors on both sides. And an equivalence candidate, the marker the
absorption leaves for a named class the node may turn out to belong to, is treated as an
optional class: a merge that stays satisfiable with the class added is satisfiable without it,
so the candidates of both sides are added, with their saturations, before a merge is called
satisfiable, and a clash that only appears with them added proves nothing.

The candidates are resolved once, not once per merge. A completed label is closed under the
rules with its candidates added when it is first used, and what the closure with the
candidates derives is marked as resting on a choice; a label whose candidate needs its own
label completed first is extended once that has arrived. The query's merged label resolves
its candidates the same way when it is built, since the saturations its closure pulls in carry
the markers too. Without this every node of a query went through the closure to resolve the
markers of the query's side, 1.1 million closures for three selections, and the heavy classes
stayed at 12 s.

Checked against the tableau path with the decision switched off: 400 mixed negated queries
over pizza, 80 over the 41 000 class subset, and six of the classes of the Protege session on
the full SNOMED CT, answer for answer. On the full ontology the classes that took 50 to 63 s
per selection take 5 to 8 s, the first of them with the 16 700 completion tests, which run
once, the others 2.5 to 5 s; six queries leave 43 nodes to the tableau instead of 2.2 million,
and the process stays at 21 GB instead of 35. The merges an implication fires in, about 50 000
per selection, read the node's label in place instead of copying it into the closure, and the
closure looks only at the implications of the query's side that wait for a trigger the node
supplies, through the trigger hash the query's label already has, instead of walking its whole
implication list; the heavy classes are at 5 to 7 s with that. Switchable with
`Konclude.Answering.SaturationBasedSubClassLabelCompletion`.

A completed label knew only the roles of its successors, so a universal restriction of the
query that reached one of them, the remodelled SNOMED CT closures `only group (only r C)` in
particular, sent the candidate to the tableau: about 5 100 tests and 12 to 14 s per selection
of a defined class on the remodelled AU edition. The decider now reads the successors off the
saturations of the fillers of the completed label's existential restrictions (not off the class's
own saturation node, which lists no successors at all when the saturation gave up on it, so that a
restriction would count as satisfied by nothing; the scenario's primitive P2 was lost that way by
the first version), and settles the restriction where every such successor satisfies the filler:
it is there, or it is a nested universal restriction over a role the successor has no successor
under. Successors whose saturation is insufficient, has a nominal or an unresolved
disjunction, roles with chains or transitivity, fillers that could add successors, and nested
restrictions that lead back over an inverse role stay with the tableau. The same selections
take 0.8 to 2.8 s, with no memory added; the OWL API scenario `completion-successors` holds the
corners (the filler missing, the inverse of the link, a self loop, a successor that repeats the
root). Switchable with `Konclude.Answering.SaturationBasedSubClassLabelCompletionSuccessors`.

### THE EVENT CHANNEL THAT LOST EVENTS

Stress runs of the comparisons, twelve at a time, hung once and crashed once on the way. The
channel that hands events from one thread to another, `CEventLinkerChannel`, was lock free
and stopped its walk over the posted list at the address of the last event it had taken. Once
the pools of finished events are reused at once, which the shared store of released pools
made the common case, the next event is often allocated at exactly that address, the walk
stops before it, and every event posted since is lost, which is the hang; the poster writing
the marker while the receiver reads it is the crash. The list is now taken whole with an
atomic exchange and pushed with compare-and-swap, so no address of a taken event is compared
with anything, and a receiver that finds its channel empty writes nothing, since it polls its
channels in a tight loop and a write on every poll bounced the cache line between it and the
posters, which cost the full classification 15 s. Measured against the same tree without the
change, three rounds each: 42.4, 42.4 and 45.1 s with it, 43.5, 43.9 and 45.8 s without.
Twelve stress runs of the comparison that hung and crashed before pass with it, as do both
test suites and the full classification with its 1 238 944 subclass axioms.


### WHAT THE FAST PATH DOES BEYOND EL

SNOMED CT is EL, so the question was what the saturation-based path is worth once definitions
carry universal restrictions, disjunctions and cardinalities. A variant of the July 2026
International Edition was generated for that (`VariantBuilder.java` beside the other probes in
`~/Documents/konclude-benchmarks/2026-09-23-variant-comparison/probes/`): the additions of the
older edition ported, `only` added inside the role group of 10 % of the classes, a covering
disjunction to 5 % and `max 1` inside the role group to 2 %. The first build put `max 1` on
roles that SNOMED CT makes non-simple through property chains and transitivity, which OWL 2 DL
forbids and Konclude accepts without a word; the OWL API's `OWL2DLProfile` check found the 506
violations (`ProfileProbe.java`), the builder now skips non-simple roles and checks its output,
and every measurement below is on the valid rebuild. A generated ontology gets that check before
it is measured with.

On the valid variant, 383 204 classes, the fast path gives nothing: the direct sub classes of
`not C` for twelve heavy classes take 47 to 123 s with the decider on and 47 to 108 s with it
off, existential, universal and complement queries the same, with identical answers. The
statistics line explains it: for one `not C` query 348 565 label completions are requested
and run, one per class, and 281 487 nodes still go to the tableau, because the saturation of
most nodes is insufficient (code -300) once a universal restriction or a cardinality sits in a
role group that the node refers to. The completion tests are paid on top of the tableau tests
that follow, so the decider is slower than the tableau on such an ontology, and it should skip
completion when most nodes are insufficient rather than complete them one by one. The full
measurement, with memory figures, is in
`~/Documents/konclude-benchmarks/2026-09-24-variant-queries/README.md`.


The answers were compared with the tableau path on 120 mixed queries over the subset and 400
over the pizza ontology, and with HermiT on 1 544 queries over pizza, where the one expression
they differ on is HermiT's: for the sub classes of `hasBase some ThinAndCrispyBase` HermiT and
FaCT++ leave out ThinAndCrispyPizza and report RealItalianPizza, a sub class of it, as direct,
while their own entailment checks confirm the subsumption. Konclude's answer is the right one,
with the decider on and off.


## CHECKING THAT A BUILD STILL PRODUCES THE SAME HIERARCHY

Most of the changes of October 2026 were checked the same way: classify SNOMED CT with the
command line, sort the output's lines (Konclude writes them in a layout-dependent order) and
compare a hash of them with the hash the previous build produced, for the International
Edition, the AU Edition and the 95k variant module. `Java/tools/check-hierarchy.sh` is that
procedure as a script:

```
Java/tools/check-hierarchy.sh hash ~/build/konclude-cli/release/Konclude.app/Contents/MacOS/Konclude snomed-intl.owl 62e1eefa88
hash 62e1eefa88 preprocessing 6218 precomputation 13973 classification 5219 (saturation extraction)
```

It classifies with `-w AUTO`, prints the hash and the phase times, and exits 1 if an expected
hash is given and differs; options after `--` go to Konclude, so two configurations can be
compared. `KEEP_OUTPUT=<file>` keeps the output, and `compare <A.owl> <B.owl>` lists the
`SubClassOf` pairs and `EquivalentClasses` groups that only one of two outputs has, which is
how the forced-mode losses of issue #95 were isolated (it uses `comm`; `diff` on two 175 MB
files can run for a very long time). The reference hashes of the three files, all produced
by master since `d5ddf4fc`, are in the script's header: International `62e1eefa88`, AU
`21f0920c63`, the 95k variant module `e84a1bbaec`, the full OWL 2 DL variant `19da30f3a9`. A
hash is a ten-character prefix of the MD5 of the output sorted with `LC_ALL=C`; a locale-aware
sort orders the lines differently and gives another hash (the 3 October comparison of the full
variant used one, which is why its `acbe9065e4` does not match).

The full pass on master `63910d61` (4 October): OWL API suite 16 scenarios passed against the
library built from it; International, AU (default and forced), the 95k module and the full
variant all at their reference hashes, the full variant also compared pair by pair with the
master of 3 October (676 356 pairs, none differing).

What it does not check: that the hierarchy is right. For that the OWL API test's `datatypes`
scenario holds HermiT's answers for the datatype cases, and `Java/tools/DhvCheck.java`
compares a locality module with HermiT or FaCT++ (see "The drug model's data values").

## THE MEMORY ALLOCATOR ON LINUX

On Linux the malloc of the GNU C library cost Konclude a third of the classification time of
SNOMED CT. Measured on 2026-09-27 on a 32 processor Graviton4 (AWS r8g.8xlarge, Debian 13, glibc
2.41, transparent huge pages `always`) with the 310 MB SNOMED CT file with additions:

| command line, `-w AUTO` | wall | precomputation | system time |
|---|---|---|---|
| glibc, default settings | 87-93 s | 40-44 s | 12-13 s |
| glibc, tuned as below | 75 s | 32-33 s | 6 s |
| jemalloc 5.3 linked or preloaded | 59-61 s | 27-28 s | 6 s |

| library through the wrapper | translate | precompute | after dispose |
|---|---|---|---|
| glibc, default settings | 19-21 s | 66-68 s | 25 GB (4.8 GB with `malloc_trim`) |
| glibc, tuned as below | 17 s | 54-57 s | 5 GB |
| jemalloc 5.3 preloaded | 14 s | 54 s | 7.8 GB |

Every run gave the same hierarchy. Two things make glibc slow here. It trims the free top of each
thread's heap as soon as it exceeds a threshold that it adapts on its own, so heaps shrink and grow
all the time while pools of tasks are freed on one thread and allocated on another; with
transparent huge pages every fault after a trim also clears a 2 MB page, which is the doubled
system time. That part is removed by fixed thresholds. The rest, about 15 s on the command line, is
jemalloc's faster allocation itself; the tunable tcache made no difference.

- `CSystemAllocatorTuning` sets the trim threshold to 1 GB (beyond the 64 MB of a thread's heap,
  so heaps are not trimmed), the top pad to 256 MB and the mmap threshold to glibc's maximum,
  32 MB, with `mallopt` once per process, at the start of the command line program and when the
  first library instance is created. The environment variable `KONCLUDE_MALLOC_TUNING=off` keeps
  glibc's defaults, for comparisons with one binary. Other C libraries are not touched.
- Since the heaps are no longer trimmed, `closeKoncludeLibraryInstance` calls `malloc_trim(0)`
  after the instance is deleted. That is also what makes the memory freed by dispose (issue #45)
  visible on Linux: glibc keeps freed pages in its arenas, so without the call the process stayed
  at 25 GB after disposing a SNOMED CT reasoner, with it at 5 GB. The peak while reasoning is 1 to
  3 GB higher with the tuning.
- The release packages of `Konclude.pro` link jemalloc. `KoncludeWithoutRedland.pro` does with
  `qmake CONFIG+=jemalloc`, with `JEMALLOC_LIB=<file>` for a particular library, e.g.
  `/usr/lib/aarch64-linux-gnu/libjemalloc.so.2` of Debian's `libjemalloc2` without the
  development package. The tuning is then harmless, since glibc's malloc is no longer used.
- The library links jemalloc itself on Linux, in the release packages (`KoncludeLIB.pro` with
  `CONFIG+=jemalloc JEMALLOC_LIB=<libjemalloc_pic.a>`, built by
  `.github/scripts/build-jemalloc-library.sh`), since how Protege or another program using the
  wrapper is started is not under Konclude's control. A shared jemalloc would not help: loaded into
  the running Java virtual machine it does not replace the malloc the process already has. A static
  one is linked in whole with its symbols hidden (`--exclude-libs,ALL`), so that the malloc and free
  of Konclude, of the static Qt and of the static C++ runtime in the library are jemalloc's, while
  the virtual machine and the other plug-ins keep glibc's. It is built with
  `--disable-initial-exec-tls`, which a library loaded with `dlopen` needs, and without its C++
  operators, the C++ runtime's call its malloc.

  What makes this safe is the boundary to the C library: memory must not be allocated by one
  allocator and freed by the other. Of the 273 functions the library imports (the same on x64 and
  arm64, `nm -D --undefined-only`), four return memory the C library allocated for the caller to
  free: `realpath` and `getcwd` with a null buffer, `strdup`, all three used by Qt, and
  `backtrace_symbols`. The linker renames the library's calls to them (`--wrap`) to the functions
  of `Source/Utilities/Memory/CLibraryAllocatorWrappers.cpp`, which return the same in jemalloc's
  memory. The others either take the caller's buffer or a static one (`vsnprintf`, `readlink`,
  `getpwuid_r`, `strerror`, `setlocale`, `getenv` ...), or allocate and free on the C library's
  side through their own pair (`getaddrinfo`/`freeaddrinfo`, `opendir`/`closedir`,
  `iconv_open`/`iconv_close`, `newlocale`/`freelocale`, the `FILE` functions, and zlib, a shared
  library of its own). None frees or keeps memory the library allocated.

  The other boundary is the C++ runtime. The library exports the C++ template instances it
  contains, and the virtual machine loads the system's `libstdc++.so`, which has some of them as
  well. Without `-Bsymbolic` the dynamic linker binds the library's calls to those to
  `libstdc++.so`'s, which allocate with glibc: `std::wstring`'s `_M_construct` built the static
  token names of the functional-syntax parser in glibc's memory, and the library's inlined delete
  handed them to jemalloc when the virtual machine exited, which crashed it. With `-Bsymbolic` the
  library's calls to its own functions stay inside it.

  The release workflow checks that the library neither imports nor exports an allocation function
  and that it is linked with `-Bsymbolic`. With the input `jemalloc_debug` it links a debug
  jemalloc, which checks more of what it is handed, though a pointer from another allocator can
  still crash it rather than fail an assertion.

  With jemalloc in the library, `CSystemAllocatorTuning` leaves glibc's malloc alone, since the
  library no longer uses it, and `closeKoncludeLibraryInstance` purges jemalloc's arenas
  (`arena.4096.purge`, all arenas) instead of calling `malloc_trim`. Libraries built without the
  option, the macOS and Windows ones and local Linux builds, use the program's malloc as before.


### THE DISPOSE PROBE

`Java/tools/DisposeProbe.java`, run by `run-dispose-probe.sh`, is the measurement behind the
work on #45: it creates a reasoner, classifies the ontology, disposes the reasoner, and prints
the process size and the thread count after every dispose, as many times as asked.

```
ONTOLOGY=<ontology file> CYCLES=4 ./run-dispose-probe.sh ~/build/konclude-lib-master/release
```

On the 95k-class SNOMED CT module (Mac M3) a healthy library prints a footprint after each
dispose of about 0.5 to 1.1 GB that does not rise from cycle to cycle (master after #85 and
#89); before #72 the whole knowledge base stayed, about 7 GB per cycle, and between #72 and
#85 about four small allocations per entity stayed and pinned the freed pages, so the footprint
rose by 1 to 3 GB per cycle. The probe runs a few minutes on the module and cannot be part of
the suites, whose ontologies are too small for a leak to show above the noise of the virtual
machine; it is the check to run after a change to the memory pools, the JNI bridge or the
ontology's destructors.

The pool blocks are mapped in whole pages, and since #110 they are also sized in whole pages:
the providers ask CMemoryPool::mappedBlockSize for the size a mapping of the default 50 000
bytes really occupies (65 536 on Apple silicon with its 16 KiB pages, 53 248 on Linux) and the
pool uses all of it. Before, a pool used 50 008 bytes of a block whose fourth page its first
856 bytes had already dirtied, so 23.7 % of every block on macOS (6 % on Linux) was charged to
the process and never used. The pool blocks are over 90 % of the footprint while reasoning:
at the peak of the full OWL 2 DL variant 463 292 blocks held 28.3 GB. With page-sized blocks
the peak footprint of SNOMED CT AU fell from 24.2 to 19.6 GiB and that of the variant from
35.1 to 28.2 GiB (Mac M3, command line, five and two alternating runs; hashes unchanged, times
equal within the noise, AU slightly faster). The measurements are in
`~/Documents/konclude-benchmarks/2026-10-08-pool-blocks/`.


## OTHER LIMITATIONS

- Annotations and annotation axioms are dropped by the translator, which matches the
  behaviour of the other interfaces of Konclude, where annotations are parsed but ignored.
- `HasKey`, `DatatypeDefinition` and SWRL rules have no entry point in the bridge. By default
  `KoncludeOWLAPITranslator` reports them with a `KoncludeUnsupportedConstructException`,
  since dropping an axiom makes the reasoner report entailments that do not hold.
  `KoncludeReasoner` switches the translator to collecting them instead and offers them
  through `getUnsupportedAxioms()`.
- The language tag of a literal is lost, `buildOWLLiteral` takes a lexical form and a
  datatype only.
- The bridge cannot update an installed ontology, so `flush()` translates the whole ontology
  into a fresh library instance.
- The shared library depends on the Qt libraries when it is built against a shared Qt, in
  contrast to the self-contained native libraries of other reasoners. Building it against a
  static Qt removes that, see below.
- A non-empty configuration string for `initKoncludeLibraryInstance` replaces the default
  loading arguments instead of extending them, so it also has to contain
  `-JNICommandProcessorLoader`. The `missing-processor` scenario of the smoke test documents
  the current behaviour.
