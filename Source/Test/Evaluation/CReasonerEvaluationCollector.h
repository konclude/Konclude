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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONCOLLECTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONCOLLECTOR_H

// Libraries includes
#include <QDir>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationSummarizer.h"
#include "CReasonerEvaluationExtractor.h"
#include "CReasonerEvaluationFiltering.h"
#include "CReasonerEvaluationDataValueGroupCollection.h"
#include "CReasonerEvaluationDataValueGroupCollectionReasonerComparison.h"


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
			 *		\class		CReasonerEvaluationCollector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationCollector : public CLogIdentifier {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationCollector(CReasonerEvaluationExtractor* extractor, CReasonerEvaluationSummarizer* summarizer, CReasonerEvaluationFiltering* filtering = nullptr);

					//! Destructor
					virtual ~CReasonerEvaluationCollector();

					bool collectReasonerEvaluationDataValues(CReasonerEvaluationDataValueGroupCollectionReasonerComparison *reasonerComp, const QString& reasonerName, const QString& reasonerPath);

					CReasonerEvaluationDataValueGroupCollection* collectEvaluationDataValues(const QString& baseDirectory);

				// protected methods
				protected:
					bool collectEvaluationDatas(const QString& directory, const QString& subDirectory, const QString& baseDirectory, CReasonerEvaluationDataValueGroupCollection* groupCollection);
					bool collectGroupEvaluationDatas(const QString& directory, const QString& requestFileString, CReasonerEvaluationDataValueGroup* group);

				// protected variables
				protected:

				// private methods
				private:
					CReasonerEvaluationExtractor* mExtractor;
					CReasonerEvaluationSummarizer* mSummarizer;
					CReasonerEvaluationFiltering* mFiltering;

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONCOLLECTOR_H
