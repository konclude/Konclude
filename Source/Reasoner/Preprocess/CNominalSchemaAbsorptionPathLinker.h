/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_PREPROCESS_CNOMINALSCHEMAABSORPTIONPATHLINKER_H
#define KONCLUDE_REASONER_PREPROCESS_CNOMINALSCHEMAABSORPTIONPATHLINKER_H

// Libraries includes


// Namespace includes
#include "Reasoner/Ontology/CConcept.h"

// Other includes
#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Utilities::Container;

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CNominalSchemaAbsorptionPathLinker
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CNominalSchemaAbsorptionPathLinker : public CLinkerBase<CNominalSchemaAbsorptionPathLinker*,CNominalSchemaAbsorptionPathLinker> {
				// public methods
				public:
					//! Constructor
					CNominalSchemaAbsorptionPathLinker();

					CNominalSchemaAbsorptionPathLinker* initNominalSchemaAbsorptionPathLinker(CNominalSchemaAbsorptionPathLinker* nomSchAbsorpPathLinker);

					QHash<CConcept*,CConcept*>* getNominalSchemaBindedConceptTriggerHash();
					QList<CConcept*>* getAncestorConceptTriggerList();
					QList<CVariable*>* getVariableList();
					CVariable* getLastVariable();
					bool hasVariables();

					CConcept* getLastPathTriggerConcept();
					CNominalSchemaAbsorptionPathLinker* setLastPathTriggerConcept(CConcept* lastPathTriggerConcept);
					
					CNominalSchemaAbsorptionPathLinker* addVariable(CVariable* variable);

				// protected methods
				protected:

				// private methods
				private:
					QHash<CConcept*,CConcept*> mNomSchBindConceptHash;
					QList<CConcept*> mAncestorList;
					QList<CVariable*> mVariableList;
					CConcept* mLastPathConcept;

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CNOMINALSCHEMAABSORPTIONPATHLINKER_H
