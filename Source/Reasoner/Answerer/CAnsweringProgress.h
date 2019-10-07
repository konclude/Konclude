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

#ifndef KONCLUDE_REASONER_ANSWERER_CANSWERINGPROGRESS_H
#define KONCLUDE_REASONER_ANSWERER_CANSWERINGPROGRESS_H

// Libraries includes


// Namespace includes
#include "AnswererSettings.h"

// Other includes

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Answerer {


			/*! 
			 *
			 *		\class		CAnsweringProgress
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CAnsweringProgress {
				// public methods
				public:
					//! Constructor
					CAnsweringProgress();

					qint64 getTestedInstances();
					qint64 getTotalInstances();
					double getRemainingMilliSeconds();
					double getProgessPercent();

					CAnsweringProgress *setTestedInstances(qint64 testedInstances);
					CAnsweringProgress *setTotalInstances(qint64 totalInstances);
					CAnsweringProgress *setRemainingMilliSeconds(double remainingMilliSeconds);
					CAnsweringProgress *setProgessPercent(double progessPercent);

					CAnsweringProgress *resetValues();

				// protected methods
				protected:

				// protected variables
				protected:
					qint64 mTestedInstances;
					qint64 mTotalInstances;
					double mRemainingMilliSeconds;
					double mProgessPercent;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Answerer

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ANSWERER_CANSWERINGPROGRESS_H
