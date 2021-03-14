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

#include "COntologyRDFRendererBase.h"


namespace Konclude {

	namespace Renderer {


		COntologyRDFRendererBase::COntologyRDFRendererBase(const QString& prefixNamespace) {
			mAxiomRenderedItemLinker = nullptr;
			mFacetRenderedItemLinker = nullptr;
			mListRenderedItemLinker = nullptr;
			mObjRenderedItemLinker = nullptr;
		}

		COntologyRDFRendererBase::~COntologyRDFRendererBase() {
		}



		bool COntologyRDFRendererBase::beginRender() {
			return true;
		}

		bool COntologyRDFRendererBase::endRender(CRenderedItemLinker* renderedItemLinker) {
			if (renderedItemLinker) {
				mAxiomRenderedItemLinker = (CRenderedRDFTripleItemBaseLinker*)renderedItemLinker->append(mAxiomRenderedItemLinker);
			}
			return true;
		}



		COntologyRDFRendererBase* COntologyRDFRendererBase::initRenderedNodeItems() {
			mRDFSSubClassOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDFS_SUBCLASS_OF);
			mOWLEquivalentClassPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_EQUIVALENT_CLASS);
			mOWLDisjointWithPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_DISJOINT_WITH);
			mRDFTypeOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDF_TYPE);
			mOWLAllDisjointClassesNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ALL_DISJOINT_CLASSES);
			mOWLNegativePropertyAssertionNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ALL_NEGATIVE_PROPERTY_ASSERTION);
			mOWLAllDifferentNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ALL_DIFFERENT);
			mOWLAllDisjointPropertiesNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ALL_DISJOINT_PROPERTIES);
			mOWLTransetiveNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_TRANSITIVE_PROPERTY);
			mOWLFunctionalNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_FUNCTIONAL_PROPERTY);
			mOWLInverseFunctionalNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_INVERSE_FUNCTIONAL_PROPERTY);
			mOWLReflexiveNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_REFLEXIVE_PROPERTY);
			mOWLIrreflexiveNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_IRREFLEXIVE_PROPERTY);
			mOWLSymmetricNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_SYMMETRIC_PROPERTY);
			mOWLAsymmetricNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ASYMMETRIC_PROPERTY);
			mOWLClassNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_CLASS);
			mOWLRestrictionNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_RESTRICTION);
			mOWLObjectPropertyNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_OBJECT_PROPERTY);
			mOWLDataPropertyNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_DATATYPE_PROPERTY);
			mOWLNamedIndividualNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_NAMED_INDIVIDUAL);
			mRDFSDatatypeNode = createRenderedIRIResourceNodeItem(PREFIX_RDFS_DATATYPE);
			mRDFFirstPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDF_FIRST);
			mRDFRestPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDF_REST);
			mRDFNilNode = createRenderedIRIResourceNodeItem(PREFIX_RDF_NIL);
			mOWLMembersPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_MEMBERS);
			mOWLDistinctMembersPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_DISTINCT_MEMBERS);
			mOWLDisjointUnionOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_DISJOINT_UNION_OF);
			mOWLSourceIndividualPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_SOURCE_INDIVIDUAL);
			mOWLTargetIndividualPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_TARGET_INDIVIDUAL);
			mOWLTargetValuePredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_TARGET_VALUE);
			mOWLAssertionPropertyPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ASSERTION_PROPERTY);
			mOWLSameAsPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_SAME_AS);
			mOWLDifferentFromPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_DIFFERENT_FROM);
			mOWLEquivalentPropertyPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_EQUIVALENT_PROPERTY);
			mRDFSSubPropertyPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDFS_SUBPROPERTY_OF);
			mOWLPropertyChainAxiomPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_PROPERTY_CHAIN_AXIOM);
			mOWLPropertyDisjointWithPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_PROPERTY_DISJOINT_WITH);
			mRDFSDomainPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDFS_DOMAIN);
			mRDFSRangePredicateNode = createRenderedIRIResourceNodeItem(PREFIX_RDFS_RANGE);
			mOWLInverseOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_INVERSE_OF);
			mOWLComplementOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_COMPLEMENT_OF);
			mOWLDataComplementOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_DATATYPE_COMPLEMENT_OF);
			mOWLIntersectionOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_INTERSECTION_OF);
			mOWLUnionOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_UNION_OF);
			mOWLOneOfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ONE_OF);
			mOWLOnPropertyPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ON_PROPERTY);
			mOWLOnClassPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ON_CLASS);
			mOWLOnDataRangePredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ON_DATA_RANGE);
			mOWLOnDatatypePredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ON_DATATYPE);
			mOWLWithRestrictionsPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_WITH_RESTRICTION);
			mOWLSomeValuesFromPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_SOME_VALUES_FROM);
			mOWLAllValuesFromPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_ALL_VALUES_FROM);
			mOWLHasValuePredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_HAS_VALUE);
			mOWLHasSelfPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_HAS_SELF);
			mOWLMaxCardinalityPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_MAX_CARDINALITY);
			mOWLMinCardinalityPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_MIN_CARDINALITY);
			mOWLCardinalityPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_CARDINALITY);
			mOWLMaxQualifiedCardinalityPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_MAX_QUALIFIED_CARDINALITY);
			mOWLMinQualifiedCardinalityPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_MIN_QUALIFIED_CARDINALITY);
			mOWLQualifiedCardinalityPredicateNode = createRenderedIRIResourceNodeItem(PREFIX_OWL_QUALIFIED_CARDINALITY);
			return this;
		}



		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createTripleWithGivenPredicate(CRenderedRDFNodeItemBaseLinker* predicateNodeItem, CRenderedItemLinker* renderedChildrenItemLinker) {
			CRenderedRDFNodeItemBaseLinker* subjectNodeItem = (CRenderedRDFNodeItemBaseLinker*)renderedChildrenItemLinker;
			CRenderedRDFNodeItemBaseLinker* objectNodeItem = (CRenderedRDFNodeItemBaseLinker*)renderedChildrenItemLinker->getNext();
			return createTriple(subjectNodeItem, predicateNodeItem, objectNodeItem);
		}



		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createTriple(CRenderedRDFNodeItemBaseLinker* subjectNodeItem, CRenderedRDFNodeItemBaseLinker* predicateNodeItem, CRenderedRDFNodeItemBaseLinker* objectNodeItem, CRenderedRDFTripleItemBaseLinker* prevTriples) {
			CRenderedRDFTripleItemBaseLinker* tripleLinker = createTriple(subjectNodeItem, predicateNodeItem, objectNodeItem);
			if (tripleLinker && prevTriples) {
				tripleLinker = (CRenderedRDFTripleItemBaseLinker*)tripleLinker->append(prevTriples);
			} else if (prevTriples) {
				tripleLinker = prevTriples;
			}
			return tripleLinker;
		}


		CRenderedRDFNodeItemBaseLinker* COntologyRDFRendererBase::createListNode(CRenderedRDFNodeItemBaseLinker* nodeItemLinker) {
			CRenderedRDFTripleItemBaseLinker* tripleLinker = nullptr;
			CRenderedRDFNodeItemBaseLinker* nodeItemLinkerIt = nodeItemLinker;
			CRenderedRDFNodeItemBaseLinker* firstListNode = nullptr;
			CRenderedRDFNodeItemBaseLinker* lastListNode = nullptr;
			while (nodeItemLinkerIt) {

				CRenderedRDFNodeItemBaseLinker* nextListNode = createRenderedBlankNodeItem(mNextAnonymousId++);

				if (lastListNode) {
					tripleLinker = (CRenderedRDFTripleItemBaseLinker*)createTriple(lastListNode, mRDFRestPredicateNode, nextListNode)->append(tripleLinker);
				}


				tripleLinker = (CRenderedRDFTripleItemBaseLinker*)createTriple(nextListNode, mRDFFirstPredicateNode, nodeItemLinkerIt)->append(tripleLinker);

				if (!firstListNode) {
					firstListNode = nextListNode;
				}

				lastListNode = nextListNode;

				nodeItemLinkerIt = nodeItemLinkerIt->getNextItemLinker();
			}


			if (lastListNode) {
				tripleLinker = (CRenderedRDFTripleItemBaseLinker*)createTriple(lastListNode, mRDFRestPredicateNode, mRDFNilNode)->append(tripleLinker);
			}

			if (tripleLinker) {
				mListRenderedItemLinker = (CRenderedRDFTripleItemBaseLinker*)tripleLinker->append(mListRenderedItemLinker);
			}

			return firstListNode;
		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderSubClassOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTripleWithGivenPredicate(mRDFSSubClassOfPredicateNode, renderedItemLinker);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDeclarationAxiom(CRenderedItemLinker* renderedItemLinker) {
			CRenderedRDFNodeItemBaseLinker* nodeItem = (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker;
			CRenderedRDFNodeItemBaseLinker::NODE_ITEM_TYPE type = nodeItem->getNodeItemType();
			CRenderedRDFNodeItemBaseLinker* typeNnodeItem = nullptr;
			if (type == CRenderedRDFNodeItemBaseLinker::NODE_ITEM_CLASS_TYPE) {
				typeNnodeItem = mOWLClassNode;
			} else if (type == CRenderedRDFNodeItemBaseLinker::NODE_ITEM_OBJECT_PROPERTY_TYPE) {
				typeNnodeItem = mOWLObjectPropertyNode;
			} else if (type == CRenderedRDFNodeItemBaseLinker::NODE_ITEM_DATA_PROPERTY_TYPE) {
				typeNnodeItem = mOWLDataPropertyNode;
			} else if (type == CRenderedRDFNodeItemBaseLinker::NODE_ITEM_INDIVIDUAL_PROPERTY_TYPE) {
				typeNnodeItem = mOWLNamedIndividualNode;
			} else if (type == CRenderedRDFNodeItemBaseLinker::NODE_ITEM_DATATYPE_TYPE) {
				typeNnodeItem = mRDFSDatatypeNode;
			}
			return createTriple(nodeItem, mRDFTypeOfPredicateNode, typeNnodeItem);
		}


		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createRenderedPredicatePathTriples(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* predicateItem) {
			CRenderedRDFTripleItemBaseLinker* tripleLinker = nullptr;
			for (CRenderedRDFNodeItemBaseLinker* nextNodeItem = (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker; nextNodeItem && nextNodeItem->hasNext(); nextNodeItem = nextNodeItem->getNextItemLinker()) {
				CRenderedRDFTripleItemBaseLinker* nextTripleLinker = createTripleWithGivenPredicate(predicateItem, nextNodeItem);
				tripleLinker = (CRenderedRDFTripleItemBaseLinker*)nextTripleLinker->append(tripleLinker);
			}
			return tripleLinker;
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderEquivalentClassesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedPredicatePathTriples(renderedItemLinker, mOWLEquivalentClassPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDisjointClassesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedPairwiseOrMembersTriples(renderedItemLinker, mOWLDisjointWithPredicateNode, mOWLAllDisjointClassesNode);
		}



		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createRenderedPairwiseOrMembersTriples(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* pairwisePredicateItem, CRenderedRDFNodeItemBaseLinker* membersTypeNodeItem) {
			if (renderedItemLinker && renderedItemLinker->hasNext()) {
				if (!renderedItemLinker->getNext()->hasNext()) {
					return createTripleWithGivenPredicate(pairwisePredicateItem, renderedItemLinker);
				} else {
					CRenderedRDFNodeItemBaseLinker* listNodeItem = createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
					CRenderedRDFNodeItemBaseLinker* membersNode = createRenderedBlankNodeItem(mNextAnonymousId++);
					CRenderedRDFTripleItemBaseLinker* memberTriple = createTriple(membersNode, mOWLMembersPredicateNode, listNodeItem);
					CRenderedRDFTripleItemBaseLinker* membersTypeTriple = createTriple(membersNode, mRDFTypeOfPredicateNode, membersTypeNodeItem, memberTriple);
					return membersTypeTriple;
				}
			}

			return nullptr;
		}


		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createRenderedPairwiseOrDistinctMembersTriples(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* pairwisePredicateItem, CRenderedRDFNodeItemBaseLinker* membersTypeNodeItem) {
			if (renderedItemLinker && renderedItemLinker->hasNext()) {
				if (!renderedItemLinker->getNext()->hasNext()) {
					return createTripleWithGivenPredicate(pairwisePredicateItem, renderedItemLinker);
				} else {
					CRenderedRDFNodeItemBaseLinker* listNodeItem = createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
					CRenderedRDFNodeItemBaseLinker* membersNode = createRenderedBlankNodeItem(mNextAnonymousId++);
					CRenderedRDFTripleItemBaseLinker* memberTriple = createTriple(membersNode, mOWLDistinctMembersPredicateNode, listNodeItem);
					CRenderedRDFTripleItemBaseLinker* membersTypeTriple = createTriple(membersNode, mRDFTypeOfPredicateNode, membersTypeNodeItem, memberTriple);
					return membersTypeTriple;
				}
			}

			return nullptr;
		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDisjontUnionAxiom(CRenderedItemLinker* renderedItemLinker) {
			CRenderedRDFNodeItemBaseLinker* listNodeItem = createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());

			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLDisjointUnionOfPredicateNode, listNodeItem);
		}






		CRenderedItemLinker* COntologyRDFRendererBase::endRenderClassAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext()->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderNegativeObjectPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			CRenderedRDFTripleItemBaseLinker* disjointTriple = createRenderedNegativePropertyAssertionTriple(renderedItemLinker, mOWLNegativePropertyAssertionNode, mOWLTargetIndividualPredicateNode);
			return disjointTriple;
		}


		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createRenderedNegativePropertyAssertionTriple(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* typeNodeItem, CRenderedRDFNodeItemBaseLinker* targetPredicate) {

			CRenderedRDFNodeItemBaseLinker* negPropAssNode = createRenderedBlankNodeItem(mNextAnonymousId++);
			CRenderedRDFTripleItemBaseLinker* targetTriple = createTriple(negPropAssNode, targetPredicate, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext()->getNext());
			CRenderedRDFTripleItemBaseLinker* propTriple = createTriple(negPropAssNode, mOWLAssertionPropertyPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), targetTriple);
			CRenderedRDFTripleItemBaseLinker* sourceTriple = createTriple(negPropAssNode, mOWLSourceIndividualPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, propTriple);
			CRenderedRDFTripleItemBaseLinker* disjointTriple = createTriple(negPropAssNode, mRDFTypeOfPredicateNode, typeNodeItem, sourceTriple);

			return disjointTriple;
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderSameIndividualAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedPredicatePathTriples(renderedItemLinker, mOWLSameAsPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDifferentIndividualsAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedPairwiseOrDistinctMembersTriples(renderedItemLinker, mOWLDifferentFromPredicateNode, mOWLAllDifferentNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderSubObjectPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			if (renderedItemLinker->getCount() == 2) {
				if (((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext())->getNodeItemType() == CRenderedRDFNodeItemBaseLinker::NODE_ITEM_PROPERTY_CHAIN_TYPE) {
					return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLPropertyChainAxiomPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
				} else {
					return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFSSubPropertyPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
				}
			} else if (renderedItemLinker->getCount() > 2) {
				CRenderedRDFNodeItemBaseLinker* listNodeItem = createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
				return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLPropertyChainAxiomPredicateNode, listNodeItem);
			} else {
				return nullptr;
			}
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectPropertyChainAxiom(CRenderedItemLinker* renderedItemLinker) {
			CRenderedRDFNodeItemBaseLinker* listNodeItem = createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
			listNodeItem->setNodeItemType(CRenderedRDFNodeItemBaseLinker::NODE_ITEM_PROPERTY_CHAIN_TYPE);
			return listNodeItem;
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderEquivalentObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLEquivalentPropertyPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDisjointObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedPairwiseOrMembersTriples(renderedItemLinker, mOWLPropertyDisjointWithPredicateNode, mOWLAllDisjointPropertiesNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFSDomainPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFSRangePredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderInverseObjectPropertyOf(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLInverseOfPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderInverseObjectPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLInverseOfPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderTransetiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLTransetiveNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLFunctionalNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderSymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLSymmetricNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderAsymmetricObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLAsymmetricNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderReflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLReflexiveNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderIrreflexiveObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLIrreflexiveNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderInverseFunctionalObjectPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLInverseFunctionalNode);
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext()->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderNegativeDataPropertyAssertionAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedNegativePropertyAssertionTriple(renderedItemLinker, mOWLNegativePropertyAssertionNode, mOWLTargetValuePredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderSubDataPropertyOfAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFSSubPropertyPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderEquivalentDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mOWLEquivalentPropertyPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDisjointDataPropertiesAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedPairwiseOrMembersTriples(renderedItemLinker, mOWLPropertyDisjointWithPredicateNode, mOWLAllDisjointPropertiesNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataPropertyDomainAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFSDomainPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataPropertyRangeAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFSRangePredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderFunctionalDataPropertyAxiom(CRenderedItemLinker* renderedItemLinker) {
			return createTriple((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, mRDFTypeOfPredicateNode, mOWLFunctionalNode);
		}



		CRenderedRDFNodeItemBaseLinker* COntologyRDFRendererBase::createRenderedClassTermRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem) {
			CRenderedRDFNodeItemBaseLinker* objClassNode = createRenderedBlankNodeItem(mNextAnonymousId++);
			CRenderedRDFTripleItemBaseLinker* relTriple = createTriple(objClassNode, relationPredicateItem, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
			CRenderedRDFTripleItemBaseLinker* objClassTypeTriple = createTriple(objClassNode, mRDFTypeOfPredicateNode, mOWLClassNode, relTriple);
			mObjRenderedItemLinker = (CRenderedRDFTripleItemBaseLinker*)objClassTypeTriple->append(mObjRenderedItemLinker);
			return objClassNode;
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(renderedItemLinker, mOWLComplementOfPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLIntersectionOfPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLUnionOfPredicateNode);
		}





		CRenderedRDFNodeItemBaseLinker* COntologyRDFRendererBase::createRenderedClassTermOnPropertyRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem, CRenderedRDFNodeItemBaseLinker* propertyNodeItem) {
			CRenderedRDFNodeItemBaseLinker* objClassNode = createRenderedBlankNodeItem(mNextAnonymousId++);
			CRenderedRDFTripleItemBaseLinker* relTriple = createTriple(objClassNode, relationPredicateItem, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
			CRenderedRDFTripleItemBaseLinker* propTriple = createTriple(objClassNode, mOWLOnPropertyPredicateNode, propertyNodeItem, relTriple);
			CRenderedRDFTripleItemBaseLinker* objClassTypeTriple = createTriple(objClassNode, mRDFTypeOfPredicateNode, mOWLRestrictionNode, propTriple);
			mObjRenderedItemLinker = (CRenderedRDFTripleItemBaseLinker*)objClassTypeTriple->append(mObjRenderedItemLinker);
			return objClassNode;
		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermOnPropertyRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLSomeValuesFromPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectAllValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermOnPropertyRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLAllValuesFromPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}




		CRenderedRDFNodeItemBaseLinker* COntologyRDFRendererBase::createRenderedClassTermObjectCardinalRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem, CRenderedRDFNodeItemBaseLinker* propertyNodeItem, cint64 cardinality) {
			CRenderedRDFNodeItemBaseLinker* objClassNode = createRenderedBlankNodeItem(mNextAnonymousId++);
			CRenderedRDFTripleItemBaseLinker* classTriple = nullptr;
			if (renderedItemLinker) {
				classTriple = createTriple(objClassNode, mOWLOnClassPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
			}
			CRenderedRDFNodeItemBaseLinker* cardNode = createRenderedLiteralNodeItem(QString::number(cardinality), PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE);
			CRenderedRDFTripleItemBaseLinker* relTriple = createTriple(objClassNode, relationPredicateItem, cardNode, classTriple);
			CRenderedRDFTripleItemBaseLinker* propTriple = createTriple(objClassNode, mOWLOnPropertyPredicateNode, propertyNodeItem, relTriple);
			CRenderedRDFTripleItemBaseLinker* objClassTypeTriple = createTriple(objClassNode, mRDFTypeOfPredicateNode, mOWLRestrictionNode, propTriple);
			mObjRenderedItemLinker = (CRenderedRDFTripleItemBaseLinker*)objClassTypeTriple->append(mObjRenderedItemLinker);
			return objClassNode;
		}




		CRenderedRDFTripleItemBaseLinker* COntologyRDFRendererBase::createRenderedClassTermDataCardinalRelationExpression(CRenderedItemLinker* renderedItemLinker, CRenderedRDFNodeItemBaseLinker* relationPredicateItem, CRenderedRDFNodeItemBaseLinker* propertyNodeItem, cint64 cardinality) {
			CRenderedRDFNodeItemBaseLinker* objClassNode = createRenderedBlankNodeItem(mNextAnonymousId++);
			CRenderedRDFTripleItemBaseLinker* classTriple = nullptr;
			if (renderedItemLinker) {
				classTriple = createTriple(objClassNode, mOWLOnDataRangePredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
			}
			CRenderedRDFNodeItemBaseLinker* cardNode = createRenderedLiteralNodeItem(QString::number(cardinality), PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE);
			CRenderedRDFTripleItemBaseLinker* relTriple = createTriple(objClassNode, relationPredicateItem, cardNode, classTriple);
			CRenderedRDFTripleItemBaseLinker* propTriple = createTriple(objClassNode, mOWLOnPropertyPredicateNode, propertyNodeItem, relTriple);
			CRenderedRDFTripleItemBaseLinker* objClassTypeTriple = createTriple(objClassNode, mRDFTypeOfPredicateNode, mOWLRestrictionNode, propTriple);
			return objClassTypeTriple;
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			if (!renderedItemLinker->hasNext()) {
				return createRenderedClassTermObjectCardinalRelationExpression(nullptr, mOWLMaxCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			} else {
				return createRenderedClassTermObjectCardinalRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLMaxQualifiedCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			}
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			if (!renderedItemLinker->hasNext()) {
				return createRenderedClassTermObjectCardinalRelationExpression(nullptr, mOWLMinCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			} else {
				return createRenderedClassTermObjectCardinalRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLMinQualifiedCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			}
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			if (!renderedItemLinker->hasNext()) {
				return createRenderedClassTermObjectCardinalRelationExpression(nullptr, mOWLCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			} else {
				return createRenderedClassTermObjectCardinalRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLQualifiedCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			}
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectOneOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLOneOfPredicateNode);
		}




		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectHasValueExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermOnPropertyRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLHasValuePredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectHasSelfExpression(CRenderedItemLinker* renderedItemLinker) {
			CRenderedRDFNodeItemBaseLinker* trueNode = createRenderedLiteralNodeItem("true", PREFIX_XML_BOOLEAN_DATATYPE);
			return createRenderedClassTermOnPropertyRelationExpression(trueNode, mOWLHasSelfPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}



		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataAllValuesExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermOnPropertyRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLAllValuesFromPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataSomeValuesFromExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermOnPropertyRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLSomeValuesFromPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataHasValueExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermOnPropertyRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLHasValuePredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataIntersectionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLIntersectionOfPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataOneOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLOneOfPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataUnionOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLUnionOfPredicateNode);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataComplementOfExpression(CRenderedItemLinker* renderedItemLinker) {
			return createRenderedClassTermRelationExpression(createListNode((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker), mOWLDataComplementOfPredicateNode);
		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDatatypeRestrictionExpression(CRenderedItemLinker* renderedItemLinker) {
			CRenderedRDFNodeItemBaseLinker* objClassNode = createRenderedBlankNodeItem(mNextAnonymousId++);
			CRenderedRDFTripleItemBaseLinker* relTriple = createTriple(objClassNode, mOWLWithRestrictionsPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext());
			CRenderedRDFTripleItemBaseLinker* propTriple = createTriple(objClassNode, mOWLOnDatatypePredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, relTriple);
			CRenderedRDFTripleItemBaseLinker* objClassTypeTriple = createTriple(objClassNode, mRDFTypeOfPredicateNode, mRDFSDatatypeNode, propTriple);
			return objClassTypeTriple;

		}


		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataMaxCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			if (!renderedItemLinker->hasNext()) {
				return createRenderedClassTermDataCardinalRelationExpression(nullptr, mOWLMaxCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			} else {
				return createRenderedClassTermDataCardinalRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLMaxQualifiedCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			}
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataMinCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			if (!renderedItemLinker->hasNext()) {
				return createRenderedClassTermDataCardinalRelationExpression(nullptr, mOWLMinCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			} else {
				return createRenderedClassTermDataCardinalRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLMinQualifiedCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			}
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataExactCardinalityExpression(CRenderedItemLinker* renderedItemLinker, cint64 cardinality) {
			if (!renderedItemLinker->hasNext()) {
				return createRenderedClassTermDataCardinalRelationExpression(nullptr, mOWLCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			} else {
				return createRenderedClassTermDataCardinalRelationExpression((CRenderedRDFNodeItemBaseLinker*)renderedItemLinker->getNext(), mOWLQualifiedCardinalityPredicateNode, (CRenderedRDFNodeItemBaseLinker*)renderedItemLinker, cardinality);
			}
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataFacetRestrictionExpression(CRenderedItemLinker* renderedItemLinker, const QString& facet) {
			cint64 resCount = renderedItemLinker->getCount() / 2;
			CRenderedRDFNodeItemBaseLinker* blankNodeLinker = nullptr;
			for (cint64 i = 0; i < resCount; ++i) {
				CRenderedRDFNodeItemBaseLinker* nextBlankNodeLinker = createRenderedBlankNodeItem(mNextAnonymousId++);
				blankNodeLinker = (CRenderedRDFNodeItemBaseLinker*)nextBlankNodeLinker->append(blankNodeLinker);
			}
			CRenderedRDFNodeItemBaseLinker* listNode = createListNode(blankNodeLinker);

			CRenderedItemLinker* renderedItemLinkerIt = renderedItemLinker;
			CRenderedRDFNodeItemBaseLinker* blankNodeLinkerIt = blankNodeLinker;
			while (renderedItemLinkerIt && renderedItemLinkerIt->hasNext() && blankNodeLinkerIt) {

				CRenderedRDFNodeItemBaseLinker* propNodeItem = (CRenderedRDFNodeItemBaseLinker*)renderedItemLinkerIt;
				CRenderedRDFNodeItemBaseLinker* litNodeItem = (CRenderedRDFNodeItemBaseLinker*)renderedItemLinkerIt->getNext();

				CRenderedRDFTripleItemBaseLinker* restTriple = createTriple(blankNodeLinkerIt, propNodeItem, litNodeItem);
				mFacetRenderedItemLinker = (CRenderedRDFTripleItemBaseLinker*)restTriple->append(mFacetRenderedItemLinker);

				blankNodeLinkerIt = blankNodeLinkerIt->getNextItemLinker();
				renderedItemLinkerIt = renderedItemLinkerIt->getNext()->getNext();
			}
			return listNode;
		}






		CRenderedItemLinker* COntologyRDFRendererBase::endRenderClassExpression(const QString& name) {
			return createRenderedIRIResourceNodeItem(name, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_CLASS_TYPE);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderObjectPropertyExpression(const QString& name) {
			return createRenderedIRIResourceNodeItem(name, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_OBJECT_PROPERTY_TYPE);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataPropertyExpression(const QString& name) {
			return createRenderedIRIResourceNodeItem(name, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_DATA_PROPERTY_TYPE);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderNamedIndividualExpression(const QString& name) {
			return createRenderedIRIResourceNodeItem(name, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_INDIVIDUAL_PROPERTY_TYPE);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderAnonymousIndividualExpression(const QString& name) {
			return createRenderedBlankNodeItem(name, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_INDIVIDUAL_PROPERTY_TYPE);
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDatatypeExpression(const QString& name) {
			CRenderedRDFNodeItemBaseLinker* item = createRenderedIRIResourceNodeItem(name);
			item->setNodeItemType(CRenderedRDFNodeItemBaseLinker::NODE_ITEM_DATATYPE_TYPE);
			return item;
		}

		CRenderedItemLinker* COntologyRDFRendererBase::endRenderDataLiteralExpression(const QString& name, const QString& datatype) {
			CRenderedRDFNodeItemBaseLinker* item = createRenderedLiteralNodeItem(name, datatype);
			item->setNodeItemType(CRenderedRDFNodeItemBaseLinker::NODE_ITEM_LITERAL_TYPE);
			return item;
		}


		CRenderedRDFNodeItemBaseLinker* COntologyRDFRendererBase::createRenderedIRIResourceNodeItem(const QString& iri, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_TYPE type) {
			CRenderedRDFNodeItemBaseLinker* item = createRenderedIRIResourceNodeItem(iri);
			item->setNodeItemType(type);
			return item;
		}


		CRenderedRDFNodeItemBaseLinker* COntologyRDFRendererBase::createRenderedBlankNodeItem(const QString& identifier, CRenderedRDFNodeItemBaseLinker::NODE_ITEM_TYPE type) {
			CRenderedRDFNodeItemBaseLinker* item = createRenderedBlankNodeItem(identifier);
			item->setNodeItemType(type);
			return item;
		}

	}; // end namespace Renderer

}; // end namespace Konclude
