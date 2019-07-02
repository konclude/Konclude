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

#ifndef KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROFILEREQUESTSELECTOR_H
#define KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROFILEREQUESTSELECTOR_H

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
			 *		\class		CReasonerEvaluationProfileRequestSelector
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CReasonerEvaluationProfileRequestSelector : public CReasonerEvaluationGroupRequestSelector {
				// public methods
				public:
					//! Constructor
					CReasonerEvaluationProfileRequestSelector(cint64 profile, const QString& baseExpressivityDir);


					virtual bool isGroupSelected(const QString& groupString);
					virtual bool isRequestSelected(const QString& groupString, const QString& requestString);

					const static cint64 REPRS_PROFILE_DL		= 0x0001;
					const static cint64 REPRS_PROFILE_EL		= 0x0002;
					const static cint64 REPRS_PROFILE_RL		= 0x0004;
					const static cint64 REPRS_PROFILE_QL		= 0x0008;

				// protected methods
				protected:
					virtual QString getProfileKeyString(cint64 profile);

				// protected variables
				protected:
					cint64 mProfile;
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

#endif // KONCLUDE_TEST_EVALUATION_CREASONEREVALUATIONPROFILEREQUESTSELECTOR_H
