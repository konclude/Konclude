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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELROLEDATA_H
#define KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELROLEDATA_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"
#include "CClassificationClassPseudoModelDeterministicFlag.h"

// Other includes
#include "Utilities/UtilitiesSettings.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationClassPseudoModelRoleData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationClassPseudoModelRoleData : public CClassificationClassPseudoModelDeterministicFlag {
				// public methods
				public:
					//! Constructor
					CClassificationClassPseudoModelRoleData();


					cint64 getLowerAtLeastBound();
					cint64 getUpperAtLeastBound();

					cint64 getLowerAtMostBound();
					cint64 getUpperAtMostBound();

					cint64 getSuccessorModelID() const;

					bool setLowerAtLeastBound(cint64 bound);
					bool setUpperAtLeastBound(cint64 bound);

					bool setLowerAtMostBound(cint64 bound);
					bool setUpperAtMostBound(cint64 bound);

					bool setSuccessorModelID(cint64 modelID);

					bool isPossibleSubsumerOf(const CClassificationClassPseudoModelRoleData& possibleSubsumedData) const;

				// protected methods
				protected:

				// protected variables
				protected:
					// lower at-least bound, the maximum deterministically instanced at-least concept
					cint64 mLowerAtLeast;
					// upper at-least bound, the number of successor nodes
					cint64 mUpperAtLeast;

					// upper at-most bound, the minimal deterministically instanced at-most concept
					cint64 mUpperAtMost;
					// lower at-most bound, the number of successor nodes
					cint64 mLowerAtMost;

					cint64 mSuccessorModel;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CCLASSIFICATIONCLASSPSEUDOMODELROLEDATA_H
