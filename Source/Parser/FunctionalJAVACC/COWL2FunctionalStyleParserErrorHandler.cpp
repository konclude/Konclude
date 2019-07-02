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

#include "COWL2FunctionalStyleParserErrorHandler.h"


namespace Konclude {

	namespace Parser {

		namespace FunctionalJAVACC {

			COWL2FunctionalStyleParserErrorHandler::COWL2FunctionalStyleParserErrorHandler() {
			}


			void COWL2FunctionalStyleParserErrorHandler::handleUnexpectedToken(int expectedKind, JAVACC_STRING_TYPE expectedToken, Token *actual, OWL2FunctionalStyleParser *parser) {
				error_count++;
				mErrorString = QString("Expecting '%1' at line %2, column %3, but got '%4'.").arg(JAVACC_TO_QSTRING(expectedToken)).arg(actual->beginLine).arg(actual->beginColumn).arg(JAVACC_TO_QSTRING(actual->image));
			}

			void COWL2FunctionalStyleParserErrorHandler::handleParseError(Token *last, Token *unexpected, JAVACC_SIMPLE_STRING production, OWL2FunctionalStyleParser *parser) {
				error_count++;
				mErrorString = QString("Encountered %1 at line %2, column %3 while parsing '%4'.").arg(JAVACC_TO_QSTRING(unexpected->image)).arg(unexpected->beginLine).arg(unexpected->beginColumn).arg(QString::fromStdString(production));
			}

			bool COWL2FunctionalStyleParserErrorHandler::hasError() {
				return error_count >= 1;
			}

			QString COWL2FunctionalStyleParserErrorHandler::getErrorString() {
				return mErrorString;
			}


		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude
