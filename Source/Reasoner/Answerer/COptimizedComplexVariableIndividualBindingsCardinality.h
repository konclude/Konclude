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

#ifndef KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGSCARDINALITY_H
#define KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGSCARDINALITY_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"



// Other includes
#include "Reasoner/Realization/CRealizationIndividualInstanceItemReference.h"

#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;

	namespace Reasoner {

		using namespace Realization;

		namespace Answerer {


			/*! 
			 *
			 *		\class		COptimizedComplexVariableIndividualBindingsCardinality
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COptimizedComplexVariableIndividualBindingsCardinality {
				// public methods
				public:
					//! Constructor
					COptimizedComplexVariableIndividualBindingsCardinality();
					COptimizedComplexVariableIndividualBindingsCardinality(cint64 sameIndividualsSeparatlyConsideredCardinality, cint64 sameIndividualsJointlyConsideredCardinality, cint64 cardUpId = 0);

					COptimizedComplexVariableIndividualBindingsCardinality* initCardinalities(cint64 sameIndividualsSeparatlyConsideredCardinality, cint64 sameIndividualsJointlyConsideredCardinality, cint64 cardUpId = 0);
					COptimizedComplexVariableIndividualBindingsCardinality* addCardinalities(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities);
					COptimizedComplexVariableIndividualBindingsCardinality* substractCardinalities(COptimizedComplexVariableIndividualBindingsCardinality* cardinalities);


					COptimizedComplexVariableIndividualBindingsCardinality* setSameIndividualsSeparatlyConsideredCardinality(cint64 cardinality);
					COptimizedComplexVariableIndividualBindingsCardinality* setSameIndividualsJointlyConsideredCardinality(cint64 cardinality);


					cint64 getSameIndividualsSeparatlyConsideredCardinality() const;
					cint64 getSameIndividualsJointlyConsideredCardinality() const;


					cint64 getCardinalityUpdateId();
					COptimizedComplexVariableIndividualBindingsCardinality* setCardinalityUpdateId(cint64 cardUpId);

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mSameIndividualsSeparatlyConsideredCardinality;
					cint64 mSameIndividualsJointlyConsideredCardinality;

					cint64 mCardinalityUpdateId;




				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_COPTIMIZEDCOMPLEXVARIABLEINDIVIDUALBINDINGSCARDINALITY_H
