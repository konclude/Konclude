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

#include "COWL2FunctionalJAVACCOntologyStreamParser.h"


namespace Konclude {

	namespace Parser {

		namespace FunctionalJAVACC {


			COWL2FunctionalJAVACCOntologyStreamParser::COWL2FunctionalJAVACCOntologyStreamParser(COntologyBuilder* ontologyBuilder) : mOntologyBuilder(ontologyBuilder) {
				mError = false;
				mParsingError = false;
			}


			bool COWL2FunctionalJAVACCOntologyStreamParser::parseOntology(const QString& fileString) {
				QFile file(fileString);
				mError = false;
				if (file.open(QIODevice::ReadOnly)) {
					mError = !parseOntology(&file);
					file.close();
				} else {
					mError = true;
					mErrorString = QString("File '%1' does not exist or cannot be opened.").arg(fileString);
				}
				return !mError;
			}

			bool COWL2FunctionalJAVACCOntologyStreamParser::parseOntology(QIODevice* ioDevice) {
				mStreamReader = new CQtIODeviceStreamReader(ioDevice);
				mCharStream = new CharStream(mStreamReader);
				mTokenManager = new COWL2FunctionalStyleParserTokenManager(mCharStream);
				mErrorHandler = new COWL2FunctionalStyleParserErrorHandler();
				mParser = new CConcreteOWL2FunctionalStyleParser(mOntologyBuilder,mTokenManager);
				mParser->setErrorHandler(mErrorHandler);
				mParser->ontologyDocument();
				mError = mParsingError = mErrorHandler->hasError();
				mErrorString = mParsingErrorString = mErrorHandler->getErrorString();
				if (!mError && mTokenManager->hasLexicalError()) {					
					mError = mParsingError = true;
					mErrorString = mParsingErrorString = QString("Lexical error encountered");
				}
				delete mParser;
				return !mParsingError;
			}


			QString COWL2FunctionalJAVACCOntologyStreamParser::getParsingErrorString() {
				return mParsingErrorString;
			}

			bool COWL2FunctionalJAVACCOntologyStreamParser::hasParsingError() {
				return mParsingError;
			}

			QString COWL2FunctionalJAVACCOntologyStreamParser::getErrorString() {
				return mErrorString;
			}

			bool COWL2FunctionalJAVACCOntologyStreamParser::hasError() {
				return mError;
			}


		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude
