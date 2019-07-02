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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUALREFERENCE_H
#define KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUALREFERENCE_H

// Libraries includes

// Namespace includes
#include "OntologySettings.h"
#include "CIndividualIdentifier.h"
#include "CIndividual.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Ontology {

			/*! 
			 *
			 *		\class		CIndividualReference
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CIndividualReference {
				// public methods
				public:
					//! Constructor
					CIndividualReference();
					CIndividualReference(cint64 indiID);
					CIndividualReference(const CIndividualIdentifier& identifier);
					CIndividualReference(CIndividual* individual);
					CIndividualReference(CIndividual* individual, cint64 indiId);

					cint64 getIndividualID() const;
					CIndividualIdentifier getIndividualIdentifier() const;
					CIndividual* getIndividual() const;

					bool isEmpty() const;
					bool isNonEmpty() const;

					bool operator==(const CIndividualReference& indiRef) const {
						return getIndividualID() == indiRef.getIndividualID();
					}

				// protected methods
				protected:

				// protected variables
				protected:
					CIndividualIdentifier mIdentifier;
					CIndividual* mIndividual;

				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const CIndividualReference& indiRef) {
				qint64 key = (qint64)indiRef.getIndividualID();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CINDIVIDUALREFERENCE_H
