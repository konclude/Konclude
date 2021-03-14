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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#include "CSPARQLExtendedMappingBuildingParser.h"


namespace Konclude {

	namespace Parser {



		CSPARQLExtendedMappingBuildingParser::CSPARQLExtendedMappingBuildingParser(COntologyBuilder* ontoBuilder, CConcreteOntology* ontology) : CSPARQLSimpleBuildingParser(ontoBuilder, ontology) {
		}


		CSPARQLExtendedMappingBuildingParser::~CSPARQLExtendedMappingBuildingParser() {
		}


		QList<CAxiomExpression*> CSPARQLExtendedMappingBuildingParser::getTripleOWLAxioms(QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash, bool anonymousIndividualsAsVariables) {
			QList<CSPARQLBuildingParserTripleData*>* tripleDataList = createTermTripleData(subjectTripleHash);
			bool requiresMapping = analyseTermTripleDataRequiresExtendedMapping(tripleDataList, anonymousIndividualsAsVariables);
			if (!requiresMapping) {
				return CSPARQLSimpleBuildingParser::getTripleOWLAxioms(subjectTripleHash, anonymousIndividualsAsVariables);
			} else {
				return handlingMappingRequiredTermTripleData(tripleDataList, anonymousIndividualsAsVariables, nullptr);
			}
		}

		QList<CAxiomExpression*> CSPARQLExtendedMappingBuildingParser::getTripleOWLAxioms(QList<CRDFStringTriple>* tripleStringList, bool anonymousIndividualsAsVariables, QSet<CExpressionVariable*>* varExpSet) {
			QList<CSPARQLBuildingParserTripleData*>* tripleDataList = createTermTripleData(tripleStringList);
			bool requiresMapping = analyseTermTripleDataRequiresExtendedMapping(tripleDataList, anonymousIndividualsAsVariables);
			if (!requiresMapping) {
				return CSPARQLSimpleBuildingParser::getTripleOWLAxioms(tripleStringList, anonymousIndividualsAsVariables, varExpSet);
			} else {
				return handlingMappingRequiredTermTripleData(tripleDataList, anonymousIndividualsAsVariables, varExpSet);
			}
		}



		librdf_node* CSPARQLExtendedMappingBuildingParser::getRedlandRDFNode(CSPARQLBuildingParserTermData* termData, CRedlandStoredTriplesData* triplesData, QHash<CSPARQLBuildingParserTermData*, librdf_node*>* reuseExpHash, QHash<CBuildExpression*, QString>* termVariableExpressionNewEntityHash, cint64& newEntityNumber, QSet<CExpressionVariable*>* varExpSet) {


			librdf_world* world = triplesData->getRedlandWorld();

			librdf_node* node = nullptr;

			if (reuseExpHash->contains(termData)) {
				node = reuseExpHash->value(termData);
				node = librdf_new_node_from_node(node);
				return node;
			}


			CSPARQLBuildingParserTermData::TermType termType = termData->getTermType();
			const QString& termString = termData->getTermString();
			if (termType == CSPARQLBuildingParserTermData::RESOURCE_TYPE || termType == CSPARQLBuildingParserTermData::CLASS_TYPE || termType == CSPARQLBuildingParserTermData::PROPERTY_TYPE || termType == CSPARQLBuildingParserTermData::DATA_PROPERTY_TYPE || termType == CSPARQLBuildingParserTermData::NAMED_INDIVIDUAL_TYPE) {
				if (termString.startsWith("_:")) {
					QByteArray bytes = termString.toUtf8();
					node = librdf_new_node_from_blank_identifier(world, (const unsigned char*)bytes.constData());
				} else {
					QByteArray bytes = termString.toUtf8();
					raptor_uri* uri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)bytes.constData());
					node = librdf_new_node_from_uri(world, uri);
				}
			} else if (termType == CSPARQLBuildingParserTermData::DATA_LITERAL_TYPE) {

				cint64 sepPos = termString.lastIndexOf("^^");
				QByteArray datatypeBytes = termString.mid(sepPos+2).toUtf8();
				
				QByteArray literalBytes;
				if (termString.startsWith("\"")) {
					literalBytes = termString.mid(1, sepPos - 1).toUtf8(); // remove quotes
				} else {
					literalBytes = termString.mid(0, sepPos).toUtf8();
				}
				librdf_uri* datatypeUri = librdf_new_uri(world, (const unsigned char*)datatypeBytes.constData());
				node = librdf_new_node_from_typed_literal(world, (const unsigned char*)literalBytes.constData(), NULL, datatypeUri);

			} else if (termData->isVariableTerm()) {

				CExpressionVariable* varExp = nullptr;
				CBuildExpression* varBuildExp = nullptr;
				QString varString = getVariableName(termString);
				if (termType == CSPARQLBuildingParserTermData::CLASS_VARIABLE_TYPE) {
					auto exp = mOntoBuilder->getClassVariable(varString);
					varBuildExp = exp;
					varExp = exp;
				} else if (termType == CSPARQLBuildingParserTermData::OBJECT_PROPERTY_VARIABLE_TYPE) {
					auto exp = mOntoBuilder->getObjectPropertyVariable(varString);
					varBuildExp = exp;
					varExp = exp;
				} else if (termType == CSPARQLBuildingParserTermData::DATA_PROPERTY_VARIABLE_TYPE) {
					auto exp = mOntoBuilder->getDataPropertyVariable(varString);
					varBuildExp = exp;
					varExp = exp;
				} else if (termType == CSPARQLBuildingParserTermData::NAMED_INDIVIDUAL_VARIABLE_TYPE) {
					auto exp = mOntoBuilder->getIndividualVariable(varString, false);
					varBuildExp = exp;
					varExp = exp;
				} else if (termType == CSPARQLBuildingParserTermData::ANONYMOUS_INDIVIDUAL_VARIABLE_TYPE) {
					auto exp = mOntoBuilder->getIndividualVariable(varString, true);
					varBuildExp = exp;
					varExp = exp;
				} else if (termType == CSPARQLBuildingParserTermData::DATA_LITERAL_VARIABLE_TYPE) {
					auto exp = mOntoBuilder->getDataLiteralVariable(varString);
					varBuildExp = exp;
					varExp = exp;
				}

				if (varExp) {
					varExpSet->insert(varExp);
				}

				QString replacedEntityString = termVariableExpressionNewEntityHash->value(varBuildExp);
				if (replacedEntityString.isEmpty()) {
					replacedEntityString = QString("http://konclude.com/temporary/variable-encoded-as-fresh-entity/%1").arg(newEntityNumber++);
					termVariableExpressionNewEntityHash->insert(varBuildExp, replacedEntityString);
				}

				QByteArray bytes = replacedEntityString.toUtf8();
				raptor_uri* uri = (raptor_uri*)librdf_new_uri(world, (const unsigned char*)bytes.constData());
				node = librdf_new_node_from_uri(world, uri);

			}

			reuseExpHash->insert(termData, node);
			return node;
		}


		CRedlandStoredTriplesData* CSPARQLExtendedMappingBuildingParser::getUpdatingTripleData() {

			CRedlandStoredTriplesData* prevTripleData = nullptr;
			CRedlandStoredTriplesData* tripleData = nullptr;


			if (!tripleData) {
				tripleData = new CRedlandStoredTriplesData();
				tripleData->initTriplesData(CTRIPLES_DATA_UPDATE_TYPE::TRIPLES_DATA_QUERYING, prevTripleData);
			}

			librdf_world* world = tripleData->getRedlandWorld();
			if (!world) {
				world = librdf_new_world();
				librdf_world_open(world);
				tripleData->setRedlandWorldData(world);
			}

			librdf_storage* indexedStorage = tripleData->getRedlandIndexedStorage();
			if (!indexedStorage && world) {
				indexedStorage = librdf_new_storage(world, "hashes", NULL, "hash-type='memory',index-predicates='yes'");
				tripleData->setRedlandIndexedStorageData(indexedStorage);
			}

			librdf_model* indexedModel = tripleData->getRedlandIndexedModel();
			if (!indexedModel && indexedStorage) {
				indexedModel = librdf_new_model(world, indexedStorage, NULL);
				tripleData->setRedlandIndexedModelData(indexedModel);
			}

			if (!indexedModel) {
				delete tripleData;
				tripleData = nullptr;
			}

			return tripleData;

		}


		QList<CSPARQLBuildingParserTripleData*>* CSPARQLExtendedMappingBuildingParser::createTermTripleData(QHash<QString, CRDFHahsedSubjectData>* subjectTripleHash) {
			QHash<QString, CSPARQLBuildingParserTermData*> termStringDataHash;
			QList<CSPARQLBuildingParserTripleData*>* tripleDataList = new QList<CSPARQLBuildingParserTripleData *>();
			for (QHash<QString, CRDFHahsedSubjectData>::const_iterator it = subjectTripleHash->constBegin(), itEnd = subjectTripleHash->constEnd(); it != itEnd; ++it) {
				const QString& subjectString = it.key();
				const CRDFHahsedSubjectData& subjectData = it.value();

				CSPARQLBuildingParserTermData* term1 = getTermData(subjectString, &termStringDataHash);
				CSPARQLBuildingParserTermData* term2 = getTermData(subjectData.string[0], &termStringDataHash);
				CSPARQLBuildingParserTermData* term3 = getTermData(subjectData.string[1], &termStringDataHash);
				CSPARQLBuildingParserTripleData* triple = new CSPARQLBuildingParserTripleData(term1, term2, term3);
				tripleDataList->append(triple);

			}
			return tripleDataList;
		}


		QList<CSPARQLBuildingParserTripleData*>* CSPARQLExtendedMappingBuildingParser::createTermTripleData(QList<CRDFStringTriple>* tripleStringList) {
			QHash<QString, CSPARQLBuildingParserTermData*> termStringDataHash;
			QList<CSPARQLBuildingParserTripleData*>* tripleDataList = new QList<CSPARQLBuildingParserTripleData *>();
			for (CRDFStringTriple& tripleStringData : *tripleStringList) {
				CSPARQLBuildingParserTermData* term1 = getTermData(tripleStringData.string[0], &termStringDataHash);
				CSPARQLBuildingParserTermData* term2 = getTermData(tripleStringData.string[1], &termStringDataHash);
				CSPARQLBuildingParserTermData* term3 = getTermData(tripleStringData.string[2], &termStringDataHash);
				CSPARQLBuildingParserTripleData* triple = new CSPARQLBuildingParserTripleData(term1, term2, term3);
				tripleDataList->append(triple);
			}
			return tripleDataList;
		}

		CSPARQLBuildingParserTermData* CSPARQLExtendedMappingBuildingParser::getTermData(const QString& termString, QHash<QString, CSPARQLBuildingParserTermData*>* termStringDataHash) {
			CSPARQLBuildingParserTermData*& termData = (*termStringDataHash)[termString];
			if (!termData) {
				termData = new CSPARQLBuildingParserTermData(termString);
			}
			return termData;
		}

		bool CSPARQLExtendedMappingBuildingParser::analyseTermTripleDataRequiresExtendedMapping(QList<CSPARQLBuildingParserTripleData*>* tripleList, bool anonymousIndividualsAsVariables) {
			bool updated = true;
			bool mappingRequired = false;

			for (CSPARQLBuildingParserTripleData* tripleData : *tripleList) {
				if (!tripleData->areTermsCompletelyTyped()) {
					for (cint64 i = 0; i < 3; ++i) {
						CSPARQLBuildingParserTermData* term = tripleData->getTermData(i);
						if (term->getTermType() == CSPARQLBuildingParserTermData::UNKNON_TYPE) {
							const QString& termString = term->getTermString();
							if (termString.startsWith("?") || termString.startsWith("$")) {
								term->setTermType(CSPARQLBuildingParserTermData::VARIABLE_TYPE);
							} else if (termString.startsWith("_:")) {
								term->setTermType(CSPARQLBuildingParserTermData::BLANK_NODE_TYPE);
							} else if (termString.startsWith("\"")) {
								term->setTermType(CSPARQLBuildingParserTermData::DATA_LITERAL_TYPE);
							} else {
								term->setTermType(CSPARQLBuildingParserTermData::RESOURCE_TYPE);
							}
						}
					}
				}

			}


			QSet<QString> predClassConSet;
			predClassConSet.insert(PREFIX_RDFS_SUBCLASS_OF);
			predClassConSet.insert(PREFIX_OWL_DISJOINT_WITH);
			predClassConSet.insert(PREFIX_OWL_DISJOINT_UNION_OF);


			QSet<QString> mapReqPredClassConSet;
			mapReqPredClassConSet.insert(PREFIX_OWL_INTERSECTION_OF);
			mapReqPredClassConSet.insert(PREFIX_OWL_UNION_OF);
			mapReqPredClassConSet.insert(PREFIX_OWL_COMPLEMENT_OF);
			mapReqPredClassConSet.insert(PREFIX_OWL_SOME_VALUES_FROM);
			mapReqPredClassConSet.insert(PREFIX_OWL_ALL_VALUES_FROM);
			mapReqPredClassConSet.insert(PREFIX_OWL_ALL_DISJOINT_CLASSES);


			QSet<QString> mapReqPredClassSingConSet;
			mapReqPredClassSingConSet.insert(PREFIX_OWL_MIN_CARDINALITY);
			mapReqPredClassSingConSet.insert(PREFIX_OWL_MAX_CARDINALITY);
			mapReqPredClassSingConSet.insert(PREFIX_OWL_CARDINALITY);
			mapReqPredClassSingConSet.insert(PREFIX_OWL_MIN_QUALIFIED_CARDINALITY);
			mapReqPredClassSingConSet.insert(PREFIX_OWL_MAX_QUALIFIED_CARDINALITY);
			mapReqPredClassSingConSet.insert(PREFIX_OWL_QUALIFIED_CARDINALITY);
			mapReqPredClassSingConSet.insert(PREFIX_OWL_ON_DATA_RANGE);


			CBUILDHASH<CStringRefStringHasher, CObjectPropertyExpression*>* objPropEntityBuildHash = mOntology->getBuildData()->getObjectPropertyEntityBuildHash();
			CBUILDHASH<CStringRefStringHasher, CDataPropertyExpression*>* dataPropEntityBuildHash = mOntology->getBuildData()->getDataPropertyEntityBuildHash();


			while (updated) {
				updated = false;
				for (CSPARQLBuildingParserTripleData* tripleData : *tripleList) {
					if (!tripleData->areTermsCompletelyTyped()) {
						CSPARQLBuildingParserTermData* term1 = tripleData->getTermData(0);
						CSPARQLBuildingParserTermData* term2 = tripleData->getTermData(1);
						CSPARQLBuildingParserTermData* term3 = tripleData->getTermData(2);

						if (term2->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE) {
							const QString& term2String = term2->getTermString();
							const QString& term3String = term3->getTermString();

							if (objPropEntityBuildHash->contains(term2String)) {
								makeIndividualTerm(term1, anonymousIndividualsAsVariables);
								makeIndividualTerm(term3, anonymousIndividualsAsVariables);
							} else if (dataPropEntityBuildHash->contains(term2String)) {
								makeIndividualTerm(term1, anonymousIndividualsAsVariables);
								makeDataLiteralTerm(term3);
							}

							if (predClassConSet.contains(term2String)) {
								updated |= makeClassTerm(term1);
								updated |= makeClassTerm(term3);
							} else if (mapReqPredClassConSet.contains(term2String)) {
								updated |= makeClassTerm(term1);
								updated |= makeClassTerm(term3);
								mappingRequired = true;
							} else if (mapReqPredClassSingConSet.contains(term2String)) {
								updated |= makeClassTerm(term1);
								mappingRequired = true;
							}
							if (term2String == PREFIX_RDFS_SUBPROPERTY_OF) {
								bool dataProp = term1->isDataPropertyTerm() || term3->isDataPropertyTerm();
								bool objectProp = term1->isObjectPropertyTerm() || term3->isObjectPropertyTerm();
								if (dataProp && !objectProp) {
									updated |= makeDataPropertyTerm(term1);
									updated |= makeDataPropertyTerm(term3);
								} else if (!dataProp && objectProp) {
									updated |= makeObjectPropertyTerm(term1);
									updated |= makeObjectPropertyTerm(term3);
								} else {
									updated |= makePropertyTerm(term1);
									updated |= makePropertyTerm(term3);
								}
							}
							if (term2String == PREFIX_RDF_TYPE) {

								if (term3->isClassTerm() && !term1->isCompletelyTyped()) {
									makeIndividualTerm(term1, anonymousIndividualsAsVariables);
								}

								if (term3String == PREFIX_OWL_OBJECT_PROPERTY) {
									updated |= makeObjectPropertyTerm(term1);
								} else if (term3String == PREFIX_OWL_DATATYPE_PROPERTY) {
									updated |= makeDataPropertyTerm(term1);
								} else if (term3String == PREFIX_OWL_CLASS) {
									updated |= makeClassTerm(term1);
								} else if (term3String == PREFIX_OWL_RESTRICTION) {
									updated |= makeClassTerm(term1);
									mappingRequired = true;
								} else if (term3String == PREFIX_OWL_NAMED_INDIVIDUAL) {
									updated |= makeIndividualTerm(term1, anonymousIndividualsAsVariables);
								} else if (term3String == PREFIX_RDFS_DATATYPE) {
									updated |= makeDataLiteralTerm(term1);
								}

							}

							if (term2String == PREFIX_RDF_FIRST || term2String == PREFIX_RDF_REST) {
								if (term3->getTermString() != PREFIX_RDF_NIL) {
									if (term1->isCompletelyTyped() && !term3->isCompletelyTyped()) {
										updated |= makeTermTypedBasedOnTerm(term3, term1, anonymousIndividualsAsVariables);
									}
									if (!term1->isCompletelyTyped() && term3->isCompletelyTyped()) {
										updated |= makeTermTypedBasedOnTerm(term1, term3, anonymousIndividualsAsVariables);
									}
								}
							}
							else if (term2String == PREFIX_OWL_ONE_OF) {
								updated |= makeClassTerm(term1);
								updated |= makeIndividualTerm(term3, anonymousIndividualsAsVariables);
								mappingRequired = true;
							}
							else if (term2String == PREFIX_OWL_HAS_VALUE) {
								mappingRequired = true;
								updated |= makeClassTerm(term1);
							}
							else if (term2String == PREFIX_OWL_ON_PROPERTY) {
								mappingRequired = true;
								updated |= makeClassTerm(term1);
								updated |= makePropertyTerm(term3);
							}
							else if (term2String == PREFIX_OWL_INVERSE_OF) {
								mappingRequired = true;
								updated |= makePropertyTerm(term1);
								updated |= makePropertyTerm(term3);
								if (term1->isCompletelyTyped() && !term3->isCompletelyTyped()) {
									updated |= makeTermTypedBasedOnTerm(term3, term1, anonymousIndividualsAsVariables);
								}
								if (!term1->isCompletelyTyped() && term3->isCompletelyTyped()) {
									updated |= makeTermTypedBasedOnTerm(term1, term3, anonymousIndividualsAsVariables);
								}
							}


						}
					
					}
				}
			}

			return mappingRequired;
		}


		bool CSPARQLExtendedMappingBuildingParser::makeTermTypedBasedOnTerm(CSPARQLBuildingParserTermData* termData, CSPARQLBuildingParserTermData* baseData, bool anonymousIndividualsAsVariables) {
			if (baseData->isClassTerm()) {
				return makeClassTerm(termData);
			} else if (baseData->isObjectPropertyTerm()) {
				return makeObjectPropertyTerm(termData);
			} else if (baseData->isDataPropertyTerm()) {
				return makeDataPropertyTerm(termData);
			} else if (baseData->isIndividualTerm()) {
				return makeIndividualTerm(termData, anonymousIndividualsAsVariables);
			} else if (baseData->isLiteralTerm()) {
				return makeDataLiteralTerm(termData);
			}
			return false;
		}


		bool CSPARQLExtendedMappingBuildingParser::makeIndividualTerm(CSPARQLBuildingParserTermData* termData, bool anonymousIndividualsAsVariables) {
			if (termData->getTermType() == CSPARQLBuildingParserTermData::VARIABLE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::NAMED_INDIVIDUAL_VARIABLE_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::NAMED_INDIVIDUAL_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::BLANK_NODE_TYPE) {
				if (anonymousIndividualsAsVariables) {
					termData->setTermType(CSPARQLBuildingParserTermData::ANONYMOUS_INDIVIDUAL_VARIABLE_TYPE);
				} else {
					termData->setTermType(CSPARQLBuildingParserTermData::ANONYMOUS_INDIVIDUAL_TYPE);
				}
				return true;
			}
			return false;
		}

		bool CSPARQLExtendedMappingBuildingParser::makeClassTerm(CSPARQLBuildingParserTermData* termData) {
			if (termData->getTermType() == CSPARQLBuildingParserTermData::VARIABLE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::CLASS_VARIABLE_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::CLASS_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::BLANK_NODE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::CLASS_TYPE);
				return true;
			}
			return false;
		}


		bool CSPARQLExtendedMappingBuildingParser::makePropertyTerm(CSPARQLBuildingParserTermData* termData) {
			if (termData->getTermType() == CSPARQLBuildingParserTermData::VARIABLE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::PROPERTY_VARIABLE_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::PROPERTY_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::BLANK_NODE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::PROPERTY_TYPE);
				return true;
			}
			return false;
		}


		bool CSPARQLExtendedMappingBuildingParser::makeObjectPropertyTerm(CSPARQLBuildingParserTermData* termData) {
			if (termData->getTermType() == CSPARQLBuildingParserTermData::VARIABLE_TYPE || termData->getTermType() == CSPARQLBuildingParserTermData::PROPERTY_VARIABLE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::OBJECT_PROPERTY_VARIABLE_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE || termData->getTermType() == CSPARQLBuildingParserTermData::PROPERTY_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::OBJECT_PROPERTY_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::BLANK_NODE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::OBJECT_PROPERTY_TYPE);
				return true;
			}
			return false;
		}


		bool CSPARQLExtendedMappingBuildingParser::makeDataPropertyTerm(CSPARQLBuildingParserTermData* termData) {
			if (termData->getTermType() == CSPARQLBuildingParserTermData::VARIABLE_TYPE || termData->getTermType() == CSPARQLBuildingParserTermData::PROPERTY_VARIABLE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::DATA_PROPERTY_VARIABLE_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE || termData->getTermType() == CSPARQLBuildingParserTermData::PROPERTY_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::DATA_PROPERTY_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::BLANK_NODE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::DATA_PROPERTY_TYPE);
				return true;
			}
			return false;
		}


		bool CSPARQLExtendedMappingBuildingParser::makeDataLiteralTerm(CSPARQLBuildingParserTermData* termData) {
			if (termData->getTermType() == CSPARQLBuildingParserTermData::VARIABLE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::DATA_LITERAL_VARIABLE_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::RESOURCE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::DATA_LITERAL_TYPE);
				return true;
			}
			if (termData->getTermType() == CSPARQLBuildingParserTermData::BLANK_NODE_TYPE) {
				termData->setTermType(CSPARQLBuildingParserTermData::DATA_LITERAL_TYPE);
				return true;
			}
			return false;
		}

		QList<CAxiomExpression *> CSPARQLExtendedMappingBuildingParser::handlingMappingRequiredTermTripleData(QList<CSPARQLBuildingParserTripleData *>* tripleDataList, bool anonymousIndividualsAsVariables, QSet<CExpressionVariable *>* varExpSet) {
			QList<CAxiomExpression*> axList;
			CRedlandStoredTriplesData* triplesData = getUpdatingTripleData();
			librdf_world* world = triplesData->getRedlandWorld();
			librdf_model* model = triplesData->getRedlandIndexedModel();

			CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper* expressionMapper = new CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper(mOntoBuilder);
			QHash<CSPARQLBuildingParserTermData*, librdf_node*> reuseExpHash;
			QHash<CBuildExpression*, QString> termVariableExpressionNewEntityHash;
			cint64 newEntityNumber = 0;

			CXLinker<librdf_statement*>* statementLinker = nullptr;
			CXLinker<librdf_statement*>* lastStatementLinker = nullptr;

			for (CSPARQLBuildingParserTripleData* tripleData : *tripleDataList) {

				librdf_node* subjectNode = getRedlandRDFNode(tripleData->getTermData(0), triplesData, &reuseExpHash, &termVariableExpressionNewEntityHash, newEntityNumber, varExpSet);
				librdf_node* predicateNode = getRedlandRDFNode(tripleData->getTermData(1), triplesData, &reuseExpHash, &termVariableExpressionNewEntityHash, newEntityNumber, varExpSet);
				librdf_node* objectNode = getRedlandRDFNode(tripleData->getTermData(2), triplesData, &reuseExpHash, &termVariableExpressionNewEntityHash, newEntityNumber, varExpSet);
				librdf_statement* statement = librdf_new_statement_from_nodes(world, subjectNode, predicateNode, objectNode);

				librdf_statement* newStatement = librdf_new_statement_from_statement(statement);
				librdf_model_add_statement(model, newStatement);

				CXLinker<librdf_statement*>* newStatementLinker = new CXLinker<librdf_statement*>();
				newStatementLinker->initLinker(statement, nullptr);
				if (statementLinker) {
					lastStatementLinker->setNext(newStatementLinker);
					lastStatementLinker = newStatementLinker;
				} else {
					statementLinker = newStatementLinker;
					lastStatementLinker = newStatementLinker;
				}

			}
			triplesData->setRedlandStatementLinker(statementLinker);

			QHash<QString, CBuildExpression*> freshEntityUseVariableExpressionHash;
			for (QHash<CBuildExpression*, QString>::const_iterator it = termVariableExpressionNewEntityHash.constBegin(), itEnd = termVariableExpressionNewEntityHash.constEnd(); it != itEnd; ++it) {
				freshEntityUseVariableExpressionHash.insert(it.value(), it.key());
			}
			expressionMapper->useEntitiesVariableExpressionMapping(freshEntityUseVariableExpressionHash, triplesData);
			COntologyTriplesData ontTripData;
			ontTripData.addTriplesData(triplesData);
			expressionMapper->mapTriples(mOntology, &ontTripData);
			axList = expressionMapper->takeQueryingCollectedAxiomExpressions();

			return axList;
		}

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_REDLAND_INTEGRATION
