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

#ifndef KONCLUDE_PARSER_FUNCTIONALJAVACC_CQtIODeviceStreamReader_H
#define KONCLUDE_PARSER_FUNCTIONALJAVACC_CQtIODeviceStreamReader_H


// Libraries includes
#include <QIODevice>


// Namespace includes
#include "FunctionalJAVACCSettings.h"
#include "CharStream.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;



namespace Konclude {

	namespace Parser {

		namespace FunctionalJAVACC {


			/*! 
			 *
			 *		\class		CQtIODeviceStreamReader
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CQtIODeviceStreamReader : public ReaderStream {
				// public methods
				public:
					//! Constructor
					CQtIODeviceStreamReader(QIODevice* device);


					// Read block of data into a buffer and return the actual number read.
					virtual size_t read(JAVACC_CHAR_TYPE *bufptr, int offset, size_t len);
					virtual bool endOfInput();


				// protected methods
				protected:

				// protected variables
				protected:
					QIODevice* mDevice;
					QString mConvBuffer;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_FUNCTIONALJAVACC_CQtIODeviceStreamReader_H
