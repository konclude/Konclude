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
#include "COntologyAxiomExpressionsRenderVisitor.h"


namespace Konclude {

	namespace Renderer {


		COntologyAxiomExpressionsRenderVisitor::COntologyAxiomExpressionsRenderVisitor() {
		}






		bool COntologyAxiomExpressionsRenderVisitor::visitAndRender(const CQtList<CAxiomExpression*>& axiomList, CConcreteOntology* ontology, COntologyRenderer* renderer) {
			mOntology = ontology;
			mRenderer = renderer;
			COntologyBuildData* buildDataBox = ontology->getBuildData();
			mConceptClassTermMapHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();
			mClassTermExpClassAxiomHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();

			CRenderedItemLinker* renderedLinker = nullptr;
			CRenderedItemLinker* lastRenderedLinker = nullptr;
			mRenderer->beginRender();
			foreach (CAxiomExpression* axiomExpression, axiomList) {
				CRenderedItemLinker* tmpRenderedLinker = getRenderedAxiomExpression(axiomExpression);
				if (!renderedLinker) {
					renderedLinker = tmpRenderedLinker;
					lastRenderedLinker = renderedLinker->getLastListLink();
				}
				else {
					lastRenderedLinker->append(tmpRenderedLinker);
					lastRenderedLinker = tmpRenderedLinker->getLastListLink();
				}
			}
			mRenderer->endRender(renderedLinker);
			return true;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedAxiomExpression(CAxiomExpression* axiomExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			CBuildExpression::ExpressionType type = axiomExp->getType();

			if (type == CBuildExpression::BETDECLARATION) {
				tmpRenderedLinker = getRenderedDeclarationAxiom(axiomExp);

			} else if(type == CBuildExpression::BETESUBCLASSOF) {
				tmpRenderedLinker = getRenderedSubClassOfAxiom(axiomExp);
			} else if (type == CBuildExpression::BETEQUIVALENTCLASSES) {
				tmpRenderedLinker = getRenderedEquivalentClassesAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDISJOINTUNION) {
				tmpRenderedLinker = getRenderedDisjointUnionAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDISJOINTCLASSES) {
				tmpRenderedLinker = getRenderedDisjointClassesAxiom(axiomExp);

			} else if (type == CBuildExpression::BETSUBOBJECTPROPERTYOF) {
				tmpRenderedLinker = getRenderedSubObjectPropertyOfAxiom(axiomExp);
			} else if (type == CBuildExpression::BETOBJECTPROPERTYDOMAIN) {
				tmpRenderedLinker = getRenderedObjectPropertyDomainAxiom(axiomExp);
			} else if (type == CBuildExpression::BETOBJECTPROPERTYRANGE) {
				tmpRenderedLinker = getRenderedObjectPropertyRangeAxiom(axiomExp);
			} else if (type == CBuildExpression::BETFUNCTIONALOBJECTPROPERTY) {
				tmpRenderedLinker = getRenderedFunctionalObjectPropertyAxiom(axiomExp);
			} else if (type == CBuildExpression::BETSYMMETRICPROPERTY) {
				tmpRenderedLinker = getRenderedSymmetricObjectPropertyAxiom(axiomExp);
			} else if (type == CBuildExpression::BETASYMMETRICPROPERTY) {
				tmpRenderedLinker = getRenderedAsymmetricObjectPropertyAxiom(axiomExp);
			} else if (type == CBuildExpression::BETTRANSITIVEOBJECTPROPERTY) {
				tmpRenderedLinker = getRenderedTransetiveObjectPropertyAxiom(axiomExp);
			} else if (type == CBuildExpression::BETINVERSEOBJECTPROPERTIES) {
				tmpRenderedLinker = getRenderedInverseObjectPropertiesAxiom(axiomExp);
			} else if (type == CBuildExpression::BETEQUIVALENTOBJECTPROPERTIES) {
				tmpRenderedLinker = getRenderedEquivalentObjectPropertiesAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDISJOINTOBJECTPROPERTIES) {
				tmpRenderedLinker = getRenderedDisjointObjectPropertiesAxiom(axiomExp);
			} else if (type == CBuildExpression::BETREFLEXIVEPROPERTY) {
				tmpRenderedLinker = getRenderedReflexiveObjectPropertyAxiom(axiomExp);
			} else if (type == CBuildExpression::BETIRREFLEXIVEPROPERTY) {
				tmpRenderedLinker = getRenderedIrreflexiveObjectPropertyAxiom(axiomExp);

			} else if (type == CBuildExpression::BETDATAPROPERTYDOMAIN) {
				tmpRenderedLinker = getRenderedDataPropertyDomainAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDATAPROPERTYRANGE) {
				tmpRenderedLinker = getRenderedDataPropertyRangeAxiom(axiomExp);
			} else if (type == CBuildExpression::BETSUBDATAPROPERTYOF) {
				tmpRenderedLinker = getRenderedSubDataPropertyOfAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDISJOINTDATAPROPERTIES) {
				tmpRenderedLinker = getRenderedDisjointDataPropertiesAxiom(axiomExp);
			} else if (type == CBuildExpression::BETEQUIVALENTDATAPROPERTIES) {
				tmpRenderedLinker = getRenderedEquivalentDataPropertiesAxiom(axiomExp);
			} else if (type == CBuildExpression::BETFUNCTIONALDATAPROPERTY) {
				tmpRenderedLinker = getRenderedFunctionalDataPropertyAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDATAPROPERTYASSERTION) {
				tmpRenderedLinker = getRenderedDataPropertyAssertionAxiom(axiomExp);
			} else if (type == CBuildExpression::BETNEGATIVEDATAPROPERTYASSERTION) {
				tmpRenderedLinker = getRenderedNegativeDataPropertyAssertionAxiom(axiomExp);


			} else if (type == CBuildExpression::BETOBJECTPROPERTYASSERTION) {
				tmpRenderedLinker = getRenderedObjectPropertyAssertionAxiom(axiomExp);
			} else if (type == CBuildExpression::BETNEGATIVEOBJECTPROPERTYASSERTION) {
				tmpRenderedLinker = getRenderedNegativeObjectPropertyAssertionAxiom(axiomExp);
			} else if (type == CBuildExpression::BETCLASSASSERTION) {
				tmpRenderedLinker = getRenderedClassAssertionAxiom(axiomExp);
			} else if (type == CBuildExpression::BETSAMEINDIVIDUAL) {
				tmpRenderedLinker = getRenderedSameIndividualAxiom(axiomExp);
			} else if (type == CBuildExpression::BETDIFFERENTINDIVIDUAL) {
				tmpRenderedLinker = getRenderedDifferentIndividualsAxiom(axiomExp);

			}
			return appendPrepandRenderedItemLinker(tmpRenderedLinker, prependRenderedLinker, appendRenderedLinker);
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDifferentIndividualsAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDifferentIndividualsAxiom()) {
				CDifferentIndividualsExpression* diffIndExp = (CDifferentIndividualsExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpressions(diffIndExp->getIndividualTermExpressionList());
				renderedLinker = mRenderer->endRenderDifferentIndividualsAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedSameIndividualAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderSameIndividualAxiom()) {
				CSameIndividualExpression* sameIndExp = (CSameIndividualExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpressions(sameIndExp->getIndividualTermExpressionList());
				renderedLinker = mRenderer->endRenderSameIndividualAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedClassAssertionAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderClassAssertionAxiom()) {
				CClassAssertionExpression* classAssExp = (CClassAssertionExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpression(classAssExp->getIndividualTermExpression());
				renderedLinker = getRenderedClassTermExpression(classAssExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderClassAssertionAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedNegativeObjectPropertyAssertionAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderNegativeObjectPropertyAssertionAxiom()) {
				CNegativeObjectPropertyAssertionExpression* negObjPropAssExp = (CNegativeObjectPropertyAssertionExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpression(negObjPropAssExp->getFirstIndividualTermExpression());
				renderedLinker = getRenderedObjectPropertyTermExpression(negObjPropAssExp->getObjectPropertyTermExpression(), renderedLinker);
				renderedLinker = getRenderedIndividualTermExpression(negObjPropAssExp->getSecondIndividualTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderNegativeObjectPropertyAssertionAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyAssertionAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectPropertyAssertionAxiom()) {
				CObjectPropertyAssertionExpression* objPropAssExp = (CObjectPropertyAssertionExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpression(objPropAssExp->getFirstIndividualTermExpression());
				renderedLinker = getRenderedObjectPropertyTermExpression(objPropAssExp->getObjectPropertyTermExpression(), renderedLinker);
				renderedLinker = getRenderedIndividualTermExpression(objPropAssExp->getSecondIndividualTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectPropertyAssertionAxiom(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedNegativeDataPropertyAssertionAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderNegativeDataPropertyAssertionAxiom()) {
				CNegativeDataPropertyAssertionExpression* negDataPropAssExp = (CNegativeDataPropertyAssertionExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpression(negDataPropAssExp->getIndividualTermExpression());
				renderedLinker = getRenderedDataPropertyTermExpression(negDataPropAssExp->getDataPropertyTermExpression(), renderedLinker);
				renderedLinker = getRenderedDataRangeTermExpression(negDataPropAssExp->getDataLiteralTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderNegativeDataPropertyAssertionAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataPropertyAssertionAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataPropertyAssertionAxiom()) {
				CDataPropertyAssertionExpression* dataPropAssExp = (CDataPropertyAssertionExpression*)axiomExp;
				renderedLinker = getRenderedIndividualTermExpression(dataPropAssExp->getIndividualTermExpression());
				renderedLinker = getRenderedDataPropertyTermExpression(dataPropAssExp->getDataPropertyTermExpression(), renderedLinker);
				renderedLinker = getRenderedDataRangeTermExpression(dataPropAssExp->getDataLiteralTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataPropertyAssertionAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedFunctionalDataPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderFunctionalDataPropertyAxiom()) {
				CFunctionalDataPropertyExpression* funDataPropExp = (CFunctionalDataPropertyExpression*)axiomExp;
				renderedLinker = getRenderedDataPropertyTermExpression(funDataPropExp->getFunctionalDataPropertyExpression());
				renderedLinker = mRenderer->endRenderFunctionalDataPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedEquivalentDataPropertiesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderEquivalentDataPropertiesAxiom()) {
				CEquivalentDataPropertiesExpression* eqDataPropExp = (CEquivalentDataPropertiesExpression*)axiomExp;
				renderedLinker = getRenderedDataPropertyTermExpressions(eqDataPropExp->getDataPropertyTermExpressionList());
				renderedLinker = mRenderer->endRenderEquivalentDataPropertiesAxiom(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDisjointDataPropertiesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDisjointDataPropertiesAxiom()) {
				CDisjointDataPropertiesExpression* disjDataPropExp = (CDisjointDataPropertiesExpression*)axiomExp;
				renderedLinker = getRenderedDataPropertyTermExpressions(disjDataPropExp->getDataPropertyTermExpressionList());
				renderedLinker = mRenderer->endRenderDisjointDataPropertiesAxiom(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedSubDataPropertyOfAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderSubDataPropertyOfAxiom()) {
				CSubDataPropertyOfExpression* subDataPropExp = (CSubDataPropertyOfExpression*)axiomExp;
				renderedLinker = getRenderedDataPropertyTermExpression(subDataPropExp->getSubDataPropertyTermExpression());
				renderedLinker = getRenderedDataPropertyTermExpression(subDataPropExp->getSuperDataPropertyTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderSubDataPropertyOfAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataPropertyRangeAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataPropertyRangeAxiom()) {
				CDataPropertyRangeExpression* rangeDataPropExp = (CDataPropertyRangeExpression*)axiomExp;
				renderedLinker = getRenderedDataPropertyTermExpression(rangeDataPropExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(rangeDataPropExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataPropertyRangeAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataPropertyDomainAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataPropertyDomainAxiom()) {
				CDataPropertyDomainExpression* domainDataPropExp = (CDataPropertyDomainExpression*)axiomExp;
				renderedLinker = getRenderedDataPropertyTermExpression(domainDataPropExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(domainDataPropExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataPropertyDomainAxiom(renderedLinker);
			}
			return renderedLinker;
		}













		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedInverseFunctionalObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderInverseFunctionalObjectPropertyAxiom()) {
				CInverseFunctionalObjectPropertyExpression* invFuncObjectPropExp = (CInverseFunctionalObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(invFuncObjectPropExp->getInverseFunctionalObjectPropertyExpression());
				renderedLinker = mRenderer->endRenderInverseFunctionalObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedIrreflexiveObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderIrreflexiveObjectPropertyAxiom()) {
				CIrreflexiveObjectPropertyExpression* irreflObjectPropExp = (CIrreflexiveObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(irreflObjectPropExp->getIreflexiveObjectPropertyExpression());
				renderedLinker = mRenderer->endRenderIrreflexiveObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedReflexiveObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderReflexiveObjectPropertyAxiom()) {
				CReflexiveObjectPropertyExpression* reflObjectPropExp = (CReflexiveObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(reflObjectPropExp->getReflexiveObjectPropertyExpression());
				renderedLinker = mRenderer->endRenderReflexiveObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDisjointObjectPropertiesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDisjointObjectPropertiesAxiom()) {
				CDisjointObjectPropertiesExpression* disjObjectPropExp = (CDisjointObjectPropertiesExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpressions(disjObjectPropExp->getObjectPropertyTermExpressionList());
				renderedLinker = mRenderer->endRenderDisjointObjectPropertiesAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedEquivalentObjectPropertiesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderInverseObjectPropertiesAxiom()) {
				CEquivalentObjectPropertiesExpression* equivObjectPropExp = (CEquivalentObjectPropertiesExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpressions(equivObjectPropExp->getObjectPropertyTermExpressionList());
				renderedLinker = mRenderer->endRenderInverseObjectPropertiesAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedInverseObjectPropertiesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderInverseObjectPropertiesAxiom()) {
				CInverseObjectPropertiesExpression* invObjectPropExp = (CInverseObjectPropertiesExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(invObjectPropExp->getFirstObjectPropertyTermExpression());
				renderedLinker = getRenderedObjectPropertyTermExpression(invObjectPropExp->getSecondObjectPropertyTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderInverseObjectPropertiesAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyRangeAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectPropertyRangeAxiom()) {
				CObjectPropertyRangeExpression* rangeObjectPropExp = (CObjectPropertyRangeExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(rangeObjectPropExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(rangeObjectPropExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectPropertyRangeAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyDomainAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectPropertyDomainAxiom()) {
				CObjectPropertyDomainExpression* domainObjectPropExp = (CObjectPropertyDomainExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(domainObjectPropExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(domainObjectPropExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectPropertyDomainAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedSubObjectPropertyOfAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderSubObjectPropertyOfAxiom()) {
				CSubObjectPropertyOfExpression* subObjectPropOfExp = (CSubObjectPropertyOfExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpressions(subObjectPropOfExp->getSubObjectPropertyTermExpressionList());
				renderedLinker = getRenderedObjectPropertyTermExpression(subObjectPropOfExp->getSuperObjectPropertyTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderSubObjectPropertyOfAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedFunctionalObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderFunctionalObjectPropertyAxiom()) {
				CFunctionalObjectPropertyExpression* funcObjectPropExp = (CFunctionalObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(funcObjectPropExp->getFunctionalObjectPropertyExpression());
				renderedLinker = mRenderer->endRenderFunctionalObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedSymmetricObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderSymmetricObjectPropertyAxiom()) {
				CSymmetricObjectPropertyExpression* symmObjectPropExp = (CSymmetricObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(symmObjectPropExp->getSymmetricObjectPropertyExpression());
				renderedLinker = mRenderer->endRenderSymmetricObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedAsymmetricObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderAsymmetricObjectPropertyAxiom()) {
				CAsymmetricObjectPropertyExpression* asymmObjectPropExp = (CAsymmetricObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(asymmObjectPropExp->getAsymmetricObjectPropertyExpression());
				renderedLinker = mRenderer->endRenderAsymmetricObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedTransetiveObjectPropertyAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderTransetiveObjectPropertyAxiom()) {
				CTransetiveObjectPropertyExpression* transObjectPropExp = (CTransetiveObjectPropertyExpression*)axiomExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(transObjectPropExp->getTransetivePropertyExpression());
				renderedLinker = mRenderer->endRenderTransetiveObjectPropertyAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDisjointClassesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDisjointClassesAxiom()) {
				CDisjointClassesExpression* disjClassesExp = (CDisjointClassesExpression*)axiomExp;
				renderedLinker = getRenderedClassTermExpressions(disjClassesExp->getClassTermExpressionList());
				renderedLinker = mRenderer->endRenderDisjointClassesAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDisjointUnionAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDisjontUnionAxiom()) {
				CDisjointUnionExpression* disjUnionExp = (CDisjointUnionExpression*)axiomExp;
				renderedLinker = getRenderedClassTermExpression(disjUnionExp->getClassExpression());
				renderedLinker = getRenderedClassTermExpressions(disjUnionExp->getClassTermExpressionList(), renderedLinker);
				renderedLinker = mRenderer->endRenderDisjontUnionAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedEquivalentClassesAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderEquivalentClassesAxiom()) {
				CEquivalentClassesExpression* equivClassesExp = (CEquivalentClassesExpression*)axiomExp;
				renderedLinker = getRenderedClassTermExpressions(equivClassesExp->getClassTermExpressionList());
				renderedLinker = mRenderer->endRenderEquivalentClassesAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedSubClassOfAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderSubClassOfAxiom()) {
				CSubClassOfExpression* subClassOfExp = (CSubClassOfExpression*)axiomExp;
				renderedLinker = getRenderedClassTermExpression(subClassOfExp->getSubClassTermExpression(), renderedLinker);
				renderedLinker = getRenderedClassTermExpression(subClassOfExp->getSuperClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderSubClassOfAxiom(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDeclarationAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDeclarationAxiom()) {
				CDeclarationAxiomExpression* declExp = (CDeclarationAxiomExpression*)axiomExp;
				CExpressionEntity* entExp = declExp->getEntity();
				if (dynamic_cast<CNamedIndividualExpression*>(entExp)) {
					renderedLinker = getRenderedIndividualTermExpression((CNamedIndividualExpression*)entExp);
				} else if (dynamic_cast<CAnonymousIndividualExpression*>(entExp)) {
					renderedLinker = getRenderedIndividualTermExpression((CAnonymousIndividualExpression*)entExp);
				} else if (dynamic_cast<CClassExpression*>(entExp)) {
					renderedLinker = getRenderedClassTermExpression((CClassExpression*)entExp);
				} else if (dynamic_cast<CObjectPropertyExpression*>(entExp)) {
					renderedLinker = getRenderedObjectPropertyTermExpression((CObjectPropertyExpression*)entExp);
				} else if (dynamic_cast<CDataPropertyExpression*>(entExp)) {
					renderedLinker = getRenderedDataPropertyTermExpression((CDataPropertyExpression*)entExp);
				} else if (dynamic_cast<CDatatypeExpression*>(entExp)) {
					renderedLinker = getRenderedDataRangeTermExpression((CDatatypeExpression*)entExp);
				}
				renderedLinker = mRenderer->endRenderDeclarationAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedClassTermExpressions(CEXPRESSIONLIST<CClassTermExpression*>* classTermExpList, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			foreach(CClassTermExpression* nextClassTermExp, *classTermExpList) {
				tmpRenderedLinker = getRenderedClassTermExpression(nextClassTermExp, tmpRenderedLinker);
			}
			return tmpRenderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedClassTermExpression(CClassTermExpression* classTermExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			CBuildExpression::ExpressionType type = classTermExp->getType();
			if (type == CBuildExpression::BETCLASS) {
				tmpRenderedLinker = getRenderedClassExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTINTERSECTION) {
				tmpRenderedLinker = getRenderedObjectIntersectionOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTUNION) {
				tmpRenderedLinker = getRenderedObjectUnionOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTCOMPLEMENTOF) {
				tmpRenderedLinker = getRenderedObjectComplementOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTSOMEVALUEFROM) {
				tmpRenderedLinker = getRenderedObjectSomeValuesFromExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTALLVALUEFROM) {
				tmpRenderedLinker = getRenderedObjectAllValuesFromExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTEXACTLYCARDINALITY) {
				tmpRenderedLinker = getRenderedObjectExactCardinalityExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTMINCARDINALITY) {
				tmpRenderedLinker = getRenderedObjectMinCardinalityExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTMAXCARDINALITY) {
				tmpRenderedLinker = getRenderedObjectMaxCardinalityExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTHASSELF) {
				tmpRenderedLinker = getRenderedObjectHasSelfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTONEOF) {
				tmpRenderedLinker = getRenderedObjectOneOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTHASVALUE) {
				tmpRenderedLinker = getRenderedObjectHasValueFromExpression(classTermExp);

			} else if (type == CBuildExpression::BETDATASOMEVALUEFROM) {
				tmpRenderedLinker = getRenderedObjectDataSomeValueFromExpression(classTermExp);
			} else if (type == CBuildExpression::BETDATAHASVALUE) {
				tmpRenderedLinker = getRenderedObjectDataHasValueExpression(classTermExp);
			} else if (type == CBuildExpression::BETDATAALLVALUEFROM) {
				tmpRenderedLinker = getRenderedObjectDataAllValuesFromExpression(classTermExp);
			} else if (type == CBuildExpression::BETDATAMINCARDINALITY) {
				tmpRenderedLinker = getRenderedObjectDataMinCardinalityExpression(classTermExp);
			} else if (type == CBuildExpression::BETDATAMAXCARDINALITY) {
				tmpRenderedLinker = getRenderedObjectDataMaxCardinalityExpression(classTermExp);
			} else if (type == CBuildExpression::BETDATAEXACTCARDINALITY) {
				tmpRenderedLinker = getRenderedObjectDataExactCardinalityExpression(classTermExp);

			}
			return appendPrepandRenderedItemLinker(tmpRenderedLinker,prependRenderedLinker,appendRenderedLinker);
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectDataExactCardinalityExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CDataExactCardinalityExpression* dataExactCardExp = (CDataExactCardinalityExpression*)classTermExp;
			cint64 cardinality = dataExactCardExp->getCardinality();
			if (mRenderer->beginRenderDataExactCardinalityExpression(cardinality)) {
				renderedLinker = getRenderedDataPropertyTermExpression(dataExactCardExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(dataExactCardExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataExactCardinalityExpression(renderedLinker, cardinality);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectDataMaxCardinalityExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CDataMaxCardinalityExpression* dataMaxCardExp = (CDataMaxCardinalityExpression*)classTermExp;
			cint64 cardinality = dataMaxCardExp->getCardinality();
			if (mRenderer->beginRenderDataMaxCardinalityExpression(cardinality)) {
				renderedLinker = getRenderedDataPropertyTermExpression(dataMaxCardExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(dataMaxCardExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataMaxCardinalityExpression(renderedLinker, cardinality);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectDataMinCardinalityExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CDataMinCardinalityExpression* dataMinCardExp = (CDataMinCardinalityExpression*)classTermExp;
			cint64 cardinality = dataMinCardExp->getCardinality();
			if (mRenderer->beginRenderDataMinCardinalityExpression(cardinality)) {
				renderedLinker = getRenderedDataPropertyTermExpression(dataMinCardExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(dataMinCardExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataMinCardinalityExpression(renderedLinker,cardinality);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectDataAllValuesFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataAllValuesExpression()) {
				CDataAllValuesFromExpression* dataAllValuesFromExp = (CDataAllValuesFromExpression*)classTermExp;
				renderedLinker = getRenderedDataPropertyTermExpression(dataAllValuesFromExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(dataAllValuesFromExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataAllValuesExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectDataHasValueExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataHasValueExpression()) {
				CDataHasValueExpression* dataHasValueExp = (CDataHasValueExpression*)classTermExp;
				renderedLinker = getRenderedDataPropertyTermExpression(dataHasValueExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(dataHasValueExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataHasValueExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectDataSomeValueFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataSomeValuesFromExpression()) {
				CDataSomeValuesFromExpression* dataSomeValueExp = (CDataSomeValuesFromExpression*)classTermExp;
				renderedLinker = getRenderedDataPropertyTermExpression(dataSomeValueExp->getDataPropertyTermExpression());
				renderedLinker = getRenderedDataRangeTermExpression(dataSomeValueExp->getDataRangeTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderDataSomeValuesFromExpression(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectOneOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectOneOfExpression()) {
				CObjectOneOfExpression* objectOneOfExp = (CObjectOneOfExpression*)classTermExp;
				renderedLinker = getRenderedIndividualTermExpressions(objectOneOfExp->getIndividualTermExpressionList());
				renderedLinker = mRenderer->endRenderObjectOneOfExpression(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectHasValueFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectHasValueExpression()) {
				CObjectHasValueExpression* hasValueExp = (CObjectHasValueExpression*)classTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(hasValueExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedIndividualTermExpression(hasValueExp->getIndividualTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectHasValueExpression(renderedLinker);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectHasSelfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectHasSelfExpression()) {
				CObjectHasSelfExpression* hasSelfExp = (CObjectHasSelfExpression*)classTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(hasSelfExp->getObjectPropertyTermExpression());
				renderedLinker = mRenderer->endRenderObjectHasSelfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectMinCardinalityExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CObjectMinCardinalityExpression* minCardExp = (CObjectMinCardinalityExpression*)classTermExp;
			cint64 cardinality = minCardExp->getCardinality();
			if (mRenderer->beginRenderObjectExactCardinalityExpression(cardinality)) {
				renderedLinker = getRenderedObjectPropertyTermExpression(minCardExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(minCardExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectExactCardinalityExpression(renderedLinker, cardinality);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectMaxCardinalityExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CObjectMaxCardinalityExpression* maxCardExp = (CObjectMaxCardinalityExpression*)classTermExp;
			cint64 cardinality = maxCardExp->getCardinality();
			if (mRenderer->beginRenderObjectExactCardinalityExpression(cardinality)) {
				renderedLinker = getRenderedObjectPropertyTermExpression(maxCardExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(maxCardExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectExactCardinalityExpression(renderedLinker, cardinality);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectExactCardinalityExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CObjectExactlyCardinalityExpression* exactCardExp = (CObjectExactlyCardinalityExpression*)classTermExp;
			cint64 cardinality = exactCardExp->getCardinality();
			if (mRenderer->beginRenderObjectExactCardinalityExpression(cardinality)) {
				renderedLinker = getRenderedObjectPropertyTermExpression(exactCardExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(exactCardExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectExactCardinalityExpression(renderedLinker, cardinality);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::appendPrepandRenderedItemLinker(CRenderedItemLinker* tmpRenderedLinker, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			if (tmpRenderedLinker) {
				if (appendRenderedLinker) {
					tmpRenderedLinker->append(appendRenderedLinker);
				}
				if (prependRenderedLinker) {
					tmpRenderedLinker = prependRenderedLinker->append(tmpRenderedLinker);
				}
			} else {
				if (appendRenderedLinker) {
					tmpRenderedLinker = appendRenderedLinker;
				} else if (prependRenderedLinker) {
					tmpRenderedLinker = prependRenderedLinker;
				} else if (appendRenderedLinker && prependRenderedLinker) {
					tmpRenderedLinker = prependRenderedLinker->append(appendRenderedLinker);
				}
			}
			return tmpRenderedLinker;
		}




		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedIndividualTermExpressions(CEXPRESSIONLIST<CIndividualTermExpression*>* indiTermExpList, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			foreach (CIndividualTermExpression* nextIndiTermExp, *indiTermExpList) {
				tmpRenderedLinker = getRenderedIndividualTermExpression(nextIndiTermExp, tmpRenderedLinker);
			}
			return tmpRenderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedIndividualTermExpression(CIndividualTermExpression* indiTermExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CBuildExpression::ExpressionType type = indiTermExp->getType();
			if (type == CBuildExpression::BETNAMEDINDIVIDUAL) {
				renderedLinker = getRenderedNamedIndividualExpression(indiTermExp);
			} else if (type == CBuildExpression::BETANONYMOUSINDIVIDUAL) {
				renderedLinker = getRenderedAnonymousIndividualExpression(indiTermExp);
			}
			return appendPrepandRenderedItemLinker(renderedLinker, prependRenderedLinker, appendRenderedLinker);
		}





		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedAnonymousIndividualExpression(CIndividualTermExpression* indiTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CAnonymousIndividualExpression* anonymousIndiExpression = (CAnonymousIndividualExpression*)indiTermExp;
			const QString& name = getNameFromEntityExpression(anonymousIndiExpression);
			if (mRenderer->beginRenderAnonymousIndividualExpression(name)) {
				renderedLinker = mRenderer->endRenderAnonymousIndividualExpression(name);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedNamedIndividualExpression(CIndividualTermExpression* indiTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CNamedIndividualExpression* namedIndiExpression = (CNamedIndividualExpression*)indiTermExp;
			const QString& name = getNameFromEntityExpression(namedIndiExpression);
			if (mRenderer->beginRenderNamedIndividualExpression(name)) {
				renderedLinker = mRenderer->endRenderNamedIndividualExpression(name);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyTermExpressions(CEXPRESSIONLIST<CObjectPropertyTermExpression*>* objPropTermExpList, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			foreach(CObjectPropertyTermExpression* nextObjPropTermExp, *objPropTermExpList) {
				tmpRenderedLinker = getRenderedObjectPropertyTermExpression(nextObjPropTermExp, tmpRenderedLinker);
			}
			return tmpRenderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyTermExpression(CObjectPropertyTermExpression* objPropTermExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CBuildExpression::ExpressionType type = objPropTermExp->getType();
			if (type == CBuildExpression::BETOBJECTPROPERTY) {
				renderedLinker = getRenderedObjectPropertyExpression(objPropTermExp);
			} else if (type == CBuildExpression::BETOBJECTPROPERTYCHAIN) {
				renderedLinker = getRenderedObjectPropertyChainExpression(objPropTermExp);
			} else if (type == CBuildExpression::BETINVERSEOBJECTPROPERTYOF) {
				renderedLinker = getRenderedInverseObjectPropertyOfExpression(objPropTermExp);
			}
			return appendPrepandRenderedItemLinker(renderedLinker, prependRenderedLinker, appendRenderedLinker);
		}




		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedInverseObjectPropertyOfExpression(CObjectPropertyTermExpression* objPropTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderInverseObjectPropertyOf()) {
				CInverseObjectPropertyOfExpression* invObjPropExp = (CInverseObjectPropertyOfExpression*)objPropTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(invObjPropExp->getInverseOfExpression());
				renderedLinker = mRenderer->endRenderInverseObjectPropertyOf(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyChainExpression(CObjectPropertyTermExpression* objPropTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectPropertyChainAxiom()) {
				CObjectPropertyChainExpression* objPropChainExp = (CObjectPropertyChainExpression*)objPropTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpressions(objPropChainExp->getOrderedObjectPropertyTermExpressionList());
				renderedLinker = mRenderer->endRenderObjectPropertyChainAxiom(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataPropertyTermExpressions(CEXPRESSIONLIST<CDataPropertyTermExpression*>* dataPropTermExpList, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			foreach(CDataPropertyTermExpression* nextDataPropTermExp, *dataPropTermExpList) {
				tmpRenderedLinker = getRenderedDataPropertyTermExpression(nextDataPropTermExp, tmpRenderedLinker);
			}
			return tmpRenderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataPropertyTermExpression(CDataPropertyTermExpression* dataPropTermExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CBuildExpression::ExpressionType type = dataPropTermExp->getType();
			if (type == CBuildExpression::BETDATAPROPERTY) {
				renderedLinker = getRenderedDataPropertyExpression(dataPropTermExp);
			}
			return appendPrepandRenderedItemLinker(renderedLinker, prependRenderedLinker, appendRenderedLinker);
		}





		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataRangeTermExpressions(CEXPRESSIONLIST<CDataRangeTermExpression*>* dataTermExpList, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			foreach(CDataRangeTermExpression* dataTermExp, *dataTermExpList) {
				tmpRenderedLinker = getRenderedDataRangeTermExpression(dataTermExp, tmpRenderedLinker);
			}
			return tmpRenderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataRangeTermExpression(CDataRangeTermExpression* dataRangeTermExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			CBuildExpression::ExpressionType type = dataRangeTermExp->getType();
			if (type == CBuildExpression::BETDATATYPE) {
				tmpRenderedLinker = getRenderedDatatypeExpression(dataRangeTermExp);
			} else if (type == CBuildExpression::BETDATALITERAL) {
				tmpRenderedLinker = getRenderedDataLiteralExpression(dataRangeTermExp);
			} else if (type == CBuildExpression::BETDATAINTERSECTIONOF) {
				tmpRenderedLinker = getRenderedDataIntersectionOfExpression(dataRangeTermExp);
			} else if (type == CBuildExpression::BETDATAUNIONOF) {
				tmpRenderedLinker = getRenderedDataUnionOfExpression(dataRangeTermExp);
			} else if (type == CBuildExpression::BETDATACOMPLEMENTOF) {
				tmpRenderedLinker = getRenderedDataComplementOfExpression(dataRangeTermExp);
			} else if (type == CBuildExpression::BETDATAONEOF) {
				tmpRenderedLinker = getRenderedDataOneOfExpression(dataRangeTermExp);
			} else if (type == CBuildExpression::BETDATATYPERESTRICTION) {
				tmpRenderedLinker = getRenderedDatatypeRestrictionExpression(dataRangeTermExp);
			}
			return appendPrepandRenderedItemLinker(tmpRenderedLinker, prependRenderedLinker, appendRenderedLinker);
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDatatypeRestrictionExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDatatypeRestrictionExpression()) {
				CDatatypeRestrictionExpression* datatypeRestExp = (CDatatypeRestrictionExpression*)dataRangeTermExp;
				renderedLinker = getRenderedDataRangeTermExpression(datatypeRestExp->getDatatypeExpression());
				renderedLinker = getRenderedDataFacetRestrictionExpressions(datatypeRestExp->getDataFacetRestrictionExpressionList(), renderedLinker);
				renderedLinker = mRenderer->endRenderDatatypeRestrictionExpression(renderedLinker);
			}
			return renderedLinker;
		}




		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataFacetRestrictionExpressions(CEXPRESSIONLIST<CDataFacetRestrictionExpression*>* dataFacetRestExpList, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* tmpRenderedLinker = nullptr;
			foreach(CDataFacetRestrictionExpression* dataFacetRestExp, *dataFacetRestExpList) {
				tmpRenderedLinker = getRenderedDataFacetRestrictionExpression(dataFacetRestExp, tmpRenderedLinker);
			}
			return tmpRenderedLinker;
		}




		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataFacetRestrictionExpression(CDataFacetRestrictionExpression* dataFacetRestExp, CRenderedItemLinker* prependRenderedLinker, CRenderedItemLinker* appendRenderedLinker) {
			CRenderedItemLinker* renderedLinker = nullptr;
			const QString& facetName = getNameFromEntityExpression(dataFacetRestExp->getDataFacetExpression());
			if (mRenderer->beginRenderDataFacetRestrictionExpression(facetName)) {
				renderedLinker = getRenderedDataLiteralExpression(dataFacetRestExp->getDataLiteralExpression());
				renderedLinker = mRenderer->endRenderDataFacetRestrictionExpression(renderedLinker, facetName);
			}
			return appendPrepandRenderedItemLinker(renderedLinker,prependRenderedLinker,appendRenderedLinker);
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataOneOfExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataOneOfExpression()) {
				CDataOneOfExpression* dataOneExp = (CDataOneOfExpression*)dataRangeTermExp;
				renderedLinker = getRenderedDataRangeTermExpressions(dataOneExp->getDataRangeTermExpressionList());
				renderedLinker = mRenderer->endRenderDataOneOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataComplementOfExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataComplementOfExpression()) {
				CDataComplementOfExpression* dataComplExp = (CDataComplementOfExpression*)dataRangeTermExp;
				renderedLinker = getRenderedDataRangeTermExpression(dataComplExp->getDataRangeTermExpression());
				renderedLinker = mRenderer->endRenderDataComplementOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataUnionOfExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataUnionOfExpression()) {
				CDataUnionOfExpression* dataUnionExp = (CDataUnionOfExpression*)dataRangeTermExp;
				renderedLinker = getRenderedDataRangeTermExpressions(dataUnionExp->getDataRangeTermExpressionList());
				renderedLinker = mRenderer->endRenderDataUnionOfExpression(renderedLinker);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataIntersectionOfExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderDataIntersectionOfExpression()) {
				CDataIntersectionOfExpression* dataIntExp = (CDataIntersectionOfExpression*)dataRangeTermExp;
				renderedLinker = getRenderedDataRangeTermExpressions(dataIntExp->getDataRangeTermExpressionList());
				renderedLinker = mRenderer->endRenderDataIntersectionOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataLiteralExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CDataLiteralExpression* dataLiteralExpression = (CDataLiteralExpression*)dataRangeTermExp;
			const QString& literalValue = getNameFromEntityExpression(dataLiteralExpression->getDataLexicalValueExpression());
			const QString& datatypeName = getNameFromEntityExpression(dataLiteralExpression->getDatatypeExpression());
			if (mRenderer->beginRenderDataLiteralExpression(literalValue,datatypeName)) {
				renderedLinker = mRenderer->endRenderDataLiteralExpression(literalValue, datatypeName);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectPropertyExpression(CObjectPropertyTermExpression* objPropTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CObjectPropertyExpression* objPropExpression = (CObjectPropertyExpression*)objPropTermExp;
			const QString& name = getNameFromEntityExpression(objPropExpression);
			if (mRenderer->beginRenderObjectPropertyExpression(name)) {
				renderedLinker = mRenderer->endRenderObjectPropertyExpression(name);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDataPropertyExpression(CDataPropertyTermExpression* dataPropTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CDataPropertyExpression* dataPropExpression = (CDataPropertyExpression*)dataPropTermExp;
			const QString& name = getNameFromEntityExpression(dataPropExpression);
			if (mRenderer->beginRenderDataPropertyExpression(name)) {
				renderedLinker = mRenderer->endRenderDataPropertyExpression(name);
			}
			return renderedLinker;
		}




		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedClassExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CClassExpression* classExpression = (CClassExpression*)classTermExp;
			const QString& name = getNameFromEntityExpression(classExpression);
			if (mRenderer->beginRenderClassExpression(name)) {
				renderedLinker = mRenderer->endRenderClassExpression(name);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedDatatypeExpression(CDataRangeTermExpression* dataRangeTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CDatatypeExpression* datatypeExpression = (CDatatypeExpression*)dataRangeTermExp;
			const QString& name = getNameFromEntityExpression(datatypeExpression);
			if (mRenderer->beginRenderDatatypeExpression(name)) {
				renderedLinker = mRenderer->endRenderDatatypeExpression(name);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectSomeValuesFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectSomeValuesFromExpression()) {
				CObjectSomeValuesFromExpression* someValuesFromExp = (CObjectSomeValuesFromExpression*)classTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(someValuesFromExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(someValuesFromExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectSomeValuesFromExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectAllValuesFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectAllValuesFromExpression()) {
				CObjectAllValuesFromExpression* allValuesFromExp = (CObjectAllValuesFromExpression*)classTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(allValuesFromExp->getObjectPropertyTermExpression());
				renderedLinker = getRenderedClassTermExpression(allValuesFromExp->getClassTermExpression(), renderedLinker);
				renderedLinker = mRenderer->endRenderObjectAllValuesFromExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectIntersectionOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CRenderedItemLinker* lastRenderedLinker = nullptr;
			if (mRenderer->beginRenderObjectIntersectionOfExpression()) {
				CObjectIntersectionOfExpression* intersectionOfExp = (CObjectIntersectionOfExpression*)classTermExp;
				renderedLinker = getRenderedClassTermExpressions(intersectionOfExp->getClassTermExpressionList());
				renderedLinker = mRenderer->endRenderObjectIntersectionOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectUnionOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectUnionOfExpression()) {
				CObjectUnionOfExpression* unionOfExp = (CObjectUnionOfExpression*)classTermExp;
				renderedLinker = getRenderedClassTermExpressions(unionOfExp->getClassTermExpressionList());
				renderedLinker = mRenderer->endRenderObjectUnionOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyAxiomExpressionsRenderVisitor::getRenderedObjectComplementOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectComplementOfExpression()) {
				CObjectComplementOfExpression* compOfExp = (CObjectComplementOfExpression*)classTermExp;
				renderedLinker = getRenderedClassTermExpression(compOfExp->getComplementExpression());
				renderedLinker = mRenderer->endRenderObjectComplementOfExpression(renderedLinker);
			}
			return renderedLinker;
		}



		QString COntologyAxiomExpressionsRenderVisitor::getNameFromEntityExpression(CExpressionEntity* entity) {
			return entity->getName();
		}



	}; // end namespace Renderer

}; // end namespace Konclude
