/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
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
						new CIntegerConfigType(1024*1024*25)),
						new CIntegerConfigType(1024*1024*25));





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

				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.NominalSaturationCachingByNodeReactivation",
						"Determines whether the reasoner using saturated concepts with connections to nominals for caching with reactivation.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Optimization.NominalSaturation",
						"Determines whether the reasoner uses the completion graph from the consistency test to improve the saturation of concept that have a connection to nominals.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));




				// Preprocessing configurations

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
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.GCIAbsorption.TriggeredImplicationGCIAbsorption",
						"Determines whether the ontology is preprocessed with general concept axioms (GCI) absorption into triggered implications.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Preprocessing.SubroleTransformation",
						"Determines whether the ontology is preprocessed with subrole transformation.",
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
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
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
						"Determines whether unnecessary calculations for ontology classification are derivated.",
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















				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.MaximumParallelTestingCalculationCount",
						"Determines how many calculation jobs are maximally created per realizer at one time.",
						new CIntegerConfigType(1)),
						((CConfigData*)nullptr));
				addConfigProperty(new CConfigDescription("Konclude.Calculation.Realization.COptimizedKPSetOntologyConceptRealizer.MultipliedUnitsParallelTestingCalculationCount",
						"Determines how many calculation jobs are created per realizer.",
						new CIntegerConfigType(8)),
						new CIntegerConfigType(8));
















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







				// Testing/Debugging configurations


				// Evaluation configurations

				addConfigProperty(new CConfigDescription("Konclude.Debugging.WriteDebuggingData",
						"If enabled (true) the reasoner writes some debugging data to standard files.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));

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

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.OntologyStatisticsDirectory",
						"Output directory for tested evaluation tests.",
						new CStringConfigType("Evaluation/Infos/Statistics/Ontologies/")),
						new CStringConfigType("Evaluation/Infos/Statistics/Ontologies/"));

				addConfigProperty(new CConfigDescription("Konclude.Test.Generator.FilterFile",
						"Path to filter file.",
						new CStringConfigType("Evaluation/Filters/ABoxFilter.txt")),
						new CStringConfigType("Evaluation/Filters/ABoxFilter.txt"));


				
				addConfigProperty(new CConfigDescription("Konclude.Debug.CacheLineWriteViewer",
						"Widget that shows written Cache lines.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.Debug.VisualizeCalcBoxIndividuals",
						"Shows individuals from CalcBoxes in the CalcBox-Tracking-View.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));
				
				addConfigProperty(new CConfigDescription("Konclude.Debug.PrecheckReasoning",
						"Prechecking Query with Default Reasoner before Debugging.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));










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
						"Close program after the OWLlink response has processed.",
						new CBooleanConfigType(false)),
						new CBooleanConfigType(false));
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.BlockUntilProcessedRequest",
						"Blocks afterwards processing until request has finished.",
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


				
				addConfigProperty(new CConfigDescription("Konclude.OWLlink.AbbreviatedIRIs",
						"Using abbreviated IRIs.",
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

				addConfigProperty(new CConfigDescription("Konclude.Evaluation.WaitingTimeBetweenTests",
						"Waiting time between tests in milliseconds.",
						new CIntegerConfigType(0)),
						new CIntegerConfigType(0));

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







				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.WriteDeclarations",
						"Determines whether declarations are also written to the output file.",
						new CBooleanConfigType(true)),
						new CBooleanConfigType(true));


				addConfigProperty(new CConfigDescription("Konclude.CLI.Output.AbbreviatedIRIs",
						"Determines whether abbreviated IRIs are used to write the output.",
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
						"Determines whether declarations are also written to the output file.",
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

			}


			CReasonerConfigurationGroup::~CReasonerConfigurationGroup() {
			}



		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
