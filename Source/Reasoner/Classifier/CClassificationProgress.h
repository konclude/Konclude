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

#ifndef KONCLUDE_REASONER_CLASSIFIER_CClassificationProgress_H
#define KONCLUDE_REASONER_CLASSIFIER_CClassificationProgress_H

// Libraries includes


// Namespace includes
#include "ClassifierSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Classifier {


			/*! 
			 *
			 *		\class		CClassificationProgress
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CClassificationProgress {
				// public methods
				public:
					//! Constructor
					CClassificationProgress();

					qint64 getClassificationCount();
					qint64 getTestedSatisfiable();
					qint64 getTotalSatisfiable();
					qint64 getTestedSubsumptions();
					qint64 getTotalSubsumptions();
					double getRemainingMilliSeconds();
					double getProgessPercent();

					CClassificationProgress *setClassificationCount(qint64 classificationCount);
					CClassificationProgress *setTestedSatisfiable(qint64 testedClasses);
					CClassificationProgress *setTotalSatisfiable(qint64 totalClasses);
					CClassificationProgress *setTestedSubsumptions(qint64 testedSubsumptions);
					CClassificationProgress *setTotalSubsumptions(qint64 totalSubsumptions);
					CClassificationProgress *setRemainingMilliSeconds(double remainingMilliSeconds);
					CClassificationProgress *setProgessPercent(double progessPercent);

					CClassificationProgress *resetValues();

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 mClassificationCount;
					qint64 mTestedClasses;
					qint64 mTotalClasses;
					qint64 mTestedSubsum;
					qint64 mTotalSubsum;
					double mRemainingMilliSeconds;
					double mProgessPercent;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Classifier

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_CLASSIFIER_CClassificationProgress_H
