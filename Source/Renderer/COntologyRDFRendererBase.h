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

#ifndef KONCLUDE_RENDERER_CONTOLOGYRDFRENDERERBASE_H
#define KONCLUDE_RENDERER_CONTOLOGYRDFRENDERERBASE_H

// Libraries includes


// Namespace includes
#include "RendererSettings.h"
#include "COntologyRenderer.h"
#include "COntologyEmptyDefaultRenderer.h"
#include "CRenderedRDFTripleItemBaseLinker.h"


// Other includes
#include "Reasoner/Ontology/OntologySettings.h"


// Logger includes
#include "Logger/CLogger.h"

using namespace Konclude::Logger;

namespace Konclude {

	using namespace Reasoner::Ontology;

	namespace Renderer {



		/*! 
		*
		*		\class		COntologyRDFRendererBase
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyRDFRendererBase : public COntologyEmptyDefaultRenderer {
			// public methods
			public:
				//! Constructor
				COntologyRDFRendererBase(const QString& prefixNamespace = "owl:");

				//! Destructor
				virtual ~COntologyRDFRendererBase();


				virtual bool beginRender();
				virtual bool endRender(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker);



				virtual CRenderedItemLinker* endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker);

				virtual CRenderedItemLinker* endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker);





				virtual CRenderedItemLinker* endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker);


				virtual CRenderedItemLinker* endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker);
				virtual CRenderedItemLinker* endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality);
				virtual CRenderedItemLinker* endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet);




				virtual CRenderedItemLinker* endRenderClassExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderObjectPropertyExpression(const QString& name);

				virtual CRenderedItemLinker* endRenderDataPropertyExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderNamedIndividualExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderAnonymousIndividualExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderDatatypeExpression(const QString& name);
				virtual CRenderedItemLinker* endRenderDataLiteralExpression(const QString& name, const QString& datatype);


				virtual CRenderedRDFTripleItemBaseLinker* createRenderedTripleItemLinker() = 0;
				virtual COntologyRDFRendererBase* releaseRenderedTripleItemLinkers(CRenderedItemLinker* linkers) = 0;

				virtual CRenderedRDFNodeItemBaseLinker* createRenderedNodeItemLinker() = 0;
				virtual COntologyRDFRendererBase* releaseRenderedNodeItemLinkers(CRenderedItemLinker* linkers) = 0;



			// protected methods
			protected:


				virtual CRenderedRDFNodeItemBaseLinker* createRenderedIRIResourceNodeItem(const QString& iri) = 0;
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedBlankNodeItem(const QString& identifier) = 0;
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedBlankNodeItem(cint64 id) = 0;
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedLiteralNodeItem(const QString& literal, const QString& datatype) = 0;
				virtual CRenderedRDFTripleItemBaseLinker* createTriple(CRenderedRDFNodeItemBaseLinker* subjectNodeItem, CRenderedRDFNodeItemBaseLinker* predicateNodeItem, CRenderedRDFNodeItemBaseLinker* objectNodeItem) = 0;




				CRenderedRDFTripleItemBaseLinker* createTripleWithGivenPredicate(CRenderedRDFNodeItemBaseLinker* predicateNode, CRenderedItemLinker* renderedChildrenItemLinker);

				virtual CRenderedRDFTripleItemBaseLinker* createTriple(CRenderedRDFNodeItemBaseLinker* subjectNodeItem, CRenderedRDFNodeItemBaseLinker* predicateNodeItem, CRenderedRDFNodeItemBaseLinker* objectNodeItem, CRenderedRDFTripleItemBaseLinker* prevTriples);


				virtual CRenderedRDFNodeItemBaseLinker* createRenderedIRIResourceNodeItem(const QString& iri, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_TYPE type);
				virtual CRenderedRDFNodeItemBaseLinker* createRenderedBlankNodeItem(const QString& identifier, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_TYPE type);


				COntologyRDFRendererBase* initRenderedNodeItems();


				CRenderedRDFNodeItemBaseLinker* createListNode(CRenderedRDFNodeItemBaseLinker* nodeItemLinker);
				CRenderedRDFTripleItemBaseLinker* createRenderedPredicatePathTriples(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* predicateItem);

				CRenderedRDFTripleItemBaseLinker* createRenderedPairwiseOrMembersTriples(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* pairwisePredicateItem, CRenderedRDFNodeItemBaseLinker* membersTypeNodeItem);
				CRenderedRDFTripleItemBaseLinker* createRenderedNegativePropertyAssertionTriple(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* typeNodeItem, CRenderedRDFNodeItemBaseLinker* targetPredicate);
				CRenderedRDFNodeItemBaseLinker* createRenderedClassTermRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem);
				CRenderedRDFNodeItemBaseLinker* createRenderedClassTermOnPropertyRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem, CRenderedRDFNodeItemBaseLinker* propertyNodeItem);
				CRenderedRDFNodeItemBaseLinker* createRenderedClassTermObjectCardinalRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem, CRenderedRDFNodeItemBaseLinker* propertyNodeItem, cint64 cardinality);
				CRenderedRDFTripleItemBaseLinker* createRenderedPairwiseOrDistinctMembersTriples(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* pairwisePredicateItem, CRenderedRDFNodeItemBaseLinker* membersTypeNodeItem);

				CRenderedRDFTripleItemBaseLinker* createRenderedClassTermDataCardinalRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem, CRenderedRDFNodeItemBaseLinker* propertyNodeItem, cint64 cardinality);



			// protected variables
			protected:

				CRenderedRDFTripleItemBaseLinker* mAxiomRenderedItemLinker;
				CRenderedRDFTripleItemBaseLinker* mListRenderedItemLinker;
				CRenderedRDFTripleItemBaseLinker* mFacetRenderedItemLinker;
				CRenderedRDFTripleItemBaseLinker* mObjRenderedItemLinker;


				cint64 mNextAnonymousId = 0;

				CRenderedRDFNodeItemBaseLinker* mRDFSSubClassOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLEquivalentClassPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLDisjointWithPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mRDFTypeOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLAllDisjointClassesNode;
				CRenderedRDFNodeItemBaseLinker* mOWLNegativePropertyAssertionNode;
				CRenderedRDFNodeItemBaseLinker* mOWLAllDifferentNode;
				CRenderedRDFNodeItemBaseLinker* mOWLAllDisjointPropertiesNode;

				CRenderedRDFNodeItemBaseLinker* mOWLTransetiveNode;
				CRenderedRDFNodeItemBaseLinker* mOWLFunctionalNode;
				CRenderedRDFNodeItemBaseLinker* mOWLInverseFunctionalNode;
				CRenderedRDFNodeItemBaseLinker* mOWLReflexiveNode;
				CRenderedRDFNodeItemBaseLinker* mOWLIrreflexiveNode;
				CRenderedRDFNodeItemBaseLinker* mOWLSymmetricNode;
				CRenderedRDFNodeItemBaseLinker* mOWLAsymmetricNode;


				CRenderedRDFNodeItemBaseLinker* mOWLClassNode;
				CRenderedRDFNodeItemBaseLinker* mOWLRestrictionNode;
				CRenderedRDFNodeItemBaseLinker* mOWLObjectPropertyNode;
				CRenderedRDFNodeItemBaseLinker* mOWLDataPropertyNode;
				CRenderedRDFNodeItemBaseLinker* mOWLNamedIndividualNode;
				CRenderedRDFNodeItemBaseLinker* mRDFSDatatypeNode;

				CRenderedRDFNodeItemBaseLinker* mRDFFirstPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mRDFRestPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mRDFNilNode;


				CRenderedRDFNodeItemBaseLinker* mOWLMembersPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLDistinctMembersPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLDisjointUnionOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLSourceIndividualPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLTargetIndividualPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLTargetValuePredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLAssertionPropertyPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLSameAsPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLDifferentFromPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLEquivalentPropertyPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mRDFSSubPropertyPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLPropertyChainAxiomPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLPropertyDisjointWithPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mRDFSDomainPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mRDFSRangePredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLInverseOfPredicateNode;

				CRenderedRDFNodeItemBaseLinker* mOWLComplementOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLDataComplementOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLIntersectionOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLUnionOfPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLOneOfPredicateNode;

				CRenderedRDFNodeItemBaseLinker* mOWLOnPropertyPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLOnClassPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLOnDataRangePredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLOnDatatypePredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLWithRestrictionsPredicateNode;

				CRenderedRDFNodeItemBaseLinker* mOWLSomeValuesFromPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLAllValuesFromPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLHasValuePredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLHasSelfPredicateNode;

				CRenderedRDFNodeItemBaseLinker* mOWLMaxCardinalityPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLMinCardinalityPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLCardinalityPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLMaxQualifiedCardinalityPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLMinQualifiedCardinalityPredicateNode;
				CRenderedRDFNodeItemBaseLinker* mOWLQualifiedCardinalityPredicateNode;


			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Renderer

}; // end namespace Konclude

#endif // KONCLUDE_RENDERER_CONTOLOGYRDFRENDERERBASE_H
