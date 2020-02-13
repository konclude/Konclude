/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_PARSER_CRDFREDLANDEXTERNALTRIPLESDSNCONNECTINGPARSER_H
#define KONCLUDE_PARSER_CRDFREDLANDEXTERNALTRIPLESDSNCONNECTINGPARSER_H

// Libraries includes
#include <Redland.h>
#include <rasqal.h>
#include <stdio.h>
#include <ios>
#include <QIODevice>

// Namespace includes
#include "CParsingUtilities.h"
#include "COntologyBuilder.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"

#include "Reasoner/Triples/CRedlandStoredTriplesData.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Config/CConfiguration.h"
#include "Config/CConfigDataReader.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Reasoner::Triples;
	using namespace Reasoner::Ontology;
	using namespace std;

	namespace Parser {




		/*! 
		 *
		 *		\class		CRDFRedlandExternalTriplesDSNConnectingParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CRDFRedlandExternalTriplesDSNConnectingParser : public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				CRDFRedlandExternalTriplesDSNConnectingParser(COntologyBuilder* ontologyBuilder, CConfiguration* configuration = nullptr);

				//! Destructor
				virtual ~CRDFRedlandExternalTriplesDSNConnectingParser();


				//virtual bool linkRemoteData(const QString& remoteAddress, const QString& remoteGraph);
				bool parseTriplesFile(const QString& filename);

				virtual bool connectExternalTriples(const QString& triplesDBName, const QString& dsnConfig);

				QString getErrorString();
				bool hasError();



			// protected methods
			protected:


			// protected variables
			protected:
				COntologyBuilder* mOntologyBuilder;
				bool mParsingError = false;
				QString mErrorString;



				CConfiguration* mConfiguration;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CRDFREDLANDEXTERNALTRIPLESDSNCONNECTINGPARSER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
