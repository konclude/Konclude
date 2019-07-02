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

#ifndef KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATION_H
#define KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATION_H

// Libraries includes


// Namespace includes
#include "RealizationSettings.h"
#include "CConceptRealizationInstanceVisitor.h"
#include "CConceptRealizationInstantiatedVisitor.h"
#include "CConceptRealizationConceptVisitor.h"
#include "CConceptRealizationIndividualVisitor.h"
#include "CPossibleAssertionsCollectionSet.h"
#include "CRealizationIndividualInstanceItemReference.h"
#include "CRealizationIndividualInstanceItemReferenceIterator.h"
#include "CRealizationIndividualSorting.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Realization {

			/*! 
			 *
			 *		\class		CConceptRealization
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptRealization {
				// public methods
				public:
					//! Constructor
					CConceptRealization();


					virtual CRealizationIndividualInstanceItemReference getInstanceItemReference(CIndividual* individual);
					virtual bool isConceptInstance(CIndividual* individual, CConcept* concept);
					virtual bool visitSameIndividuals(CIndividual* individual, CConceptRealizationIndividualVisitor* visitor);
					virtual bool visitDirectTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitAllTypes(CIndividual* individual, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitTypes(CIndividual* individual, bool direct, CConceptRealizationInstantiatedVisitor* visitor);


					virtual bool visitDirectInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitAllInstances(CConcept* concept, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitInstances(CConcept* concept, bool direct, CConceptRealizationInstanceVisitor* visitor);


					virtual bool visitSameIndividuals(const CIndividualReference& indiRef, CConceptRealizationIndividualVisitor* visitor);
					virtual bool visitDirectTypes(const CIndividualReference& indiRef, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitAllTypes(const CIndividualReference& indiRef, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool visitTypes(const CIndividualReference& indiRef, bool direct, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool isConceptInstance(const CIndividualReference& indiRef, CConcept* concept);

					virtual bool visitDirectInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) = 0;
					virtual bool visitAllInstances(CConceptInstantiatedItem* item, CConceptRealizationInstanceVisitor* visitor) = 0;
					virtual bool visitInstances(CConceptInstantiatedItem* item, bool direct, CConceptRealizationInstanceVisitor* visitor);
					virtual bool visitDirectTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationInstantiatedVisitor* visitor) = 0;
					virtual bool visitAllTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationInstantiatedVisitor* visitor) = 0;
					virtual bool visitTypes(const CRealizationIndividualInstanceItemReference& indiRealItemRef, bool direct, CConceptRealizationInstantiatedVisitor* visitor);
					virtual bool isConceptInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConcept* concept) = 0;

					virtual bool visitIndividuals(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealizationIndividualVisitor* visitor) = 0;
					virtual bool visitConcepts(CConceptInstantiatedItem* item, CConceptRealizationConceptVisitor* visitor) = 0;



					virtual CSameInstanceItem* getSameInstanceItem(const CRealizationIndividualInstanceItemReference& indiRealItemRef) = 0;

					virtual CConceptInstantiatedItem* getInstantiatedItem(CConcept* concept) = 0;
					virtual CRealizationIndividualInstanceItemReference getInstanceItemReference(const CIndividualReference& indiRef) = 0;


					virtual CPossibleAssertionsCollectionSet* getPossibleAssertionCollectionSet() = 0;


					virtual CRealizationIndividualInstanceItemReferenceIterator* getConceptInstancesIterator(CConcept* concept, bool direct, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getIterationIntersectionIterator(QList<CRealizationIndividualInstanceItemReferenceIterator*> takeIteratorList) = 0;
					virtual CRealizationIndividualInstanceItemReferenceIterator* getIterationCandidateIndividualIdIterator(const QMap<cint64, IndividualIdCandidateData>& candidateIndiIdsMap, const CRealizationIndividualSorting& sorting = CRealizationIndividualSorting(), const CRealizationIndividualInstanceItemReference& indiInstItemRefCursor = CRealizationIndividualInstanceItemReference(), bool moveOverCursor = true) = 0;


				// protected methods
				protected:

				// protected variables
				protected:

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CCONCEPTREALIZATION_H
