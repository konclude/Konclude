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

#ifndef KONCLUDE_PARSER_CRDFSIMPLETURTLEPARSER_H
#define KONCLUDE_PARSER_CRDFSIMPLETURTLEPARSER_H

// Libraries includes


// Namespace includes
#include "CParsingUtilities.h"


// Other includes
#include "Utilities/UtilitiesSettings.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Utilities;

	namespace Parser {



		class CRDFStringTriple {
			public: 
				QString string[3];
		};


		class CRDFHahsedSubjectData {
			public: 
				QString string[2];
				CRDFHahsedSubjectData(CRDFStringTriple& rdfStringTriple) {
					string[0] = rdfStringTriple.string[1];
					string[1] = rdfStringTriple.string[2];
				}
		};

		/*! 
		 *
		 *		\class		CRDFSimpleTurtleParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		TODO
		 *
		 */
		class CRDFSimpleTurtleParser {
			// public methods
			public:
				//! Constructor
				CRDFSimpleTurtleParser();

				//! Destructor
				virtual ~CRDFSimpleTurtleParser();


			// protected methods
			protected:
				QString getNextPart(const QString& text, cint64& pos);
				QString getIRI(const QString& iriString);
				QString getBlankNode(const QString& blankNodeString);

				QString getNextAnonymousNode();

				QList<CRDFStringTriple>* getRDFStringTriples(QStringList& partList);
				QHash<QString, CRDFHahsedSubjectData>* getRDFSubjectHash(QStringList& partList);

				QStringList getBracketContent(QStringList& partList, QChar openingBracket, QChar closingBracket, bool bracketOpened = false, bool exculdeOthers = false);

				bool getNextRDFStringTriple(QStringList& partList, CRDFStringTriple& lastNextTriple, QList<CRDFStringTriple>& nextTripleList, int startingSopPos = 0);

				QPair<QString,QString> extractDataLiteral(const QString& string);


			// protected variables
			protected:
				QString mBaseIRI;
				QHash<QString, QString> mPrefixHash;
				cint64 nextAnonymousNodeId;

				bool mParsingError = false;
				QString mLastParsingError;


				CParsingUtilities mParsingUtilities;



			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CRDFSIMPLETURTLEPARSER_H
