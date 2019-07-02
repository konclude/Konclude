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

#ifndef KONCLUDE_REASONER_QUERY_CClassSynsetsResultVisitorGenerator_H
#define KONCLUDE_REASONER_QUERY_CClassSynsetsResultVisitorGenerator_H

// Libraries includes


// Namespace includes
#include "CClassSynsetsResult.h"

// Other includes
#include "Reasoner/Realization/CRealization.h"
#include "Reasoner/Realization/CConceptRealizationInstantiatedVisitor.h"
#include "Reasoner/Realization/CConceptRealizationConceptVisitor.h"

#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CClassSynsetsResultVisitorGenerator
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassSynsetsResultVisitorGenerator : public CConceptRealizationInstantiatedVisitor, public CConceptRealizationConceptVisitor {
				// public methods
				public:
					//! Constructor
					CClassSynsetsResultVisitorGenerator(CClassSynsetsResult* classSynsetsResult, bool abbreviatedIRIs);

					virtual bool visitType(CConceptInstantiatedItem* item, CConceptRealization* conRealization);
					virtual bool visitConcept(CConcept* concept, CConceptRealization* conRealization);

				// protected methods
				protected:

				// protected variables
				protected:
					CClassSynsetsResult* mClassSynsetsResult;
					bool mAbbreviatedIRIs;

					CClassSynsetResult* mTmpClassSynsetResult;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CClassSynsetsResultVisitorGenerator_H
