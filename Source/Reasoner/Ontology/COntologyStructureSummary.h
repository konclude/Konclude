/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYSTRUCTURESUMMARY_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYSTRUCTURESUMMARY_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyStructureSummary
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyStructureSummary {
				// public methods
				public:
					//! Constructor
					COntologyStructureSummary(COntologyContext* ontologyContext = nullptr);

					//! Destructor
					virtual ~COntologyStructureSummary();


					COntologyStructureSummary* referenceStructureSummary(COntologyStructureSummary* structureSummary);


					COntologyStructureSummary *clear();


					bool hasOnlyDeterministicConceptClasses();
					COntologyStructureSummary* setOnlyDeterministicConceptClasses(bool allClassesDeterministic);

					bool hasOnlyELConceptClasses();
					COntologyStructureSummary* setOnlyELConceptClasses(bool allClassesEL);

					qint64 getClassCount();
					qint64 getConceptCount();
					qint64 getObjectPropertyCount();
					qint64 getDataPropertyCount();
					qint64 getGCICount();

					qint64 getConceptIntersectionCount();
					qint64 getConceptUnionCount();
					qint64 getConceptNotCount();
					qint64 getConceptSomeCount();
					qint64 getConceptOnlyCount();
					qint64 getConceptAtmostCount();
					qint64 getConceptAtleastCount();
					qint64 getConceptQualifiedAtleastCount();
					qint64 getConceptQualifiedAtmostCount();
					qint64 getConceptNotQualifiedAtleastCount();
					qint64 getConceptNotQualifiedAtmostCount();
					qint64 getConceptEquivalenceCount();
					qint64 getConceptSubclassCount();

					bool hasConceptIntersections();
					bool hasConceptUnions();
					bool hasConceptNots();
					bool hasConceptSomes();
					bool hasConceptOnlys();
					bool hasConceptAtmosts();
					bool hasConceptAtleasts();
					bool hasConceptQualifiedAtleasts();
					bool hasConceptQualifiedAtmosts();
					bool hasConceptNotQualifiedAtleasts();
					bool hasConceptNotQualifiedAtmosts();
					bool hasConceptEquivalences();
					bool hasConceptSubclasses();

					qint64 getPrimitiveConceptCount();
					qint64 getComplexConceptCount();
					qint64 getAtomicClassCount();
					qint64 getComplexClassCount();


					COntologyStructureSummary *incClassCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptCount(qint64 incCount = 1);
					COntologyStructureSummary *incObjectPropertyCount(qint64 incCount = 1);
					COntologyStructureSummary *incDataPropertyCount(qint64 incCount = 1);
					COntologyStructureSummary *incGCICount(qint64 incCount = 1);



					COntologyStructureSummary *incConceptIntersectionCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptUnionCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptNegCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptSomeCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptOnlyCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptAtmostCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptAtleastCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptQualifiedAtleastCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptQualifiedAtmostCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptNotQualifiedAtleastCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptNotQualifiedAtmostCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptEquivalenceCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptSubclassCount(qint64 incCount = 1);

					COntologyStructureSummary *incPrimitiveConceptCount(qint64 incCount = 1);
					COntologyStructureSummary *incComplexConceptCount(qint64 incCount = 1);
					COntologyStructureSummary *incAtomicClassCount(qint64 incCount = 1);
					COntologyStructureSummary *incComplexClassCount(qint64 incCount = 1);


					COntologyStructureSummary *incRoleInverseCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleEquivalenceCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleDisjointCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleRangeConceptCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleDomainConceptCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleAsymmetricCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleSymmetricCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleFunctionalCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleInverseFunctionalCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleTransetivCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleReflexivityCount(qint64 incCount = 1);
					COntologyStructureSummary *incRoleIrreflexivityCount(qint64 incCount = 1);

					COntologyStructureSummary *incConceptFunctionalAtmostCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptValueCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptNominalIndividualCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptSelfCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptAutomateChooseCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptAutomateStateCount(qint64 incCount = 1);
					COntologyStructureSummary *incConceptAutomateTransitiveCount(qint64 incCount = 1);


					qint64 getRoleInverseCount();
					qint64 getRoleEquivalenceCount();
					qint64 getRoleDisjointCount();
					qint64 getRoleRangeConceptCount();
					qint64 getRoleDomainConceptCount();
					qint64 getRoleAsymmetricCount();
					qint64 getRoleSymmetricCount();
					qint64 getRoleFunctionalCount();
					qint64 getRoleInverseFunctionalCount();
					qint64 getRoleTransetivCount();
					qint64 getRoleReflexivityCount();
					qint64 getRoleIrreflexivityCount();

					qint64 getConceptFunctionalAtmostCount();
					qint64 getConceptValueCount();
					qint64 getConceptIndividualCount();
					qint64 getConceptSelfCount();
					qint64 getConceptAutomateChooseCount();
					qint64 getConceptAutomateStateCount();
					qint64 getConceptAutomateTransitiveCount();



					bool hasRoleInverses();
					bool hasRoleEquivalences();
					bool hasRoleDisjoints();
					bool hasRoleRangeConcepts();
					bool hasRoleDomainConcepts();
					bool hasRoleAsymmetrics();
					bool hasRoleSymmetrics();
					bool hasRoleFunctionals();
					bool hasRoleInverseFunctionals();
					bool hasRoleTransetivs();
					bool hasRoleReflexivitys();
					bool hasRoleIrreflexivitys();

					bool hasConceptFunctionalAtmosts();
					bool hasConceptValues();
					bool hasConceptNominalIndividual();
					bool hasConceptSelfs();
					bool hasConceptAutomateChooses();
					bool hasConceptAutomateStates();
					bool hasConceptAutomateTransitives();


					COntologyStructureSummary *setNegationDisjunctionsOccurrence(bool occured = true);
					COntologyStructureSummary *setExistensialOccurrence(bool occured = true);
					COntologyStructureSummary *setCardinalityOccurrence(bool occured = true);
					COntologyStructureSummary *setQualifiedCardinalityOccurrence(bool occured = true);
					COntologyStructureSummary *setFunctionalityOccurrence(bool occured = true);
					COntologyStructureSummary *setTransitivityOccurrence(bool occured = true);
					COntologyStructureSummary *setRoleHierarchyOccurrence(bool occured = true);
					COntologyStructureSummary *setComplexSubRoleOccurrence(bool occured = true);
					COntologyStructureSummary *setUniversalRoleOccurrence(bool occured = true);
					COntologyStructureSummary *setInverseOccurrence(bool occured = true);
					COntologyStructureSummary *setNominalIndividualOccurrence(bool occured = true);
					COntologyStructureSummary *setNominalOccurrence(bool occured = true);
					COntologyStructureSummary *setGroundingOccurrence(bool occured = true);
					COntologyStructureSummary *setDatatypeOccurrence(bool occured = true);


					bool hasNegationDisjunctionsOccurrence();
					bool hasCardinalityOccurrence();
					bool hasExistensialOccurrence();
					bool hasQualifiedCardinalityOccurrence();
					bool hasFunctionalityOccurrence();
					bool hasTransitivityOccurrence();
					bool hasRoleHierarchyOccurrence();
					bool hasComplexSubRoleOccurrence();
					bool hasUniversalRoleOccurrence();
					bool hasInverseOccurrence();
					bool hasNominalIndividualOccurrence();
					bool hasNominalOccurrence();
					bool hasGroundingOccurrence();
					bool hasDatatypeOccurrence();


					QString getExpressivenessString();


				// protected methods
				protected:
					COntologyStructureSummary *calculateExpressiveness();

				// protected variables
				protected:

					bool mAllClassesDeterministic;
					bool mAllClassesEL;

					qint64 mConClassCount;
					qint64 mConCount;
					qint64 mRoleObPropCount;
					qint64 mDataPropCount;
					qint64 mConGCICount;

					qint64 mConIntersectionCount;
					qint64 mConUnionCount;
					qint64 mConNegCount;
					qint64 mConSomeCount;
					qint64 mConOnlyCount;
					qint64 mConAtmostCount;
					qint64 mConAtleastCount;
					qint64 mConQualAtleastCount;
					qint64 mConQualAtmostCount;
					qint64 mConNonqualAtleastCount;
					qint64 mConNonqualAtmostCount;
					qint64 mConSubclassCount;
					qint64 mConEquivalenceCount;


					qint64 mConPrimCount;
					qint64 mConCompCount;
					qint64 mPrimClassCount;
					qint64 mCompClassCount;


					qint64 mConFuncAtmostCount;
					qint64 mConValueCount;
					qint64 mConNominalIndividualCount;
					qint64 mConSelfCount;
					qint64 mConAutChooceCount;
					qint64 mConAutStateCount;
					qint64 mConAutTransCount;

					qint64 mRoleInvCount;
					qint64 mRoleEqCount;
					qint64 mRoleDisjCount;
					qint64 mRoleRangeConCount;
					qint64 mRoleDomainConCount;
					qint64 mRoleAsymmetricCount;
					qint64 mRoleSymmetricCount;
					qint64 mRoleFuncCount;
					qint64 mRoleInvFuncCount;
					qint64 mRoleTransCount;
					qint64 mRoleRefCount;
					qint64 mRoleIrrefCount;

					qint64 mIndiCount;



					bool mNegDisOccurrence;
					bool mExistOccurrence;
					bool mCardOccurrence;
					bool mQualCardOccurrence;
					bool mFunctionalityOccurrence;
					bool mTransitivityOccurrence;
					bool mRoleHierarchyOccurrence;
					bool mComplexSubRoleOccurrence;
					bool mUniversalRoleOccurrence;
					bool mInverseOccurrence;
					bool mNominalIndividualOccurrence;
					bool mNominalOccurrence;
					bool mGroundingOccurrence;
					bool mDatatypeOccurrence;


					QString expressString;
					bool mExpressStringCalc;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYSTRUCTURESUMMARY_H
