/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify it under
 *		the terms of version 2.1 of the GNU Lesser General Public License (LGPL2.1)
 *		as published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details, see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_PREPROCESS_CROLECHAINAUTOMATATRANSFORMATIONPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CROLECHAINAUTOMATATRANSFORMATIONPREPROCESS_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"
#include "CConceptRoleIndividualLocator.h"


// Other includes
#include "Reasoner/Ontology/CBoxContext.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRoleDependence.h"
#include "Reasoner/Ontology/CMBox.h"

#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CSortedNegLinker.hpp"

#include "Parser/Expressions/CInverseObjectPropertyOfExpression.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {


			typedef QPair<CRole*,bool> TRoleNegationPair;


			class CRoleSubRoleChainData {
				public:
					CRole* mRole;
					CRoleChain* mRoleChain;
					bool mInverse;

					CRoleSubRoleChainData(CRole* role = nullptr, CRoleChain* roleChain = nullptr, bool inverse = nullptr) {
						mRole = role;
						mRoleChain = roleChain;
						mInverse = inverse;
					}
			};

			class CRoleSubRoleChainDataItem {
				public:
					CRoleSubRoleChainDataItem(CRoleSubRoleChainData& roleChain) {
						mChainData = roleChain;
						mNegated = false;
						mAllowPropagated = true;
					}
					CRoleSubRoleChainDataItem(CRoleSubRoleChainData& roleChain, bool negated) {
						mChainData = roleChain;
						mNegated = negated;
						mAllowPropagated = false;
					}
				public:
					CRoleSubRoleChainData mChainData;
					bool mNegated;
					bool mAllowPropagated;
			};


			class CRecTravSubRoleChainDataItem {
				public:
					CRecTravSubRoleChainDataItem() {
						mRole = nullptr;
					}


				public:
					CRole* mRole;
					QList<CRoleSubRoleChainData> mDirectSubRoleChainDataList;
					QList<TRoleNegationPair> mRecTraversalSubRoleList;
			};


			/*! 
			 *
			 *		\class		CRoleChainAutomataTransformationPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRoleChainAutomataTransformationPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CRoleChainAutomataTransformationPreProcess();

					//! Destructor
					virtual ~CRoleChainAutomataTransformationPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:

				// private methods
				private:
					enum TRANSLATIONTYPE { TTNORMAL, TTIMPL, TTBRANCH, TTPROPBIND, TTBACKPROP, TTVARBIND };

					CRole* getLocalizedRole(CRole* role);
					CRole* getLocalizedRole(cint64 roleTag);
					bool hasInverseRole(CRole* role, bool searchInverseEquivalent = true);
					CRole* getInverseRole(CRole* role, bool searchInverseEquivalent = true);
					bool hasSuperRole(CRole* role, CRole* testingSuperRole, bool testInversed, bool superRoleInversedRequired);
					bool hasSuperRole(CRole* role, CRole* testingSuperRole, bool superRoleInversedRequired);
					bool hasInversedSuperRole(CRole* role, CRole* testingSuperRole);
					bool hasNonInversedSuperRole(CRole* role, CRole* testingSuperRole);
					bool hasInversedOrNonInversedSuperRole(CRole* role, CRole* testingSuperRole);

					CRoleChainAutomataTransformationPreProcess* convertAutomatConcept(CConcept* concept);

					CConcept* createAutomatGeneratingConcept(CSortedNegLinker<CConcept*>* opLinker, bool negate, CRole* role);

					CRoleChainAutomataTransformationPreProcess* collectSubRoleChains();
					CRoleChainAutomataTransformationPreProcess* createMissingInverseChainedRoles();
					CRoleChainAutomataTransformationPreProcess* createInverseRoleChainLinkers();
					CRoleChainAutomataTransformationPreProcess* createRecursiveTraversalData();

					
					QList<CRoleSubRoleChainData> getRelevantChainDataList(CRole* role, const QList<CRoleSubRoleChainData>& roleSubChainDataList);
					CRoleChainAutomataTransformationPreProcess* addRecursiveTraversalData(CRole* role, const QList<CRoleSubRoleChainData>& roleSubChainDataList);
					

					bool isChainDataImplicit(const CRoleSubRoleChainData& testingImplicitChainData, const CRoleSubRoleChainData& chainData);
					bool isChainLinkerImplicit(CXLinker<CRole*>* testingImplicitChainLinker, CXLinker<CRole*>* chainLinker, bool inversedTesting);
					bool isTransitiveChainData(const CRoleSubRoleChainData& chainData);

					bool isChainDataRecursiveTraversalCritical(CRole* role, const CRoleSubRoleChainData& chainData);
					bool collectRecursiveTraversalCriticalRoles(CRole* role, const CRoleSubRoleChainData& chainData, QHash<CRole*,bool>* criticalRoleNegationHash);
					QHash<CRole*,bool> getRelevantRecursiveTraversalCriticalRoles(QHash<CRole*,bool>* criticalRoleNegationHash);

					bool requiresRecursiveTraversalForRole(CRole* role, const CRoleSubRoleChainData& chainData, QHash<CRole*,bool>* criticalRoleNegationHash);


					bool generateRoleChainAutomatConcept(CRole* lastRole, const CRecTravSubRoleChainDataItem& recTravSubRoleChainDataItem, QSet<CRole*>& alreadyUnfoldRoleSet, CConcept* beginConcept, CConcept* endConcept, TRANSLATIONTYPE transType);
					bool generateRoleChainAutomatConcept(CRole* lastRole, const QList<CRoleSubRoleChainData>& subRoleChainDataList, QSet<CRole*>& alreadyUnfoldRoleSet, CConcept* beginConcept, CConcept* endConcept, TRANSLATIONTYPE transType);
					bool generateRoleChainAutomatConcept(CRole* lastRole, CRole* superRole, CRoleChain* descendingRoleChain, bool negatedChain, QSet<CRole*>& alreadyUnfoldRoleSet, CConcept* beginConcept, CConcept* endConcept, TRANSLATIONTYPE transType);
					CConcept* createTransitionConcept(CRole* role, TRANSLATIONTYPE transType);
					CSortedNegLinker<CConcept*>* createTransitionOperandConceptLinker(CConcept* operandConcept, bool negation);
					CConcept* appendTransitionOperandConceptLinker(CConcept* concept, CSortedNegLinker<CConcept*>* opLinker);
					CConcept* createStateConcept(TRANSLATIONTYPE transType);

					bool hasPropagationConcept(bool negated, CConcept* concept, CRole* role, CSortedNegLinker<CRole*>* roleList, bool inverseDomRange);
					bool hasPropagatedConcept(bool negated, CConcept* concept, CSortedNegLinker<CRole*>* roleList, bool inverseDomRange);
					CRoleChainAutomataTransformationPreProcess* createDomainRangePropagations();
					CRoleChainAutomataTransformationPreProcess* transformVALUERestrictions();


				// private variables
				private:
					CConcreteOntology* mOntology;

					CRoleVector *mRoleVec;
					CConceptVector *mConVec;
					cint64 mNextConceptTag;

					CMemoryAllocationManager* mMemMan;

					typedef QPair<CConcept*,bool> TConceptNegPair;


					QHash<CRole*,CRecTravSubRoleChainDataItem> mRoleRecTravSubRoleChainDataHash;


					QHash<CRole*,CRoleSubRoleChainData> mRoleSubRoleChainDataHash;
					QSet<CRoleChain*> mInverseUpdateRoleChainSet;

					QHash<TConceptNegPair,CConcept*> mDomConPropConceptHash;
					QHash<TConceptNegPair,CConcept*> mRanConPropConceptHash;



					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* mRoleObjPropTermHash;
					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* mObjPropTermRoleHash;
					CBUILDLIST<CBuildExpression*>* mExpressionBuildContainerList;
					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mInverseObjectPropertyHash;
					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mExpressionBuildHash;


					cint64 mStatAutomateStateConceptCount;
					cint64 mStatAutomateTransitionConceptCount;
					cint64 mStatAutomateTransformedConceptCount;
					cint64 mStatAutomateTransitiveSavedCount;

					cint64 mStatRangePropagationCount;
					cint64 mStatDomainPropagationCount;
					cint64 mStatCreatedRangePropagationCount;
					cint64 mStatCreatedDomainPropagationCount;

					cint64 mStatPropagationAlreadyInDomainRangeCount;
					cint64 mStatPropagatedAlreadyInDomainRangeCount;



					bool mConfSaveTransitiveTransitions;


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CROLECHAINAUTOMATATRANSFORMATIONPREPROCESS_H
