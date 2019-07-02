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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONSTRINGMATCHGROUPSELECTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONSTRINGMATCHGROUPSELECTOR_H

// Libraries includes
#include <QSet>


// Namespace includes
#include "EvaluationSettings.h"
#include "CReasonerEvaluationGroupRequestSelector.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Test {

		namespace Evaluation {


			/*! 
			 *
			 *		\class		CReasonerEvaluationStringMatchGroupSelector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationStringMatchGroupSelector : public CReasonerEvaluationGroupRequestSelector {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationStringMatchGroupSelector();
					CReasonerEvaluationStringMatchGroupSelector(const QSet<QString>& groupStringSet);


					virtual bool isGroupSelected(const QString& groupString);
					virtual bool isRequestSelected(const QString& groupString, const QString& requestString);

					CReasonerEvaluationStringMatchGroupSelector* addGroupString(const QString& groupString);

				// protected methods
				protected:

				// protected variables
				protected:
					QSet<QString> mGroupStringSet;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Evaluation

	}; // end namespace Test

}; // end namespace Konclude

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONSTRINGMATCHGROUPSELECTOR_H
