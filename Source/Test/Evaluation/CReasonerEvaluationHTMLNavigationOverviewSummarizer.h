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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONHTMLNAVIGATIONOVERVIEWSUMMARIZER_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONHTMLNAVIGATIONOVERVIEWSUMMARIZER_H

// Libraries includes
#include <QDir>

// Namespace includes
#include "EvaluationSettings.h"


// Other includes
#include "Config/CConfiguration.h"
#include "CKoncludeInfo.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationHTMLNavigationOverviewSummarizer
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationHTMLNavigationOverviewSummarizer {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationHTMLNavigationOverviewSummarizer();

					//! Destructor
					virtual ~CReasonerEvaluationHTMLNavigationOverviewSummarizer();

				
					
					CReasonerEvaluationHTMLNavigationOverviewSummarizer* createHTMLNavigationOverview(const QString& directory, const QString& evaluationProgramName, const QStringList& reasonerNameList, const QStringList& filterNameList, cint64 testTimeout, cint64 testCount);


				// protected methods
				protected:
					bool loadConditionedFiltersFromFile(const QString& fileString, QHash<QString,QStringList>* filterConditionedHash);
					bool loadFiltersFromFile(const QString& fileString, QSet<QString>* filterSet);
					QString loadTemplateFromFile(const QString& fileString);

					QString createHTMLForDirectory(const QString& directory, const QString& relativePathString, const QString& groupFilterString);


					bool isAcceptedByFilters(const QString& name);

				// protected variables
				protected:
					CReasonerEvaluationAnalyseContext* mContext;

					QString mDirLinConString;
					QString mDirLinOveString;
					QString mDirLinSubString;
					QString mDirLinSecString;


					QHash<QString,QStringList> mFilterConditionedHash;
					QSet<QString> mFilterSet;



				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONHTMLNAVIGATIONOVERVIEWSUMMARIZER_H
