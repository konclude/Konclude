/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOBSQUERY_H
#define KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOBSQUERY_H

// Libraries includes
#include <QString>


// Namespace includes
#include "QuerySettings.h"
#include "CJobProvidedQuery.h"
#include "CSatisfiableCalculationJob.h"
#include "CBooleanQueryResult.h"
#include "CSatisfiableCalculationJobQueryData.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Query {

			/*! 
			 *
			 *		\class		CSatisfiableCalculationJobsQuery
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSatisfiableCalculationJobsQuery : public CJobProvidedQuery {
				// public methods
				public:
					//! Constructor
					CSatisfiableCalculationJobsQuery();

					//! Destructor
					virtual ~CSatisfiableCalculationJobsQuery();

					virtual QString getQueryName();
					virtual QString getQueryString();
					virtual QString getAnswerString();
					virtual bool hasAnswer();
					virtual bool hasError();
					virtual bool canCancelRemainingJobs();

					virtual CQueryResult* getQueryResult();
					virtual CQUERYLIST<CCalculationJob*> getJobList();
					virtual CQUERYLIST<CSatisfiableCalculationJob*> getSatisfiableCalculationJobList();

					CSatisfiableCalculationJobsQuery* setQueryName(const QString& queryName);
					CSatisfiableCalculationJobsQuery* setQueryString(const QString& text);
					CSatisfiableCalculationJobsQuery* setAnswerString(const QString& positiveText, const QString& negativeText, const QString& noAnswerText);

					CSatisfiableCalculationJobsQuery* addTestSatisfiableCalculationJob(CSatisfiableCalculationJob* satCalculationJob, bool mustBeSatisfiableForPositive = true);
					virtual bool interpreteJobSatisfiableCalculation(CSatisfiableCalculationJob* satCalculationJob, bool satisfiable, bool calculationError = false, QString* newAnserString = nullptr);

					CSatisfiableCalculationJobsQuery* setQueryStatistics(CQueryStatistics* stats);
					virtual CQueryStatistics* getQueryStatistics();

					virtual CQuery* addErrorString(const QString& errorString);

					bool getResult(bool queryPositive = true);

				// protected methods
				protected:

				// protected variables
				protected:
					CQUERYLIST<CSatisfiableCalculationJob*> mJobDataList;

					cint64 mJobTotalCount;
					cint64 mJobCalcedCount;
					bool mJobCalcedNegative;
					bool mJobCalculationError;
					bool mQueryError;

					QString mQueryString;
					QString mQueryName;
					QString mPositiveAnswerString;
					QString mNegativeAnswerString;
					QString mNoAnswerString;
					QString mErrorString;

					CBooleanQueryResult* mQueryResult;

					CQueryStatistics* mQueryStats;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSATISFIABLECALCULATIONJOBSQUERY_H
