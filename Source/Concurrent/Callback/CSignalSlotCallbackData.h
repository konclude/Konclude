/*
 *		Copyright (C) 2013, 2014 by the Konclude Developer Team.
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
/*
 *
 *		Author:		Andreas Steigmiller
 *		Copyright:	2009 Andreas Steigmiller
 *		Project:	UUPR
 *
 */

#ifndef UUPR_CONCURRENT_CALLBACK_CSIGNALSLOTCALLBACKDATA_H
#define UUPR_CONCURRENT_CALLBACK_CSIGNALSLOTCALLBACKDATA_H


#include <QObject>

#include "CCallbackData.h"



namespace UUPR {

	namespace Concurrent {

		namespace Callback {


			/*! 
			 *
			 *		\class		CSignalSlotCallbackData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CSignalSlotCallbackData : public QObject, public CCallbackData {
					Q_OBJECT

				public:
					CSignalSlotCallbackData(int idNumber = 0);
					virtual ~CSignalSlotCallbackData();

					virtual void doCallback();

					int getIdNumber();
					int setIdNumber(int idNumber);

				signals:
					void callback(CSignalSlotCallbackData *callbackData);

				protected:
					int id;

				
			};

		}; // end namespace Callback

	}; // end namespace Concurrent

}; // end namespace UUPR

#endif // UUPR_CONCURRENT_CALLBACK_CSIGNALSLOTCALLBACKDATA_H
