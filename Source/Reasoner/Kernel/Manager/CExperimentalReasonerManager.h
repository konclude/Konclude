/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_MANAGER_CEXPERIMENTALREASONERMANAGER_H
#define KONCLUDE_REASONER_KERNEL_MANAGER_CEXPERIMENTALREASONERMANAGER_H

// Libraries includes
#include <QLinkedList>

// Namespace includes
#include "CReasonerManager.h"
#include "CAnalyseReasonerManager.h"


// Other includes
#include "Concurrent/CThread.h"
#include "Concurrent/Callback/CBlockingCallbackData.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent;
	using namespace Callback;

	namespace Reasoner {

		namespace Kernel {

			namespace Manager {

				/*! 
				 *
				 *		\class		CExperimentalReasonerManager
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CExperimentalReasonerManager : public CAnalyseReasonerManager {
					// public methods
					public:
						//! Constructor
						CExperimentalReasonerManager(CWatchDog *threadWatchDog = 0);

						//! Destructor
						virtual ~CExperimentalReasonerManager();


					// protected methods
					protected:

						virtual void threadStarted();

					// protected variables
					protected:

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Manager

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_MANAGER_CEXPERIMENTALREASONERMANAGER_H
