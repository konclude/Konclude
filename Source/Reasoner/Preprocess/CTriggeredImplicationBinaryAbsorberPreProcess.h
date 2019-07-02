/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_PREPROCESS_CTRIGGEREDIMPLICATIONBINARYABSORBERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CTRIGGEREDIMPLICATIONBINARYABSORBERPREPROCESS_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "CConcreteOntologyContinuablePreProcess.h"
#include "CConceptRoleIndividualLocator.h"
#include "CConceptTriggerLinker.h"
#include "CConceptRoleIndividualLocator.h"
#include "CNominalSchemaAbsorptionPathLinker.h"
#include "CNominalSchemaAbsorptionBranchLinker.h"


// Other includes
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CSortedNegLinker.hpp"

#include "Config/CConfigDataReader.h"

#include "Reasoner/Generator/CConcreteOntologyInverseRoleBuilder.h"

#include "Reasoner/Ontology/CConceptTextFormater.h"
#include "Reasoner/Ontology/COntologyTextFormater.h"
#include "Reasoner/Ontology/CNominalSchemaTemplate.h"
#include "Reasoner/Ontology/CNominalSchemaTemplateVector.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Generator;
		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CTriggeredImplicationBinaryAbsorberPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTriggeredImplicationBinaryAbsorberPreProcess : public CConcreteOntologyContinuablePreProcess {
				// public methods
				public:
					//! Constructor
					CTriggeredImplicationBinaryAbsorberPreProcess();

					//! Destructor
					virtual ~CTriggeredImplicationBinaryAbsorberPreProcess();

					virtual CConcreteOntology* preprocess(CConcreteOntology *ontology, CPreProcessContext* context);
					virtual CConcreteOntology* continuePreprocessing();


				// protected methods
				protected:
					typedef QPair<CConcept*,bool> TConceptNegationPair;
					typedef QPair<bool,bool> TOccuredAbsorbablePair;
					typedef QPair<CConcept*,CConcept*> TConceptPair;
					typedef QPair<CNominalSchemaAbsorptionBranchLinker*,CNominalSchemaAbsorptionBranchLinker*> TAbsorptionBranchPair;


					void createDisjunctionAbsorptions(bool continuationExtension = false);
					void createConceptOfInterestAbsorptions(bool continuationExtension = false);

					void asorbForallsToRanges(CConcept* topConcept);

					bool collectConjunctions(CConcept* concept, bool negated, QList< TConceptNegationPair >& conjList);

					void collectPositiveConcepts(CConcept* concept, bool negated, QSet<TConceptNegationPair>* positiveConceptSet, bool recursiveDefinition = false);

					CConcept* getRoleDomainTriggerConcept(CRole* role);

					bool addConceptToImplied(CConcept* impliedConcept, CConcept* concept, bool negated);
					bool addSubClassConcept(CConcept* subclassConcept, CConcept* addedConcept, bool negated);
					bool addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated);
					bool addRoleRangeConcept(CRole* role, CConcept* addedConcept, bool negated);
					CConcept* createCandidateEquivalentConcept(CConcept* eqConcept);
					CConcept* createImpliedConcept();
					CConcept* createImplicationConcept(CConcept* impliedConcept, bool negated);
					CConcept* createImplicationTriggerConcept(CConcept* impConcept, bool negated);
					bool addImplicationTrigger(CConcept* implConcept, CConcept* triggerConcept, bool negated);
					CConcept* createTriggerPropagationConcept(CConcept* destConcept, CRole* backwardPropRole, bool branchTiggerCreation, bool invsereRolePropagation = true);
					CConcept* createNominalImplication(CConcept* destConcept, CIndividual* nominalIndi);
					CConcept* createDatatypeImplication(CConcept* destConcept, CDatatype* datatype);
					CConcept* createDataLiteralImplication(CConcept* destConcept, CDataLiteral* dataLiteral);
					CConcept* createDataRestrictionImplication(CConcept* destConcept, CDatatype* datatype, CDataLiteral* dataliteral, cint64 restrictionCode);

					bool isEquivalenceConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount = nullptr);
					bool isEquivalenceConceptCandidateExtractable(CConcept* orConcept, bool negated);
					CConcept* getEquivalenceConceptCandidateTriggeredImplication(CConcept* eqConcept, bool negated);
					bool isGCIConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount = nullptr);
					bool isConceptImplicationTriggerable(CConcept* concept, bool negated, QHash<CConcept*,TOccuredAbsorbablePair>* conceptEqConAbsorbed);
					bool isConceptImplicationTriggerable(CConcept* concept, bool negated, QHash<CConcept*,TOccuredAbsorbablePair>* conceptEqConAbsorbed, cint64* openMultipleOccurCount, QList<TConceptNegationPair>** dependingCacheList);
					bool isConceptSimpleImplicationTriggerable(CConcept* concept, bool negated);
					void createEquivalenceAbsorbedTriggeredImplication(CConcept* eqConcept, bool negated, CSortedNegLinker<CConcept*>* absorpOpLinker);
					CConcept* createDisjunctionAbsorbedTriggeredImplication(CConcept* orConcept, bool negated);
					CConcept* createDisjunctionAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& candList);


					void mergeDependingCacheList(QList<TConceptNegationPair>** dependingCacheList, QList<TConceptNegationPair>** subDependingCacheList);



					CConceptTriggerLinker* getTriggersForConcept(CConcept* concept, bool negated, CConcept* backPropConcept = nullptr);
					CConceptTriggerLinker* createTriggerLinker();
					CConceptTriggerLinker* copyTriggerLinkers(CConceptTriggerLinker* triggers);
					void releaseTriggerLinkers(CConceptTriggerLinker* triggers);


					CDatatypeValueSpaceConceptTriggerLinker* createValueSpaceConceptTriggerLinker();

					CConcept* getImplicationImpliedConcept(CConcept* implicationConcept);
					CConceptTriggerLinker* getImplicationTriggeredConceptForTriggers(CConceptTriggerLinker* triggers, CConcept** firstImplicationConcept);

					CConceptTriggerLinker* getUpdatedTriggerComplexities(CConceptTriggerLinker* triggers);

					bool findAndReplaceImplicationFromTriggers(CConceptTriggerLinker* trigger, CConceptTriggerLinker*& otherTriggers);


					CConcept* createTriggerConcept(bool branchTrigger = false);
					CConcept* createMarkerConcept();
					void addUnfoldingConceptForConcept(CConcept* concept, CConcept* addingUnfoldingConcept, bool addingNegation);
					void addUnfoldingConceptForImplicationConcept(CConcept* implicationConcept, CConcept* addingUnfoldingConcept, bool addingNegation);

					CConceptTriggerLinker* getPartialTriggersForConcept(CConcept* concept, bool negated, CConcept* backPropConcept = nullptr);
					CConceptRoleBranchingTrigger* getSimpleBranchTriggersForConcept(CConcept* concept, bool negated);

					CConceptRoleBranchingTrigger* getBranchTiggers(CConceptTriggerLinker* triggers);

					CRole* getInverseRole(CRole* role);

					bool isDisjunctionConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated);
					bool isDisjunctionConceptSimpleTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated);
					
					bool isDisjunctionConceptPartialTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated);

					bool isConceptPartialImplicationTriggerable(CConcept* concept, bool negated, QSet<CConcept*>* conceptEqConAbsorbed);
					

					

					bool isCardinalityConceptQualificationTriggerAbsorbable(CConcept* cardConcept, bool negated);
					bool isCardinalityConceptPartialQualificationTriggerAbsorbable(CConcept* cardConcept, bool negated);


					CConceptRoleBranchingTrigger* createCardinalityQualificationPartialAbsorbedTriggers(CConcept* cardConcept, bool negated);


					bool absorbGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated);

					void createGCIAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& candList);


					bool absorbPartialGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated);
					void createGCIPartialAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, const QList<TConceptNegationPair>& absorbList);

					CConceptRoleBranchingTrigger* createDisjunctionPartialAbsorbedBranchTriggers(CConcept* orConcept, bool negated);
					CConceptRoleBranchingTrigger* createDisjunctionPartialAbsorbedBranchTriggers(CConcept* orConcept, bool negated, const QList<TConceptNegationPair>& absorbList);


					bool createConceptOfInterestAbsorption(CConcept* intConcept, bool negated, bool continuationExtension = false);
					bool createDirectConceptOfInterestAbsorption(CConcept* intConcept, bool negated, bool continuationExtension = false);
					bool createPropagationConceptOfInterestAbsorption(CConcept* intConcept, bool negated, bool continuationExtension = false);
					CConcept* getConceptOfInterestTrigger(CConcept* intConcept, bool negated, CConcept* backPropTriggerCon);
					CConcept* getConceptOfInterestCandidate(CConcept* intConcept, bool negated, CConcept* backPropTriggerCon);
					CConceptTriggerLinker* createBackpropPartialExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon);
					CConceptTriggerLinker* createBackpropExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon);
					CConcept* createDisjunctionAbsorbedBackpropagationTriggeredImplication(CConcept* orConcept, bool negated, CConcept* backPropTriggerCon);
					CConcept* createDisjunctionAbsorbedBackpropagationTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& partialAbsorbList, QList<TConceptNegationPair>& candList, CConcept* backPropTriggerCon);
					CConcept* createPartialBackpropagationExtendedAbsorbedImpliedTriggerCocnept(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon);
					CConceptTriggerLinker* createPartialBackpropagationExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList, CConcept* backPropTriggerCon);


					CConcept* createPartialExtendedAbsorbedTriggeredImplication(CConcept* impliedConcept, bool impliedNegation, const QList<TConceptNegationPair>& absorbList);
					CConcept* createPartialExtendedAbsorbedImpliedTriggerCocnept(const QList<TConceptNegationPair>& absorbList);
					CConceptTriggerLinker* createPartialExtendedAbsorbedImpliedTrigger(const QList<TConceptNegationPair>& absorbList);

					bool addAbsorbableDisjunctCandidates(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& list);
					QList<TConceptNegationPair> splitAbsorbableDisjuncts(QList<TConceptNegationPair>& list);
					QList<TConceptNegationPair> getPartialAbsorbableDisjuncts(QList<TConceptNegationPair>& list);


					QString generateDebugGCIConceptString(CConcept* concept, bool conceptNegation);
					QString generateDebugGeneratedAbsorbedImplicationString(cint64 minNewConceptTag, cint64 lastNewConceptTag);


					void optimizeTriggerStructure();
					void replaceTrigger(CConcept* baseConcept, CConcept* replaceConcept, bool negated);
					void replaceOneTrigger(CConcept* baseConcept, CConcept* replaceConcept, bool negated);




					bool absorbNominalConceptsToAssertions(CConcept* concept, bool negated);
					bool absorbNominalSchemaGCIConceptsToTriggeredImplications(CConcept* concept, bool negated);

					void createGCINominalSchemaAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList<TConceptNegationPair>& absorbList, QList<TConceptNegationPair>& partialAbsorbList, QList<TConceptNegationPair>& candList, CNominalSchemaTemplate* nsTemplate);

					CNominalSchemaAbsorptionPathLinker* getAbsorbedNominalSchemaPaths(const QSet<TConceptNegationPair>& conceptSet, CNominalSchemaAbsorptionPathLinker* absorptionPathLinker, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate);
					void collectNominalSchemaRelatedConcepts(CConcept* concept, bool negated, QSet<TConceptNegationPair>& conceptSet, CNominalSchemaTemplate* nsTemplate);
					void sortNominalSchemaRelatedConcepts(QSet<TConceptNegationPair>& conceptSet, CNominalSchemaTemplate* nsTemplate, QList<TConceptNegationPair>& nomSchConList, QList<TConceptNegationPair>& otherConList, QList<TConceptNegationPair>& orConList);

					CConcept* createBindingImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept);
					CConcept* createBindingCycleCloseConcept(CConcept* cycleCloseTriggerConcept, CConcept* impliedTriggerConcept);
					CConcept* createBindingTriggerConcept();
					CConcept* createBindingVariableConcept(CConcept* followingConcept, CVariable* variable);
					
					CConcept* createBindingToSuccessorPropagationConcept(CConcept* followingConcept, CRole* role);
					CConcept* createBindingFromSuccessorPropagationConcept(CConcept* followingConcept, CRole* role);


					CConcept* createBindingGroundingConcept(CNominalSchemaTemplate* nsTemplate, cint64 groundOpCode);
					void createBindingGroundConcept(CConcept*& groundingConcept, bool& groundingConNegation, const QList<TConceptNegationPair>& groundConceptList, CNominalSchemaTemplate* nsTemplate, cint64 groundOpCode);

					CNominalSchemaAbsorptionPathLinker* duplicateNominalSchemaAbsorptionPaths(CNominalSchemaAbsorptionPathLinker* abPathLinker, cint64 duplicateCount);




					CNominalSchemaAbsorptionBranchLinker* getAbsorbedJoinedNominalSchemaBranchLinker(CConcept* absorbingConcept, bool absorbingNegation, CConcept* backPropConTrigger, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate);
					CNominalSchemaAbsorptionBranchLinker* getAbsorbedJoinedNominalSchemaBranchLinker(const QSet<TConceptNegationPair>& conceptSet, CConcept* backPropConTrigger, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate);
					CNominalSchemaAbsorptionBranchLinker* getAbsorbedNominalSchemaBranchLinker(CConcept* absorbingConcept, bool absorbingNegation, CConcept* backPropConTrigger, QHash<CConcept*,CVariable*>* nomSchConceptVariableHash, CNominalSchemaTemplate* nsTemplate);

					CConcept* createBindingBackPropagationActivationTriggerConcept();
					CConcept* createBindingBackPropagationTriggerConcept();
					CConcept* createBindingBackPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept);
					bool addBindingBackPropagationTriggeredToImplicationConcept(CConcept* implicationConcept, CConcept* triggeredConcept);
					CConcept* createBindingBackPropagationToSuccessorConcept(CConcept* followingConcept, CRole* role);
					
					CConcept* createVariableBindingPropagationFromSuccessorConcept(CConcept* followingConcept, CRole* role);
					CConcept* createVariableBindingPropagationTriggerConcept();
					CConcept* createVariableBindingPropagationJoiningConcept(CConcept* joinConcept1, CConcept* joinConcept2, CConcept* impliedConcept);
					CConcept* createVariableBindingPropagationBindVariableConcept(CConcept* followingConcept, CVariable* variable);
					CConcept* createVariableBindingPropagationImplicationTriggeredConcept(CConcept* triggeredConcept, CConcept* impliedConcept);

					void createJoiningConceptVariableList(CConcept* joiningConcept, CNominalSchemaAbsorptionBranchLinker* abLinker1, CNominalSchemaAbsorptionBranchLinker* abLinker2);


					void addConceptOperand(CConcept* concept, CConcept* operandConcept, bool operandNegation);
					bool hasNominalSchemaAbsorbableConceptFillHash(CConcept* concept, bool negation, CBOXHASH<CConcept*,CConcept*>* absorbableConceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet);
					bool hasNominalSchemaConceptFillHash(CConcept* concept, CBOXHASH<CConcept*,CConcept*>* conceptNominalSchemaConceptHash, CConcept* nominalSchemaConcept, QSet<CConcept*>* testedConceptSet);
					void collectNominalSchemaConcepts(CConcept* concept, CBOXSET<CConcept*>* nominalSchemaConceptSet);



					bool isConceptPropagationTriggerable(CConcept* concept, bool negated);
					CConceptTriggerLinker* getPropagationTriggersForConcept(CConcept* concept, bool negated, TConceptNegPair& intConceptNegationPair, bool completelyAbsorbable);
					CConceptTriggerLinker* getPropagationTriggersForConceptOperandList(CSortedNegLinker<CConcept*>* opConLinker, bool operandsNegation, bool chooseInsteadOfNegation, CConcept* concept, bool negated, TConceptNegPair& intConceptNegationPair, bool completelyAbsorbable);


				// protected variables
				protected:
					class CAbsorpEquivalentClassItem {
						public:
							CAbsorpEquivalentClassItem(CConcept* eqConcept, CSortedNegLinker<CConcept*>* absorpOpLinker) {
								mEqConcept = eqConcept;
								mAbsorpOpLinker = absorpOpLinker;
							}
							CConcept* mEqConcept;
							CSortedNegLinker<CConcept*>* mAbsorpOpLinker;
					};


					CMemoryAllocationManager* mMemMan;
					CMBox* mMBox;
					CTBox* mTBox;

					QHash<TConceptPair,CConceptTriggerLinker*> mConceptImplicationImpliedHash;
					QHash<TConceptNegationPair,CConceptTriggerLinker*> mConceptTriggerLinkerHash;
					CConceptTriggerLinker* mTmpTriggerLinker;

					QHash<TConceptNegationPair, CConcept*> mBackPropActivationConHash;

					QSet<CConcept*> mCreatedImpliedDisjunctions;


					QList<CConceptTriggerLinker*> mTriggerContainer;
					QList<CSortedNegLinker<CConcept*>*> mLaterTopConceptTriggerAddList;


					QHash<TConceptNegationPair,CConcept*> mConceptTriggerHash;
					QHash<CConcept*,cint64> mTriggerCountHash;
					CConceptVector* mConceptVec;
					CConcept* mBottomConcept;
					CConcept* mTopConcept;
					CConcreteOntology* mOnto;
					CBranchingTriggerVector* mBranchTriggVec;
					CNominalSchemaTemplateVector* mNomSchemaTemplVec;

					CBOXHASH<CConcept*,CConcept*>* mTriggerImplHash;

					QHash<TConceptNegationPair,bool> mConceptTotalAbsorbableHash;
					QHash<TConceptNegationPair,bool> mConceptPartialAbsorbableHash;
					QSet<CConcept*> mEQNotConceptPartialAbsorbableSet;
					QHash<TConceptNegationPair, bool> mConceptPropagationAbsorbableHash;


					CBOXHASH<CConcept*,CConcept*>* mEquivConCandidateHash;
					CBOXHASH<CConcept*,CConcept*>* mCandidateEquivConHash;
					CBOXSET<CConcept*>* mEquivConNonCandidateSet;


					QHash<CRole*,CConcept*>* mRoleDomainTriggerConceptHash;

					QHash<TConceptNegationPair,CNominalSchemaAbsorptionBranchLinker*> mConceptNomSchAbsorpBranchLinkerHash;
					QHash<TAbsorptionBranchPair,CNominalSchemaAbsorptionBranchLinker*> mAbsorptionBranchPairAbsorptionBranchHash;

					QHash<CRole*,CRole*> mRoleInverseRoleCreateHash;
					CBOXHASH<cint64,CConcept*>* mIndividualTriggerConceptHash;

					QSet<TConceptNegationPair> mPositiveConceptReferencedSet;
					CImplicationReplacementVector* mImpRepVec = nullptr;

					cint64 mNextVariableID;

					bool mConfRequiresExistingInverseRole;
					bool mConfAbsorbEqClassDefinitions;
					bool mConfPartialAbsorbEqClassDefinitions;
					bool mConfPartialGCIAbsorption;
					bool mConfAbsorbDisjunctions;
					bool mConfAbsorbSimpleDisjunctions;
					bool mConfPartialDisjunctionsAbsorption;
					bool mConfOnlyPositiveDisjunctionsAbsorption;


					bool mConfCardinalityQualificationTriggerAbsorption;
					bool mConfPartialCardinalityQualificationTriggerAbsorption;
					bool mConfOnlyPositiveCardinalityQualificationTriggerAbsorption;


					bool mConfNominalSchemaPathPreabsorption;

					bool mConfVariableMultipleTriggeredBackwardChainingAbsorption;

					bool mConfNominalSchemaBackPropagation;
					bool mConfNominalSchemaBackPropagationMultiTrigger;
					bool mConfImplicationsForTriggerReusue;
					bool mConfPunishOftenSameTriggerUse;
					cint64 mConfOftenTriggerUseCountPunishment;

					bool mConfDatatypeAbsorption;


					class CTriggCount {
						public:
							inline CTriggCount() {
								mCount = 0;
							}
							inline void incCount(cint64 count = 1) {
								mCount += count;
							}
							cint64 mCount;
					};

					QHash<CConcept*,CTriggCount> mTriggCountHash;

					class CBackPropTrigHashData {
						public:
							inline CBackPropTrigHashData(CConcept* activationTriggerConcept, CConcept* implicationConcept) {
								mActivationTriggerConcept = activationTriggerConcept;
								mImplicationConcept = implicationConcept;
							}
							CConcept* mActivationTriggerConcept;
							CConcept* mImplicationConcept;
					};


					QHash<CConcept*,CBackPropTrigHashData*> mVarBackPropTrigHash;
					QList<CBackPropTrigHashData*> mVarBackPropTrigContainer;


					cint64 mStatInvRolesGenerated;
					cint64 mStatGCIAbsorbed;
					cint64 mStatGCINormalAbsorbed;
					cint64 mStatGCIPartialAbsorbed;
					cint64 mStatGCIAbsorptionPartialExtended;
					cint64 mStatGCITotal;
					cint64 mStatEqAbsorbed;
					cint64 mStatEqTotal;
					cint64 mStatImplConceptsGenerated;
					cint64 mStatReusedImplications;
					cint64 mStatOptimizedRemovedTriggers;
					cint64 mStatGCINormalSchemaAbsorbed;
					cint64 mStatGCIAssertionAbsorbed;

					QString mDebugGCIString;
					QString mDebugAbsorbedString;

					QString mDebugLastGCIString;
					QString mDebugLastAbsorbedGCIConceptString;
					QString mDebugLastAbsorbedGCITechniqueString;
					QString mDebugLastAbsorbedGCIString;
					QString mDebugTotalAbsorbedGCIString;
					cint64 mLastNewConceptTag;
					cint64 mFirstNewConceptTag;

					cint64 mLastDisjunctId;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CTRIGGEREDIMPLICATIONBINARYABSORBERPREPROCESS_H
