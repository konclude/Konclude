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

#ifndef KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETSRESULTVISITORGENERATOR_H
#define KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETSRESULTVISITORGENERATOR_H

// Libraries includes


// Namespace includes
#include "CIndividualSynsetsResult.h"

// Other includes
#include "Reasoner/Realization/CRealization.h"
#include "Reasoner/Realization/CConceptRealizationInstanceVisitor.h"
#include "Reasoner/Realization/CConceptRealizationIndividualVisitor.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CIndividualSynsetsResultVisitorGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualSynsetsResultVisitorGenerator : public CConceptRealizationInstanceVisitor, public CConceptRealizationIndividualVisitor, public CRoleRealizationInstanceVisitor, public CRoleRealizationIndividualVisitor {
				// public methods
				public:
					//! Constructor
					CIndividualSynsetsResultVisitorGenerator(CIndividualSynsetsResult* classSynsetsResult, bool abbreviatedIRIs, bool integrateAnonymousIndividuals, CIndividualNameResolver* indiNameResolver);

					virtual bool visitInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CConceptRealization* conRealization);
					virtual bool visitRoleInstance(const CRealizationIndividualInstanceItemReference& indiRealItemRef, CRoleRealization* roleRealization);
					virtual bool visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization);
					virtual bool visitIndividual(const CIndividualReference& indiRef, CRoleRealization* roleRealization);

				// protected methods
				protected:

				// protected variables
				protected:
					CIndividualNameResolver* mIndiNameResolver;
					CIndividualSynsetsResult* mIndividualSynsetsResult;
					bool mAbbreviatedIRIs;
					bool mIntegrateAnonymousIndividuals;

					CIndividualSynsetResult* mTmpIndividualSynsetResult;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETSRESULTVISITORGENERATOR_H
