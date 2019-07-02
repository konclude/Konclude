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

#ifndef KONCLUDE_PARSER_CKRSSONTOLOGYPARSER_H
#define KONCLUDE_PARSER_CKRSSONTOLOGYPARSER_H

// Libraries includes
#include <QDomDocument>
#include <QDomElement>

// Namespace includes
#include "COntologyParser.h"
#include "COntologyBuilder.h"


// Other includes
#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CNotSupportedNodeErrorRecord.h"

#include "Logger/CLogIdentifier.h"

#include "Utilities/Container/CQtHash.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

using namespace std;


namespace Konclude {

	using namespace Utilities::Container;
	using namespace Control::Command;
	using namespace Control::Command::Records;

	namespace Parser {



#define KONCLUDE_PARSER_USEPARSEJUMPFUNCTIONHASH


		/*! 
		 *
		 *		\class		CKRSSOntologyParser
		 *		\author		Andreas Steigmiller
		 *		\version	0.1
		 *		\brief		Parser for KRSS Text
		 *
		 */
		class CKRSSOntologyParser : public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				CKRSSOntologyParser(COntologyBuilder* ontoBuilder);
				CKRSSOntologyParser(COntologyBuilder* ontoBuilder, const QString& defNameString, CCommandRecordRouter* commandRecordRouter);

				//! Destructor
				virtual ~CKRSSOntologyParser();

				virtual bool parseOntologyFile(const QString& filename);
				virtual bool parseOntologyText(const QString& text);
				virtual CBuildExpression* parseKRSSLine(const QString& line);

			// protected methods
			protected:
				bool initializeParseFunctionJumpHash();

				CBuildExpression* parseKRSSExpression(const QString& text, cint64& index);
				QList<CBuildExpression* > parseClassChildNodes(const QString& text, cint64& index);
				QList<CBuildExpression* > parseSpecialClassChildNodes(const QString& text, const QString& identicalChild, cint64& index);

				QList<CBuildExpression* > parsePropertyClassChildNodes(const QString& text, cint64& index);


				QString parseIdentifierNode(const QString& text, cint64& index);
				bool checkIdentifierNode(const QString& text, const QString& expectedString, cint64& index);

				CClassTermExpression* parseClassNode(const QString& text, cint64& index);
				CObjectPropertyExpression* parseObjectPropertyNode(const QString& text, cint64& index);

				CDeclarationAxiomExpression* parseDeclarationNode(CBuildExpression* expression);


				CBuildExpression* parseObjectIntersectionOfNode(const QString& text, cint64& index);
				CBuildExpression* parseObjectUnionOfNode(const QString& text, cint64& index);
				CBuildExpression* parseObjectSomeValuesFromNode(const QString& text, cint64& index);
				CBuildExpression* parseObjectAllValuesFromNode(const QString& text, cint64& index);
				CBuildExpression* parseObjectComplementOfNode(const QString& text, cint64& index);





				CBuildExpression* jumpFunctionParseObjectIntersectionOfNode(const QString& text, cint64& index);
				CBuildExpression* jumpFunctionParseObjectUnionOfNode(const QString& text, cint64& index);
				CBuildExpression* jumpFunctionParseObjectSomeValuesFromNode(const QString& text, cint64& index);
				CBuildExpression* jumpFunctionParseObjectAllValuesFromNode(const QString& text, cint64& index);
				CBuildExpression* jumpFunctionParseObjectComplementOfNode(const QString& text, cint64& index);


			// protected variables
			protected:
				CCommandRecordRouter recordRouter;
				COntologyBuilder* mOntoBuilder;

				typedef CBuildExpression* (CKRSSOntologyParser::*ParseFunction) (const QString& text, cint64& index);

				CQtHash<QString,ParseFunction> mParseFunctionJumpHash;

				QString mDefName;
				cint64 mDefNumber;


			// private methods
			private:


			// private variables
			private:



		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CKRSSONTOLOGYPARSER_H
