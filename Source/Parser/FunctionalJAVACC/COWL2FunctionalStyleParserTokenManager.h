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

#ifndef KONCLUDE_PARSER_FUNCTIONALJAVACC_COWL2FUNCTIONALSTYLEPARSERTOKENMANAGER_H
#define KONCLUDE_PARSER_FUNCTIONALJAVACC_COWL2FUNCTIONALSTYLEPARSERTOKENMANAGER_H


// Libraries includes
#include <QString>


// Namespace includes
#include "FunctionalJAVACCSettings.h"
#include "OWL2FunctionalStyleParserTokenManager.h"


// Other includes



// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;



namespace Konclude {

	namespace Parser {

		using namespace Expression;

		namespace FunctionalJAVACC {


			/*! 
			 *
			 *		\class		COWL2FunctionalStyleParserTokenManager
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COWL2FunctionalStyleParserTokenManager : public OWL2FunctionalStyleParserTokenManager {
				// public methods
				public:
					//! Constructor
					COWL2FunctionalStyleParserTokenManager(JAVACC_CHARSTREAM *stream);

					virtual void lexicalError();
					bool hasLexicalError();

				// protected methods
				protected:

				// protected variables
				protected:
					bool mLexicalError;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_FUNCTIONALJAVACC_COWL2FUNCTIONALSTYLEPARSERTOKENMANAGER_H
