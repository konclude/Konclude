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

#include "CCalculationManager.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Calculation {


				CCalculationManager::CCalculationManager() {
				}


				CCalculationManager::~CCalculationManager() {
				}

				CCalculationManager* CCalculationManager::calculateJobs(const QList< QPair<CCalculationJob*,CCallbackData*> >& jobCallbackList) {
					for (QList< QPair<CCalculationJob*,CCallbackData*> >::const_iterator it = jobCallbackList.constBegin(), itEnd = jobCallbackList.constEnd(); it != itEnd; ++it) {
						QPair<CCalculationJob*,CCallbackData*> jobCallbackPair(*it);
						CCalculationJob* job(jobCallbackPair.first);
						CCallbackData* callbackData(jobCallbackPair.second);
						calculateJob(job,callbackData);
					}
					return this;
				}

			}; // end namespace Calculation

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude
