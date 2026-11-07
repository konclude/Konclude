#ifndef KONCLUDE_PARSER_CRULEEXPRESSIONCREATOR_H
#define KONCLUDE_PARSER_CRULEEXPRESSIONCREATOR_H

// Libraries includes
#include <QString>
#include <QHash>
#include <QMap>
#include <QSet>
#include <QList>

// Namespace includes
#include "ParserSettings.h"
#include "CParseRuleAtom.h"
#include "COntologyBuilder.h"
#include "Expressions/CBuildExpression.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	using namespace Parser::Expression;

	namespace Parser {


		/*!
		 *
		 *	\class		CRuleExpressionCreator
		 *	\author		Andreas Steigmiller
		 *	\version	0.1
		 *	\brief		Translates the Body/Head atoms of one OWL 2 DL-safe rule into
		 *				the nominal-schema DL axioms that Konclude reasons with.
		 *
		 *	Both the OWL/XML handler and the functional-syntax parser build the
		 *	atom list (CParseRuleAtom) and call this creator, so the two input
		 *	formats produce identical axioms.
		 *
		 */
		class CRuleExpressionCreator {
			// public methods
			public:
				//! Constructor
				CRuleExpressionCreator(COntologyBuilder* ontologyBuilder);

				//! Translates the atoms of one rule into SubClassOf axioms.
				QList<CBuildExpression*> createRuleAxioms(const QList<CParseRuleAtom*>& bodyAtomList,
						const QList<CParseRuleAtom*>& headAtomList, cint64& axiomNumber);

			// protected methods
			protected:
				QHash<QString,CParseRuleAtom*> createVariableAtomHash(const QList<CParseRuleAtom*>& atomList);
				QMap<cint64,QString> createVariableCountMap(const QList<CParseRuleAtom*>& atomList);

				CBuildExpression* createBodyExpresions(const QString& varString, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet,
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash, QSet<QString>& backTestProcessedVarSet, QSet<CParseRuleAtom*>& backTestProcessedAtomSet);

				bool hasBackExpressionLinks(const QString& varString, CParseRuleAtom* notRuleAtom, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet,
						QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				CBuildExpression* createBodyExpresions(const QString& varString, bool direct, QHash<QString,CParseRuleAtom*>& bodyVariableAtomHash, QSet<QString>& processedVarSet, QSet<CParseRuleAtom*>& processedAtomSet, QHash<QString,CParseRuleAtom*>& headVariableAtomHash);

				CBuildExpression* getNeighbourAtomObjectPropertyExpression(CParseRuleAtom* ruleAtom, bool inversed);

			// protected variables
			protected:
				COntologyBuilder* mOntoBuilder;

			// private methods
			private:

			// private variables
			private:
				cint64 mAxiomNumber;
		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CRULEEXPRESSIONCREATOR_H
