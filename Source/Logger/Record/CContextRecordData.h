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

#ifndef LOGGER_RECORD_CCONTEXTRECORDDATA_H
#define LOGGER_RECORD_CCONTEXTRECORDDATA_H


// Libraries includes
#include <QString>


// Namespace includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Logger {

		namespace Record {


			/*! 
			*
			*		\class		CContextRecordData
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CContextRecordData {

				// public methods
				public:
					//! Constructor
					CContextRecordData(double recordErrorLevel = 0., CContextRecordData *nextRecordData = 0);

					//! Destructor
					virtual ~CContextRecordData();


					virtual CContextRecordData *getNextRecordData();
					virtual CContextRecordData *setNextRecordData(CContextRecordData *recordData);

					virtual double getReflexiveTransetiveMaximumRecordErrorLevel();


					virtual double getRecordErrorLevel();
					virtual CContextRecordData *setRecordErrorLevel(double recordErrorLevel);



				// protected methods
				protected:

				// protected variables
				protected:
					CContextRecordData *prevRecordData;
					double recordLevel;
					double prevRecordLevel;


				// private methods
				private:

				// private variables
				private:
				
			};

		}; // end namespace Record

	}; // end namespace Logger

}; // end namespace Konclude

#endif // LOGGER_RECORD_CCONTEXTRECORDDATA_H
