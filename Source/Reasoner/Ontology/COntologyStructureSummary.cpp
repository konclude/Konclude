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

#include "COntologyStructureSummary.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyStructureSummary::COntologyStructureSummary(COntologyContext* ontologyContext) {
				clear();
			}


			COntologyStructureSummary::~COntologyStructureSummary() {
			}


			COntologyStructureSummary *COntologyStructureSummary::clear() {
				mConClassCount = 0;
				mConCount = 0;
				mRoleObPropCount = 0;
				mDataPropCount = 0;
				mConGCICount = 0;

				mConIntersectionCount = 0;
				mConUnionCount = 0;
				mConNegCount = 0;
				mConSomeCount = 0;
				mConOnlyCount = 0;
				mConAtmostCount = 0;
				mConAtleastCount = 0;
				mConQualAtmostCount = 0;
				mConQualAtleastCount = 0;
				mConNonqualAtmostCount = 0;
				mConNonqualAtleastCount = 0;
				mConSubclassCount = 0;
				mConEquivalenceCount = 0;

				mConPrimCount = 0;
				mConCompCount = 0;
				mPrimClassCount = 0;
				mCompClassCount = 0;


				mConFuncAtmostCount = 0;
				mConValueCount = 0;
				mConNominalIndividualCount = 0;
				mConSelfCount = 0;
				mConAutChooceCount = 0;
				mConAutStateCount = 0;
				mConAutTransCount = 0;


				mRoleInvCount = 0;
				mRoleEqCount = 0;
				mRoleDisjCount = 0;
				mRoleRangeConCount = 0;
				mRoleDomainConCount = 0;
				mRoleAsymmetricCount = 0;
				mRoleSymmetricCount = 0;
				mRoleFuncCount = 0;
				mRoleInvFuncCount = 0;
				mRoleTransCount = 0;
				mRoleRefCount = 0;
				mRoleIrrefCount = 0;

				mNegDisOccurrence = false;
				mCardOccurrence = false;
				mExistOccurrence = false;
				mQualCardOccurrence = false;
				mFunctionalityOccurrence = false;
				mTransitivityOccurrence = false;
				mRoleHierarchyOccurrence = false;
				mComplexSubRoleOccurrence = false;
				mUniversalRoleOccurrence = false;
				mInverseOccurrence = false;
				mNominalIndividualOccurrence = false;
				mNominalOccurrence = false;
				mGroundingOccurrence = false;
				mDatatypeOccurrence = false;

				mExpressStringCalc = false;
				mAllClassesDeterministic = false;
				mAllClassesEL = false;

				return this;
			}


			COntologyStructureSummary* COntologyStructureSummary::referenceStructureSummary(COntologyStructureSummary* structureSummary) {
				clear();
				mAllClassesDeterministic = structureSummary->mAllClassesDeterministic;
				mAllClassesEL = structureSummary->mAllClassesEL;

				mConClassCount = structureSummary->mConClassCount;
				mConCount = structureSummary->mConCount;
				mRoleObPropCount = structureSummary->mRoleObPropCount;
				mDataPropCount = structureSummary->mDataPropCount;
				mConGCICount = structureSummary->mConGCICount;

				mConIntersectionCount = structureSummary->mConIntersectionCount;
				mConUnionCount = structureSummary->mConUnionCount;
				mConNegCount = structureSummary->mConNegCount;
				mConSomeCount = structureSummary->mConSomeCount;
				mConOnlyCount = structureSummary->mConOnlyCount;
				mConAtmostCount = structureSummary->mConAtmostCount;
				mConAtleastCount = structureSummary->mConAtleastCount;
				mConQualAtmostCount = structureSummary->mConQualAtmostCount;
				mConQualAtleastCount = structureSummary->mConQualAtleastCount;
				mConNonqualAtmostCount = structureSummary->mConNonqualAtmostCount;
				mConNonqualAtleastCount = structureSummary->mConNonqualAtleastCount;
				mConSubclassCount = structureSummary->mConSubclassCount;
				mConEquivalenceCount = structureSummary->mConEquivalenceCount;

				mConPrimCount = structureSummary->mConPrimCount;
				mConCompCount = structureSummary->mConCompCount;
				mPrimClassCount = structureSummary->mPrimClassCount;
				mCompClassCount = structureSummary->mCompClassCount;


				mConFuncAtmostCount = structureSummary->mConFuncAtmostCount;
				mConValueCount = structureSummary->mConValueCount;
				mConNominalIndividualCount = structureSummary->mConNominalIndividualCount;
				mConSelfCount = structureSummary->mConSelfCount;
				mConAutChooceCount = structureSummary->mConAutChooceCount;
				mConAutStateCount = structureSummary->mConAutStateCount;
				mConAutTransCount = structureSummary->mConAutTransCount;


				mRoleInvCount = structureSummary->mRoleInvCount;
				mRoleEqCount = structureSummary->mRoleEqCount;
				mRoleDisjCount = structureSummary->mRoleDisjCount;
				mRoleRangeConCount = structureSummary->mRoleRangeConCount;
				mRoleDomainConCount = structureSummary->mRoleDomainConCount;
				mRoleAsymmetricCount = structureSummary->mRoleAsymmetricCount;
				mRoleSymmetricCount = structureSummary->mRoleSymmetricCount;
				mRoleFuncCount = structureSummary->mRoleFuncCount;
				mRoleInvFuncCount = structureSummary->mRoleInvFuncCount;
				mRoleTransCount = structureSummary->mRoleTransCount;
				mRoleRefCount = structureSummary->mRoleRefCount;
				mRoleIrrefCount = structureSummary->mRoleIrrefCount;

				mNegDisOccurrence = structureSummary->mNegDisOccurrence;
				mCardOccurrence = structureSummary->mCardOccurrence;
				mExistOccurrence = structureSummary->mExistOccurrence;
				mQualCardOccurrence = structureSummary->mQualCardOccurrence;
				mFunctionalityOccurrence = structureSummary->mFunctionalityOccurrence;
				mTransitivityOccurrence = structureSummary->mTransitivityOccurrence;
				mRoleHierarchyOccurrence = structureSummary->mRoleHierarchyOccurrence;
				mComplexSubRoleOccurrence = structureSummary->mComplexSubRoleOccurrence;
				mUniversalRoleOccurrence = structureSummary->mUniversalRoleOccurrence;
				mInverseOccurrence = structureSummary->mInverseOccurrence;
				mNominalIndividualOccurrence = structureSummary->mNominalIndividualOccurrence;
				mNominalOccurrence = structureSummary->mNominalOccurrence;
				mGroundingOccurrence = structureSummary->mGroundingOccurrence;
				mDatatypeOccurrence = structureSummary->mDatatypeOccurrence;

				return this;
			}



			bool COntologyStructureSummary::hasOnlyDeterministicConceptClasses() {
				return mAllClassesDeterministic;
			}

			COntologyStructureSummary* COntologyStructureSummary::setOnlyDeterministicConceptClasses(bool allClassesDeterministic) {
				mAllClassesDeterministic = allClassesDeterministic;
				return this;
			}

			bool COntologyStructureSummary::hasOnlyELConceptClasses() {
				return mAllClassesEL;
			}

			COntologyStructureSummary* COntologyStructureSummary::setOnlyELConceptClasses(bool allClassesEL) {
				mAllClassesEL = allClassesEL;
				return this;
			}



			QString COntologyStructureSummary::getExpressivenessString() {
				if (!mExpressStringCalc) {
					calculateExpressiveness();
				}
				return expressString;
			}


			COntologyStructureSummary *COntologyStructureSummary::calculateExpressiveness() {
				QString appendString;
				expressString.clear();
				expressString = "AL";
				if (hasNegationDisjunctionsOccurrence()) {
					expressString = "ALC";
				} else if (hasExistensialOccurrence()) {
					expressString = "ALE";
				}
				if (hasTransitivityOccurrence()) {
					appendString = "+";
				}
				if (expressString == "ALC" && appendString.contains("+")) {
					expressString = "S";
					appendString = appendString.replace("+","");
				}

				if (hasComplexSubRoleOccurrence()) {
					expressString = "SR";
				} else if (hasRoleHierarchyOccurrence()) {
					expressString += "H";
				}

				if (hasNominalOccurrence()) {
					expressString += "O";
				}
				if (hasInverseOccurrence()) {
					expressString += "I";
				}

				if (hasQualifiedCardinalityOccurrence()) {
					expressString += "Q";
				} else if (hasCardinalityOccurrence()) {
					expressString += "N";
				} else if (hasFunctionalityOccurrence()) {
					expressString += "F";
				}

				if (hasGroundingOccurrence()) {
					expressString += "V";
				}

				if (hasDatatypeOccurrence()) {
					expressString += "(D)";
				}

				expressString += appendString;

				mExpressStringCalc = true;
				return this;
			}


			COntologyStructureSummary *COntologyStructureSummary::incClassCount(qint64 incCount) {
				mConClassCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptCount(qint64 incCount) {
				mConCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incObjectPropertyCount(qint64 incCount) {
				mRoleObPropCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incDataPropertyCount(qint64 incCount) {
				mDataPropCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incGCICount(qint64 incCount) {
				mConGCICount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptIntersectionCount(qint64 incCount) {
				mConIntersectionCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptUnionCount(qint64 incCount) {
				mConUnionCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptNegCount(qint64 incCount) {
				mConNegCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptSomeCount(qint64 incCount) {
				mConSomeCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptOnlyCount(qint64 incCount) {
				mConOnlyCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptAtmostCount(qint64 incCount) {
				mConAtmostCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptAtleastCount(qint64 incCount) {
				mConAtleastCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptQualifiedAtleastCount(qint64 incCount) {
				mConQualAtleastCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptQualifiedAtmostCount(qint64 incCount) {
				mConQualAtmostCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptNotQualifiedAtleastCount(qint64 incCount) {
				mConNonqualAtleastCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptNotQualifiedAtmostCount(qint64 incCount) {
				mConNonqualAtmostCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptEquivalenceCount(qint64 incCount) {
				mConEquivalenceCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptSubclassCount(qint64 incCount) {
				mConSubclassCount += incCount;
				return this;
			}


			COntologyStructureSummary *COntologyStructureSummary::incPrimitiveConceptCount(qint64 incCount) {
				mConPrimCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incComplexConceptCount(qint64 incCount) {
				mConCompCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incAtomicClassCount(qint64 incCount) {
				mPrimClassCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incComplexClassCount(qint64 incCount) {
				mCompClassCount += incCount;
				return this;
			}


			COntologyStructureSummary *COntologyStructureSummary::incRoleInverseCount(qint64 incCount) {
				mRoleInvCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleEquivalenceCount(qint64 incCount) {
				mRoleEqCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleDisjointCount(qint64 incCount) {
				mRoleDisjCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleRangeConceptCount(qint64 incCount) {
				mRoleRangeConCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleDomainConceptCount(qint64 incCount) {
				mRoleDomainConCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleAsymmetricCount(qint64 incCount) {
				mRoleAsymmetricCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleSymmetricCount(qint64 incCount) {
				mRoleSymmetricCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleFunctionalCount(qint64 incCount) {
				mRoleFuncCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleInverseFunctionalCount(qint64 incCount) {
				mRoleInvFuncCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleTransetivCount(qint64 incCount) {
				mRoleTransCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleReflexivityCount(qint64 incCount) {
				mRoleRefCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incRoleIrreflexivityCount(qint64 incCount) {
				mRoleIrrefCount += incCount;
				return this;
			}



			COntologyStructureSummary *COntologyStructureSummary::incConceptFunctionalAtmostCount(qint64 incCount) {
				mConFuncAtmostCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptValueCount(qint64 incCount) {
				mConValueCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptNominalIndividualCount(qint64 incCount) {
				mConNominalIndividualCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptSelfCount(qint64 incCount) {
				mConSelfCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptAutomateChooseCount(qint64 incCount) {
				mConAutChooceCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptAutomateStateCount(qint64 incCount) {
				mConAutStateCount += incCount;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::incConceptAutomateTransitiveCount(qint64 incCount) {
				mConAutTransCount += incCount;
				return this;
			}

			

			qint64 COntologyStructureSummary::getClassCount() {
				return mConClassCount;
			}

			qint64 COntologyStructureSummary::getConceptCount() {
				return mConCount;
			}

			qint64 COntologyStructureSummary::getObjectPropertyCount() {
				return mRoleObPropCount;
			}

			qint64 COntologyStructureSummary::getDataPropertyCount() {
				return mDataPropCount;
			}

			qint64 COntologyStructureSummary::getGCICount() {
				return mConGCICount;
			}


			qint64 COntologyStructureSummary::getConceptIntersectionCount() {
				return mConIntersectionCount;
			}

			qint64 COntologyStructureSummary::getConceptUnionCount() {
				return mConUnionCount;
			}

			qint64 COntologyStructureSummary::getConceptNotCount() {
				return mConNegCount;
			}

			qint64 COntologyStructureSummary::getConceptSomeCount() {
				return mConSomeCount;
			}

			qint64 COntologyStructureSummary::getConceptOnlyCount() {
				return mConOnlyCount;
			}

			qint64 COntologyStructureSummary::getConceptAtmostCount() {
				return mConAtmostCount;
			}

			qint64 COntologyStructureSummary::getConceptAtleastCount() {
				return mConAtleastCount;
			}

			qint64 COntologyStructureSummary::getConceptQualifiedAtleastCount() {
				return mConQualAtleastCount;
			}

			qint64 COntologyStructureSummary::getConceptQualifiedAtmostCount() {
				return mConQualAtmostCount;
			}

			qint64 COntologyStructureSummary::getConceptNotQualifiedAtleastCount() {
				return mConNonqualAtleastCount;
			}

			qint64 COntologyStructureSummary::getConceptNotQualifiedAtmostCount() {
				return mConNonqualAtmostCount;
			}

			qint64 COntologyStructureSummary::getConceptEquivalenceCount() {
				return mConEquivalenceCount;
			}

			qint64 COntologyStructureSummary::getConceptSubclassCount() {
				return mConSubclassCount;
			}


			bool COntologyStructureSummary::hasConceptIntersections() {
				return mConIntersectionCount > 0;
			}

			bool COntologyStructureSummary::hasConceptUnions() {
				return mConUnionCount > 0;
			}

			bool COntologyStructureSummary::hasConceptNots() {
				return mConNegCount > 0;
			}

			bool COntologyStructureSummary::hasConceptSomes() {
				return mConSomeCount > 0;
			}

			bool COntologyStructureSummary::hasConceptOnlys() {
				return mConOnlyCount > 0;
			}

			bool COntologyStructureSummary::hasConceptAtmosts() {
				return mConAtmostCount > 0;
			}

			bool COntologyStructureSummary::hasConceptAtleasts() {
				return mConAtleastCount > 0;
			}

			bool COntologyStructureSummary::hasConceptQualifiedAtleasts() {
				return mConQualAtleastCount > 0;
			}

			bool COntologyStructureSummary::hasConceptQualifiedAtmosts() {
				return mConQualAtmostCount > 0;
			}

			bool COntologyStructureSummary::hasConceptNotQualifiedAtleasts() {
				return mConNonqualAtleastCount > 0;
			}

			bool COntologyStructureSummary::hasConceptNotQualifiedAtmosts() {
				return mConNonqualAtmostCount > 0;
			}

			bool COntologyStructureSummary::hasConceptEquivalences() {
				return mConEquivalenceCount > 0;
			}

			bool COntologyStructureSummary::hasConceptSubclasses() {
				return mConSubclassCount > 0;
			}


			qint64 COntologyStructureSummary::getPrimitiveConceptCount() {
				return mConPrimCount;
			}

			qint64 COntologyStructureSummary::getComplexConceptCount() {
				return mConCompCount;
			}

			qint64 COntologyStructureSummary::getAtomicClassCount() {
				return mPrimClassCount;
			}

			qint64 COntologyStructureSummary::getComplexClassCount() {
				return mCompClassCount;
			}



			qint64 COntologyStructureSummary::getRoleInverseCount() {
				return mRoleInvCount;
			}

			qint64 COntologyStructureSummary::getRoleEquivalenceCount() {
				return mRoleEqCount;
			}

			qint64 COntologyStructureSummary::getRoleDisjointCount() {
				return mRoleDisjCount;
			}

			qint64 COntologyStructureSummary::getRoleRangeConceptCount() {
				return mRoleRangeConCount;
			}

			qint64 COntologyStructureSummary::getRoleDomainConceptCount() {
				return mRoleDomainConCount;
			}

			qint64 COntologyStructureSummary::getRoleAsymmetricCount() {
				return mRoleAsymmetricCount;
			}

			qint64 COntologyStructureSummary::getRoleSymmetricCount() {
				return mRoleSymmetricCount;
			}

			qint64 COntologyStructureSummary::getRoleFunctionalCount() {
				return mRoleFuncCount;
			}

			qint64 COntologyStructureSummary::getRoleInverseFunctionalCount() {
				return mRoleInvFuncCount;
			}

			qint64 COntologyStructureSummary::getRoleTransetivCount() {
				return mRoleTransCount;
			}

			qint64 COntologyStructureSummary::getRoleReflexivityCount() {
				return mRoleRefCount;
			}

			qint64 COntologyStructureSummary::getRoleIrreflexivityCount() {
				return mRoleIrrefCount;
			}


			qint64 COntologyStructureSummary::getConceptFunctionalAtmostCount() {
				return mConFuncAtmostCount;
			}

			qint64 COntologyStructureSummary::getConceptValueCount() {
				return mConValueCount;
			}

			qint64 COntologyStructureSummary::getConceptIndividualCount() {
				return mConNominalIndividualCount;
			}

			qint64 COntologyStructureSummary::getConceptSelfCount() {
				return mConSelfCount;
			}

			qint64 COntologyStructureSummary::getConceptAutomateChooseCount() {
				return mConAutChooceCount;
			}

			qint64 COntologyStructureSummary::getConceptAutomateStateCount() {
				return mConAutStateCount;
			}

			qint64 COntologyStructureSummary::getConceptAutomateTransitiveCount() {
				return mConAutTransCount;
			}



			bool COntologyStructureSummary::hasRoleInverses() {
				return mRoleInvCount;
			}

			bool COntologyStructureSummary::hasRoleEquivalences() {
				return mRoleEqCount;
			}

			bool COntologyStructureSummary::hasRoleDisjoints() {
				return mRoleDisjCount;
			}

			bool COntologyStructureSummary::hasRoleRangeConcepts() {
				return mRoleRangeConCount;
			}

			bool COntologyStructureSummary::hasRoleDomainConcepts() {
				return mRoleDomainConCount;
			}

			bool COntologyStructureSummary::hasRoleAsymmetrics() {
				return mRoleAsymmetricCount;
			}

			bool COntologyStructureSummary::hasRoleSymmetrics() {
				return mRoleSymmetricCount;
			}

			bool COntologyStructureSummary::hasRoleFunctionals() {
				return mRoleFuncCount;
			}

			bool COntologyStructureSummary::hasRoleInverseFunctionals() {
				return mRoleInvFuncCount;
			}

			bool COntologyStructureSummary::hasRoleTransetivs() {
				return mRoleTransCount;
			}

			bool COntologyStructureSummary::hasRoleReflexivitys() {
				return mRoleRefCount;
			}

			bool COntologyStructureSummary::hasRoleIrreflexivitys() {
				return mRoleIrrefCount;
			}


			bool COntologyStructureSummary::hasConceptFunctionalAtmosts() {
				return mConFuncAtmostCount;
			}

			bool COntologyStructureSummary::hasConceptValues() {
				return mConValueCount;
			}

			bool COntologyStructureSummary::hasConceptNominalIndividual() {
				return mConNominalIndividualCount;
			}

			bool COntologyStructureSummary::hasConceptSelfs() {
				return mConSelfCount;
			}

			bool COntologyStructureSummary::hasConceptAutomateChooses() {
				return mConAutChooceCount;
			}

			bool COntologyStructureSummary::hasConceptAutomateStates() {
				return mConAutStateCount;
			}

			bool COntologyStructureSummary::hasConceptAutomateTransitives() {
				return mConAutTransCount;
			}

			COntologyStructureSummary *COntologyStructureSummary::setNegationDisjunctionsOccurrence(bool occured) {
				mNegDisOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setCardinalityOccurrence(bool occured) {
				mCardOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setExistensialOccurrence(bool occured) {
				mExistOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setQualifiedCardinalityOccurrence(bool occured) {
				mQualCardOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setFunctionalityOccurrence(bool occured) {
				mFunctionalityOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setTransitivityOccurrence(bool occured) {
				mTransitivityOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setRoleHierarchyOccurrence(bool occured) {
				mRoleHierarchyOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setComplexSubRoleOccurrence(bool occured) {
				mComplexSubRoleOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setUniversalRoleOccurrence(bool occured) {
				mUniversalRoleOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setInverseOccurrence(bool occured) {
				mInverseOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setNominalIndividualOccurrence(bool occured) {
				mNominalIndividualOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setNominalOccurrence(bool occured) {
				mNominalOccurrence = occured;
				return this;
			}

			COntologyStructureSummary *COntologyStructureSummary::setGroundingOccurrence(bool occured) {
				mGroundingOccurrence = occured;
				return this;
			}


			COntologyStructureSummary *COntologyStructureSummary::setDatatypeOccurrence(bool occured) {
				mDatatypeOccurrence = occured;
				return this;
			}

			bool COntologyStructureSummary::hasNegationDisjunctionsOccurrence() {
				return mNegDisOccurrence;
			}

			bool COntologyStructureSummary::hasCardinalityOccurrence() {
				return mCardOccurrence;
			}

			bool COntologyStructureSummary::hasExistensialOccurrence() {
				return mExistOccurrence;
			}

			bool COntologyStructureSummary::hasQualifiedCardinalityOccurrence() {
				return mQualCardOccurrence;
			}

			bool COntologyStructureSummary::hasFunctionalityOccurrence() {
				return mFunctionalityOccurrence;
			}

			bool COntologyStructureSummary::hasTransitivityOccurrence() {
				return mTransitivityOccurrence;
			}

			bool COntologyStructureSummary::hasRoleHierarchyOccurrence() {
				return mRoleHierarchyOccurrence;
			}

			bool COntologyStructureSummary::hasComplexSubRoleOccurrence() {
				return mComplexSubRoleOccurrence;
			}

			bool COntologyStructureSummary::hasUniversalRoleOccurrence() {
				return mUniversalRoleOccurrence;
			}

			bool COntologyStructureSummary::hasInverseOccurrence() {
				return mInverseOccurrence;
			}

			bool COntologyStructureSummary::hasNominalIndividualOccurrence() {
				return mNominalIndividualOccurrence;
			}

			bool COntologyStructureSummary::hasNominalOccurrence() {
				return mNominalOccurrence;
			}

			bool COntologyStructureSummary::hasGroundingOccurrence() {
				return mGroundingOccurrence;
			}


			bool COntologyStructureSummary::hasDatatypeOccurrence() {
				return mDatatypeOccurrence;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
