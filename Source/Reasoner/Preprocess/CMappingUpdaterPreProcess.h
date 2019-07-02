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

#ifndef KONCLUDE_REASONER_PREPROCESS_CMAPPINGUPDATERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CMAPPINGUPDATERPREPROCESS_H

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

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CMappingUpdaterPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CMappingUpdaterPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CMappingUpdaterPreProcess();

					//! Destructor
					virtual ~CMappingUpdaterPreProcess();


					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:
					bool collectActiveOntologyItemsFromActiveEntities();
					bool collectActiveOntologyItems();

				// private methods
				private:


				// private variables
				private:
					CTBox* tbox;
					CABox* abox;
					CRBox* rbox;
					CMBox* mBox;

					CRoleVector *mRoleVec;
					CConceptVector *mConceptVec;

					COntologyBuildConstructFlags* mBuildConstructFlags;
					CActiveEntityCountVector* mActiveEntityVector;

					QSet<CConcept*> mUpdateConceptSet;
					QList<CConcept*> mUpdateConceptList;
					QSet<CRole*> mUpdateRoleSet;
					QList<CRole*> mUpdateRoleList;
					QSet<CIndividual*> mUpdateIndividualSet;
					QList<CIndividual*> mUpdateIndividualList;

					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* mRoleObjPropTermHash;
					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* mObjPropTermRoleHash;

					CBUILDHASH<CRole*,CDataPropertyTermExpression*>* mRoleDataPropTermHash;
					CBUILDHASH<CDataPropertyTermExpression*,CRole*>* mDataPropTermRoleHash;

					CBUILDHASH<CClassTermExpression*,CConcept*>* mClassTermConceptHash;
					CBUILDHASH<CConcept*,CClassTermExpression*>* mConceptClassTermHash;


					CBUILDHASH<CIndividualTermExpression*,CIndividual*>* mIndiTermIndiHash;
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* mIndiIndiTermHash;


					CBUILDSET<CDeclarationAxiomExpression*>* mDeclarationAxiomSet;

					CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* mClassTermClassAxiomHash;
					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* mObjPropTermObjPropAxiomHash;

					CMAPPINGHASH<CStringRefStringHasher,CConcept*>* mNameConceptHash;

			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CMAPPINGUPDATERPREPROCESS_H
