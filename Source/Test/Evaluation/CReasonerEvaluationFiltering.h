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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTERING_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTERING_H

// Libraries includes


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationFilter.h"


// Other includes
#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationFiltering
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationFiltering : public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationFiltering(const QString& statisticsBasePath);
					
					bool loadEmptyFilter();
					bool loadFilterFromFile(const QString& filterNameString);
					bool loadFiltersFromFile(const QStringList& filterNameStringList);

					bool testSatisfiesFilter(const QString& testFileNameString, const QString& testFilePathString, const QString& subDirPathString, const QStringList& statisticList);
					bool testSatisfiesFilter(const QStringList& statisticList);
					bool testSatisfiesFilter(const QString& testFileNameString, const QStringList& statisticList);


					QStringList loadStatistics(const QString& testFileNameString, const QString& subDirPathString);
					QStringList loadStatistics(const QString& subDirPathTestFileNameString);

				// protected methods
				protected:

				// protected variables
				protected:
					QList<CReasonerEvaluationFilter*> mFilterList;					
					QString mStatisticsBasePath;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONFILTERING_H
