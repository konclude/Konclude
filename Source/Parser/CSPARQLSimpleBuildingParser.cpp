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

					if (predicateString == "a" || predicateString == "http://www.w3.org/1999/02/22-rdf-syntax-ns#type") {
						if (objectString == "http://www.w3.org/2002/07/owl#NamedIndividual") {
							subjectType = CBuildExpression::BETNAMEDINDIVIDUAL;
							hasType = true;
							predObjDataList.erase(it);
						} else if (objectString == "http://www.w3.org/2002/07/owl#Class" || objectString == "http://www.w3.org/1999/02/22-rdf-syntax-ns#Class") {
							subjectType = CBuildExpression::BETCLASS;
							hasType = true;
							predObjDataList.erase(it);
						} else if (objectString == "http://www.w3.org/2002/07/owl#ObjectProperty") {
							subjectType = CBuildExpression::BETOBJECTPROPERTY;
							hasType = true;
							predObjDataList.erase(it);
						} else if (objectString == "http://www.w3.org/2002/07/owl#DatatypeProperty") {
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

						if (predicateString == "a" || predicateString == "http://www.w3.org/1999/02/22-rdf-syntax-ns#type") {
							CClassExpression* classExp = mOntoBuilder->getClass(objectString);
							CAxiomExpression* assExp = mOntoBuilder->getClassAssertion(indiExpression, classExp);
							axiomList.append(assExp);
						} else {
							if (!isDataProperty(predicateString)) {
								CIndividualTermExpression* otherIndiExpression = nullptr;
								if (objectString.startsWith("?") || objectString.startsWith("$")) {
									QString variableName = getVariableName(objectString);
									CIndividualVariableExpression* varIndiExpression = mOntoBuilder->getIndividualVariable(variableName, false);
									mVarExpSet.insert(varIndiExpression);
									mNameVarHash.insert(variableName, varIndiExpression);
									otherIndiExpression = varIndiExpression;
								} else if (objectString.startsWith("_:")) {
									if (anonymousIndividualsAsVariables) {
										otherIndiExpression = mOntoBuilder->getIndividualVariable(objectString.mid(2), true);
									} else {
										otherIndiExpression = mOntoBuilder->getAnonymousIndividual(mOntoBuilder->getOntologyAnonymousIdentifier(mOntoBuilder->getOntologyName()), objectString.mid(2));
									}
								} else {
									otherIndiExpression = mOntoBuilder->getNamedIndividual(objectString);
								}
								CObjectPropertyExpression* propExp = mOntoBuilder->getObjectProberty(predicateString);
								CObjectPropertyAssertionExpression* propAss = mOntoBuilder->getObjectPropertyAssertion(indiExpression, propExp, otherIndiExpression);
								axiomList.append(propAss);
							} else {
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

				QString predicateString = triple.string[1];
				QString objectString = triple.string[2];

				if (predicateString == "a" || predicateString == "http://www.w3.org/1999/02/22-rdf-syntax-ns#type") {
					CClassExpression* classExp = mOntoBuilder->getClass(objectString);
					CAxiomExpression* assExp = mOntoBuilder->getClassAssertion(indiExpression, classExp);
					axiomList.append(assExp);
				} else {
					if (!isDataProperty(predicateString)) {
						CIndividualTermExpression* otherIndiExpression = nullptr;
						if (objectString.startsWith("?") || objectString.startsWith("$")) {
							QString variableName = getVariableName(objectString);
							CIndividualVariableExpression* varIndiExpression = mOntoBuilder->getIndividualVariable(variableName, false);
							mVarExpSet.insert(varIndiExpression);
							mNameVarHash.insert(variableName, varIndiExpression);
							otherIndiExpression = varIndiExpression;
						} else if (objectString.startsWith("_:")) {
							if (anonymousIndividualsAsVariables) {
								otherIndiExpression = mOntoBuilder->getIndividualVariable(objectString.mid(2), true);
							} else {
								otherIndiExpression = mOntoBuilder->getAnonymousIndividual(mOntoBuilder->getOntologyAnonymousIdentifier(mOntoBuilder->getOntologyName()), objectString.mid(2));
							}
						} else {
							otherIndiExpression = mOntoBuilder->getNamedIndividual(objectString);
						}
						if (varExpSet && dynamic_cast<CIndividualVariableExpression*>(otherIndiExpression)) {
							varExpSet->insert((CIndividualVariableExpression*)otherIndiExpression);
						}
						CObjectPropertyExpression* propExp = mOntoBuilder->getObjectProberty(predicateString);
						CObjectPropertyAssertionExpression* propAss = mOntoBuilder->getObjectPropertyAssertion(indiExpression, propExp, otherIndiExpression);
						axiomList.append(propAss);
					} else {
						CDataLiteralTermExpression* dataRangeExpr = nullptr;
						if (objectString.startsWith("?") || objectString.startsWith("$")) {
							QString variableName = getVariableName(objectString);
							CDataLiteralVariableExpression* dataValueVarExpression = mOntoBuilder->getDataLiteralVariable(variableName);
							mVarExpSet.insert(dataValueVarExpression);
							mNameVarHash.insert(variableName,dataValueVarExpression);
							dataRangeExpr = dataValueVarExpression;
						} else {
							QPair<QString, QString> literalDatatypeStringPair = extractDataLiteral(objectString);
							CDataLexicalValueExpression* literalValueExp = mOntoBuilder->getDataLexicalValue(literalDatatypeStringPair.first);
							CDatatypeExpression* datatypeExp = mOntoBuilder->getDatatype(literalDatatypeStringPair.second);
							dataRangeExpr = mOntoBuilder->getDataLiteral(literalValueExp,datatypeExp);
						}
						if (varExpSet && dynamic_cast<CDataLiteralVariableExpression*>(dataRangeExpr)) {
							varExpSet->insert((CDataLiteralVariableExpression*)dataRangeExpr);
						}
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




	}; // end namespace Parser

}; // end namespace Konclude
