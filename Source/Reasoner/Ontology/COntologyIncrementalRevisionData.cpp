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

#include "COntologyIncrementalRevisionData.h"


namespace Konclude {

	namespace Reasoner {

		namespace Ontology {


			COntologyIncrementalRevisionData::COntologyIncrementalRevisionData(COntologyContext* ontoContext) {
				mOntoContext = ontoContext;	
				mBasementOntology = nullptr;
				mPreviousVersionOntology = nullptr;
				mLastChangeCountedAxiom = 0;
				mPreviousConsistentOntology = nullptr;
				mPreviousClassesClassifiedOntology = nullptr;
				mPreviousSameIndividualsRealizedOntology = nullptr;
				mPreviousClassTypesRealizedOntology = nullptr;
				mPreviousRoleTypesRealizedOntology = nullptr;
			}



			COntologyIncrementalRevisionData::~COntologyIncrementalRevisionData() {
			}


			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::referenceIncrementalRevision(COntologyIncrementalRevisionData* incRevData) {
				mBasementOntology = incRevData->mBasementOntology;
				mPreviousVersionOntology = incRevData->mPreviousVersionOntology;
				mLastChangeCountedAxiom = incRevData->mLastChangeCountedAxiom;
				mPreviousConsistentOntology = incRevData->mPreviousConsistentOntology;
				mPreviousClassesClassifiedOntology = incRevData->mPreviousClassesClassifiedOntology;
				mPreviousSameIndividualsRealizedOntology = incRevData->mPreviousSameIndividualsRealizedOntology;
				mPreviousClassTypesRealizedOntology = incRevData->mPreviousClassTypesRealizedOntology;
				mPreviousRoleTypesRealizedOntology = incRevData->mPreviousRoleTypesRealizedOntology;
				return this;
			}


			CConcreteOntology* COntologyIncrementalRevisionData::getBasementOntology() {
				return mBasementOntology;
			}

			CConcreteOntology* COntologyIncrementalRevisionData::getPreviousOntologyVersion() {
				return mPreviousVersionOntology;
			}


			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setBasementOntology(CConcreteOntology* ontology) {
				mBasementOntology = ontology;
				return this;
			}

			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setPreviousOntologyVersion(CConcreteOntology* ontology) {
				mPreviousVersionOntology = ontology;
				return this;
			}

			COntologyIncrementalAxiomChangeData* COntologyIncrementalRevisionData::getAxiomChangeData() {
				return &mAxiomChangeData;
			}

			cint64 COntologyIncrementalRevisionData::getLastChangeCountedAxiom() {
				return mLastChangeCountedAxiom;
			}

			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setLastChangeCountedAxiom(cint64 lastAxiom) {
				mLastChangeCountedAxiom = lastAxiom;
				return this;
			}


			bool COntologyIncrementalRevisionData::isBasementOntology() {
				return !mPreviousVersionOntology;
			}

			bool COntologyIncrementalRevisionData::isInitialOntology() {
				return mPreviousVersionOntology == mBasementOntology;
			}

			bool COntologyIncrementalRevisionData::isIncrementalOntology() {
				return !isInitialOntology() && !isBasementOntology();
			}


			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setPreviousConsistentOntology(CConcreteOntology* ontology) {
				mPreviousConsistentOntology = ontology;
				return this;
			}

			CConcreteOntology* COntologyIncrementalRevisionData::getPreviousConsistentOntology() {
				return mPreviousConsistentOntology;				
			}



			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setPreviousClassesClassifiedOntology(CConcreteOntology* ontology) {
				mPreviousClassesClassifiedOntology = ontology;
				return this;
			}

			CConcreteOntology* COntologyIncrementalRevisionData::getPreviousClassesClassifiedOntology() {
				return mPreviousClassesClassifiedOntology;
			}

			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setPreviousSameIndividualsRealizedOntology(CConcreteOntology* ontology) {
				mPreviousSameIndividualsRealizedOntology = ontology;
				return this;
			}

			CConcreteOntology* COntologyIncrementalRevisionData::getPreviousSameIndividualsRealizedOntology() {
				return mPreviousSameIndividualsRealizedOntology;
			}


			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setPreviousClassTypesRealizedOntology(CConcreteOntology* ontology) {
				mPreviousClassTypesRealizedOntology = ontology;
				return this;
			}

			CConcreteOntology* COntologyIncrementalRevisionData::getPreviousClassTypesRealizedOntology() {
				return mPreviousClassTypesRealizedOntology;
			}


			COntologyIncrementalRevisionData* COntologyIncrementalRevisionData::setPreviousRoleTypesRealizedOntology(CConcreteOntology* ontology) {
				mPreviousRoleTypesRealizedOntology = ontology;
				return this;
			}

			CConcreteOntology* COntologyIncrementalRevisionData::getPreviousRoleTypesRealizedOntology() {
				return mPreviousRoleTypesRealizedOntology;
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude
