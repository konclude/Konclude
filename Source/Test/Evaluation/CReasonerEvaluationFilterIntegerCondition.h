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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTERINTEGERCONDITION_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTERINTEGERCONDITION_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationFilterCondition.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationFilterIntegerCondition
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationFilterIntegerCondition : public CReasonerEvaluationFilterCondition {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationFilterIntegerCondition(const QString& statisticNameString, const QString& statisticComperator, const QString& statisticCompareValueString);
					CReasonerEvaluationFilterIntegerCondition(const QString& statisticNameString, const QString& statisticComperator, cint64 statisticCompareValue);


					virtual bool isStatisticConditionSatisfied(const QString& statisticString, const QString& statisticValueString);
						

				// protected methods
				protected:

				// protected variables
				protected:
					cint64 mStatisticCompareValue;
					QString mStatisticComperator;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTERINTEGERCONDITION_H
