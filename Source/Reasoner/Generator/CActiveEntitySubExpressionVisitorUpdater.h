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

#ifndef KONCLUDE_REASONER_GENERATOR_CACTIVEENTITYSUBEXPRESSIONVISITORUPDATER_H
#define KONCLUDE_REASONER_GENERATOR_CACTIVEENTITYSUBEXPRESSIONVISITORUPDATER_H


// Libraries includes
#include <QHash>

// Namespace includes


// Other includes
#include "Reasoner/Ontology/CActiveEntityCountVector.h"

#include "Parser/Expressions/CBuildExpression.h"
#include "Parser/Expressions/CSubExpressionVisitor.h"
#include "Parser/Expressions/CExpressionEntity.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Ontology;

		namespace Generator {

			/*! 
			 *
			 *		\class		CActiveEntitySubExpressionVisitorUpdater
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CActiveEntitySubExpressionVisitorUpdater : public CSubExpressionVisitor  {
				// public methods
				public:
					//! Constructor
					CActiveEntitySubExpressionVisitorUpdater(CActiveEntityCountVector* countVector);


					bool updateEntitiesForAxioms(CBuildExpression* expression, bool added);


					virtual bool visitSubExpression(CBuildExpression* expression, CBuildExpression* subExpression, CSubExpressionVisitor* visitor);
					virtual bool visitEntity(CBuildExpression* expression, CExpressionEntity* entity, CSubExpressionVisitor* visitor);

				// protected methods
				protected:

				// protected variables
				protected:
					bool mVisitingAxiomAdded;
					CActiveEntityCountVector* mCountVector;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CACTIVEENTITYSUBEXPRESSIONVISITORUPDATER_H
