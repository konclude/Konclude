/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_PARSER_CBUILDEXPRESSION_H
#define KONCLUDE_PARSER_CBUILDEXPRESSION_H


// Libraries includes


// Namespace includes
#include "ExpressionSettings.h"
#include "CExpressionHashMixer.h"
#include "CSubExpressionVisitor.h"

// Other includes


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;


namespace Konclude {

	namespace Parser {

		namespace Expression {

			/*! 
			 *	\class		CBuildExpression
			 *	\author		Andreas Steigmiller
			 *	\version	0.1
			 *	\brief		TODO
			 */
			class CBuildExpression {
				// public methods
				public:
					//! Constructor
					CBuildExpression();

					enum ExpressionType {	BETCLASS, BETOBJECTPROPERTY,
						
											BETEQUIVALENTCLASSES, BETESUBCLASSOF, BETDISJOINTUNION, BETDISJOINTCLASSES,

											BETOBJECTSOMEVALUEFROM, BETOBJECTINTERSECTION, BETOBJECTALLVALUEFROM, BETOBJECTEXACTLYCARDINALITY, BETOBJECTHASSELF,
											BETOBJECTMINCARDINALITY, BETOBJECTMAXCARDINALITY, BETOBJECTCOMPLEMENTOF, BETOBJECTUNION, BETOBJECTONEOF, BETOBJECTHASVALUE,

											BETSUBOBJECTPROPERTYOF, BETTRANSETIVEPROPERTYOF, BETINVERSEOBJECTPROPERTYOF, BETFUNCTIONALPROPERTYOF,
											BETOBJECTPROPERTYDOMAIN, BETOBJECTPROPERTYRANGE, BETFUNCTIONALPROPERTY, BETTRANSITIVEOBJECTPROPERTY, 
											BETINVERSEOBJECTPROPERTIES, BETEQUIVALENTOBJECTPROPERTIES, BETSYMMETRICPROPERTY, BETASYMMETRICPROPERTY, 
											BETREFLEXIVEPROPERTY, BETIRREFLEXIVEPROPERTY, BETOBJECTPROPERTYCHAIN, BETDISJOINTOBJECTPROPERTIES, BETINVERSEFUNCTIONALPROPERTY,

											BETCLASSASSERTION, BETOBJECTPROPERTYASSERTION, BETSAMEINDIVIDUAL, BETDIFFERENTINDIVIDUAL, BETNEGATIVEOBJECTPROPERTYASSERTION,
											
											BETNAMEDINDIVIDUAL, BETANONYMOUSINDIVIDUAL, BETINDIVIDUALVARIABLE,

											BETISCLASSSATISFIABLE, BETARECLASSESEQUIVALENCE, BETISCLASSSUBSUMTIONBY, BETARECLASSESDISJOINT, BETISINSTANCEOF, 
											BETPRECEDENTCLASSBUILD, BETPRECEDENTPROPERTYBUILD, BETPRECEDENTINDIVIDUALBUILD, BETGETFLATTENEDTYPES, BETGETFLATTENEDINDIVIDUALS,
											BETGETINDIVIDUALS, BETGETTYPES, BETGETSUBCLASSES, BETGETSUPERCLASSES, BETGETEQUIVALENTCLASSES, BETISENTAILED,
											
											BETDECLARATION,

											BETRULECLASSATOM, BETRULEPROPERTYATOM, BETRULEDIFFERENTINDIVIDUALSATOM, BETRULEATOMBODYLIST, BETRULEATOMHEADLIST

										};

					virtual ExpressionType getType() = 0;

					virtual cint64 getStructuralHashValue() = 0;
					virtual bool compareStructuralEquivalence(CBuildExpression* compareExpression) = 0;


					virtual bool visitSubExpressions(CSubExpressionVisitor* subExpressionVisitor);


				// protected methods
				protected:

				// private methods
				private:
			};

		}; // end namespace Expression

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CBUILDEXPRESSION_H
