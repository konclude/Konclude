/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTIMEOUTERROREXTRACTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTIMEOUTERROREXTRACTOR_H

// Libraries includes
#include <QList>
#include <QFile>
#include <QDomDocument>

// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationDataValue.h"
#include "CReasonerEvaluationExtractor.h"
#include "CReasonerEvaluationDoubleDataValue.h"


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
			 *		\class		CReasonerEvaluationTimeoutErrorExtractor
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationTimeoutErrorExtractor : public CReasonerEvaluationExtractor {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationTimeoutErrorExtractor(double timelimit = -1);

					//! Destructor
					virtual ~CReasonerEvaluationTimeoutErrorExtractor();


					virtual CReasonerEvaluationDataValue* extractEvaluationData(const QString& responseFile);



				// protected methods
				protected:

				// protected variables
				protected:
					double mTimelimit;

				// private methods
				private:

				// private variables
				private:

			};

	}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONTIMEOUTERROREXTRACTOR_H
