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

#ifndef KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETRESULTVISITORGENERATOR_H
#define KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETRESULTVISITORGENERATOR_H

// Libraries includes


// Namespace includes
#include "CIndividualSynsetsResult.h"

// Other includes
#include "Reasoner/Realization/CRealization.h"
#include "Reasoner/Realization/CConceptRealizationInstanceVisitor.h"
#include "Reasoner/Realization/CConceptRealizationIndividualVisitor.h"
#include "Reasoner/Realization/CSameRealizationIndividualVisitor.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CIndividualSynsetResultVisitorGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualSynsetResultVisitorGenerator : public CConceptRealizationIndividualVisitor, public CSameRealizationIndividualVisitor {
				// public methods
				public:
					//! Constructor
					CIndividualSynsetResultVisitorGenerator(CIndividualSynsetResult* indiSynsetResult, bool abbreviatedIRIs, bool integrateAnonymousIndividuals, CIndividualNameResolver* indiNameResolver);

					virtual bool visitIndividual(const CIndividualReference& indiRef, CConceptRealization* conRealization);
					virtual bool visitIndividual(const CIndividualReference& indiRef, CSameRealization* sameRealization);

				// protected methods
				protected:

				// protected variables
				protected:
					CIndividualNameResolver* mIndiNameResolver;
					CIndividualSynsetResult* mIndividualSynsetResult;
					bool mAbbreviatedIRIs;
					bool mIntegrateAnonymousIndividuals;


				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CINDIVIDUALSYNSETRESULTVISITORGENERATOR_H
