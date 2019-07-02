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

#ifndef KONCLUDE_PARSER_CAXIOMONTOLOGYBUILDER_H
#define KONCLUDE_PARSER_CAXIOMONTOLOGYBUILDER_H

// Libraries includes
#include <QThread>


// Namespace includes


// Other includes
#include "Expressions/CDeclarationAxiomExpression.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	namespace Parser {

		using namespace Expression;


		/*! 
		*
		*		\class		CAxiomOntologyBuilder
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class CAxiomOntologyBuilder {
			// public methods
			public:
				//! Constructor
				CAxiomOntologyBuilder();

				//! Destructor
				virtual ~CAxiomOntologyBuilder();

				virtual CDeclarationAxiomExpression* getDeclaration(const CEXPRESSIONLIST<CBuildExpression*>& expressions) = 0;
				virtual CDeclarationAxiomExpression* getDeclaration(CBuildExpression* buildExpression) = 0;
				virtual CDeclarationAxiomExpression* getDeclaration(CExpressionEntity* entity) = 0;

				virtual bool tellOntologyAxiom(CAxiomExpression* axiom) = 0;
				virtual bool tellOntologyAxiom(CBuildExpression* axiom);
				virtual bool retractOntologyAxiom(CAxiomExpression* axiom) = 0;

				virtual bool addNameAbbreviation(const QString &name, const QString &abbreviatedName) = 0;
				virtual QString resolveAbbreviationName(const QString& name) = 0;
				virtual QString resolveAbbreviationName(const QStringRef& name) = 0;


				virtual bool addOntologyImport(const QStringRef& name) = 0;
				virtual bool addOntologyImport(const QString& name) = 0;


				virtual QString getOntologyName() = 0;

				virtual cint64 getAxiomNumber() = 0;

			// protected methods
			protected:

			// protected variables
			protected:

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CAXIOMONTOLOGYBUILDER_H
