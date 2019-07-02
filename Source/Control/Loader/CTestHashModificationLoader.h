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

#ifndef KONCLUDE_Control_LOADER_CTESTHASHMODIFICATIONLOADER_H
#define KONCLUDE_Control_LOADER_CTESTHASHMODIFICATIONLOADER_H

// Libraries includes
#include <QString>
#include <QRegExp>

// Namespace includes
#include "CLoader.h"

// Other includes
#include "Test/CConcurrentHashModificationTester.h"

#include "Concurrent/CIntervalThread.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Logger;
	using namespace Test;
	using namespace Concurrent;

	namespace Control {

		namespace Loader {

			/*! 
			 *
			 *		\class		CTestHashModificationLoader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTestHashModificationLoader : public CIntervalThread, public CLoader {
				// public methods
				public:
					//! Constructor
					CTestHashModificationLoader();

					//! Destructor
					virtual ~CTestHashModificationLoader();


					virtual CLoader* init(CLoaderFactory* loaderFactory = 0, CConfiguration* config = 0);
					virtual CLoader* load();
					virtual CLoader* exit();


				// protected methods
				protected:
					virtual bool processTimer(qint64 timerID);

				// protected variables
				protected:
					cint64 mRemainingSteps;
					cint64 mSwitchRemainingSteps;
					CConfiguration* mConfiguration;

					cint64 mThreadCount;
					cint64 mMaxThreadCount;
					cint64 mHashModCountPerStep;
					cint64 mMaxHashModCountPerStep;
					bool mUseModificatedHashes;
					bool mExtendBaseHashes;
					cint64 mOneThreadScore;


					CConcurrentHashModificationTester* tester;

					static const cint64 TESTTIMERID = 1;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Loader

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_Control_LOADER_CTESTHASHMODIFICATIONLOADER_H
