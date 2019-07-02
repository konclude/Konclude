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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONSPECIFIEDTIMEEXTRACTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONSPECIFIEDTIMEEXTRACTOR_H

// Libraries includes
#include <QList>
#include <QFile>
#include <QDomDocument>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationDataValue.h"
#include "CReasonerEvaluationExtractor.h"
#include "CReasonerEvaluationDoubleDataValue.h"
#include "CReasonerEvaluationDataValueCacher.h"
#include "CReasonerEvaluationStringValue.h"


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
			 *		\class		CReasonerEvaluationSpecifiedTimeExtractor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationSpecifiedTimeExtractor : public CReasonerEvaluationExtractor {
				// public methods
				public:

					//! Constructor
					CReasonerEvaluationSpecifiedTimeExtractor(CReasonerEvaluationExtractor::EXTRACTORTYPE extractorType, double timelimit = -1, double errorPunishmentTime = 0, CReasonerEvaluationDataValueCacher* cacher = nullptr);

					virtual CReasonerEvaluationDataValue* extractEvaluationData(const QString& responseFileString);


				// protected methods
				protected:
					virtual CReasonerEvaluationDoubleDataValue* extractSpecifiedTimesEvaluationData(QDomDocument& document, const QString& responseFileString, const QStringList& extractionTagList, CReasonerEvaluationExtractor::EXTRACTORTYPE extractionType);

					virtual CReasonerEvaluationDoubleDataValue* extractCalculationEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractSatisfiabilityEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractConsistencyEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractClassificationEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractLoadingEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractTotalResponseEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractTimeoutErrorEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractErrorEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationDoubleDataValue* extractTimeoutEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationStringValue* extractResponseFileEvaluationData(QDomDocument& document, const QString& responseFileString);
					virtual CReasonerEvaluationStringValue* extractRequestFileEvaluationData(QDomDocument& document, const QString& responseFileString);

				// protected variables
				protected:
					CReasonerEvaluationDataValueCacher* mCacher;
					double mTimelimit;
					double mErrorPunishmentTime;

					QStringList mClassificationExtractionTagList;
					QStringList mConsistencyExtractionTagList;
					QStringList mSatisfiabilityExtractionTagList;
					QStringList mCalculationExtractionTagList;

					EXTRACTORTYPE mExtractorType;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONSPECIFIEDTIMEEXTRACTOR_H
