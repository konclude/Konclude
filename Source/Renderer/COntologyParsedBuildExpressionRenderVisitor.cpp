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
#include "COntologyParsedBuildExpressionRenderVisitor.h"


namespace Konclude {

	namespace Renderer {


		COntologyParsedBuildExpressionRenderVisitor::COntologyParsedBuildExpressionRenderVisitor() {
		}





		bool COntologyParsedBuildExpressionRenderVisitor::visitAndRender(const QStringList& entityStringList, CConcreteOntology* ontology, COntologyRenderer* renderer) {
			mOntology = ontology;
			mRenderer = renderer;
			COntologyBuildData* buildDataBox = ontology->getBuildData();
			mConceptClassTermMapHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();
			mClassTermExpClassAxiomHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();

			CBUILDHASH<CStringRefStringHasher,CClassExpression*>* classEntityHash = buildDataBox->getClassEntityBuildHash();

			CQtList<CExpressionEntity*> entityList;
			foreach (QString entityName, entityStringList) {
				CClassExpression* classExp = classEntityHash->value(entityName);
				if (classExp) {
					entityList.append(classExp);
				}
			}
			visitAndRender(entityList);
			return true;
		}


		bool COntologyParsedBuildExpressionRenderVisitor::visitAndRender(const CQtList<CConcept*>& conceptList, CConcreteOntology* ontology, COntologyRenderer* renderer) {
			mOntology = ontology;
			mRenderer = renderer;
			COntologyBuildData* buildDataBox = ontology->getBuildData();
			mConceptClassTermMapHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptClassTermMappingHash();
			mClassTermExpClassAxiomHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getClassTermExpressionClassAxiomExpressionHash();

			CQtList<CExpressionEntity*> entityList;
			foreach (CConcept* concept, conceptList) {
				CClassTermExpression* classTermExp = mConceptClassTermMapHash->value(concept);
				if (classTermExp) {
					CClassExpression* classExp = dynamic_cast<CClassExpression*>(classTermExp);
					if (classExp) {
						entityList.append(classExp);
					}
				}
			}
			visitAndRender(entityList);
			return true;
		}


		bool COntologyParsedBuildExpressionRenderVisitor::visitAndRender(const CQtList<CExpressionEntity*>& entityList) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CRenderedItemLinker* lastRenderedLinker = nullptr;

			if (mRenderer->beginRender()) {
				foreach (CExpressionEntity* entityExp, entityList) {
					addRemainingVisitingAndRenderEntity(entityExp);
				}

				while (!mRemainingEntityList.isEmpty() || !mRemainingAxiomList.isEmpty()) {

					while (!mRemainingEntityList.isEmpty()) {
						CExpressionEntity* entity = mRemainingEntityList.takeFirst();

						mRenderer->beginRenderDeclarationAxiom();
						CRenderedItemLinker* tmpRenderedLinker = getRenderedEntityExpression(entity);
						tmpRenderedLinker = mRenderer->endRenderDeclarationAxiom(tmpRenderedLinker);

						if (tmpRenderedLinker) {
							if (lastRenderedLinker) {
								lastRenderedLinker = lastRenderedLinker->append(tmpRenderedLinker);
							} else {
								renderedLinker = tmpRenderedLinker;
							}
							lastRenderedLinker = tmpRenderedLinker;
						}
					}

					while (!mRemainingAxiomList.isEmpty()) {
						CAxiomExpression* axiomExp = mRemainingAxiomList.takeFirst();
						CRenderedItemLinker* tmpRenderedLinker = getRenderedAxiomExpression(axiomExp);

						if (tmpRenderedLinker) {
							if (lastRenderedLinker) {
								lastRenderedLinker = lastRenderedLinker->append(tmpRenderedLinker);
							} else {
								renderedLinker = tmpRenderedLinker;
							}
							lastRenderedLinker = tmpRenderedLinker;
						}
					}
				}
				mRenderer->endRender(renderedLinker);
				return true;
			}
			return false;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedAxiomExpression(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CBuildExpression::ExpressionType type = axiomExp->getType();
			if (type == CBuildExpression::BETESUBCLASSOF) {
				renderedLinker = getRenderedSubClassOfAxiom(axiomExp);
			}
			return renderedLinker;
		}

		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedSubClassOfAxiom(CAxiomExpression* axiomExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderSubClassOfAxiom()) {
				CSubClassOfExpression* subClassOfExp = (CSubClassOfExpression*)axiomExp;
				CClassTermExpression* subClassTermExp = subClassOfExp->getSubClassTermExpression();
				renderedLinker = getRenderedClassTermExpression(subClassTermExp);
				CClassTermExpression* superClassTermExp = subClassOfExp->getSuperClassTermExpression();
				CRenderedItemLinker* tmpRenderedLinker = getRenderedClassTermExpression(superClassTermExp);
				if (renderedLinker) {
					renderedLinker->append(tmpRenderedLinker);
				} else {
					renderedLinker = tmpRenderedLinker;
				}
				renderedLinker = mRenderer->endRenderSubClassOfAxiom(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedClassTermExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CBuildExpression::ExpressionType type = classTermExp->getType();
			if (type == CBuildExpression::BETCLASS) {
				renderedLinker = getRenderedClassExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTINTERSECTION) {
				renderedLinker = getRenderedObjectIntersectionOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTUNION) {
				renderedLinker = getRenderedObjectUnionOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTCOMPLEMENTOF) {
				renderedLinker = getRenderedObjectComplementOfExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTSOMEVALUEFROM) {
				renderedLinker = getRenderedObjectSomeValuesFromExpression(classTermExp);
			} else if (type == CBuildExpression::BETOBJECTALLVALUEFROM) {
				renderedLinker = getRenderedObjectAllValuesFromExpression(classTermExp);
			}
			return renderedLinker;
		}



		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectPropertyTermExpression(CObjectPropertyTermExpression* objPropTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CBuildExpression::ExpressionType type = objPropTermExp->getType();
			if (type == CBuildExpression::BETOBJECTPROPERTY) {
				renderedLinker = getRenderedObjectPropertyExpression(objPropTermExp);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectPropertyExpression(CObjectPropertyTermExpression* objPropTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CObjectPropertyExpression* objPropExpression = (CObjectPropertyExpression*)objPropTermExp;
			const QString& name = getNameFromEntityExpression(objPropExpression);
			if (mRenderer->beginRenderObjectPropertyExpression(name)) {
				addRemainingVisitingAndRenderEntity(objPropExpression);
				renderedLinker = mRenderer->endRenderObjectPropertyExpression(name);
			}
			return renderedLinker;
		}




		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedClassExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CClassExpression* classExpression = (CClassExpression*)classTermExp;
			const QString& name = getNameFromEntityExpression(classExpression);
			if (mRenderer->beginRenderClassExpression(name)) {
				addRemainingVisitingAndRenderEntity(classExpression);
				renderedLinker = mRenderer->endRenderClassExpression(name);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectSomeValuesFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectSomeValuesFromExpression()) {
				serachRemainingVisitingAndRenderAxiom(classTermExp);
				CObjectSomeValuesFromExpression* someValuesFromExp = (CObjectSomeValuesFromExpression*)classTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(someValuesFromExp->getObjectPropertyTermExpression());
				CRenderedItemLinker* tmpRenderedLinker = getRenderedClassTermExpression(someValuesFromExp->getClassTermExpression());
				if (renderedLinker) {
					renderedLinker->append(tmpRenderedLinker);
				} else {
					renderedLinker = tmpRenderedLinker;
				}
				renderedLinker = mRenderer->endRenderObjectSomeValuesFromExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectAllValuesFromExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectAllValuesFromExpression()) {
				serachRemainingVisitingAndRenderAxiom(classTermExp);
				CObjectAllValuesFromExpression* allValuesFromExp = (CObjectAllValuesFromExpression*)classTermExp;
				renderedLinker = getRenderedObjectPropertyTermExpression(allValuesFromExp->getObjectPropertyTermExpression());
				CRenderedItemLinker* tmpRenderedLinker = getRenderedClassTermExpression(allValuesFromExp->getClassTermExpression());
				if (renderedLinker) {
					renderedLinker->append(tmpRenderedLinker);
				} else {
					renderedLinker = tmpRenderedLinker;
				}
				renderedLinker = mRenderer->endRenderObjectAllValuesFromExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectIntersectionOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CRenderedItemLinker* lastRenderedLinker = nullptr;
			if (mRenderer->beginRenderObjectIntersectionOfExpression()) {
				serachRemainingVisitingAndRenderAxiom(classTermExp);
				CObjectIntersectionOfExpression* intersectionOfExp = (CObjectIntersectionOfExpression*)classTermExp;
				foreach (CClassTermExpression* nextClassTermExp, *intersectionOfExp->getClassTermExpressionList()) {
					CRenderedItemLinker* tmpRenderedLinker = getRenderedClassTermExpression(nextClassTermExp);
					if (tmpRenderedLinker) {
						if (lastRenderedLinker) {
							lastRenderedLinker = lastRenderedLinker->append(tmpRenderedLinker);
						} else {
							renderedLinker = tmpRenderedLinker;
						}
						lastRenderedLinker = tmpRenderedLinker;
					}
				}
				renderedLinker = mRenderer->endRenderObjectIntersectionOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectUnionOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			CRenderedItemLinker* lastRenderedLinker = nullptr;
			if (mRenderer->beginRenderObjectUnionOfExpression()) {
				serachRemainingVisitingAndRenderAxiom(classTermExp);
				CObjectUnionOfExpression* unionOfExp = (CObjectUnionOfExpression*)classTermExp;
				foreach (CClassTermExpression* nextClassTermExp, *unionOfExp->getClassTermExpressionList()) {
					CRenderedItemLinker* tmpRenderedLinker = getRenderedClassTermExpression(nextClassTermExp);
					if (tmpRenderedLinker) {
						if (lastRenderedLinker) {
							lastRenderedLinker = lastRenderedLinker->append(tmpRenderedLinker);
						} else {
							renderedLinker = tmpRenderedLinker;
						}
						lastRenderedLinker = tmpRenderedLinker;
					}
				}
				renderedLinker = mRenderer->endRenderObjectUnionOfExpression(renderedLinker);
			}
			return renderedLinker;
		}


		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedObjectComplementOfExpression(CClassTermExpression* classTermExp) {
			CRenderedItemLinker* renderedLinker = nullptr;
			if (mRenderer->beginRenderObjectComplementOfExpression()) {
				serachRemainingVisitingAndRenderAxiom(classTermExp);
				CObjectComplementOfExpression* unionOfExp = (CObjectComplementOfExpression*)classTermExp;
				renderedLinker = getRenderedClassTermExpression(unionOfExp->getComplementExpression());
				renderedLinker = mRenderer->endRenderObjectComplementOfExpression(renderedLinker);
			}
			return renderedLinker;
		}



		QString COntologyParsedBuildExpressionRenderVisitor::getNameFromEntityExpression(CExpressionEntity* entity) {
			return entity->getName();
		}

		CRenderedItemLinker* COntologyParsedBuildExpressionRenderVisitor::getRenderedEntityExpression(CExpressionEntity* entity) {
			CRenderedItemLinker* renderedLinker = nullptr;
			const QString& name = getNameFromEntityExpression(entity);
			CClassExpression* classExpression = dynamic_cast<CClassExpression*>(entity);
			if (classExpression) {
				if (mRenderer->beginRenderClassExpression(name)) {
					renderedLinker = mRenderer->endRenderClassExpression(name);
					serachRemainingVisitingAndRenderAxiom(classExpression);
				}
			} else {
				CObjectPropertyExpression* propertyExpression = dynamic_cast<CObjectPropertyExpression*>(entity);
				if (propertyExpression) {
					if (mRenderer->beginRenderObjectPropertyExpression(name)) {
						renderedLinker = mRenderer->endRenderObjectPropertyExpression(name);
						serachRemainingVisitingAndRenderAxiom(propertyExpression);
					}
				}
			}
			return renderedLinker;
		}


		bool COntologyParsedBuildExpressionRenderVisitor::serachRemainingVisitingAndRenderAxiom(CClassTermExpression* expression) {
			bool foundAxiom = false;
			if (!mTestedAxiomSet.contains(expression)) {
				mTestedAxiomSet.insert(expression);
				foreach (CClassAxiomExpression* axiomExpression, mClassTermExpClassAxiomHash->values(expression)) {
					if (!mVisitedAxiomSet.contains(axiomExpression)) {
						mVisitedAxiomSet.insert(axiomExpression);
						mRemainingAxiomList.append(axiomExpression);
						foundAxiom = true;
					}
				}
			}
			return foundAxiom;
		}


		bool COntologyParsedBuildExpressionRenderVisitor::serachRemainingVisitingAndRenderAxiom(CObjectPropertyTermExpression* expression) {
			bool foundAxiom = false;
			if (!mTestedAxiomSet.contains(expression)) {
				mTestedAxiomSet.insert(expression);
			}
			return foundAxiom;
		}


		bool COntologyParsedBuildExpressionRenderVisitor::addRemainingVisitingAndRenderEntity(CExpressionEntity* entity) {
			if (!mVisitedEntitySet.contains(entity)) {
				mVisitedEntitySet.insert(entity);
				mRemainingEntityList.append(entity);
				return true;
			}
			return false;
		}


	}; // end namespace Renderer

}; // end namespace Konclude
