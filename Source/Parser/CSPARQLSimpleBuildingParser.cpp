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

#include "CSPARQLSimpleBuildingParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLSimpleBuildingParser::CSPARQLSimpleBuildingParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology) {
			mOntoBuilder = ontoBuilder;
			mOntology = ontology;

		}


		CSPARQLSimpleBuildingParser::~CSPARQLSimpleBuildingParser() {
		}



		QList<CAxiomExpression*> CSPARQLSimpleBuildingParser::getTripleOWLAxioms(QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash, bool anonymousIndividualsAsVariables) {
			QList<CAxiomExpression*> axiomList;
			for (QHash<QString, CRDFHahsedSubjectData>::const_iterator it = subjectTripleHash->begin(), itEnd = subjectTripleHash->end(); it != itEnd;) {
				QString subjectString(it.key());
				const CRDFHahsedSubjectData& predObjData = it.value();
				QList<CRDFHahsedSubjectData> predObjDataList;
				predObjDataList.append(predObjData);

				++it;
				if (it != itEnd) {
					QString nextSubject(it.key());
					while (nextSubject == subjectString && it != itEnd) {
						const CRDFHahsedSubjectData& predObjData = it.value();
						predObjDataList.append(predObjData);
						++it;
						if (it != itEnd) {
							nextSubject = it.key();
						}
					}
				}

				CBuildExpression::ExpressionType subjectType;				
				bool hasType = false;
				for (QList<CRDFHahsedSubjectData>::iterator it = predObjDataList.begin(), itEnd = predObjDataList.end(); it != itEnd && !hasType; ++it) {
					const CRDFHahsedSubjectData& predObjData = *it;

					const QString& predicateString = predObjData.string[0];
					const QString& objectString = predObjData.string[1];

					if (predicateString == "a" || predicateString == PREFIX_RDF_TYPE) {
						if (objectString == PREFIX_OWL_NAMED_INDIVIDUAL) {
							subjectType = CBuildExpression::BETNAMEDINDIVIDUAL;
							hasType = true;
							predObjDataList.erase(it);
						} else if (objectString == PREFIX_OWL_CLASS || objectString == "http://www.w3.org/1999/02/22-rdf-syntax-ns#Class") {
							subjectType = CBuildExpression::BETCLASS;
							hasType = true;
							predObjDataList.erase(it);
						} else if (objectString == PREFIX_OWL_OBJECT_PROPERTY) {
							subjectType = CBuildExpression::BETOBJECTPROPERTY;
							hasType = true;
							predObjDataList.erase(it);
						} else if (objectString == PREFIX_OWL_DATATYPE_PROPERTY) {
							subjectType = CBuildExpression::BETDATAPROPERTY;
							hasType = true;
							predObjDataList.erase(it);
						}
					}
				}


				for (QList<CRDFHahsedSubjectData>::iterator it = predObjDataList.begin(), itEnd = predObjDataList.end(); it != itEnd; ++it) {
					const CRDFHahsedSubjectData& predObjData = *it;

					const QString& predicateString = predObjData.string[0];
					const QString& objectString = predObjData.string[1];

					if (!hasType || subjectType == CBuildExpression::BETNAMEDINDIVIDUAL) {

						CIndividualTermExpression* indiExpression = getIndividualTermExpression(subjectString, anonymousIndividualsAsVariables);						

						if (predicateString == "a" || predicateString == PREFIX_RDF_TYPE) {
							CClassTermExpression* classTermExp = getClassTermExpression(objectString);
							CAxiomExpression* assExp = mOntoBuilder->getClassAssertion(indiExpression, classTermExp);
							axiomList.append(assExp);
						} else if (predicateString == PREFIX_RDFS_SUBCLASS_OF) {
							CClassTermExpression* class1Expression = getClassTermExpression(subjectString);
							CClassTermExpression* class2Expression = getClassTermExpression(objectString);
							CAxiomExpression* assExp = mOntoBuilder->getSubClassOf(class1Expression, class2Expression);
							axiomList.append(assExp);
						} else if (predicateString == PREFIX_RDFS_SUBPROPERTY_OF) {
							bool dataProps = subjectType == CBuildExpression::BETDATAPROPERTY || isDataProperty(subjectString) || isDataProperty(objectString);
							if (dataProps) {
								CDataPropertyTermExpression* dataProp1Expression = getDataPropertyTermExpression(subjectString);
								CDataPropertyTermExpression* dataProp2Expression = getDataPropertyTermExpression(objectString);
								CAxiomExpression* assExp = mOntoBuilder->getSubDataPropertyOf(dataProp1Expression, dataProp2Expression);
								axiomList.append(assExp);
							} else {
								CObjectPropertyTermExpression* objProp1Expression = getObjectPropertyTermExpression(subjectString);
								CObjectPropertyTermExpression* objProp2Expression = getObjectPropertyTermExpression(objectString);
								CAxiomExpression* assExp = mOntoBuilder->getSubObjectPropertyOf((CBuildExpression*)objProp1Expression, (CBuildExpression*)objProp2Expression);
								axiomList.append(assExp);
							}
						} else {
							if (!isDataProperty(predicateString)) {
								CIndividualTermExpression* otherIndiExpression = getIndividualTermExpression(objectString, anonymousIndividualsAsVariables);
								CObjectPropertyTermExpression* propExp = getObjectPropertyTermExpression(predicateString);
								CObjectPropertyAssertionExpression* propAss = mOntoBuilder->getObjectPropertyAssertion(indiExpression, propExp, otherIndiExpression);
								axiomList.append(propAss);
							} else {
								CDataLiteralTermExpression* dataRangeExpr = getDataLiteralTermExpression(objectString);							
								CDataPropertyExpression* propExp = mOntoBuilder->getDataProberty(predicateString);
								CDataPropertyAssertionExpression* propAss = mOntoBuilder->getDataPropertyAssertion(indiExpression, propExp, dataRangeExpr);
								axiomList.append(propAss);
							}
						}
					}
				}

			}
			return axiomList;
		}



		QString CSPARQLSimpleBuildingParser::getVariableName(const QString& variableName) {
			return variableName.mid(1);
		}


		QList<CAxiomExpression*> CSPARQLSimpleBuildingParser::getTripleOWLAxioms(QList<CRDFStringTriple>* tripleStringList, bool anonymousIndividualsAsVariables, QSet<CExpressionVariable*>* varExpSet) {
			QList<CAxiomExpression*> axiomList;
			for (auto triple : *tripleStringList) {

				QString subjectString = triple.string[0];

				QString predicateString = triple.string[1];
				QString objectString = triple.string[2];

				if (predicateString == "a" || predicateString == PREFIX_RDF_TYPE) {
					CIndividualTermExpression* indiExpression = getIndividualTermExpression(subjectString, anonymousIndividualsAsVariables, varExpSet);
					CClassTermExpression* classTermExp = getClassTermExpression(objectString, varExpSet);
					CAxiomExpression* assExp = mOntoBuilder->getClassAssertion(indiExpression, classTermExp);
					axiomList.append(assExp);
				} else if (predicateString == PREFIX_RDFS_SUBCLASS_OF) {
					CClassTermExpression* class1Expression = getClassTermExpression(subjectString, varExpSet);
					CClassTermExpression* class2Expression = getClassTermExpression(objectString, varExpSet);
					CAxiomExpression* assExp = mOntoBuilder->getSubClassOf(class1Expression, class2Expression);
					axiomList.append(assExp);
				} else if (predicateString == PREFIX_RDFS_SUBPROPERTY_OF) {
					bool dataProps = isDataProperty(subjectString) || isDataProperty(objectString);
					if (dataProps) {
						CDataPropertyTermExpression* dataProp1Expression = getDataPropertyTermExpression(subjectString, varExpSet);
						CDataPropertyTermExpression* dataProp2Expression = getDataPropertyTermExpression(objectString, varExpSet);
						CAxiomExpression* assExp = mOntoBuilder->getSubDataPropertyOf(dataProp1Expression, dataProp2Expression);
						axiomList.append(assExp);
					} else {
						CObjectPropertyTermExpression* objProp1Expression = getObjectPropertyTermExpression(subjectString, varExpSet);
						CObjectPropertyTermExpression* objProp2Expression = getObjectPropertyTermExpression(objectString, varExpSet);
						CAxiomExpression* assExp = mOntoBuilder->getSubObjectPropertyOf((CBuildExpression*)objProp1Expression, (CBuildExpression*)objProp2Expression);
						axiomList.append(assExp);
					}
				} else {
					CIndividualTermExpression* indiExpression = getIndividualTermExpression(subjectString, anonymousIndividualsAsVariables, varExpSet);
					if (!isDataProperty(predicateString)) {
						CIndividualTermExpression* otherIndiExpression = getIndividualTermExpression(objectString, anonymousIndividualsAsVariables, varExpSet);
						CObjectPropertyTermExpression* propExp = getObjectPropertyTermExpression(predicateString, varExpSet);
						CObjectPropertyAssertionExpression* propAss = mOntoBuilder->getObjectPropertyAssertion(indiExpression, propExp, otherIndiExpression);
						axiomList.append(propAss);
					} else {
						CDataLiteralTermExpression* dataRangeExpr = getDataLiteralTermExpression(objectString, varExpSet);

						CDataPropertyExpression* propExp = mOntoBuilder->getDataProberty(predicateString);
						CDataPropertyAssertionExpression* propAss = mOntoBuilder->getDataPropertyAssertion(indiExpression, propExp, dataRangeExpr);
						axiomList.append(propAss);
					}
				}
			}
			return axiomList;
		}


		bool CSPARQLSimpleBuildingParser::isDataProperty(const QString& propertyName) {
			return mOntology->getBuildData()->getDataPropertyEntityBuildHash()->contains(propertyName);
		}

		CClassTermExpression* CSPARQLSimpleBuildingParser::getClassTermExpression(const QString &subjectString, QSet<CExpressionVariable*>* varExpSet) {
			CClassTermExpression* classExpression = nullptr;
			if (subjectString.startsWith("?") || subjectString.startsWith("$")) {
				QString variableName = getVariableName(subjectString);
				CClassVariableExpression* varClassExpression = mOntoBuilder->getClassVariable(variableName);
				mVarExpSet.insert(varClassExpression);
				mNameVarHash.insert(variableName, varClassExpression);
				classExpression = varClassExpression;
			} else {
				classExpression = mOntoBuilder->getClass(subjectString);
			}				
			if (varExpSet && dynamic_cast<CClassVariableExpression*>(classExpression)) {
				varExpSet->insert((CClassVariableExpression*)classExpression);
			}
			return classExpression;
		}




		CDataPropertyTermExpression* CSPARQLSimpleBuildingParser::getDataPropertyTermExpression(const QString &subjectString, QSet<CExpressionVariable*>* varExpSet) {
			CDataPropertyTermExpression* dataPropExpression = nullptr;
			if (subjectString.startsWith("?") || subjectString.startsWith("$")) {
				QString variableName = getVariableName(subjectString);
				CDataPropertyVariableExpression* varDataPropExpression = mOntoBuilder->getDataPropertyVariable(variableName);
				mVarExpSet.insert(varDataPropExpression);
				mNameVarHash.insert(variableName, varDataPropExpression);
				dataPropExpression = varDataPropExpression;
			} else {
				dataPropExpression = mOntoBuilder->getDataProberty(subjectString);
			}
			if (varExpSet && dynamic_cast<CDataPropertyVariableExpression*>(dataPropExpression)) {
				varExpSet->insert((CDataPropertyVariableExpression*)dataPropExpression);
			}
			return dataPropExpression;
		}



		CObjectPropertyTermExpression* CSPARQLSimpleBuildingParser::getObjectPropertyTermExpression(const QString &subjectString, QSet<CExpressionVariable*>* varExpSet) {
			CObjectPropertyTermExpression* objPropExpression = nullptr;
			if (subjectString.startsWith("?") || subjectString.startsWith("$")) {
				QString variableName = getVariableName(subjectString);
				CObjectPropertyVariableExpression* varObjPropExpression = mOntoBuilder->getObjectPropertyVariable(variableName);
				mVarExpSet.insert(varObjPropExpression);
				mNameVarHash.insert(variableName, varObjPropExpression);
				objPropExpression = varObjPropExpression;
			} else {
				objPropExpression = mOntoBuilder->getObjectProberty(subjectString);
			}
			if (varExpSet && dynamic_cast<CObjectPropertyVariableExpression*>(objPropExpression)) {
				varExpSet->insert((CObjectPropertyVariableExpression*)objPropExpression);
			}
			return objPropExpression;
		}

		CIndividualTermExpression* CSPARQLSimpleBuildingParser::getIndividualTermExpression(const QString& subjectString, bool anonymousIndividualsAsVariables, QSet<CExpressionVariable*>* varExpSet) {
			CIndividualTermExpression* indiExpression = nullptr;
			if (subjectString.startsWith("?") || subjectString.startsWith("$")) {
				QString variableName = getVariableName(subjectString);
				CIndividualVariableExpression* varIndiExpression = mOntoBuilder->getIndividualVariable(variableName, false);
				mVarExpSet.insert(varIndiExpression);
				mNameVarHash.insert(variableName, varIndiExpression);
				indiExpression = varIndiExpression;
			} else if (subjectString.startsWith("_:")) {
				if (anonymousIndividualsAsVariables) {
					indiExpression = mOntoBuilder->getIndividualVariable(subjectString.mid(2), true);
				} else {
					indiExpression = mOntoBuilder->getAnonymousIndividual(mOntoBuilder->getOntologyAnonymousIdentifier(mOntoBuilder->getOntologyName()), subjectString.mid(2));
				}
			} else {
				indiExpression = mOntoBuilder->getNamedIndividual(subjectString);
			}				
			if (varExpSet && dynamic_cast<CIndividualVariableExpression*>(indiExpression)) {
				varExpSet->insert((CIndividualVariableExpression*)indiExpression);
			}
			return indiExpression;
		}

		CDataLiteralTermExpression* CSPARQLSimpleBuildingParser::getDataLiteralTermExpression(const QString &objectString, QSet<CExpressionVariable*>* varExpSet) {
			CDataLiteralTermExpression* dataRangeExpr = nullptr;
			if (objectString.startsWith("?") || objectString.startsWith("$")) {
				QString variableName = getVariableName(objectString);
				CDataLiteralVariableExpression* dataValueVarExpression = mOntoBuilder->getDataLiteralVariable(variableName);
				mVarExpSet.insert(dataValueVarExpression);
				mNameVarHash.insert(variableName, dataValueVarExpression);
				dataRangeExpr = dataValueVarExpression;
			} else {
				QPair<QString, QString> literalDatatypeStringPair = extractDataLiteral(objectString);
				CDataLexicalValueExpression* literalValueExp = mOntoBuilder->getDataLexicalValue(literalDatatypeStringPair.first);
				CDatatypeExpression* datatypeExp = mOntoBuilder->getDatatype(literalDatatypeStringPair.second);
				dataRangeExpr = mOntoBuilder->getDataLiteral(literalValueExp, datatypeExp);
			}						
			if (varExpSet && dynamic_cast<CDataLiteralVariableExpression*>(dataRangeExpr)) {
				varExpSet->insert((CDataLiteralVariableExpression*)dataRangeExpr);
			}
			return dataRangeExpr;
		}


	}; // end namespace Parser

}; // end namespace Konclude
