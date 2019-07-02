/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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

#ifndef KONCLUDE_REASONER_PREPROCESS_CCONCEPTUNAMBIGUOUSHASHER_H
#define KONCLUDE_REASONER_PREPROCESS_CCONCEPTUNAMBIGUOUSHASHER_H


// Libraries includes
#include <QHash>

// Namespace includes


// Other includes
#include "Reasoner/Ontology/CConcept.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Preprocess {


			/*! 
			 *
			 *		\class		CConceptUnambiguousHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptUnambiguousHasher  {
				// public methods
				public:
					//! Constructor
					CConceptUnambiguousHasher(CConcept* concept, bool tagRequired);

					cint64 getConceptHashValue() const;

					bool operator==(const CConceptUnambiguousHasher& expressionHasher) const;

				// protected methods
				protected:
					cint64 calculateHashValue();

				// protected variables
				protected:
					CConcept* mConcept;
					cint64 mHashValue;
					bool mTagRequired;

				// private methods
				private:

				// private variables
				private:

			};

			inline uint qHash(const CConceptUnambiguousHasher& expressionHasher) {
				qint64 key = (qint64)expressionHasher.getConceptHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_PREPROCESS_CCONCEPTUNAMBIGUOUSHASHER_H
