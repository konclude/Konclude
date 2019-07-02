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
#ifndef KONCLUDE_CONCURRENT_CWATCHDOG_H
#define KONCLUDE_CONCURRENT_CWATCHDOG_H


#include "CThread.h"


namespace Konclude {

	namespace Concurrent {


		class CThread;


		/*! 
		 *	\class		CWatchDog
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		
		 */
		class CWatchDog {

			// public methods
			public:
				//! Constructor
				CWatchDog();

				//! Destructor
				virtual ~CWatchDog();



				virtual bool registerThread(CThread *thread) = 0;
				virtual void feedbackThread(CThread *thread, qint64 updateNumber) = 0;
				virtual void checkoutThread(CThread *thread) = 0;
				virtual void crashedThread(CThread *thread) = 0;



			// protected methods
			protected:


			// private variables
			private:


		};

	}; // end namespace Concurrent

}; // end namespace Konclude

#endif // KONCLUDE_CONCURRENT_CWATCHDOG_H
