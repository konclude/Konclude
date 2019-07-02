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

#ifndef KONCLUDE_RENDERER_COntologyAxiomExpressionsRenderVisitor_H
#define KONCLUDE_RENDERER_COntologyAxiomExpressionsRenderVisitor_H

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
#include "Parser/Expressions/CObjectExactlyCardinalityExpression.h"
#include "Parser/Expressions/CObjectMinCardinalityExpression.h"
#include "Parser/Expressions/CObjectMaxCardinalityExpression.h"
#include "Parser/Expressions/CObjectHasSelfExpression.h"
#include "Parser/Expressions/CObjectHasValueExpression.h"
#include "Parser/Expressions/CObjectOneOfExpression.h"
#include "Parser/Expressions/CDataSomeValuesFromExpression.h"
#include "Parser/Expressions/CDataHasValueExpression.h"
#include "Parser/Expressions/CDataAllValuesFromExpression.h"
#include "Parser/Expressions/CDataMinCardinalityExpression.h"
#include "Parser/Expressions/CDataMaxCardinalityExpression.h"
#include "Parser/Expressions/CDataExactCardinalityExpression.h"
#include "Parser/Expressions/CDataLiteralExpression.h"
#include "Parser/Expressions/CEquivalentClassesExpression.h"
#include "Parser/Expressions/CDisjointUnionExpression.h"
#include "Parser/Expressions/CDisjointClassesExpression.h"
#include "Parser/Expressions/CSubObjectPropertyOfExpression.h"
#include "Parser/Expressions/CTransetiveObjectPropertyExpression.h"
#include "Parser/Expressions/CFunctionalObjectPropertyExpression.h"
#include "Parser/Expressions/CObjectPropertyDomainExpression.h"
#include "Parser/Expressions/CObjectPropertyRangeExpression.h"
#include "Parser/Expressions/CInverseObjectPropertiesExpression.h"
#include "Parser/Expressions/CEquivalentObjectPropertiesExpression.h"
#include "Parser/Expressions/CSymmetricObjectPropertyExpression.h"
#include "Parser/Expressions/CAsymmetricObjectPropertyExpression.h"
#include "Parser/Expressions/CDisjointObjectPropertiesExpression.h"
#include "Parser/Expressions/CReflexiveObjectPropertyExpression.h"
#include "Parser/Expressions/CIrreflexiveObjectPropertyExpression.h"
#include "Parser/Expressions/CInverseFunctionalObjectPropertyExpression.h"
#include "Parser/Expressions/CObjectPropertyChainExpression.h"
#include "Parser/Expressions/CInverseObjectPropertyOfExpression.h"
#include "Parser/Expressions/CDataPropertyRangeExpression.h"
#include "Parser/Expressions/CDataPropertyDomainExpression.h"
#include "Parser/Expressions/CSubDataPropertyOfExpression.h"
#include "Parser/Expressions/CDisjointDataPropertiesExpression.h"
#include "Parser/Expressions/CEquivalentDataPropertiesExpression.h"
#include "Parser/Expressions/CFunctionalDataPropertyExpression.h"
#include "Parser/Expressions/CDataPropertyAssertionExpression.h"
#include "Parser/Expressions/CNegativeDataPropertyAssertionExpression.h"
#include "Parser/Expressions/CObjectPropertyAssertionExpression.h"
#include "Parser/Expressions/CNegativeObjectPropertyAssertionExpression.h"
#include "Parser/Expressions/CClassAssertionExpression.h"
#include "Parser/Expressions/CSameIndividualExpression.h"
#include "Parser/Expressions/CDifferentIndividualsExpression.h"
#include "Parser/Expressions/CDataIntersectionOfExpression.h"
#include "Parser/Expressions/CDataUnionOfExpression.h"
#include "Parser/Expressions/CDataComplementOfExpression.h"
#include "Parser/Expressions/CDataOneOfExpression.h"
#include "Parser/Expressions/CDatatypeRestrictionExpression.h"

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
		*		\class		COntologyAxiomExpressionsRenderVisitor
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyAxiomExpressionsRenderVisitor : public COntologyRenderVisitor {
			// public methods
			public:
				//! Constructor
				COntologyAxiomExpressionsRenderVisitor();


				bool visitAndRender(const CQtList<CAxiomExpression*>& axiomList, CConcreteOntology* ontology, COntologyRenderer* renderer);


			// protected methods
			protected:

				bool addRemainingVisitingAndRenderEntity(CExpressionEntity* entity);

				QString getNameFromEntityExpression(CExpressionEntity* entity);


				CRenderedItemLinker* getRenderedAxiomExpression(CAxiomExpression* axiomExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);

				CRenderedItemLinker* getRenderedDeclarationAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedSubClassOfAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedEquivalentClassesAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedDisjointUnionAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedDisjointClassesAxiom(CAxiomExpression* axiomExp);

				CRenderedItemLinker* getRenderedSubObjectPropertyOfAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedTransetiveObjectPropertyAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedSymmetricObjectPropertyAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedAsymmetricObjectPropertyAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedFunctionalObjectPropertyAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedObjectPropertyDomainAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedObjectPropertyRangeAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedInverseObjectPropertiesAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedEquivalentObjectPropertiesAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedDisjointObjectPropertiesAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedReflexiveObjectPropertyAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedIrreflexiveObjectPropertyAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedInverseFunctionalObjectPropertyAxiom(CAxiomExpression* axiomExp);


				CRenderedItemLinker* getRenderedDataPropertyDomainAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedDataPropertyRangeAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedSubDataPropertyOfAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedDisjointDataPropertiesAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedEquivalentDataPropertiesAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedFunctionalDataPropertyAxiom(CAxiomExpression* axiomExp);

				CRenderedItemLinker* getRenderedDataPropertyAssertionAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedNegativeDataPropertyAssertionAxiom(CAxiomExpression* axiomExp);


				CRenderedItemLinker* getRenderedObjectPropertyAssertionAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedNegativeObjectPropertyAssertionAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedClassAssertionAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedSameIndividualAxiom(CAxiomExpression* axiomExp);
				CRenderedItemLinker* getRenderedDifferentIndividualsAxiom(CAxiomExpression* axiomExp);


				CRenderedItemLinker* getRenderedClassTermExpressions(CEXPRESSIONLIST<CClassTermExpression*>* classTermExpList, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedClassTermExpression(CClassTermExpression* classTermExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* appendPrepandRenderedItemLinker(CRenderedItemLinker* tmpRenderedLinker, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);


				CRenderedItemLinker* getRenderedObjectIntersectionOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectUnionOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectComplementOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectSomeValuesFromExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectAllValuesFromExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectExactCardinalityExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectMinCardinalityExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectMaxCardinalityExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectHasSelfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectHasValueFromExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectOneOfExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectDataSomeValueFromExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectDataHasValueExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectDataAllValuesFromExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectDataMinCardinalityExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectDataMaxCardinalityExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedObjectDataExactCardinalityExpression(CClassTermExpression* classTermExp);


				CRenderedItemLinker* getRenderedDataRangeTermExpressions(CEXPRESSIONLIST<CDataRangeTermExpression*>* dataTermExpList, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedDataRangeTermExpression(CDataRangeTermExpression* dataRangeTermExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);

				CRenderedItemLinker* getRenderedDataFacetRestrictionExpressions(CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* dataFacetRestExpList, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedDataFacetRestrictionExpression(CDataFacetRestrictionExpression* dataFacetRestExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);



				CRenderedItemLinker* getRenderedObjectPropertyTermExpressions(CEXPRESSIONLIST<CObjectPropertyTermExpression*>* objPropTermExpList, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedObjectPropertyTermExpression(CObjectPropertyTermExpression* objPropTermExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedDataPropertyTermExpressions(CEXPRESSIONLIST<CDataPropertyTermExpression*>* dataPropTermExpList, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedDataPropertyTermExpression(CDataPropertyTermExpression* dataPropTermExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);

				CRenderedItemLinker* getRenderedObjectPropertyChainExpression(CObjectPropertyTermExpression* objPropTermExp);
				CRenderedItemLinker* getRenderedInverseObjectPropertyOfExpression(CObjectPropertyTermExpression* objPropTermExp);

				
				CRenderedItemLinker* getRenderedIndividualTermExpressions(CEXPRESSIONLIST<CIndividualTermExpression*>* indiTermExpList, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				CRenderedItemLinker* getRenderedIndividualTermExpression(CIndividualTermExpression* indiTermExp, CRenderedItemLinker* prependRenderedLinker = nullptr, CRenderedItemLinker* appendRenderedLinker = nullptr);
				
				
				CRenderedItemLinker* getRenderedClassExpression(CClassTermExpression* classTermExp);
				CRenderedItemLinker* getRenderedDatatypeExpression(CDataRangeTermExpression* dataRangeTermExp);
				CRenderedItemLinker* getRenderedObjectPropertyExpression(CObjectPropertyTermExpression* objPropTermExp);
				CRenderedItemLinker* getRenderedDataPropertyExpression(CDataPropertyTermExpression* dataPropTermExp);
				CRenderedItemLinker* getRenderedNamedIndividualExpression(CIndividualTermExpression* indiTermExp);
				CRenderedItemLinker* getRenderedAnonymousIndividualExpression(CIndividualTermExpression* indiTermExp);

				CRenderedItemLinker* getRenderedDataLiteralExpression(CDataRangeTermExpression* dataRangeTermExp);
			
				
				CRenderedItemLinker* getRenderedDataIntersectionOfExpression(CDataRangeTermExpression* dataRangeTermExp);
				CRenderedItemLinker* getRenderedDataUnionOfExpression(CDataRangeTermExpression* dataRangeTermExp);
				CRenderedItemLinker* getRenderedDataComplementOfExpression(CDataRangeTermExpression* dataRangeTermExp);
				CRenderedItemLinker* getRenderedDataOneOfExpression(CDataRangeTermExpression* dataRangeTermExp);
				CRenderedItemLinker* getRenderedDatatypeRestrictionExpression(CDataRangeTermExpression* dataRangeTermExp);

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

#endif // KONCLUDE_RENDERER_COntologyAxiomExpressionsRenderVisitor_H
