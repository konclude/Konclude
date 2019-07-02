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

#ifndef KONCLUDE_RENDERER_CONTOLOGYPARSEDBUILDEXPRESSIONRENDERVISITOR_H
#define KONCLUDE_RENDERER_CONTOLOGYPARSEDBUILDEXPRESSIONRENDERVISITOR_H

// Libraries includes


// Namespace includes
#include "RendererSettings.h"
#include "CRenderedItemLinker.h"
#include "COntologyRenderer.h"
#include "COntologyRenderVisitor.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Parser/Expressions/CSubClassOfExpression.h"
#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CClassExpression.h"
#include "Parser/Expressions/CObjectPropertyExpression.h"
#include "Parser/Expressions/CObjectIntersectionOfExpression.h"
#include "Parser/Expressions/CObjectUnionOfExpression.h"
#include "Parser/Expressions/CObjectComplementOfExpression.h"
#include "Parser/Expressions/CObjectSomeValuesFromExpression.h"
#include "Parser/Expressions/CObjectAllValuesFromExpression.h"

#include "Reasoner/Generator/CStringRefStringHasher.h"

// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	using namespace Reasoner::Ontology;
	using namespace Reasoner::Generator;
	using namespace Parser::Expression;

	namespace Renderer {



		/*! 
		*
		*		\class		COntologyParsedBuildExpressionRenderVisitor
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyParsedBuildExpressionRenderVisitor : public COntologyRenderVisitor {
			// public methods
			public:
				//! Constructor
				COntologyParsedBuildExpressionRenderVisitor();


				bool visitAndRender(const CQtList<CConcept*>& conceptList, CConcreteOntology* ontology, COntologyRenderer* renderer);
				bool visitAndRender(const QStringList& entityStringList, CConcreteOntology* ontology, COntologyRenderer* renderer);


			// protected methods
			protected:
				bool visitAndRender(const CQtList<CExpressionEntity*>& entityList);

				bool addRemainingVisitingAndRenderEntity(CExpressionEntity* entity);

				CRenderedItemLinker* getRenderedEntityExpression(CExpressionEntity* entity);
				QString getNameFromEntityExpression(CExpressionEntity* entity);

				bool serachRemainingVisitingAndRenderAxiom(CClassTermExpression* expression);
				bool serachRemainingVisitingAndRenderAxiom(CObjectPropertyTermExpression* expression);

				CRenderedItemLinker* getRenderedAxiomExpression(CAxiomExpression* axiomExp);

				CRenderedItemLinker* getRenderedSubClassOfAxiom(CAxiomExpression* axiomExp);

				CRenderedItemLinker* getRenderedClassTermExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedClassExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectIntersectionOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectUnionOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectComplementOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectSomeValuesFromExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectAllValuesFromExpression(CClassTermExpression* classTermExp);

				CRenderedItemLinker* getRenderedObjectPropertyTermExpression(CObjectPropertyTermExpression* objPropTermExp);
				CRenderedItemLinker* getRenderedObjectPropertyExpression(CObjectPropertyTermExpression* objPropTermExp);

			// protected variables
			protected:
				CBUILDHASH<CConcept*,CClassTermExpression*>* mConceptClassTermMapHash;
				CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* mClassTermExpClassAxiomHash;
				COntologyRenderer* mRenderer;
				CConcreteOntology* mOntology;

				QSet<CExpressionEntity*> mVisitedEntitySet;
				QList<CExpressionEntity*> mRemainingEntityList;

				QSet<CBuildExpression*> mTestedAxiomSet;
				QSet<CAxiomExpression*> mVisitedAxiomSet;
				QList<CAxiomExpression*> mRemainingAxiomList;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CONTOLOGYPARSEDBUILDEXPRESSIONRENDERVISITOR_H
