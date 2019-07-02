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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCONCEPTNEGATIONOVERROLETUBLE_H
#define KONCLUDE_REASONER_CLASSIFIER_CCONCEPTNEGATIONOVERROLETUBLE_H

// Libraries includes
#include <QHash>

// Namespace includes
#include "ClassifierSettings.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CConceptNegationOverRoleTuble
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConceptNegationOverRoleTuble {
				// public methods
				public:
					//! Constructor
					CConceptNegationOverRoleTuble(qint64 conceptTag, bool conceptNegation, bool conceptOverRole);

					//! Destructor
					virtual ~CConceptNegationOverRoleTuble();

					qint64 getConceptTag() const;
					bool isConceptNegation() const;
					bool isConceptOverRole() const;

					bool operator==(const CConceptNegationOverRoleTuble& tuple) const;

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 conTag;
					bool conNeg;
					bool overRole;

				// private methods
				private:

				// private variables
				private:

			};


			inline uint qHash(const CConceptNegationOverRoleTuble& tuple) {
				qint64 key = tuple.getConceptTag();
				key += 17*tuple.isConceptNegation();
				key += 13*tuple.isConceptOverRole();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				} else {
					return uint(key);
				}
			}


		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCONCEPTNEGATIONOVERROLETUBLE_H
