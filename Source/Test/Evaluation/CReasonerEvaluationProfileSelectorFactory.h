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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROFILESELECTORFACTORY_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROFILESELECTORFACTORY_H

// Libraries includes
#include <QString>
#include <QDir>


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGroupRequestSelector.h"
#include "CReasonerEvaluationProfileRequestSelector.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationProfileSelectorFactory
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationProfileSelectorFactory {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationProfileSelectorFactory(const QString& baseEvalName, const QString& baseOutputDir, const QString& expressivityDir);


					CReasonerEvaluationGroupRequestSelector* getSelectors(CReasonerEvaluationGroupRequestSelector* prevSelector);

					CReasonerEvaluationGroupRequestSelector* getSelectorsForDLProfile(CReasonerEvaluationGroupRequestSelector* prevSelector);
					CReasonerEvaluationGroupRequestSelector* getSelectorsForELProfile(CReasonerEvaluationGroupRequestSelector* prevSelector);
					CReasonerEvaluationGroupRequestSelector* getSelectorsForQLProfile(CReasonerEvaluationGroupRequestSelector* prevSelector);
					CReasonerEvaluationGroupRequestSelector* getSelectorsForRLProfile(CReasonerEvaluationGroupRequestSelector* prevSelector);


				// protected methods
				protected:

				// protected variables
				protected:
					QString mBaseEvalName;
					QString mBaseOutputDir;
					QString mExpressivityDir;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROFILESELECTORFACTORY_H
