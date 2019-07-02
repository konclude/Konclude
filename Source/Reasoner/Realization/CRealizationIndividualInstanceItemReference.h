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

#ifndef KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALINSTANCEITEMREFERENCE_H
#define KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALINSTANCEITEMREFERENCE_H

// Libraries includes

// Namespace includes
#include "RealizationSettings.h"

// Other includes
#include "Reasoner/Realization/CSameInstanceItem.h"

#include "Reasoner/Ontology/CIndividualReference.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Realization {

			/*! 
			 *
			 *		\class		CRealizationIndividualInstanceItemReference
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CRealizationIndividualInstanceItemReference : public CIndividualReference {
				// public methods
				public:
					//! Constructor
					CRealizationIndividualInstanceItemReference();
					CRealizationIndividualInstanceItemReference(cint64 indiID, CSameInstanceItem* realizationInstanceItem);
					CRealizationIndividualInstanceItemReference(const CIndividualIdentifier& identifier, CSameInstanceItem* realizationInstanceItem);
					CRealizationIndividualInstanceItemReference(CIndividual* individual, CSameInstanceItem* realizationInstanceItem);
					CRealizationIndividualInstanceItemReference(CIndividual* individual, cint64 indiId, CSameInstanceItem* realizationInstanceItem);
					CRealizationIndividualInstanceItemReference(const CIndividualReference& indiRef, CSameInstanceItem* realizationInstanceItem);




					CSameInstanceItem* getRealizationInstanceItem() const;


					bool operator==(const CRealizationIndividualInstanceItemReference& indiRef) const {
						return getIndividualID() == indiRef.getIndividualID();
					}

				// protected methods
				protected:

				// protected variables
				protected:
					CSameInstanceItem* mRealizationInstanceItem;

				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const CRealizationIndividualInstanceItemReference& indiRef) {
				qint64 key = (qint64)indiRef.getIndividualID();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Realization

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_REALIZATION_CREALIZATIONINDIVIDUALINSTANCEITEMREFERENCE_H
