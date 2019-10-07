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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTER_H

// Libraries includes
#include <QFile>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationFilterCondition.h"
#include "CReasonerEvaluationFilterStringCondition.h"
#include "CReasonerEvaluationFilterIntegerCondition.h"


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
			 *		\class		CReasonerEvaluationFilter
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationFilter {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationFilter();
					
					bool loadFilterConditionFile(const QString& filterNameString);

					bool testSatisfiesFilterConditions(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList);
					bool testSatisfiesFilterConditions(const QStringList& statisticList);
					bool testSatisfiesFilterConditions(const QString& testFileNameString, const QStringList& statisticList);

				// protected methods
				protected:

				// protected variables
				protected:
					QList<CReasonerEvaluationFilterCondition*> mCondList;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTER_H
