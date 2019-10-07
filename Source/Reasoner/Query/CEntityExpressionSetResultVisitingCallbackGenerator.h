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

#ifndef KONCLUDE_REASONER_QUERY_CENTITYEXPRESSIONSETRESULTVISITINGCALLBACKGENERATOR_H
#define KONCLUDE_REASONER_QUERY_CENTITYEXPRESSIONSETRESULTVISITINGCALLBACKGENERATOR_H

// Libraries includes


// Namespace includes
#include "CEntityExpressionSetResultVisitingCallback.h"

// Other includes
#include "Reasoner/Realization/CRealization.h"
#include "Reasoner/Realization/CConceptRealizationInstantiatedVisitor.h"
#include "Reasoner/Realization/CConceptRealizationConceptVisitor.h"
#include "Reasoner/Realization/CConceptRealizationInstanceVisitor.h"
#include "Reasoner/Realization/CConceptRealizationIndividualVisitor.h"
#include "Reasoner/Realization/CRoleRealizationInstanceVisitor.h"
#include "Reasoner/Realization/CRoleRealizationIndividualVisitor.h"
#include "Reasoner/Realization/CSameRealizationIndividualVisitor.h"
#include "Reasoner/Realization/CRoleRealizationInstantiatedVisitor.h"
#include "Reasoner/Realization/CRoleRealizationRoleVisitor.h"

#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CEntityExpressionSetResultVisitingCallbackGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CEntityExpressionSetResultVisitingCallbackGenerator : public CConceptRealizationInstantiatedVisitor, public CConceptRealizationConceptVisitor, public CConceptRealizationInstanceVisitor, public CConceptRealizationIndividualVisitor, public CRoleRealizationInstanceVisitor, public CRoleRealizationIndividualVisitor, public CSameRealizationIndividualVisitor, public CRoleRealizationInstantiatedVisitor, public CRoleRealizationRoleVisitor {
				// public methods
				public:
					//! Constructor
					CEntityExpressionSetResultVisitingCallbackGenerator(CEntityExpressionSetResultVisitingCallback* visitingCallback, CConcreteOntology* ontology);

					virtual bool visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization);
					virtual bool visitConcept(CConcept* concept, CConceptRealization* conRealization);
					
					virtual bool visitRoleInstantiated(CRoleInstantiatedItem* item, CRoleRealization* roleRealization);
					virtual bool visitRole(CRole* concept, CRoleRealization* roleRealization);

					virtual bool visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization);
					virtual bool visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization);

					virtual bool visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization);
					virtual bool visitIndividual(const CIndividualReference& indiRef, CRoleRealization* roleRealization);
					virtual bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* conRealization);


				// protected methods
				protected:

				// protected variables
				protected:
					CEntityExpressionSetResultVisitingCallback* mVisitingCallback;
					CConcreteOntology* mOntology;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CENTITYEXPRESSIONSETRESULTVISITINGCALLBACKGENERATOR_H
