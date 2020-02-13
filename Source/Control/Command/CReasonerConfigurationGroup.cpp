/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "CReasonerConfigurationGroup.h"


namespace Konclude {

	namespace Control {

		namespace Command {


			CReasonerConfigurationGroup::CReasonerConfigurationGroup() {

				// Konclude reasoner configurations

				addConfigProperty(new CConfigDescription("Konclude.Version.Major",
						"Major version of Konclude.",
						new CIntegerConfigType(CKoncludeInfo::getKoncludeMajorVersionNumber())),
						new CIntegerConfigType(CKoncludeInfo::getKoncludeMajorVersionNumber()));
				addConfigProperty(new CConfigDescription("Konclude.Version.Minor",
						"Minor version of Konclude.",
						new CIntegerConfigType(CKoncludeInfo::getKoncludeMinorVersionNumber())),
						new CIntegerConfigType(CKoncludeInfo::getKoncludeMinorVersionNumber()));
				addConfigProperty(new CConfigDescription("Konclude.Version.Build",
						"Build of Konclude.",
						new CIntegerConfigType(CKoncludeInfo::getKoncludeBuildVersionNumber())),
						new CIntegerConfigType(CKoncludeInfo::getKoncludeBuildVersionNumber()));
				addConfigProperty(new CConfigDescription("Konclude.Name",
						"Name of Konclude.",
						new CStringConfigType(CKoncludeInfo::getKoncludeName())),
						new CStringConfigType(CKoncludeInfo::getKoncludeName()));
				addConfigProperty(new CConfigDescription("Konclude.Description",
						"Description of Konclude.",
						new CStringConfigType(CKoncludeInfo::getKoncludeDescription())),
						new CStringConfigType(CKoncludeInfo::getKoncludeDescription()));
				
				addConfigProperty(new CConfigDescription("Konclude.Execution.CommanderManager",
						"Commander Manager.",
						new CCommanderManagerConfigType(0)));
				addConfigProperty(new CConfigDescription("Konclude.Execution.ReasonerManager",
						"Reasoner Manager.",
						new CReasonerManagerConfigType(0)));
				addConfigProperty(new CConfigDescription("Konclude.Execution.ClassificationManager",
						"Classification Manager.",
						new CClassificationManagerConfigType(0)));
				addConfigProperty(new CConfigDescription("Konclude.Execution.OntologyRevisionManager",
						"Ontology Revision Manager.",
						new COntologyRevisionManagerConfigType(0)));
				addConfigProperty(new CConfigDescription("Konclude.Execution.DefaultReasonerManager",
						"The Reasoner Manager that is loaded by default.",
						new CStringConfigType(KONCLUDE_DEFAULTREASONERMANAGER)),
						new CStringConfigType(KONCLUDE_DEFAULTREASONERMANAGER));
				addConfigProperty(new CConfigDescription("Konclude.Execution.CalculationManager",
						"Determines which Calculation Manager will be used for calculation. Value can be 'Konclude.Calculation.Calculator.ConcurrentTaskCalculationManager'.",
						new CStringConfigType(KONCLUDE_DEFAULTCALCULATIONMANAGER)),
						new CStringConfigType(KONCLUDE_DEFAULTCALCULATIONMANAGER));



				addConfigProperty(new CConfigDescription("Konclude.Logging.MinLoggingLevel",
						"Minimum logging level.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));

				addConfigProperty(new CConfigDescription("Konclude.Logging.MaxLogMessageCount",
						"Maximum number of log messages that are kept by the logger.",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));


				// Parallelisation configurations

				addConfigProperty(new CConfigDescription("Konclude.Calculation.ProcessorCount",
						"Number of Processor-Threads that are used for task based reasoning.",
						new CStringConfigType("1")),
						new CStringConfigType("1"));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.UniqueNameAssumption",
						"Determines whether the reasoner treats each individual with distinct name as different of each other.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Memory.AllocationLimitation",
						"Determines whether the reasoner uses memory limits for the calculation process.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Memory.MaximumAllocationSize",
						"Configures the memory limit for the calculation.",
						new CIntegerConfigType(1024*1024*1800)),
						new CIntegerConfigType(1024*1024*1800));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Memory.CriticalAllocationSize",
						"Configures the critical memory limit for the calculation.",
						new CIntegerConfigType(1024*1024*1200)),
						new CIntegerConfigType(1024*1024*1200));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Memory.IncreaseAllocationSize",
						"Sets the memory allocation increasing size for the calculation.",
						new CIntegerConfigType(1024*1024*512)),
						new CIntegerConfigType(1024*1024*512));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.DatatypeReasoning",
						"Determines whether datatype reasoning is activated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.MaximumRecursiveProcessingConceptCount",
						"Determines the maximum recursion for the processing of added concepts in completion graphs with tableau expansion rules.",
						new CIntegerConfigType(200)),
						new CIntegerConfigType(200));


				// Processing configurations

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.Backjumping",
						"Determines whether the reasoner performs dependency directed backjumping/cancellation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.UnsatisfiableCacheRetrieval",
						"Determines whether the reasoner uses unsatisfiable cache retrievals for calculation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SatisfiableCacheRetrieval",
						"Determines whether the reasoner uses satisfiable cache retrievals for calculation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.UnsatisfiableCacheSingleLevelWriting",
						"Determines whether the reasoner writes single individual level clashes to unsatisfiable cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.UnsatisfiableCacheTestingConceptWriting",
						"Determines whether the reasoner writes unsatisfiability tested concepts to unsatisfiable cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SatisfiableCacheSingleLevelWriting",
						"Determines whether the reasoner writes single individual level satisfiable to satisfiable cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.DependencyTracking",
						"Determines whether the reasoner performs dependency tracking.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ProxyIndividuals",
						"Determines whether the reasoner uses proxy individuals.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.MinimizedMergingBranches",
						"Determines whether the reasoner minimizes merging branches.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ProcessedIndividualNodePropagation",
						"Determines whether the reasoner propagates the individual node complete processed flags to successors.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.IndividualNodeIDPriorization",
						"Determines whether the reasoner uses the individual node IDs to prioritize the individual node processing queue.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.StrictIndividualNodeProcessingOrder",
						"Determines whether the reasoner processes only one individual node at one time.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.BranchTriggering",
						"Determines whether the reasoner uses triggers to delay and avoid branches.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SemanticBranching",
						"Determines whether the reasoner uses semantic branching.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AtomicSemanticBranching",
						"Determines whether the reasoner uses semantic branching for atomic concepts that has no additional effort.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AnywhereCoreConceptCandidateHashBlockingSearch",
						"Determines whether the reasoner assists the anywhere blocking search with a candidate hash for marked core concepts.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AnywhereCandidateHashBlockingSearch",
						"Determines whether the reasoner assists the anywhere blocking search with a candidate hash.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AnywhereBlockingSearch",
						"Determines whether the reasoner uses an anywhere blocking search.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AncestorBlockingSearch",
						"Determines whether the reasoner uses an ancestor blocking search.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.PairwiseEqualSetBlockingTest",
						"Determines whether the reasoner checks blocking with pairwise equal concept set tests.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.EqualSetBlockingTest",
						"Determines whether the reasoner checks blocking with equal concept set tests.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SubSetBlockingTest",
						"Determines whether the reasoner checks blocking with sub concept set tests.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.OptimizedBlockingTest",
						"Determines whether the reasoner checks blocking with optimized blocking tests.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.DirectRulePreprocessing",
						"Determines whether the reasoner is direct preprocessing some rules.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.LazyNewMergingNominalCreation",
						"Determines whether the reasoner generates new nominals with the NN-rule as late as possible to avoid superfluous branches.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ConsistenceRestrictedNonStrictProcessing",
						"Determines whether the reasoner generates the deterministic part of the consistence model with non strict individual node processing order.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SatisfiableExpansionCacheRetrieval",
						"Determines whether the reasoner checks for cached nodes in the satisfiable expansion cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SatisfiableExpansionCacheConceptExpansion",
						"Determines whether the reasoner is expanding nodes with the satisfiable expansion cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SatisfiableExpansionCacheSatisfiableBlocking",
						"Determines whether the reasoner blocks processing for satisfiable cached nodes and successor nodes.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SatisfiableExpansionCacheWriting",
						"Determines whether the reasoner writes satisfiable nodes to satisfiable expansion cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SignatureMirroringBlocking",
						"Determines whether the reasoner can block nodes by mirroring other nodes.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SignatureSaving",
						"Determines whether the reasoner writes nodes concept set signatures into hash.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ANDConceptSkipping",
						"Determines whether the reasoner can skip adding AND concepts to individual nodes.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.CompletionGraphCaching",
						"Determines whether the reasoner can use the generated completion graph in the persistence check to block unnecessary individual node processing.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.DelayedCompletionGraphCachingReactivation",
						"Determines whether the reactivation of completion graph cached nodes is delayed.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AvoidRepeatedIndividualProcessing",
						"Determines whether the reprocessing of nodes for individuals is avoided if nominals do not occur in the ontology and tests do not directly refers to individuals.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ForceNodesRecreationForRepeatedIndividualProcessing",
						"Determines whether for the repeated reprocessing of individuals new nodes are created.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.CompletionGraphReuseCachingRetrieval",
						"Determines whether the reasoner queries the completion graph reuse cache for compatible entries.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.CompletionGraphDeterministicReuse",
						"Determines whether the reasoner is allowed to reuse deterministic completion graph reuse cache entries.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.CompletionGraphNonDeterministicReuse",
						"Determines whether the reasoner is allowed to reuse non-deterministic completion graph reuse cache entries.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.UnsatisfiableCachingFullDependency",
						"Determines whether the reasoner experimentally uses for the unsatisfiable caching additional all dependencies of all constructs of the unsatisfiable node.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.UnsatisfiableCachingFullSignature",
						"Determines whether the reasoner experimentally uses for the unsatisfiable caching additional the unsatisfiable node signatures for retrieval.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.PairwiseMerging",
						"Determines whether the reasoner experimentally uses the naive pairwise merging approach.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationPiling",
						"Determines whether the reasoner experimentally uses piling individual nodes for saturation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.RepresentativePropagation",
						"Determines whether the reasoner propagates representatives instead of sets of variable mappings.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.IndividualsBackendCacheLoading",
						"Determines whether the reasoner loads data for individuals from backend cache.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));




				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SuccessorConceptSaturationExpansion",
						"Determines whether the reasoner using saturated concepts for expanding newly created successor nodes.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationCaching",
						"Determines whether the reasoner using saturated concepts for caching.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationCriticalConceptTesting",
						"Determines whether the reasoner tests critical concepts after the saturation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationDirectCriticalToInsufficient",
						"Determines whether the reasoner directly marks nodes with critical concepts as insufficiently saturated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationSuccessorExtension",
						"Determines whether successors are extended in the saturation with concepts that are propagated into successor directions.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationReferredNodeManyConceptCount",
						"The number of concepts for which referred nodes are identified to have many concepts.",
						new CIntegerConfigType(500)),
						new CIntegerConfigType(500));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationManyConceptReferredNodeCountProcessLimit",
						"The number of referred nodes with many concept for which saturation is not performed.",
						new CIntegerConfigType(2)),
						new CIntegerConfigType(2));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationReferredNodeConceptCountProcessLimit",
						"The number of concepts of the referred nodes for which saturation is not performed.",
						new CIntegerConfigType(1500)),
						new CIntegerConfigType(1500));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationReferredNodeUnprocessedCountProcessLimit",
						"The number of unprocessed referred nodes for which saturation is not performed.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationReferredNodeCheckingDepth",
						"The recursive checking depth for referred nodes for saturation.",
						new CIntegerConfigType(5)),
						new CIntegerConfigType(5));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ForceManyConceptNodeSaturation",
						"Determines whether saturation is enforced even if there are many concepts.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));




				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.NominalSaturationCachingByNodeReactivation",
						"Determines whether the reasoner using saturated concepts with connections to nominals for caching with reactivation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.NominalSaturation",
						"Determines whether the reasoner uses the completion graph from the consistency test to improve the saturation of concept that have a connection to nominals.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationExpansionSatisfiabilityCacheWriting",
						"Determines whether the reasoner caches expansions and satisfiability for saturated nodes.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationExpansionSatisfiabilityCacheCount",
						"Determines how many expansions are cached by the reasoner for each saturated nodes.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.SaturationUnsatisfiabilityCacheWriting",
						"Determines whether the reasoner caches and propagates unsatisfiability for saturated nodes.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.EquivalentAlternativesSaturationMerging",
						"Determinses whether model merging with the saturation for alternatives of equivalent candidate concepts is used to prune possible subsumers.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ConceptSaturation",
						"Determinses whether concepts are saturated for the assisting of the tableau algorithm.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
 
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.IndividualSaturation",
						"Determines whether individual are saturated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ComputedTypesCaching",
						"Determines whether computed types are cached for reusing in other tests.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.ConstructionIndividualNodeMerging",
						"Determines whether constructed individual nodes are merged into nominal nodes.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.Cache.SatisfiableExpander.InitialMemoryLevelForIncreaseRequiredSignatureReferenceCount",
						"Specifies the initial memory level for which the required signature references for the creation of cache entries is increased.",
						new CIntegerConfigType(200*1024*1024)),
						new CIntegerConfigType(200*1024*1024));

				addConfigProperty(new CConfigDescription("Konclude.Cache.SatisfiableExpander.NextMemoryLevelIncreaseForIncreaseRequiredSignatureReferenceCount",
						"Specifies the increase for the next memory level for which the required signature references is increased.",
						new CIntegerConfigType(100*1024*1024)),
						new CIntegerConfigType(100*1024*1024));

				addConfigProperty(new CConfigDescription("Konclude.Cache.SatisfiableExpander.InitialRequiredSignatureReferenceCount",
						"Specifies the initial required signature references count.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));


				addConfigProperty(new CConfigDescription("Konclude.Cache.SatisfiableExpander.RequiredSignatureReferenceCountIncrease",
						"Specifies the increase for the required signature references count.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.BlockingTestsIgnoringCompletionGraphCachedNonBlockedNodes",
						"Determines whether blocking tests are performed for nodes that are present in the cached completion graph and are not blocked in it.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AllowBackendNeighbourExpansionBlocking",
						"Determines whether the expansion to neighbour nodes/individuals is blocked if the expansion is clear w.r.t. the backend.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.AllowBackendSuccessorExpansionBlocking",
						"Determines whether the expansion to blockable successor nodes is blocked in completion graphs if the expansion is clear w.r.t. the backend.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.OccurrenceStatisticsCollecting",
						"Determines whether the occurrence statistics are collected while building completion/saturation graph for consistency checking.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				// Preprocessing configurations

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.IndividualDataConvertion",
						"Determines whether the triple encoded assertions are converted to individual data.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.ExternalTriplesDatabaseCaching",
						"Determines whether the indexing data is stored in an external database.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.ExternalTriplesDatabaseCaching.DSNConfig",
						"The DSN config for the external triples database for caching ABox individuals indexing data.",
						new CStringConfigType("")),
						new CStringConfigType("dsn='VirtuosoTest',user='dba',password='dba'"));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.ExternalTriplesDatabaseCaching.TriplesDatabaseName",
						"The name of the external triples database for caching ABox individuals indexing data.",
						new CStringConfigType("http://Konclude.com/ReasoningData/$PercentEncodedKnowledgeBaseName/ABoxIndexingCache")),
						new CStringConfigType("http://Konclude.com/ReasoningData/$PercentEncodedKnowledgeBaseName/ABoxIndexingCache"));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.ExternalTriplesDatabaseCaching.CacheSize",
						"Specifies the size of the cache for indexing data of ABox individuals.",
						new CIntegerConfigType(1500000)),
						new CIntegerConfigType(1500000));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.TripleEncodedAssertionsIndexing.ExternalTriplesDatabaseCaching.ClearAtInit",
						"Determines whether the external database is cleared on initializing the ABox individuals indexing cache, i.e., all present triples are rmoved.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.ForceInverseRoleCreation",
						"Determines whether the creation of inverse roles is forced.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing",
						"Determines whether the ontology is preprocessed. Will disable all preprocessing steps.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.NegationNormalization",
						"Determines whether the ontology is preprocessed with the essential negation normalization.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.BranchTriggerExtraction",
						"Determines whether the branch triggers are generated in preprocessing phase.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.LexicalNormalization",
						"Determines whether the ontology is preprocessed with lexical normalization.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.RoleChainAutomataTransformation",
						"Determines whether the ontology is preprocessed with role chain automata transformation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.GCIAbsorption.TriggeredImplicationBinaryGCIAbsorption",
						"Determines whether the ontology is preprocessed with general concept axioms (GCI) absorption into triggered binary implications.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.SubroleTransformation",
						"Determines whether the ontology is preprocessed with subrole transformation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DatatypeNormalizer",
						"Determines whether the ontology is preprocessed with datatype normalization.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DataLiteralValueNormalizer",
						"Determines whether the ontology is preprocessed with data literal value normalization.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.OntologyAssignmentTransformation",
						"Determines whether the ontology is preprocessed with ontology assignment transformation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.OntologyMappingUpdate",
						"Determines whether the ontology is preprocessed with ontology mapping update.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.ProcessingDataExtender",
						"Determines whether the ontology is extended with processing data.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.ReverseRoleAssertionGeneration",
						"Determines whether reverse role assertions are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.OntologyStructureInspection",
						"Determines whether the ontology is preprocessed with ontology structure inspection.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CheckingOntologyConsistency",
						"Determines whether in the preprocessing phase the ontology consistency is checked.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.OntologyPrecomputation",
						"Determines whether in the preprocessing phase the ontology consistency is checked and starts some other precomputations.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CoreConceptCyclesPrecomputation",
						"Determines whether in the preprocessing phase the core concept cycles are precomputated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CoreConceptCyclesExtraction",
						"Determines whether in the preprocessing phase the core concept cycles are extracted from the ontology.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CoreBlockingConceptMarking",
						"Determines whether in the preprocessing phase some concepts are marked as core blocking concepts.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DisjunctionToImplicationAbsorption",
						"Determines whether in the preprocessing phase disjunctions are absorbed to implications.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DisjunctionToImplicationAbsorptionByExistingTriggers",
						"Determines whether in the preprocessing phase disjunctions are absorbed to implications with existing triggers.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.EquivalentDefinitionToSubclassImplicationAbsorption",
						"Determines whether in the preprocessing phase equivalent defined concepts are absorbed to subclasses and implications.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.PartialEquivalentDefinitionToCandidatesAbsorption",
						"Determines whether in the preprocessing phase equivalent defined concepts are partially absorbed to candidates.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.PartialDisjunctionAbsorption",
						"Determines whether in the preprocessing phase disjunctions are partially.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CardinalityQualificationTriggerAbsorption",
						"Determines whether in the preprocessing phase the qualification concepts of cardinality restrictions are absorbed to candidates.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CardinalityQualificationTriggerPartialAbsorption",
						"Determines whether in the preprocessing phase the qualification concepts of cardinality restrictions are partially absorbed to candidates.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CardinalityQualificationTriggerOnlyPositiveAbsorption",
						"Determines whether in the preprocessing phase the qualification concepts of cardinality restrictions are only absorbed if they occur positive.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.AssuranceSatisfactionAbsorption",
						"Determines whether an (additional) assurance-based satisfaction absorption is used for GCIs.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.PartialGCIImplicationAbsorption",
						"Determines whether in the preprocessing phase GCIs are partially absorbed.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CommonDisjunctConceptExtraction",
						"Determines whether in the preprocessing phase common disjunct concepts are extracted, so they can be added before branching.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DisjunctSorting",
						"Determines whether the disjuncts in disjunctions are sorted.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.BranchingStatisticsExtender",
						"Determines whether the concepts are extended by branch statistics.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.NominalSchemaTemplateExtraction",
						"Determines whether in the preprocessing phase nominal schema templates are extracted.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.NominalSchemaGrounding",
						"Determines whether in the preprocessing phase axioms with nominal schemas are grounded.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.FullNominalSchemaGrounding",
						"Determines whether in the preprocessing phase axioms with nominal schemas are fully grounded.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.AbsorbableNominalSchemaGrounding",
						"Determines whether in the preprocessing phase axioms with absorbable nominal schemas are grounded.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.NominalSchemaPathPreabsorption",
						"Determines whether in the preprocessing phase absorbed axioms with nominal schemas are extended by backward chaining with the propagation of binding candidates.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.NominalSchemaBackPropagation",
						"Determines whether in the preprocessing phase absorbed axioms with nominal schemas are extended by basic backward chaining.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.ExtractPropagationIntoCreationDirection",
						"Determines whether in the preprocessing phase all concepts with a possibly propagation into the creation direction are identified.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.RelevantConceptRoleMarker",
						"Determines whether in the preprocessing phase all concepts and roles that are relevant for inferences are marked.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DatatypeAbsorption",
						"Determines whether datatypes are absorbed.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CoreConceptCyclesExtraction.SkipForELFragment",
						"Determines whether the extraction of core concept cycles is skipped for EL ontologies.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.ExtractPropagationIntoCreationDirection.SkipForELFragment",
						"Determines whether the extraction of propagation in creation direction is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CoreBlockingConceptMarking.SkipForELFragment",
						"Determines whether the marking of core concepts is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.ProcessingDataExtender.SkipForELFragment",
						"Determines whether the extension with processing data is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.CommonDisjunctConceptExtraction.SkipForELFragment",
						"Determines whether the extraction of common disjunct concepts is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.LexicalNormalization.SkipForELFragment",
						"Determines whether the lexical normalization is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.BranchTriggerExtraction.SkipForELFragment",
						"Determines whether the extraction of branch triggers is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.DisjunctSorting.SkipForELFragment",
						"Determines whether the sorting of disjunctions is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.BranchingStatisticsExtender.SkipForELFragment",
						"Determines whether the extensions of concepts by branch statistics is skipped for EL ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));




				// Precomputation configurations

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.TotalPrecomputor.MaximumParallelCalculationCount",
						"Determines the maximum number of calculation jobs that are created at the same time.",
						new CIntegerConfigType(10)),
						((CConfigData*)nullptr));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.TotalPrecomputor.MultipliedUnitsParallelCalculationCount",
						"Determines how many calculation jobs are created.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.TotalPrecomputor.IndividualsSaturationSize",
						"Determines how many individuals are saturated together.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.ForceFullCompletionGraphConstruction",
						"Determines whether the construction of a completion graph is enforced for the consistency check.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.ConditionalFullCompletionGraphConstruction",
						"Determines whether a full completion graph is constructed for the consistency check if certain conditions are satisfied (i.e., if one SuggestionCondition but no ExclusionCondition is satisfied).",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.ExclusionCondition.MaximumIndividualLimit",
						"Specifies the maximum number of individual for which a full completion graph can be constructed.",
						new CIntegerConfigType(300000)),
						new CIntegerConfigType(300000));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.SuggestionCondition.MaximumIndividualLimit",
						"Specifies the number of individual for which a full completion graph should be constructed.",
						new CIntegerConfigType(10000)),
						new CIntegerConfigType(10000));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.ExclusionCondition.MaximumIndividualConceptRatio",
						"Specifies the maximum ratio (in percent) between individuals and concepts with which a full completion graph can be constructed.",
						new CIntegerConfigType(5000)),
						new CIntegerConfigType(5000));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.FullCompletionGraphConstruction.SuggestionCondition.MaximumIndividualConceptRatio",
						"Specifies the maximum ratio between individuals and concepts with which a full completion graph should be constructed.",
						new CIntegerConfigType(500)),
						new CIntegerConfigType(500));




				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.CollectProcessStatistics",
						"Determines whether calculation statistics for created tasks are collected.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Precomputation.TrivialConsistencyChecking",
						"Determines whether consistency is pretested by analysing the axioms and expressions in the ontology.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));




				// Classification configurations

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.Classifier",
						"Determines which subsumption classifier is used for classification. Value can be 'Konclude.Calculation.Classification.Classifier.BruteForceClassifier' or 'Konclude.Calculation.Classification.Classifier.EnhancedTraversalClassifier' or 'Konclude.Calculation.Classification.Classifier.OptimizedSubClassClassifier' or 'Konclude.Calculation.Classification.Classifier.ExperimentalEnhancedTraversalClassifier' or 'Konclude.Calculation.Classification.Classifier.OptimizedKPSetClassClassifier'.",
						new CStringConfigType("Konclude.Calculation.Classification.Classifier.OptimizedKPSetClassClassifier")),
						new CStringConfigType("Konclude.Calculation.Classification.Classifier.OptimizedKPSetClassClassifier"));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.PruneSubsumptionRelations",
						"Determines whether unnecessary calculations for ontology classification are pruned.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.TransitiveReduction.AddAllPredecessors",
						"Determines whether all predecessors are added to the hierarchy nodes in the transitive reduction.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.DerivateSubsumptionRelations",
						"Determines whether unnecessary calculations for ontology classification are derrived.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.PseudoModelSubsumptionMerging",
						"Determines whether subsumption relations are calculated with pseudo model merging for classification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.PseudoModelSubsumptionMerging.MaximumFastPseudoModelMergingDepth",
						"Determines how depth the pseudo models are merged for a fast foregoing subsumption check.",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.MaximumParallelSubsumptionCalculationCount",
						"Determines how many subsumption calculation jobs are created per classifier.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.PseudoModelSubsumptionMerging.DirectMerging",
						"Determines whether subsumption relations are calculated with direct pseudo model merging for classification. The config 'Konclude.Calculation.Classification.PseudoModelSubsumptionMerging' must be also activated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.PseudoModelSubsumptionMerging.ProofMerging",
						"Determines whether subsumption relations are calculated with proof pseudo model merging for classification. The config 'Konclude.Calculation.Classification.PseudoModelSubsumptionMerging' must be also activated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.ObviousSubsumptionCalculation",
						"Determines whether obvious subsumption relations are calculated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SatisfiableSubsumptionExtraction",
						"Determines whether subsumption relations are extracted while satisfiable tests.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.AutomatedOptimizedClassifierSelection",
						"Determines whether an optimized classifier is choosen automatically when possible.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.ForceCompleteSatisfiableTest",
						"Determines whether satisfiable tests are executed for each concept independent from the result of other satisfiable tests.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.TaxonomySubsumptionCalculationSponsor",
						"Determines whether the subsumption calculation sponsor is checked for each subsumption calculation.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.TaxonomySatisfiableCalculationSponsor",
						"Determines whether the satisfiable calculation sponsor is checked for each satisfiable calculation.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.ConsecutivelyConstructedSatisfiableTests",
						"Determines whether for the satisfiable calculation a previous satisfiable calculation is extended.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SubclassConsideration",
						"Determines whether any subclass optimization is used.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SubclassConsideration.SubclassSubsummingCheck",
						"Determines whether subclass relations are considered.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SubclassConsideration.SubclassSubsummingCheck.PseudoModelGenerationSupport",
						"Determines whether pseudo models are inspected for generating possible subclass relations. Requires the config 'Konclude.Calculation.Classification.SubclassConsideration' for the possible subclass generation.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SubclassConsideration.SubclassIdentifierPossiblePredecessorOccurCheck",
						"Determines whether deterministic subclass occurrences in satisfiable classes are used to derivate non subsumptions.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SubclassConsideration.PossibleSubclassSubsummingOccurCheck",
						"Uses non-deterministic occured identifier in satisfiable tests for restricting possible subsumptions.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.CollectProcessStatistics",
						"Determines whether the statistics for building the model are collected for the classification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.OptimizedSubClassSubsumptionClassifier.MaximumParallelSatisfiableCalculationCount",
						"Determines how many satisfiable calculation jobs are created per classifier at one time.",
						new CIntegerConfigType(1)),
						((CConfigData*)nullptr));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.OptimizedSubClassSubsumptionClassifier.MultipliedUnitsParallelSatisfiableCalculationCount",
						"Determines how many subsumption calculation jobs are created per classifier.",
						new CIntegerConfigType(8)),
						new CIntegerConfigType(8));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.MaximumParallelSatisfiableCalculationCount",
						"Determines how many satisfiable calculation jobs are created per classifier at one time.",
						new CIntegerConfigType(1)),
						((CConfigData*)nullptr));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.MultipliedUnitsParallelSatisfiableCalculationCount",
						"Determines how many subsumption calculation jobs are created per classifier.",
						new CIntegerConfigType(8)),
						new CIntegerConfigType(8));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.OptimizedKPSetClassSubsumptionClassifier.ParallelSatisfiableCalculationCreationFactor",
						"Determines the factor where the classifier starts to create new calculation jobs.",
						new CIntegerConfigType(600)),
						new CIntegerConfigType(600));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.SaturationSubsumerExtraction",
						"Determines whether the subsumption relations are extracted from the saturation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Classification.IndividualDependenceTracking",
						"Determines whether dependent individuals are tracked for classification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));










				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.MaximumParallelTestingCalculationCount",
						"Determines how many calculation jobs are maximally created per realizer at one time.",
						new CIntegerConfigType(1)),
						((CConfigData*)nullptr));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.MultipliedUnitsParallelTestingCalculationCount",
						"Determines how many calculation jobs are created per realizer.",
						new CIntegerConfigType(8)),
						new CIntegerConfigType(8));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.PossibleInstanceConceptsUpfrontMerging",
						"Determines whether a merging of possible instances with the to be checked concept should be tried upfront.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMerging",
						"Determines whether the reasoner should try to merge other possible instances into the checked concept instance.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMergingOnlyWithSameRepresentativeLabel",
						"Determines whether the reasoner should only provide individuals with same representative labels for the afterwards possible instance merging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.AlternatePossibleConceptInstanceTestingItemsOnCalculations",
						"Determines whether possible concept instance testing is alternated for the concepts.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMergingProvidingCount",
						"Determines how many individuals are provided for merging possible instances after instance checking (negative numbers are interpreted as infinite).",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.OptimizedKPSetOntologyConceptRealizer.PossibleInstanceIndividualsAfterwardsMergingMaximumAttemptCount",
						"Determines how often it can be tried to merge another possible instance individual into the result of the currently checked concept instance.",
						new CIntegerConfigType(3)),
						new CIntegerConfigType(3));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.NonDeterministicCachedCompletionGraphContinuationPropagationTests",
						"Determines whether the non-deterministic derived completion graph that is cached from the consistency check is used for the propagation tests (e.g., for propagating marker concepts for role realization).",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.RealizePrecomputation.SameIndividuals",
						"Determines whether same individuals are precomputed if realization is requested.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.RealizePrecomputation.ConceptInstances",
						"Determines whether concept instances are precomputed if realization is requested.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.RealizePrecomputation.RoleInstances",
						"Determines whether role instances are precomputed if realization is requested.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.IndividualDependenceTracking",
						"Determines whether dependent individuals are tracked for realization.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.PossibleAssertionsCollecting",
						"Determines whether possible assertions are collected for realization.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.ExtraConsistencyTesting",
						"Determines whether a separate consistency test is performed for realization.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));










				addConfigProperty(new CConfigDescription("Konclude.Calculation.Querying.ComplexQueryingSupport",
						"Determines whether the ontology is prepared for complex querying.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));














				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.BuildAfterModification",
						"Determines whether an ontology is automatically build after modification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.PreprocessAfterModification",
						"Determines whether an ontology is automatically preprocess after modification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.ConsistencyPrecomputationAfterModification",
						"Determines whether an ontology is automatically test consistency after modification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.CyclesPrecomputationAfterModification",
						"Determines whether an ontology is automatically test consistency after modification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.ClassifyAfterModification",
						"Determines whether an ontology is automatically classified after modification.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.IncrementalRebuild",
						"Determines whether an ontology is incrementally rebuild.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.Ontology.Revision.BasementResolveKBName",
						"Determines which knowledge bases is used as basement for the creation of new knowledge bases.",
						new CStringConfigType("")),
						new CStringConfigType(""));












				addConfigProperty(new CConfigDescription("Konclude.Persistence.OntologyRevisionPersistencer",
						"Determines the persistencer for ontology revisions.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Persistence.DirectoryOWL2XMLFilePersistencer.BaseDirectory",
						"Determines the directory where the DirectoryOWL2XMLFilePersistencer persists the ontologies.",
						new CStringConfigType("./Data/Ontologies/Persistence/")),
						new CStringConfigType("./Data/Ontologies/Persistence/"));















				addConfigProperty(new CConfigDescription("Konclude.Query.Statistics.CollectStatistics",
						"Determines whether statistics are collected for queries.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Query.Statistics.CollectCalculationStatistics",
						"Determines whether statistics about calculations are collected.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Query.Statistics.CollectProcessingStepsStatistics",
						"Determines whether statistics about processing steps are collected.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));








				addConfigProperty(new CConfigDescription("Konclude.Answering.DefaultComplexConceptAnswerer",
						"Determines the default answerer for complex concept queries.",
						new CStringConfigType("Konclude.Answering.OptimizedComplexConceptAnswerer")),
						new CStringConfigType("Konclude.Answering.OptimizedComplexConceptAnswerer"));



				addConfigProperty(new CConfigDescription("Konclude.Answering.QueryAbsorption.CreateAlwaysBindingConcepts",
						"Determines whether for each variable of the query a binding concept is created.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.QueryAbsorption.Preabsorption",
						"Determines whether a query with existential parts is pre-absorbed.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.QueryAbsorption.PreabsorptionCount",
						"Determines how often a query with existential parts is pre-absorbed.",
						new CIntegerConfigType(2)),
						new CIntegerConfigType(2));

				addConfigProperty(new CConfigDescription("Konclude.Answering.QueryAbsorption.InverseIndividualVariableSinglePathAbsorption",
						"Determines whether single paths to individual variables are absorbed in inverse direction.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.QueryAbsorption.OccurrenceStatisticsBasedAbsorptionOrder",
						"Determines whether occurrence statistics are used for selecting the next query term for the absorption.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.RedundantTermElimination",
						"Determines whether redundant query terms are eliminated by using class/object property hierarchy and inverses.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.RedundantlyRestrictedVariablesElimination",
						"Determines whether redundantly restricted variables are eliminated from the query.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.AnonymousToDistinguishedVariableConversion",
						"Determines whether anonymous variables are converted to distinguished variables if there exist only individual nodes that satisfy the restrictions of the variables.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.AllowBindingReduction",
						"Determines whether bindings for variable mappings are reduced/eliminated if some bindings are not further required.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.ImplicitJoiningBindingReduction",
						"Determines whether bindings for variable mappings are reduced/eliminated while joining.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.BindingReducingForNonDistinctAnswerQueries",
						"Determines whether bindings for variable mappings can be reduced even for queries with answer variables.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.BindingReductionMappingSize",
						"Determines at which variable mappings sizes a reduction is allowed.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));

				addConfigProperty(new CConfigDescription("Konclude.Answering.SamplingBasedJoinMappingSize",
						"Determines at which variable mappings sizes a sampling based joining is used.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Answering.PropagationJoining",
						"Determines whether bindings for variable mappings are joined with other mappings while propagating them over roles.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.PropagationReplacement",
						"Determines whether bindings for variable mappings are replaced with other mappings while propagating them over roles.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.AnonymousToDistinguishedVariableOnlyFullConversion",
						"Determines whether anonymous variables are converted to distinguished variables only if it is possible for all.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.UnsatisfiableNonInstanceStatisticsChecking",
						"Determines whether unsatisfiability of a query is pre-tested by checking whether all restrictions principally occur in the model of the ontology.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.FailOnUnknownEntity",
						"Determines whether an error is reported if an unknown entity is used in the query.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.ExtendedLogging",
						"Determines whether additional logging is output for complex query answering.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.InterpretNonAnswerIndividualVariablesAsAnonymousVariables",
						"Determines whether non-answer individual variables are interpreted as anonymous variables.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.NonAnswerIndividualVariablesAsAnonymousVariablesInterpretingPrefixString",
						"Determines whether the prefix that non-answer individual variables must have to be interpreted as anonymous variables.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Answering.InterpretQueriesAsDistinct",
						"Determines whether all queries are interpreted with the distinct modifier.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));






				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentComputationThreadPoolSize",
						"Determines the maximum number of threads for the concurrent query answer computations (0 stands for automatically using the number of computer cores/threads).",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));




				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentJoinComputation",
						"Determines whether bindings are joined concurrently.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentJoinComputationTaskCount",
						"Determines with how many tasks bindings are concurrently joined.",
						new CIntegerConfigType(211)),
						new CIntegerConfigType(211));


				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentJoinPerformanceLogging",
						"Determines whether some performance metrics are logged for the concurrent join.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentJoinDirectJoiningForCheckingSide",
						"Determines whether joined variable bindings are directly created for the checking side, i.e., the side that is not inserted into the joining hash.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentJoinPartitionizedMemoryManagement",
						"Determines whether the memory management bindings for the concurrent joins of bindings is partitionized.",
						new CBooleanConfigType(false)), 
						new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.Answering.ConcurrentAnswerGeneration",
						"Determines whether answers are concurrently generated from computed bindings (if possible).",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Answering.MaximumParallelTestingCalculationCount",
						"Determines how many calculation jobs are maximally created per answerer at one time.",
						new CIntegerConfigType(1)),
						((CConfigData*)nullptr));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Answering.MultipliedUnitsParallelTestingCalculationCount",
						"Determines how many calculation jobs are created per answerer.",
						new CIntegerConfigType(8)),
						new CIntegerConfigType(8));



				addConfigProperty(new CConfigDescription("Konclude.Calculation.Answering.CollectProcessStatistics",
						"Determines whether statistics for answering queries are collected.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Answering.NonDeterministicCachedCompletionGraphContinuationPropagationTests",
						"Determines whether the non-deterministic derived completion graph that is cached from the consistency check is used for the propagation tests.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Answering.MaximumBatchMappingsComputationSize",
						"Determines the maximum size for the batch based variable mappings computations.",
						new CIntegerConfigType(100000000)),
						new CIntegerConfigType(100000000));

				addConfigProperty(new CConfigDescription("Konclude.Answering.BatchMappingsComputationSizeIncreasingFactorPercent",
						"Determines the increasing factor (in percent) for the size of the variable mapping computations batches.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Answering.FirstBatchMappingsComputationSize",
						"Determines the size of the first batch for batch-based variable mappings computations.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Answering.MinimalMappingsComputationSize",
						"Determines the minimum size for the variable mappings computations, i.e., how many mappings are computed by default in each step.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));

				addConfigProperty(new CConfigDescription("Konclude.Answering.MappingsRepeatedlyInsufficientDependencyComputationIncreasingFactorPercent",
						"Determines the factor (in percent) that is used to increase the required mapping size of dependent steps if they are repeatedly insufficiently.",
						new CIntegerConfigType(130)),
						new CIntegerConfigType(130));

				addConfigProperty(new CConfigDescription("Konclude.Answering.MappingsComputationUnlimitedInterpretationSize",
						"Determines the size for variable mappings computations that is interpreted as unlimited.",
						new CIntegerConfigType(Q_UINT64_C(100000000000000))),
						new CIntegerConfigType(Q_UINT64_C(100000000000000)));

				addConfigProperty(new CConfigDescription("Konclude.Answering.ContinueMappingsCompuationWhenResultsNotWriteable",
						"Determines whether the computation of variable mappings is continued even if the answers cannot be written/sent (e.g., for benchmarking).",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Answering.LazyExistentialQueryPartAbsorption",
						"Determines whether existential parts of query are lazily absorbed, i.e., only if the non-existential part is computed.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.BooleanQueryExistentialPartOrdinaryEvaluation",
						"Determines whether existential parts of Boolean queries are first ordinarily evaluated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.FullQueryMaterialization",
						"Determines whether the query is materialized at the beginning before query answers computation is started.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.Answering.DistinctUnecessaryVariablesReductionBeforeAnswerGeneration",
						"Determines whether the all bindings for non-answer variables are reduced for queries with distinct modifier.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.AlwaysUnecessaryVariablesReductionBeforeAnswerGeneration",
						"Determines whether the all bindings for non-answer variables are reduced for all queries.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Answering.ConceptItemExpectedCountSamplingSize",
						"Determines the sampling size for determining the expected instances count for complex concept items.",
						new CIntegerConfigType(30)),
						new CIntegerConfigType(30));

				addConfigProperty(new CConfigDescription("Konclude.Answering.ConceptItemExpectedCountSamplingRetrievedPercentageLimit",
						"Determines the limit (in percent) for the retrieved instances up to which the expected count is sampled.",
						new CIntegerConfigType(75)),
						new CIntegerConfigType(75));


				addConfigProperty(new CConfigDescription("Konclude.Answering.MaxVariableMappingsPropagationsInitializationIndividualCount",
						"Determines the maximum number of initialization individuals for variable mapping propagations tests.",
						new CIntegerConfigType(50000)),
						new CIntegerConfigType(50000));









				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalSPARQLQueryProcessing",
					"Determines whether SPARQL queries are processed with Redland Rasqal.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalReasoningSubBGPQueryAnswerVariableReduction",
					"Determines whether the answer variables for extracted sub-queries of BGPs from SPARQL queries processed with Redland Rasqal are reduced by checking whether they have other occurrences in the query.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalMinimizeBGPSubQueryTriples",
					"Determines whether the triples of BGPs are reduced/minimized if bindings can be transferred with less triples.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalReasoningSubBGPQueryDistinctPropagation",
					"Determines whether the DISTINCT modifier is used for suitable BGP sub-queries of SPARQL queries processed with Redland Rasqal (if there are no aggregates, etc.).",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalSingleSubBGPQueryAllowsDirectResultStreaming",
					"Determines whether the results of the only BGP sub-query of a SPARQL query are directly streamed (if supported) instead of processing the query with Redland Rasqal.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Answering.LogCompositionQueriesProcessingProgressStatistics",
					"Determines whether the processing progress statistics are logged for complex composition queries.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Answering.LogCompositionQueriesProcessingFinishedStatistics",
					"Determines whether the processing statistics are logged for complex composition queries if the processing is finished.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteRedlandRasqalQueryStructure",
					"Determines whether the parsed Redland Rasqal query structure/patterns are written to file.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalReasoningSubBGPQueryForceDistinct",
					"Determines whether the extracted sub-queries of BGPs from SPARQL queries are always considered distinct.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalSubQueriesDependencyIdentification",
					"Determines whether dependencies between extracted sub-queries of BGPs from SPARQL queries are identified such that results from dependent sub-queries can be used to restrict bindings.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Answering.RedlandRasqalSubQueriesFollowingRestrictionsIntegration",
					"Determines whether (triple pattern) restrictions of following BGPs from SPARQL queries are integrated in extracted sub-queries (without influencing cardinality of answers) such that less results may be determined.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				// Testing/Debugging configurations


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingData",
					"If enabled (true) the reasoner is allowed to write debugging data to files.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataSaturationTasks",
					"If enabled (true) the reasoner is allowed to write debugging data of saturation testing tasks to files.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasks",
					"If enabled (true) the reasoner is allowed to write debugging data of completion graph testing tasks to files.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksOnlyOnSatisfiability",
					"If enabled (true) the reasoner writes debugging data of completion graph testing tasks only if the tasks are satisfiable.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksForConsistencyTests",
					"If enabled (true) the reasoner writes debugging data of completion graph consistency testing tasks.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksForClassificationTests",
					"If enabled (true) the reasoner writes debugging data of completion graph consistency testing tasks.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksForIncrementalExpansionTests",
					"If enabled (true) the reasoner writes debugging data of completion graph incremental expansion testing tasks.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksForRepresentativeCacheRecomputationTests",
					"If enabled (true) the reasoner writes debugging data of completion graph representative cache recomputation testing tasks.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksForAllTests",
					"If enabled (true) the reasoner writes debugging data of all completion graph testing tasks.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingDataCompletionTasksForAnsweringPropagationTests",
					"If enabled (true) the reasoner writes debugging data of completion graph propagation testing tasks for query answering.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteAnsweringVariableCompositionItems",
					"If enabled (true) the reasoner writes variable composition items to files.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritePreprocessingDebuggingData",
					"If enabled (true) the reasoner writes preprocessing debugging data to files.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.BaseWritingPath",
					"Determines the path/directory where debugging files are written.",
					new CStringConfigType("./Debugging")),
					new CStringConfigType("./Debugging"));



				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritePreprocessingDebuggingData.BuildedTBox",
					"If enabled (true) the reasoner writes the corresponding preprocessing debugging data to files.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritingPath.Preprocessing.BuildedTBox",
					"Determines the path where the corresponding debugging data is written.",
					new CStringConfigType("Preprocessing/1-BuildedTBox.txt")),
					new CStringConfigType("Preprocessing/1-BuildedTBox.txt"));



				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritePreprocessingDebuggingData.NormalizedTBox",
					"If enabled (true) the reasoner writes the corresponding preprocessing debugging data to files.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritingPath.Preprocessing.NormalizedTBox",
					"Determines the path where the corresponding debugging data is written.",
					new CStringConfigType("Preprocessing/2-NormalizedTBox.txt")),
					new CStringConfigType("Preprocessing/2-NormalizedTBox.txt"));



				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritePreprocessingDebuggingData.GroundedTBox",
					"If enabled (true) the reasoner writes the corresponding preprocessing debugging data to files.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritingPath.Preprocessing.GroundedTBox",
					"Determines the path where the corresponding debugging data is written.",
					new CStringConfigType("Preprocessing/3-GroundedTBox.txt")),
					new CStringConfigType("Preprocessing/3-GroundedTBox.txt"));




				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritePreprocessingDebuggingData.AbsorbedTBox",
					"If enabled (true) the reasoner writes the corresponding preprocessing debugging data to files.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritingPath.Preprocessing.AbsorbedTBox",
					"Determines the path where the corresponding debugging data is written.",
					new CStringConfigType("Preprocessing/4-AbsorbedTBox.txt")),
					new CStringConfigType("Preprocessing/4-AbsorbedTBox.txt"));




				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritePreprocessingDebuggingData.ProcessedTBox",
					"If enabled (true) the reasoner writes the corresponding preprocessing debugging data to files.",
					new CBooleanConfigType(true)),
					new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.WritingPath.Preprocessing.ProcessedTBox",
					"Determines the path where the corresponding debugging data is written.",
					new CStringConfigType("Preprocessing/5-ProcessedTBox.txt")),
					new CStringConfigType("Preprocessing/5-ProcessedTBox.txt"));


				addConfigProperty(new CConfigDescription("Konclude.Debugging.QueryPreprocessingBaseWritingPathExtension",
					"Determines the relative path/directory to the BaseWritingPath where the query related preprocessing is written.",
					new CStringConfigType("/Answering")),
					new CStringConfigType("/Answering"));



				// Evaluation configurations



				addConfigProperty(new CConfigDescription("Konclude.Analyse.UpdateInterval",
						"Update interval for reading analyse data.",
						new CIntegerConfigType(100)),
						new CIntegerConfigType(100));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.DelayedStartTime",
						"Wait time before start analyse data.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.CommonScale",
						"Plot analyse data with a common scale.",
						new CStringConfigType("AUTO")),
						new CStringConfigType("AUTO"));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.PlotDifference",
						"Plot the difference of succeeding analyse data.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Analyse.LimitThreadCount",
						"Limits the threads for reading analyse data.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.FirstThreadNumber",
						"The first thread number that is used to read analyse data.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.LastThreadNumber",
						"The last thread number that is used to read analyse data.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));


				addConfigProperty(new CConfigDescription("Konclude.Analyse.Classification.StatusText",
						"Read Status text from specified Classifier for analyse data.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Analyse.ReasonerManager.QueryTotalWorkCount",
						"Read QueryTotalWorkCount from Reasoner-Manager for analyse data.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.ReasonerManager.QueryCalcedWorkCount",
						"Read QueryCalcedWorkCount from Reasoner-Manager for analyse data.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Analyse.ReasonerManager.QueryProgress",
						"Read QueryProgress from Reasoner-Manager for analyse data.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));





				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.CompletionGraphRandomWalks",
						"Determines whether conjunctive queries are generated by random walking over fully expanded and clash-free completion graphs.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.MaximumQueryGenerationCount",
						"Determines how many conjunctive queries are maximal generated with the current settings.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.MaximumOntologyQueryCount",
						"Determines how many conjunctive queries are allowed for the given ontology.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));


				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.RealCycleQueries",
						"Determines whether real cycle queries are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.TreeFoldedCylceQueries",
						"Determines whether tree folded cycles queries are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.TreeShapedQueries",
						"Determines whether tree shaped queries are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.GenerateConceptTerms",
						"Determines whether concept terms are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.GenerateTreeExtensions",
						"Determines whether tree extensions that can be rolled-up are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.OnlyWalkOverIndividualNodes",
						"Determines whether only over nodes representing individuals is walked.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.QueryFileBasePath",
						"Determines the base path for queries.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.OntologyFileBasePath",
						"Determines the base path of the ontologies.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.LoadFileBasePath",
						"Determines the base path prefix used for load commands.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.QueryTemplateFile",
						"Determines the query template file.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.MinimalVariableSelectCount",
						"Determines the minimum of selected variables.",
						new CIntegerConfigType(1)),
						new CIntegerConfigType(1));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.MaximumVariableSelectCount",
						"Determines the maximum of selected variables.",
						new CIntegerConfigType(4)),
						new CIntegerConfigType(4));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.IndividualNodeAsExistentialVariableInterpretationRatio",
						"Determines the percentage with which individual nodes are interpreted as existential variables.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));


				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.OntologyStatisticsFileBasePath",
						"Determines the base path prefix for ontology statistics.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.QueryStatisticsFileBasePath",
						"Determines the base path prefix for the to be generated query statistics.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.MaximumTreeExtensionSize",
						"Determines the maximum size of tree extensions.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.TreeExtensionCreationRatio",
						"Determines the ratio of how often are tree extensions created in percent.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));


				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.TreeExtensionsOnlyOverIndividualNodes",
						"Determines whether tree extensions that can be rolled-up are generated.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.Test.ConjunctiveQueryGeneration.TreeExtensionExistentialVariableConversionRatio",
						"Determines the percentage with which individual nodes are converted to existential variables.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));












				addConfigProperty(new CConfigDescription("Konclude.Test.PigeonholePrincipleGenerator.ProblemSize",
						"Generating problem size of the pigeonhole principle.",
						new CIntegerConfigType(2)),
						new CIntegerConfigType(2));

				addConfigProperty(new CConfigDescription("Konclude.Test.PigeonholePrincipleGenerator.OWLlinkOutputFile",
						"Output file for the generated pigeonhole principle.",
						new CStringConfigType("PHP-request-file.xml")),
						new CStringConfigType("PHP-request-file.xml"));

				addConfigProperty(new CConfigDescription("Konclude.Test.RecursionTestDataGenerator.IndividualCount",
						"Number of individual for the recursion test data.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Test.RecursionTestDataGenerator.PropertyAssertionCount",
						"Number of the property assertions for the recursion test data.",
						new CIntegerConfigType(200)),
						new CIntegerConfigType(200));

				addConfigProperty(new CConfigDescription("Konclude.Test.RecursionTestDataGenerator.OWLlinkOutputFile",
						"Output file for the generated recursion test data.",
						new CStringConfigType("RecursionTestData.owl.xml")),
						new CStringConfigType("RecursionTestData.owl.xml"));

				addConfigProperty(new CConfigDescription("Konclude.Test.LargeJoinTestDataGenerator.IndividualCount",
						"Number of individual for the large join test data.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Test.LargeJoinTestDataGenerator.PropertyAssertionCount",
						"Number of the property assertions for the large join test data.",
						new CIntegerConfigType(200)),
						new CIntegerConfigType(200));

				addConfigProperty(new CConfigDescription("Konclude.Test.LargeJoinTestDataGenerator.OWLlinkOutputFile",
						"Output file for the generated large join test data.",
						new CStringConfigType("LargeJoinTestData.owl.xml")),
						new CStringConfigType("LargeJoinTestData.owl.xml"));





				addConfigProperty(new CConfigDescription("Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.ClassesCount",
						"Number of classes for the transitive connected EL classes test data.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Test.TransitiveConnectedELClassesTestDataGenerator.OWLlinkOutputFile",
						"Output file for the generated transitive connected EL classes test data.",
						new CStringConfigType("TransitivityConnectedELClassesTestData.owl.xml")),
						new CStringConfigType("TransitivityConnectedELClassesTestData.owl.xml"));



				addConfigProperty(new CConfigDescription("Konclude.Test.Converter.InputDirectory",
						"Input directory for converter.",
						new CStringConfigType("Tests/Converter/Input")),
						new CStringConfigType("Tests/Converter/Input"));

				addConfigProperty(new CConfigDescription("Konclude.Test.Converter.OutputDirectory",
						"Output directory for converter.",
						new CStringConfigType("Tests/Converter/Output")),
						new CStringConfigType("Tests/Converter/Output"));



				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.InputDirectory",
						"Input directory for converter.",
						new CStringConfigType("Evaluation/Generator/Input")),
						new CStringConfigType("Evaluation/Generator/Input"));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.OutputDirectory",
						"Output directory for converter.",
						new CStringConfigType("Tests/Generator/Output")),
						new CStringConfigType("Tests/Generator/Output"));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.FilterDirectory",
						"Filter directory for converter.",
						new CStringConfigType("")),
						new CStringConfigType(""));


				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.OntologyStatisticsDirectory",
						"Output directory for tested evaluation tests.",
						new CStringConfigType("Evaluation/Infos/Statistics/Ontologies/")),
						new CStringConfigType("Evaluation/Infos/Statistics/Ontologies/"));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.FilterFile",
						"Path to filter file.",
						new CStringConfigType("Evaluation/Filters/ABoxFilter.txt")),
						new CStringConfigType("Evaluation/Filters/ABoxFilter.txt"));




				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateClassifyTests",
						"Determines whether classification tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateConsistencyTests",
						"Determines whether consistency tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateSatisfiabilityTests",
						"Determines whether satisfiability tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateTypeTests",
						"Determines whether type tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateRealizeTests",
						"Determines whether realization tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));




				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateIncrementalAssertionConsistencyTests",
						"Determines whether incremental assertion consistency tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionAdditionCount",
						"Determines the number of assertion that are added for incremental consistency tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.TestCount",
						"Determines the number incremental consistency tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionRemovalCount",
						"Determines the number of assertion that are removed for incremental consistency tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.RatioAdditionRemoval",
						"Determines whether the incremental changed assertion count for incremental consistency tests is determined by the ontology size ratio.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionAdditionRatioDenominator",
						"Determines the denominator for the assertion ratio that are added for incremental consistency tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionConsistencyTestsGeneration.AssertionRemovalRatioDenominator",
						"Determines the denominator for the assertion ratio  that are removed for incremental consistency tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));










				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateIncrementalAssertionClassificationTests",
						"Determines whether incremental assertion classification tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionAdditionCount",
						"Determines the number of assertion that are added for incremental classification tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.TestCount",
						"Determines the number incremental classification tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionRemovalCount",
						"Determines the number of assertion that are removed for incremental classification tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.RatioAdditionRemoval",
						"Determines whether the incremental changed assertion count for incremental classification tests is determined by the ontology size ratio.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionAdditionRatioDenominator",
						"Determines the denominator for the assertion ratio that are added for incremental classification tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionClassificationTestsGeneration.AssertionRemovalRatioDenominator",
						"Determines the denominator for the assertion ratio  that are removed for incremental classification tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));








				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.GenerateIncrementalAssertionRealizationTests",
						"Determines whether incremental assertion realization tests are generated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionAdditionCount",
						"Determines the number of assertion that are added for incremental realization tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.TestCount",
						"Determines the number incremental realization tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionRemovalCount",
						"Determines the number of assertion that are removed for incremental realization tests.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.RatioAdditionRemoval",
						"Determines whether the incremental changed assertion count for incremental realization tests is determined by the ontology size ratio.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionAdditionRatioDenominator",
						"Determines the denominator for the assertion ratio that are added for incremental realization tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.IncrementalAssertionRealizationTestsGeneration.AssertionRemovalRatioDenominator",
						"Determines the denominator for the assertion ratio  that are removed for incremental realization tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));










				
				addConfigProperty(new CConfigDescription("Konclude.Debug.PrecheckReasoning",
						"Prechecking Query with Default Reasoner before Debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debug.FailAfterConsistencyConceptSaturation",
						"Fail after the saturation of concepts required for the consistency test for debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debug.FailAfterConsistencyCheck",
						"Fail after the consistency test for debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debug.FailAfterConceptSaturation",
						"Fail after the saturation of concepts for debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debug.FailAfterPreprocessing",
						"Fail after the preprocessing for debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Debug.FailAfterBuilt",
						"Fail after the built for debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));






				addConfigProperty(new CConfigDescription("Konclude.Parser.UTF8CompatibilityEnforcedXMLStreamParsing",
						"Determines whether UTF-8 compatibility is enforced for stream-based XML parsing.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Parser.RedlandRaptor.LoadTriplesIntoStore",
						"Determines whether triples are loaded into the redland triple store such that non-trivial OWL 2 axioms can be retrieved.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				// OWLLink configurations


				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Version.Major",
						"Major version of OWLlink Interface implementation.",
						new CIntegerConfigType(KONCLUDE_OWLLINK_VERSION_MAJOR)),
						new CIntegerConfigType(KONCLUDE_OWLLINK_VERSION_MAJOR));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Version.Minor",
						"Minor version of OWLlink Interface implementation.",
						new CIntegerConfigType(KONCLUDE_OWLLINK_VERSION_MINOR)),
						new CIntegerConfigType(KONCLUDE_OWLLINK_VERSION_MINOR));


				addConfigProperty(new CConfigDescription("Konclude.OWLlink.WriteResponseToStandardOutput",
						"Write OWLlink response direct to program standard output channel.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.WriteResponseToStandardError",
						"Write OWLlink response direct to program standard error output channel.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.CloseAfterProcessedRequest",
						"Close program after the OWLlink response is processed.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.BlockUntilProcessedRequest",
						"Blocks processing until request has finished.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.LoggingExtendedProcessingTimes",
						"Extended processing time logging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.SAXBasedLoadOntologiesParsing",
						"SAX-based parsing for ontologies with load ontologies command.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.StreamBasedLoadOntologiesParsing",
						"Stream-based parsing for ontologies with load ontologies command.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.OWLlink.ExtendedErrorReporting",
						"Determines whether more detailed error messages are reported via OWLlink.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Server.ForceDisconnectAfterProcessing",
						"Determines whether the OWLlink server forces a disconnect after a request is processed.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.AbbreviatedIRIs",
						"Using abbreviated IRIs.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.OWLlink.ResultAnonymousIndividualsIntegration",
						"Determines whether the results for anonymous individuals are integrated.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.OWLlink.RequestFile",
						"Filepath for OWLlink request file loader.",
						new CStringConfigType("")),
						new CStringConfigType(""));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.ResponseFile",
						"Filepath for writing OWLlink response file.",
						new CStringConfigType("")),
						new CStringConfigType(""));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.IRIName",
						"IRI name for OWLlink.",
						new CStringConfigType("")),
						new CStringConfigType(""));


				addConfigProperty(new CConfigDescription("Konclude.OWLlink.TargetsSourcesQueriesInverseCorrection",
						"Inverse property correction for targets and sources query.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.OWLlink.DirectInversePropertiesQuerySupport",
						"Inverse property correction for targets and sources query.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Test.RunTimeout",
						"Timeout for tests.",
						new CIntegerConfigType(60000)),
						new CIntegerConfigType(60000));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Test.CloseTimeout",
						"Timeout for closing running tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Test.KillTimeout",
						"Timeout for killing running tests.",
						new CIntegerConfigType(1000)),
						new CIntegerConfigType(1000));

				addConfigProperty(new CConfigDescription("Konclude.OWLlink.OWLlinkAPI.Address",
						"Address of the OWLlinkAPI based reasoner server.",
						new CStringConfigType("http://127.0.0.1:8080")),
						new CStringConfigType("http://127.0.0.1:8080"));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.OWLlinkAPI.RequestFileCompatibleModification",
						"Modifies the request file for OWLlinkAPI compatibility.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.OWLlinkAPI.ResponseFileTimeExtension",
						"Modifies the response file with response times.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.OWLlinkAPI.RequestFileAbsolutePathsModification",
						"Rewrites relative filepaths to absolute filepaths in request file.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));



				addConfigProperty(new CConfigDescription("Konclude.OWLlink.Server.Port",
						"Port of the OWLlink server.",
						new CIntegerConfigType(8080)),
						new CIntegerConfigType(8080));






				// SPARQL configurations


				addConfigProperty(new CConfigDescription("Konclude.SPARQL.RequestFile",
					"Filepath for SPARQL request file loader.",
					new CStringConfigType("")),
					new CStringConfigType(""));
				addConfigProperty(new CConfigDescription("Konclude.SPARQL.ResponseFile",
					"Filepath for writing SPARQL response file.",
					new CStringConfigType("")),
					new CStringConfigType(""));


				addConfigProperty(new CConfigDescription("Konclude.SPARQL.ComparisonFile",
						"Filepath of the SPARQL comparison file.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.SPARQL.Comparison.DifferencesLoggingLimit",
						"The number of differences that are reported as logging output (-1 is no limit).",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));


				addConfigProperty(new CConfigDescription("Konclude.SPARQL.WriteResponseToStandardOutput",
					"Write SPARQL response direct to program standard output channel.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.SPARQL.WriteResponseToStandardError",
					"Write SPARQL response direct to program standard error output channel.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.SPARQL.CloseAfterProcessedRequest",
					"Close program after the SPARQL response has processed.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.SPARQL.BlockUntilProcessedRequest",
					"Blocks processing until request has finished.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.SPARQL.LoggingExtendedProcessingTimes",
					"Extended processing time logging.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.SPARQL.DefaultResolvingOntology",
					"Name of the ontology that is used for processing the SPARQL query if no ontology name is specified, i.e., if there is no 'FROM' clause.",
					new CStringConfigType("http://Konclude.com/Ontologies/$LastCreatedKnowledgeBaseIfAvailableAndCreateNewAnonymousKnowledgeBaseOtherwise")),
					new CStringConfigType("http://Konclude.com/Ontologies/$LastCreatedKnowledgeBaseIfAvailableAndCreateNewAnonymousKnowledgeBaseOtherwise"));
				addConfigProperty(new CConfigDescription("Konclude.SPARQL.AlwaysResolveToDefault",
					"Ignores FROM clauses by always resolving the SPARQL query with the default ontology specified by 'Konclude.SPARQL.DefaultResolvingOntology'.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.SPARQL.Server.Port",
					"Port of the SPARQL server.",
					new CIntegerConfigType(8080)),
					new CIntegerConfigType(8080));


				addConfigProperty(new CConfigDescription("Konclude.SPARQL.Server.ForceDisconnectAfterProcessing",
					"Determines whether the SPARQL server forces a disconnect after a request is processed.",
					new CBooleanConfigType(false)),
					new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.SPARQL.Serialization.ChunkEncodingSize",
					"Determines whether at which size (in bytes) the results are chunked.",
					new CIntegerConfigType(104857600)),
					new CIntegerConfigType(104857600));


				addConfigProperty(new CConfigDescription("Konclude.SPARQL.Server.WriteBufferBlockingLimit",
						"Determines the network write buffer limit up to which the result generation is not blocked.",
						new CIntegerConfigType(20)),
						new CIntegerConfigType(20));

				addConfigProperty(new CConfigDescription("Konclude.SPARQL.File.WriteBufferBlockingLimit",
						"Determines the file write buffer limit up to which the result generation is not blocked.",
						new CIntegerConfigType(20)),
						new CIntegerConfigType(20));


				// CLI configurations


				addConfigProperty(new CConfigDescription("Konclude.CLI.RequestFile",
						"Filepath of request file for command line interface.",
						new CStringConfigType("")),
						new CStringConfigType(""));
				addConfigProperty(new CConfigDescription("Konclude.CLI.ResponseFile",
						"Filepath of response file for command line interface.",
						new CStringConfigType("")),
						new CStringConfigType(""));
				addConfigProperty(new CConfigDescription("Konclude.CLI.IRIName",
						"IRI name for command line interface.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.CLI.CloseAfterProcessedRequest",
						"Close program after the CLI command has processed.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.CLI.BlockUntilProcessedRequest",
						"Blocks processing until request has finished.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));





				// Evaluation configurations

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.ProgramFile",
						"Evaluation program.",
						new CStringConfigType("Evaluation/EvaluationProgram.xml")),
						new CStringConfigType("Evaluation/EvaluationProgram.xml"));




				addConfigProperty(new CConfigDescription("Konclude.Evaluation.ConfigFile",
						"Evaluation config.",
						new CStringConfigType("Evaluation/Configs/ExampleConfigFile.xml")),
						new CStringConfigType("Evaluation/Configs/ExampleConfigFile.xml"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Config.Name",
						"Evaluation config name.",
						new CStringConfigType("ExampleConfigName")),
						new CStringConfigType("ExampleConfigName"));



				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Platform",
						"Platform for the Reasoners.",
						new CStringConfigType("UNSPECIFIED")),
						new CStringConfigType("UNSPECIFIED"));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Directory",
						"Reasoner directory.",
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/")),
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Name",
						"Name of the reasoner that is evaluated.",
						new CStringConfigType("ExampleReasoner v1.0")),
						new CStringConfigType("ExampleReasoner v1.0"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.OutputDirectory",
						"Output directory for the reasoner.",
						new CStringConfigType("ExampleReasoner/v1.0/")),
						new CStringConfigType("ExampleReasoner/v1.0/"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.ConfigFile",
						"Config file of the reasoner.",
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/reasonerConfig.xml")),
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/reasonerConfig.xml"));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.InitializationFile",
						"Initialization file of the reasoner.",
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/reasonerInit.xml")),
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/reasonerInit.xml"));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.ExecutionType",
						"How the reasoner is executed.",
						new CStringConfigType("Binary")),
						new CStringConfigType("Binary"));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.Binary.File",
						"File of the binary.",
						new CStringConfigType("ExampleReasoner")),
						new CStringConfigType("ExampleReasoner"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.Binary.Arguments",
						"Arguments for the execution.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.ClassPathLibraries",
						"ClassPath libraries for the execution.",
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/libs")),
						new CStringConfigType("Evaluation/Reasoners/ExampleReasoner/v1.0/libs"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.ClassPathLibrariesSeparator",
						"Separater for class path libraries, usually ';' for Windows and ':' for Linux.",
						new CStringConfigType(";")),
						new CStringConfigType(";"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.MainClass",
						"MainClass for the execution.",
						new CStringConfigType("com.examplereasoner.main")),
						new CStringConfigType("com.examplereasoner.main"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.AdditionalParameters",
						"Additional parameters for the execution.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Execution.OWLlinkOWLAPIServerAdapter.AdditionalProcess",
						"Additional process that has to be executed.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Address.Port",
						"The port address of the reasoner for requests.",
						new CIntegerConfigType(8080)),
						new CIntegerConfigType(8080));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Reasoner.Address.IP",
						"The IP address of the reasoner for requests.",
						new CStringConfigType("127.0.0.1")),
						new CStringConfigType("127.0.0.1"));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.InputDirectory",
						"Input directory for tested evaluation tests.",
						new CStringConfigType("Evaluation/Requests/")),
						new CStringConfigType("Evaluation/Requests/"));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.OutputDirectory",
						"Output directory for tested evaluation tests.",
						new CStringConfigType("Evaluation/Responses/R-000/")),
						new CStringConfigType("Evaluation/Responses/R-000/"));




				addConfigProperty(new CConfigDescription("Konclude.Evaluation.TestingCount",
						"The number how many times the same test is executed.",
						new CIntegerConfigType(10)),
						new CIntegerConfigType(10));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.TestingTimeout",
						"The time limit for the reasoner to complete a test.",
						new CIntegerConfigType(2*1000*60)),
						new CIntegerConfigType(2*1000*60));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.ResponseDownloadSizeLimit",
						"The time limit for the response in bytes. If the limit is reached, the request is aborted and the reasoner execution is stopped. -1 stands for no limit.",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.RequestTimeout",
						"The time limit for the reasoner to complete a single request.",
						new CIntegerConfigType(1000*60)),
						new CIntegerConfigType(1000*60));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.MemoryLimit",
						"The memory limit for the reasoner in bytes.",
						new CIntegerConfigType(1024*1024*1024*10)),
						new CIntegerConfigType(1024*1024*1024*10));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Analyser.CreateGNUPlots",
						"Determines whether the analysers create plots with GNU plot.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.Analyser.CreateHighchartsPlots",
						"Determines whether the analysers create plots with Highcharts.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


 
				addConfigProperty(new CConfigDescription("Konclude.Evaluation.RequestFileAbsolutePathsModification",
						"Rewrites relative filepaths to absolute filepaths in request file.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.RequestReplaceLoadOntologiesWithTell",
						"Directly replaces load ontologies commands with tell commands in request files.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.ResolveAppreviatedIRIsForReplacedTells",
						"Directly resolves abbreviated IRIs for load ontologies commands that are replaced by tell commands in request files.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.CancelReasonerErrorCount",
						"After how many errors the evaluation for a reasoner is canceled.",
						new CIntegerConfigType(2)),
						new CIntegerConfigType(2));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.ReasonerTestErrorMaximalRetryCount",
						"How many retries are done for each test.",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.ReasonerTestErrorMaximalRetryWriting",
						"Determines whether the last response is written if the maximal number of retries for a test is reached.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.WaitingTimeBetweenTests",
						"Waiting time between tests in milliseconds.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.TestcaseEvaluationExceptionLimit",
						"Limit of testcases that are evaluated until an exception is thrown.",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.FirstTestsWarmingUp",
						"Determines whether the first test case in the evaluation is additionally executed at first without measurement.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.AllTestsWarmingUp",
						"Determines whether all test cases in the evaluation are additionally executed at first without measurement.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.WaitingTimeAfterReasonerCreation",
						"Waiting time after reasoner has created in milliseconds.",
						new CIntegerConfigType(false)),
						new CIntegerConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.SPARQLSendPrepareBeforeQuerying",
						"Sending a SPARQL PREPARE command before starting querying.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.SPARQLSupportsPrepareOperation",
						"Whether the SPARQL PREPARE command is supported.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.SPARQLResponseSizeLimit",
						"The limit for SPARQL responses in bytes. If the response exceeds the size limit, then it is not stored and not further analyzed. -1 is interpreted as no limit.",
						new CIntegerConfigType(-1)),
						new CIntegerConfigType(-1));




				addConfigProperty(new CConfigDescription("Konclude.Evaluation.SystemReadyRetestTimeForTests",
						"Waiting time for retest whether system is ready for next evaluation test in milliseconds.",
						new CIntegerConfigType(false)),
						new CIntegerConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.CriticalSystemProcessTesterProgram",
						"Program/Script/Command Line that returns 1 if there are running critical processes on the system.",
						new CStringConfigType("")),
						new CStringConfigType(""));


				addConfigProperty(new CConfigDescription("Konclude.Evaluation.TerminateAssistProgram",
						"Program/Script/Command Line that is called with the process id to assist termination.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.TerminateAssistAdditionalArgument",
						"Additional arguments for the termination program.",
						new CStringConfigType("")),
						new CStringConfigType(""));


				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.WriteDeclarations",
						"Determines whether declarations are also written to the output file.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.AbbreviatedIRIs",
						"Determines whether abbreviated IRIs are used to write the output.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));



				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.WriteOnlyDirectTypes",
						"Determines whether only direct types are written to the output file for realization tasks.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.WriteReducedInconsistency",
						"Determines whether for inconsistent ontologies only SubclassOf(Top,Bottom) is written to the output.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.WriteAnonymousIndividualResults",
						"Determines whether the results for anonymous individuals are written.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));




				addConfigProperty(new CConfigDescription("Konclude.ORE.OperationTask",
						"Operation task that has to be tested.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.ORE.OntologyFile",
						"Ontologie file name.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ResponseFile",
						"Response file name.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.ORE.IRIName",
						"IRI name for testing.",
						new CStringConfigType("")),
						new CStringConfigType(""));

				addConfigProperty(new CConfigDescription("Konclude.ORE.CloseAfterProcessed",
						"Determines whether the program is closed after the processing has finished.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.ORE.OutputTimeMeasurementInSeconds",
						"Determines the measured time is printed in seconds instead of milliseconds.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));


				addConfigProperty(new CConfigDescription("Konclude.ORE.ClassHierarchyResult.WriteDeclarations",
						"Determines whether declarations are also written to the output file for the class hierarchy.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ClassHierarchyResult.AbbreviatedIRIs",
						"Determines whether abbreviated IRIs are used for the classes.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ClassHierarchyResult.WriteDirectEquivalences",
						"Determines whether equivalences are directly written with equivalent classes axioms instead of several subclass axioms.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ClassHierarchyResult.WriteReducedTopSubclasses",
						"Determines whether only SubclassOf(Top,A) axioms are written for all classes A that are semantically equivalent to Top.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ClassHierarchyResult.WriteReducedBottomSubclasses",
						"Determines whether only SubclassOf(A,Bottom) axioms are written for all classes A that are semantically equivalent to Bottom.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ClassHierarchyResult.WriteReducedInconsistency",
						"Determines whether only SubclassOf(Top,Bottom) is written for inconsistent ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));




				addConfigProperty(new CConfigDescription("Konclude.ORE.RealizationResult.WriteDeclarations",
						"Determines whether declarations are also written to the realization output file.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.RealizationResult.AbbreviatedIRIs",
						"Determines whether abbreviated IRIs are used for the individuals and classes.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.RealizationResult.WriteReducedInconsistency",
						"Determines whether only SubclassOf(Top,Bottom) is written for inconsistent ontologies.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));

				addConfigProperty(new CConfigDescription("Konclude.ORE.RealizationResult.WriteOnlyDirectTypes",
						"Determines whether only direct types are written to the output file for realization tasks.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));




				addConfigProperty(new CConfigDescription("Konclude.ORE.ReportParsingTimeForConsistency",
						"Determines whether the parsing time is reported for the consistency checking reasoning task.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ReportParsingTimeForSatisfiability",
						"Determines whether the parsing time is reported for the satisfiability checking reasoning task.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ReportParsingTimeForRealization",
						"Determines whether the parsing time is reported for the realization reasoning task.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

				addConfigProperty(new CConfigDescription("Konclude.ORE.ReportParsingTimeForClassification",
						"Determines whether the parsing time is reported for the classification reasoning task.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));



			}


			CReasonerConfigurationGroup::~CReasonerConfigurationGroup() {
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
