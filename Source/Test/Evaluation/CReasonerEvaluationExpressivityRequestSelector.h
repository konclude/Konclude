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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEXPRESSIVITYREQUESTSELECTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEXPRESSIVITYREQUESTSELECTOR_H

// Libraries includes
#include <QSet>


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGroupRequestSelector.h"


// Other includes
#include "Reasoner/Ontology/COntologyExpressivity.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Reasoner::Ontology;

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationExpressivityRequestSelector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationExpressivityRequestSelector : public CReasonerEvaluationGroupRequestSelector {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationExpressivityRequestSelector();
					CReasonerEvaluationExpressivityRequestSelector(const QList<COntologyExpressivity>& equalBoundList, const QString& baseExpressivityDir);
					CReasonerEvaluationExpressivityRequestSelector(const QList<COntologyExpressivity>& lowerNotIncludedBoundList, const QList<COntologyExpressivity>& upperIncludedBoundList, const QString& baseExpressivityDir);


					virtual bool isGroupSelected(const QString& groupString);
					virtual bool isRequestSelected(const QString& groupString, const QString& requestString);


				// protected methods
				protected:

				// protected variables
				protected:
					QList<COntologyExpressivity> mEqualBoundList;
					QList<COntologyExpressivity> mExcludedBoundList;
					QList<COntologyExpressivity> mLowerNotIncludedBoundList;
					QList<COntologyExpressivity> mLowerIncludedBoundList;
					QList<COntologyExpressivity> mUpperNotIncludedBoundList;
					QList<COntologyExpressivity> mUpperIncludedBoundList;

					QString mBaseExpressivityDir;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONEXPRESSIVITYREQUESTSELECTOR_H
