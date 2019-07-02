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

#ifndef KONCLUDE_PARSER_COWL2FUNCTIONALJAVACCONTOLOGYSTREAMPARSER_H
#define KONCLUDE_PARSER_COWL2FUNCTIONALJAVACCONTOLOGYSTREAMPARSER_H

// Libraries includes


// Namespace includes
#include "CQtIODeviceStreamReader.h"
#include "CConcreteOWL2FunctionalStyleParser.h"
#include "COWL2FunctionalStyleParserTokenManager.h"
#include "COWL2FunctionalStyleParserErrorHandler.h"


// Other includes
#include "Parser/COntologyBuilder.h"



// Logger includes
#include "Logger/CLogger.h"


using namespace Konclude::Logger;



namespace Konclude {

	namespace Parser {

		namespace FunctionalJAVACC {


			/*! 
			 *
			 *		\class		COWL2FunctionalJAVACCOntologyStreamParser
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COWL2FunctionalJAVACCOntologyStreamParser {
				// public methods
				public:
					//! Constructor
					COWL2FunctionalJAVACCOntologyStreamParser(COntologyBuilder* ontologyBuilder);

					bool parseOntology(const QString& fileString);
					bool parseOntology(QIODevice* ioDevice);

					QString getParsingErrorString();
					bool hasParsingError();

					QString getErrorString();
					bool hasError();

				// protected methods
				protected:


				// protected variables
				protected:
					COntologyBuilder* mOntologyBuilder;

					CQtIODeviceStreamReader* mStreamReader;
					CharStream* mCharStream;
					COWL2FunctionalStyleParserTokenManager* mTokenManager;
					COWL2FunctionalStyleParserErrorHandler* mErrorHandler;
					CConcreteOWL2FunctionalStyleParser* mParser;

					QString mErrorString;
					QString mParsingErrorString;
					bool mParsingError;
					bool mError;

				// private methods
				private:


				// private variables
				private:



			};


		}; // end namespace FunctionalJAVACC

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_COWL2FUNCTIONALJAVACCONTOLOGYSTREAMPARSER_H
