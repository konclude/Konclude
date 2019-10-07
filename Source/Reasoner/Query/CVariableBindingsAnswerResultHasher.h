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

#ifndef KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERRESULTHASHER_H
#define KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERRESULTHASHER_H

// Libraries includes
#include "CVariableBindingsAnswerResult.h"

// Namespace includes

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CVariableBindingsAnswerResultHasher
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CVariableBindingsAnswerResultHasher {
				// public methods
				public:
					//! Constructor
					CVariableBindingsAnswerResultHasher(CVariableBindingsAnswerResult* bindingResult);

					bool operator==(const CVariableBindingsAnswerResultHasher& hasher) const;

					cint64 getHashValue() const;
					CVariableBindingsAnswerResult* getBindingResult() const;

				// protected methods
				protected:

				// protected variables
				protected:
					CVariableBindingsAnswerResult* mBindingResult;
					cint64 mHashValue;

				// private methods
				private:

				// private variables
				private:

			};
			

			inline uint qHash(const CVariableBindingsAnswerResultHasher& hasher) {
				qint64 key = (qint64)hasher.getHashValue();
				if (sizeof(quint64) > sizeof(uint)) {
					return uint((key >> (8 * sizeof(uint) - 1)) ^ key);
				}
				else {
					return uint(key);
				}
			}

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CVARIABLEBINDINGSANSWERRESULTHASHER_H
