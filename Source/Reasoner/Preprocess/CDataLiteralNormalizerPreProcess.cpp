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

#include "CDataLiteralNormalizerPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			bool CDataLiteralNormalizerPreProcess::mConvArraysInitialized = false;
			cint64* CDataLiteralNormalizerPreProcess::mHexStringBinaryDataConvArray = nullptr;
			cint64* CDataLiteralNormalizerPreProcess::mBase64StringBinaryDataConvArray = nullptr;

			CDataLiteralNormalizerPreProcess::CDataLiteralNormalizerPreProcess() {
				mLastConceptId = 0;
				if (!mConvArraysInitialized) {
					cint64* tmpHexStringBinaryDataConvArray = new cint64[127];
					for (cint64 i = 0; i < 127; ++i) {
						tmpHexStringBinaryDataConvArray[i] = -1;
					}
					for (cint64 i = '0'; i <= '9'; ++i) {
						tmpHexStringBinaryDataConvArray[i] = i - '0';
					}
					for (cint64 i = 'A'; i <= 'F'; ++i) {
						tmpHexStringBinaryDataConvArray[i] = i - 'A' + 10;
					}
					for (cint64 i = 'a'; i <= 'f'; ++i) {
						tmpHexStringBinaryDataConvArray[i] = i - 'a' + 10;
					}
					mHexStringBinaryDataConvArray = tmpHexStringBinaryDataConvArray;


					cint64* tmpBase64StringBinaryDataConvArray = new cint64[127];
					for (cint64 i = 0; i < 127; ++i) {
						tmpBase64StringBinaryDataConvArray[i] = -1;
					}
					for (cint64 i = 'A'; i <= 'Z'; ++i) {
						tmpBase64StringBinaryDataConvArray[i] = i - 'A';
					}
					for (cint64 i = 'a'; i <= 'z'; ++i) {
						tmpBase64StringBinaryDataConvArray[i] = i - 'a' + 26;
					}
					for (cint64 i = '0'; i <= '9'; ++i) {
						tmpBase64StringBinaryDataConvArray[i] = i - '0' + 52;
					}
					tmpBase64StringBinaryDataConvArray['+'] = 62;
					tmpBase64StringBinaryDataConvArray['/'] = 63;
					tmpBase64StringBinaryDataConvArray['-'] = 62;
					tmpBase64StringBinaryDataConvArray['_'] = 63;
					mBase64StringBinaryDataConvArray = tmpBase64StringBinaryDataConvArray;

					mConvArraysInitialized = true;
				}
			}


			CDataLiteralNormalizerPreProcess::~CDataLiteralNormalizerPreProcess() {
			}


			CConcept *CDataLiteralNormalizerPreProcess::preprocessDataConcept(CConcept* dataConcept, CConcreteOntology *ontology, CPreProcessContext* context) {
				if (ontology) {
					mTBox = ontology->getDataBoxes()->getTBox();
					mABox = ontology->getDataBoxes()->getABox();
					mRBox = ontology->getDataBoxes()->getRBox();
					mMBox = ontology->getDataBoxes()->getMBox();



					mRolesVector = mRBox->getRoleVector();
					mConceptVector = mTBox->getConceptVector();
					mIndiVec = mABox->getIndividualVector();


					mOntology = ontology;
					mBoxContext = mOntology->getDataBoxes()->getBoxContext();
					mMemMan = mBoxContext->getMemoryAllocationManager();

					cint64 conceptCount = mConceptVector->getItemCount();
					if (dataConcept) {
						CDataLiteral* dataLiteral = dataConcept->getDataLiteral();
						if (dataLiteral) {
							CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
							if (!dataLiteralValue) {
								dataLiteralValue = createDataLiteralValue(dataLiteral, context);
								dataLiteral->setDataLiteralValue(dataLiteralValue);
							}
						}

						if (dataConcept->getOperatorCode() == CCDATARESTRICTION) {
							if (normalizeDatatypeRestriction(dataConcept, context)) {
								conceptCount = mConceptVector->getItemCount();
							}
						}
						else if (dataConcept->getOperatorCode() == CCDATALITERAL) {
							normalizeDataLiteral(dataConcept, context);
						}
					}
				}
				return dataConcept;
			}


			CConcreteOntology *CDataLiteralNormalizerPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				mOntology = ontology;
				mContext = context;
				mLastConceptId = 0;
				if (ontology) {
					mTBox = ontology->getDataBoxes()->getTBox();
					mABox = ontology->getDataBoxes()->getABox();
					mRBox = ontology->getDataBoxes()->getRBox();
					mMBox = ontology->getDataBoxes()->getMBox();


					mRolesVector = mRBox->getRoleVector();
					mConceptVector = mTBox->getConceptVector();
					mIndiVec = mABox->getIndividualVector();


					mOntology = ontology;
					mBoxContext = mOntology->getDataBoxes()->getBoxContext();
					mMemMan = mBoxContext->getMemoryAllocationManager();


				}

				return continuePreprocessing();
			}



			CConcreteOntology* CDataLiteralNormalizerPreProcess::continuePreprocessing() {
				if (mOntology) {
					cint64 conceptCount = mConceptVector->getItemCount();
					for (cint64 i = mLastConceptId; i < conceptCount; ++i) {
						CConcept* concept = mConceptVector->getLocalData(i);
						if (concept) {
							CDataLiteral* dataLiteral = concept->getDataLiteral();
							if (dataLiteral) {
								CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
								if (!dataLiteralValue) {
									dataLiteralValue = createDataLiteralValue(dataLiteral, mContext);
									dataLiteral->setDataLiteralValue(dataLiteralValue);
								}
							}

							if (concept->getOperatorCode() == CCDATARESTRICTION) {
								if (normalizeDatatypeRestriction(concept, mContext)) {
									conceptCount = mConceptVector->getItemCount();
								}
							} else if (concept->getOperatorCode() == CCDATALITERAL) {
								normalizeDataLiteral(concept, mContext);
							}
						}
					}
					mLastConceptId = conceptCount;

					cint64 indiCount = mIndiVec->getItemCount();
					for (cint64 i = mLastIndiId; i < indiCount; ++i) {
						CIndividual* indi = mIndiVec->getLocalData(i);
						if (indi) {
							for (CDataAssertionLinker* dataAssLinkerIt = indi->getAssertionDataLinker(); dataAssLinkerIt; dataAssLinkerIt = dataAssLinkerIt->getNext()) {
								CDataLiteral* dataLiteral = dataAssLinkerIt->getDataLiteral();
								if (dataLiteral) {
									CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
									if (!dataLiteralValue) {
										dataLiteralValue = createDataLiteralValue(dataLiteral, mContext);
										dataLiteral->setDataLiteralValue(dataLiteralValue);
									}
								}

							}
						}
					}
					mLastIndiId = indiCount;
				}
				return mOntology;
			}


			CDataLiteralValue* CDataLiteralNormalizerPreProcess::createPreprocessedDataLiteralValue(CDataLiteral* dataLiteral, CPreProcessContext* context, CContext* boxContext) {
				mOntology = context->getOntology();
				mMemMan = context->getMemoryAllocationManager();
				mContext = context;
				mBoxContext = boxContext;


				CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
				if (!dataLiteralValue) {
					dataLiteralValue = createDataLiteralValue(dataLiteral, context);
					dataLiteral->setDataLiteralValue(dataLiteralValue);
				}
				return dataLiteralValue;
			}




			bool CDataLiteralNormalizerPreProcess::normalizeDataLiteral(CConcept* dataLiteralConcept, CPreProcessContext* context) {
				bool dataLiteralNormalized = false;
				CDatatype* datatype = dataLiteralConcept->getDatatype();
				if (!datatype) {
				}
				return dataLiteralNormalized;
			}


			bool CDataLiteralNormalizerPreProcess::normalizeDatatypeRestriction(CConcept* datatypeRestrictionConcept, CPreProcessContext* context) {
				bool datatypeRestrictionNormalized = false;
				cint64 restrictionCode = datatypeRestrictionConcept->getParameter();
				CDatatype* datatype = datatypeRestrictionConcept->getDatatype();
				if (datatype && !datatype->isBasicDatatype()) {
					CDatatype* basicDatatype = datatype->getBasicDatatype();
					CConcept* datatypeConcept = datatype->getDatatypeConcept();
					if (datatypeConcept && basicDatatype) {
						CDataLiteral* dataLiteral = datatypeRestrictionConcept->getDataLiteral();

						datatypeRestrictionConcept->setDatatype(nullptr);
						datatypeRestrictionConcept->setOperatorCode(CCAND);
						datatypeRestrictionConcept->setDataLiteral(nullptr);
						datatypeRestrictionConcept->setParameter(0);
						datatypeRestrictionConcept->setOperandCount(0);

						CConcept* newRestrictionConcept = createConcept();
						newRestrictionConcept->setDataLiteral(dataLiteral);
						newRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
						newRestrictionConcept->setParameter(restrictionCode);
						newRestrictionConcept->setDatatype(basicDatatype);
						addConceptOperand(datatypeRestrictionConcept,newRestrictionConcept,false);
						addConceptOperand(datatypeRestrictionConcept,datatypeConcept,false);

						datatypeRestrictionNormalized = true;
					}
				} else {
					if (restrictionCode == CDFC_LENGTH) {
						CConcept* datatypeConcept = datatype->getDatatypeConcept();
						CDataLiteral* dataLiteral = datatypeRestrictionConcept->getDataLiteral();

						datatypeRestrictionConcept->setDatatype(nullptr);
						datatypeRestrictionConcept->setOperatorCode(CCAND);
						datatypeRestrictionConcept->setDataLiteral(nullptr);
						datatypeRestrictionConcept->setParameter(0);
						datatypeRestrictionConcept->setOperandCount(0);

						CConcept* minLengthRestrictionConcept = createConcept();
						minLengthRestrictionConcept->setDataLiteral(dataLiteral);
						minLengthRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
						minLengthRestrictionConcept->setParameter(CDFC_MIN_LENGTH_INCLUSIVE);
						minLengthRestrictionConcept->setDatatype(datatype);
						addConceptOperand(datatypeRestrictionConcept,minLengthRestrictionConcept,false);

						CConcept* maxLengthRestrictionConcept = createConcept();
						maxLengthRestrictionConcept->setDataLiteral(dataLiteral);
						maxLengthRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
						maxLengthRestrictionConcept->setParameter(CDFC_MAX_LENGTH_INCLUSIVE);
						maxLengthRestrictionConcept->setDatatype(datatype);
						addConceptOperand(datatypeRestrictionConcept,maxLengthRestrictionConcept,false);

						datatypeRestrictionNormalized = true;


					} else if (restrictionCode == CDFC_MIN_LENGTH_INCLUSIVE) {
						CDataLiteral* dataLiteral = datatypeRestrictionConcept->getDataLiteral();
						dataLiteral = getNormalizedDataLiteralForLengthRestriction(datatype,dataLiteral,true,context);
						datatypeRestrictionConcept->setParameter(CDFC_MIN_INCLUSIVE);
						datatypeRestrictionConcept->setDataLiteral(dataLiteral);
						datatypeRestrictionNormalized = true;

					} else if (restrictionCode == CDFC_MAX_LENGTH_INCLUSIVE) {
						CDataLiteral* dataLiteral = datatypeRestrictionConcept->getDataLiteral();
						dataLiteral = getNormalizedDataLiteralForLengthRestriction(datatype,dataLiteral,false,context);
						datatypeRestrictionConcept->setParameter(CDFC_MAX_EXCLUSIVE);
						datatypeRestrictionConcept->setDataLiteral(dataLiteral);
						datatypeRestrictionNormalized = true;
					}
				}
				return datatypeRestrictionNormalized;
			}


			CDataLiteral* CDataLiteralNormalizerPreProcess::getNormalizedDataLiteralForLengthRestriction(CDatatype* datatype, CDataLiteral* dataLiteral, bool roundUp, CPreProcessContext* context) {
				CDataLiteral* adaptedDataLiteral = dataLiteral;
				if (dataLiteral->getDatatype() != datatype) {
					adaptedDataLiteral = nullptr;

					CDataLiteralValue* dataLiteralValue = dataLiteral->getDataLiteralValue();
					if (!dataLiteralValue) {
						dataLiteralValue = createDataLiteralValue(dataLiteral,context);
						dataLiteral->setDataLiteralValue(dataLiteralValue);
					}

					bool lengthParsed = false;
					bool infinite = false;
					cint64 length = 0;
					CDataLiteralRealValue* realDataLiteralValue = dynamic_cast<CDataLiteralRealValue*>(dataLiteralValue);
					if (realDataLiteralValue && !realDataLiteralValue->isNegative()) {
						if (realDataLiteralValue->isInfinite()) {
							infinite = true;
							lengthParsed = true;
						} else {
							length = realDataLiteralValue->getInteger();
							lengthParsed = true;
							if (roundUp && realDataLiteralValue->getNumerator() != 0) {
								++length;
							}
							if (!roundUp) {
								++length;
							}
						}
					}

					if (lengthParsed) {

						if (datatype->getBasicDatatypeType() == CDatatype::DBT_STRING) {
							CDataLiteralStringValue* dataLitStringValue = CObjectParameterizingAllocator<CDataLiteralStringValue, CContext*>::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							if (infinite) {
								dataLitStringValue->initValueFromInfiniteLength();
							} else {
								dataLitStringValue->initValueFromLength(length);
							}
							CDataLiteral* dataLitString = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							dataLitString->initDataLiteral("",datatype);
							dataLitString->setDataLiteralValue(dataLitStringValue);
							adaptedDataLiteral = dataLitString;


						} else if (datatype->getBasicDatatypeType() == CDatatype::DBT_IRI) {
							CDataLiteralIRIValue* dataLitIRIValue = CObjectAllocator<CDataLiteralIRIValue>::allocateAndConstruct(mMemMan);
							if (infinite) {
								dataLitIRIValue->initValueFromInfiniteLength();
							} else {
								dataLitIRIValue->initValueFromLength(length);
							}
							CDataLiteral* dataLitIRI = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							dataLitIRI->initDataLiteral("",datatype);
							dataLitIRI->setDataLiteralValue(dataLitIRIValue);
							adaptedDataLiteral = dataLitIRI;

						} else if (datatype->getBasicDatatypeType() == CDatatype::DBT_HEXBINARY) {
							CDataLiteralBinaryHexDataValue* dataLitHexBinaryValue = CObjectParameterizingAllocator< CDataLiteralBinaryHexDataValue,CContext* >::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							if (infinite) {
								dataLitHexBinaryValue->initValueFromInfiniteLength();
							} else {
								dataLitHexBinaryValue->initValueFromLength(length);
							}
							CDataLiteral* dataLitIRI = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							dataLitIRI->initDataLiteral("", datatype);
							dataLitIRI->setDataLiteralValue(dataLitHexBinaryValue);
							adaptedDataLiteral = dataLitIRI;

						} else if (datatype->getBasicDatatypeType() == CDatatype::DBT_BASE64BINARY) {
							CDataLiteralBinaryBase64DataValue* dataLitBase64BinaryValue = CObjectParameterizingAllocator< CDataLiteralBinaryBase64DataValue,CContext* >::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							if (infinite) {
								dataLitBase64BinaryValue->initValueFromInfiniteLength();
							} else {
								dataLitBase64BinaryValue->initValueFromLength(length);
							}
							CDataLiteral* dataLitIRI = CObjectParameterizingAllocator<CDataLiteral, CContext*>::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
							dataLitIRI->initDataLiteral("",datatype);
							dataLitIRI->setDataLiteralValue(dataLitBase64BinaryValue);
							adaptedDataLiteral = dataLitIRI;
						}
					}

				}
				return adaptedDataLiteral;
			}



			CConcept* CDataLiteralNormalizerPreProcess::createConcept() {
				CConcept* concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemMan);
				cint64 nextConTag = mTBox->getNextConceptID();
				concept->initConcept();
				concept->setTag(nextConTag);
				mConceptVector->setData(nextConTag,concept);
				return concept;
			}



			CConcept* CDataLiteralNormalizerPreProcess::addConceptOperand(CConcept* concept, CConcept* operand, bool negated) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				opConLinker->init(operand,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
				return concept;
			}



			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseBooleanValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				bool parsingSuccessfull = false;
				bool booleanValue = false;
				if (valueString.toUpper().contains("TRUE")) {
					booleanValue = true;
					parsingSuccessfull = true;
				} else if (valueString.toUpper().contains("FALSE")) {
					booleanValue = false;
					parsingSuccessfull = true;
				}
				if (parsingSuccessfull) {
					CDataLiteralBooleanValue* dataLitBooleanValue = CObjectAllocator<CDataLiteralBooleanValue>::allocateAndConstruct(mMemMan);
					dataLitBooleanValue->initValueFromBoolean(booleanValue);
					dataLiteralValue = dataLitBooleanValue;
				}
				return dataLiteralValue;
			}





			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseDoubleValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				bool parsingSuccessfull = false;
				double doubleDataValue = valueString.toDouble(&parsingSuccessfull);
				if (parsingSuccessfull) {
					CDataLiteralDoubleValue* dataLitDoubleValue = CObjectAllocator<CDataLiteralDoubleValue>::allocateAndConstruct(mMemMan);
					dataLitDoubleValue->initValueFromDouble(doubleDataValue);
					dataLiteralValue = dataLitDoubleValue;
				}
				return dataLiteralValue;
			}



			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseFloatValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				bool parsingSuccessfull = false;
				float floatDataValue = valueString.toFloat(&parsingSuccessfull);
				if (parsingSuccessfull) {
					CDataLiteralFloatValue* dataLitFloatValue = CObjectAllocator<CDataLiteralFloatValue>::allocateAndConstruct(mMemMan);
					dataLitFloatValue->initValueFromFloat(floatDataValue);
					dataLiteralValue = dataLitFloatValue;
				}
				return dataLiteralValue;
			}


			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseIntegerValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				bool parsingSuccessfull = false;
				QString tmpValueString(valueString);
				bool negative = tmpValueString.contains("-");
				tmpValueString = tmpValueString.replace("-","");
				cuint64 integerDataValue = tmpValueString.toULongLong(&parsingSuccessfull);
				if (parsingSuccessfull) {
					CDataLiteralRealValue* dataLitRatValue = CObjectAllocator<CDataLiteralRealValue>::allocateAndConstruct(mMemMan);
					dataLitRatValue->initRationalValueFromInteger(integerDataValue,negative);
					dataLiteralValue = dataLitRatValue;
				} else {
					if (tmpValueString.toUpper().contains("INF")) {
						CDataLiteralRealValue* dataLitRatValue = CObjectAllocator<CDataLiteralRealValue>::allocateAndConstruct(mMemMan);
						dataLitRatValue->initRationalValueFromInfinite(negative);
						dataLiteralValue = dataLitRatValue;
					}
				}
				return dataLiteralValue;
			}



			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseDecimalValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				bool parsingSuccessfull = false;
				cuint64 integerValue = 0;
				cuint64 decimalValue = 0;
				QString tmpValueString(valueString);
				bool negative = tmpValueString.contains("-");
				tmpValueString = tmpValueString.replace("-","");
				QStringList dataValueStringList = tmpValueString.split(".");
				if (dataValueStringList.size() < 2) {
					dataValueStringList = tmpValueString.split(",");
				}
				int precedingZerors = 0;
				if (dataValueStringList.size() >= 2) {
					bool integerParsingSuccessfull = false;
					bool decimalParsingSuccessfull = false;
					QString firstDataValueString = dataValueStringList.takeFirst().trimmed();
					QString secondDataValueString = dataValueStringList.takeFirst().trimmed();
					if (firstDataValueString.isEmpty()) {
						integerValue = 0;
						integerParsingSuccessfull = true;
					} else {
						integerValue = firstDataValueString.toULongLong(&integerParsingSuccessfull);
					}
					if (secondDataValueString.isEmpty()) {
						decimalValue = 0;
						decimalParsingSuccessfull = true;
					} else {
						decimalValue = secondDataValueString.toULongLong(&decimalParsingSuccessfull);
						if (secondDataValueString.startsWith("0") && secondDataValueString.size() > 1) {
							++precedingZerors;
							while (secondDataValueString.size() > precedingZerors && secondDataValueString.at(precedingZerors) == '0') {
								++precedingZerors;
							}
						}
					}
					parsingSuccessfull = integerParsingSuccessfull & decimalParsingSuccessfull;
				}
				if (parsingSuccessfull) {
					CDataLiteralRealValue* dataLitRatValue = CObjectAllocator<CDataLiteralRealValue>::allocateAndConstruct(mMemMan);
					dataLitRatValue->initRationalValueFromDecimal(integerValue,decimalValue,negative,precedingZerors);
					dataLiteralValue = dataLitRatValue;
				}
				if (!dataLiteralValue) {
					dataLiteralValue = parseIntegerValue(valueString,context);
				}
				return dataLiteralValue;
			}



			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseRationalValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				bool parsingSuccessfull = false;
				cuint64 numeratorValue = 0;
				cuint64 denominatorValue = 0;
				QString tmpValueString(valueString);
				bool negative = tmpValueString.contains("-");
				tmpValueString = tmpValueString.replace("-","");
				QStringList dataValueStringList = tmpValueString.split("/");
				if (dataValueStringList.size() >= 2) {
					bool numeratorParsingSuccessfull = false;
					bool denominatorParsingSuccessfull = false;
					QString firstDataValueString = dataValueStringList.takeFirst().trimmed();
					QString secondDataValueString = dataValueStringList.takeFirst().trimmed();
					numeratorValue = firstDataValueString.toULongLong(&numeratorParsingSuccessfull);
					denominatorValue = secondDataValueString.toULongLong(&denominatorParsingSuccessfull);
					parsingSuccessfull = numeratorParsingSuccessfull & denominatorParsingSuccessfull;
				}
				if (parsingSuccessfull) {
					CDataLiteralRealValue* dataLitRatValue = CObjectAllocator<CDataLiteralRealValue>::allocateAndConstruct(mMemMan);
					dataLitRatValue->initRationalValueFromFraction(numeratorValue,denominatorValue,negative);
					dataLiteralValue = dataLitRatValue;
				}
				if (!dataLiteralValue) {
					dataLiteralValue = parseDecimalValue(valueString,context);
				}
				return dataLiteralValue;
			}


			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseRealValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					dataLiteralValue = parseRationalValue(valueString,context);
				}
				return dataLiteralValue;
			}



			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseStringValue(const QString& valueString, bool parseLanguageString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					QString stringValueString = valueString;
					QString languageValueString;
					if (parseLanguageString) {
						cint64 lastLanguageSep = valueString.lastIndexOf("@");
						if (lastLanguageSep > 0) {
							stringValueString = valueString.mid(0,lastLanguageSep);
							if (!stringValueString.trimmed().isEmpty()) {
								languageValueString = valueString.mid(lastLanguageSep + 1);
								bool validLanguageTag = true;
								bool firstBindOccurred = false;
								for (cint64 i = 0; i < languageValueString.length() && validLanguageTag; ++i) {
									QChar ch = languageValueString.at(i);
									if (!firstBindOccurred && i > 3) {
										validLanguageTag = false;
									}
									if (ch == QChar('-')) {
										firstBindOccurred = true;
									}
									if (!ch.isLetterOrNumber() && ch != QChar('-')) {
										validLanguageTag = false;
									}
								}

								if (!validLanguageTag) {
									stringValueString = valueString;
									languageValueString = "";
								}
							} else {
								stringValueString = valueString;
								languageValueString = "";
							}
						}
					}

					languageValueString = languageValueString.toLower();
					CDataLiteralStringValue* dataLitStringValue = CObjectParameterizingAllocator<CDataLiteralStringValue,CContext*>::allocateAndConstructAndParameterize(mMemMan, mBoxContext);
					dataLitStringValue->initValueFromString(stringValueString,languageValueString);
					dataLiteralValue = dataLitStringValue;

				}
				return dataLiteralValue;
			}


			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseDateTimeValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					QString dateTimeValueString = valueString;
					QString timeZoneOffsetValueString;

					if (valueString.contains("-") || valueString.contains("+")) {
						cint64 lastZoneMinusSep = valueString.lastIndexOf("-");
						cint64 lastZonePlusSep = valueString.lastIndexOf("+");
						cint64 lastZoneSep = qMax(lastZonePlusSep,lastZoneMinusSep);
						if (lastZoneSep >= 12) {
							dateTimeValueString = valueString.mid(0,lastZoneSep);
							timeZoneOffsetValueString = valueString.mid(lastZoneSep+1);
						}
					}

					QDateTime dateTime = QDateTime::fromString(dateTimeValueString,Qt::ISODate);
					if (dateTime.isValid()) {
						QDate tmpDate(dateTime.date());
						QTime tmpTime(dateTime.time());
						QDateTime tmpDateTime(tmpDate,tmpTime,Qt::UTC);

						if (dateTime.timeSpec() == Qt::LocalTime) {
							dateTime = tmpDateTime;
						}
						cuint64 timeZoneOffset = 0;
						if (dateTime.timeSpec() == Qt::OffsetFromUTC) {
							QDateTime utcDateTime = dateTime.toUTC();
							timeZoneOffset = utcDateTime.secsTo(tmpDateTime);
							dateTime = utcDateTime;
						}

						CDataLiteralDateTimeValue* dataLitDateTimeValue = CObjectAllocator<CDataLiteralDateTimeValue>::allocateAndConstruct(mMemMan);
						dataLitDateTimeValue->initValueFromDataTime(dateTime,timeZoneOffset);
						dataLiteralValue = dataLitDateTimeValue;

					}
				}
				return dataLiteralValue;
			}

			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseIRIValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					CDataLiteralIRIValue* dataLitIRIValue = CObjectAllocator<CDataLiteralIRIValue>::allocateAndConstruct(mMemMan);
					dataLitIRIValue->initValueFromIRIString(valueString.trimmed());
					dataLiteralValue = dataLitIRIValue;

				}
				return dataLiteralValue;
			}




			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseXMLValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					QDomDocument domDocument;
					if (domDocument.setContent(valueString,true)) {
						QString xmlValueString = domDocument.toString();
						if (!xmlValueString.isEmpty()) {
							CDataLiteralXMLValue* dataLitXMLValue = CObjectAllocator<CDataLiteralXMLValue>::allocateAndConstruct(mMemMan);
							dataLitXMLValue->initValueFromXMLString(valueString.trimmed());
							dataLiteralValue = dataLitXMLValue;
						}
					}
				}
				return dataLiteralValue;
			}



			cint64 CDataLiteralNormalizerPreProcess::getBinaryFromHex(QChar character) {
				cint64 unicode = character.unicode();
				if (unicode > 127) {
					return -1;
				}
				return mHexStringBinaryDataConvArray[unicode];
			}



			cint64 CDataLiteralNormalizerPreProcess::getBinaryFromBase64(QChar character) {
				cint64 unicode = character.unicode();
				if (unicode > 127) {
					return -1;
				}
				return mBase64StringBinaryDataConvArray[unicode];
			}


			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseBinaryHexValue(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					QString dataValueString(valueString);
					int dataValueStringLength = dataValueString.length();
					unsigned char* tmpByteData = new unsigned char[dataValueStringLength/2+1];
					cint64 byteDataLength = 0;


					cint64 byteParts[2];
					cint64 currentBytePart = 0;

					for (int i = 0; i < dataValueStringLength; ++i) {
						QChar dataChar = dataValueString[i];
						cint64 dataBytePart = getBinaryFromHex(dataChar);
						if (dataBytePart != -1) {
							byteParts[currentBytePart++] = dataBytePart;
							if (currentBytePart >= 2) {
								currentBytePart = 0;
								cint64 nextDataByte = byteParts[0]<<4 + byteParts[1];
								tmpByteData[byteDataLength++] = (unsigned char)nextDataByte;
							}
						}
					}

					if (currentBytePart == 1) {
						cint64 nextDataByte = byteParts[0]<<4;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte;
					}


					CDataLiteralBinaryHexDataValue* dataLitHexBinaryValue = CObjectParameterizingAllocator< CDataLiteralBinaryHexDataValue,CContext* >::allocateAndConstructAndParameterize(mMemMan,mBoxContext);
					dataLitHexBinaryValue->initValueFromData(tmpByteData,byteDataLength);
					delete [] tmpByteData;
					dataLiteralValue = dataLitHexBinaryValue;

				}
				return dataLiteralValue;
			}







			CDataLiteralValue* CDataLiteralNormalizerPreProcess::parseBinaryBase64Value(const QString& valueString, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				if (!dataLiteralValue) {
					QString dataValueString(valueString);
					int dataValueStringLength = dataValueString.length();
					unsigned char* tmpByteData = new unsigned char[dataValueStringLength*3/4+1];
					cint64 byteDataLength = 0;


					cint64 byteParts[4];
					cint64 currentBytePart = 0;

					for (int i = 0; i < dataValueStringLength; ++i) {
						QChar dataChar = dataValueString[i];
						cint64 dataBytePart = getBinaryFromBase64(dataChar);
						if (dataBytePart != -1) {
							byteParts[currentBytePart++] = dataBytePart;
							if (currentBytePart >= 4) {
								currentBytePart = 0;
								cint64 nextDataByte1 = byteParts[0]<<2 + byteParts[1]>>4;
								tmpByteData[byteDataLength++] = (unsigned char)nextDataByte1;
								cint64 nextDataByte2 = byteParts[1]<<4 + byteParts[2]>>2;
								tmpByteData[byteDataLength++] = (unsigned char)nextDataByte2;
								cint64 nextDataByte3 = byteParts[2]<<6 + byteParts[3];
								tmpByteData[byteDataLength++] = (unsigned char)nextDataByte3;
							}
						}
					}

					if (currentBytePart == 3) {
						cint64 nextDataByte1 = byteParts[0]<<2 + byteParts[1]>>4;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte1;
						cint64 nextDataByte2 = byteParts[1]<<4 + byteParts[2]>>2;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte2;
						cint64 nextDataByte3 = byteParts[2]<<6;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte3;
					}
					if (currentBytePart == 2) {
						cint64 nextDataByte1 = byteParts[0]<<2 + byteParts[1]>>4;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte1;
						cint64 nextDataByte2 = byteParts[1]<<4;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte2;
					}
					if (currentBytePart == 1) {
						cint64 nextDataByte1 = byteParts[0]<<2;
						tmpByteData[byteDataLength++] = (unsigned char)nextDataByte1;
					}


					CDataLiteralBinaryBase64DataValue* dataLitBase64BinaryValue = CObjectParameterizingAllocator< CDataLiteralBinaryBase64DataValue,CContext* >::allocateAndConstructAndParameterize(mMemMan,mBoxContext);
					dataLitBase64BinaryValue->initValueFromData(tmpByteData,byteDataLength);
					delete [] tmpByteData;
					dataLiteralValue = dataLitBase64BinaryValue;

				}
				return dataLiteralValue;
			}



			CDataLiteralValue* CDataLiteralNormalizerPreProcess::createDataLiteralValue(CDataLiteral* dataLiteral, CPreProcessContext* context) {
				CDataLiteralValue* dataLiteralValue = nullptr;
				CDatatype* datatype = dataLiteral->getDatatype();
				if (datatype) {
					QString dataLexicalValueString = dataLiteral->getLexicalDataLiteralValueString();
					QString datatypeIRI = datatype->getDatatypeIRI();
					if (datatypeIRI == PREFIX_XML_INTEGER_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_DECIMAL_DATATYPE) {
						dataLiteralValue = parseDecimalValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_OWL_RATIONAL_DATATYPE) {
						dataLiteralValue = parseRationalValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_OWL_REAL_DATATYPE) {
						dataLiteralValue = parseRealValue(dataLexicalValueString,context);

					} else if (datatypeIRI == PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_POSITIVEINTEGER_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_NONPOSITIVEINTEGER_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_NEGATIVEINTEGER_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_LONG_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_INT_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_SHORT_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_BYTE_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_UNSIGNEDLONG_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_UNSIGNEDINT_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_UNSIGNEDSHORT_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_UNSIGNEDBYTE_DATATYPE) {
						dataLiteralValue = parseIntegerValue(dataLexicalValueString,context);


					} else if (datatypeIRI == PREFIX_XML_PLAINLITERAL_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,true,context);
					} else if (datatypeIRI == PREFIX_XML_STRING_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);
					} else if (datatypeIRI == PREFIX_XML_NORMALIZEDSTRING_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);
					} else if (datatypeIRI == PREFIX_XML_TOKEN_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);
					} else if (datatypeIRI == PREFIX_XML_NAME_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);
					} else if (datatypeIRI == PREFIX_XML_NCNAME_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);
					} else if (datatypeIRI == PREFIX_XML_NMTOKEN_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);
					} else if (datatypeIRI == PREFIX_XML_LANGUAGE_DATATYPE) {
						dataLiteralValue = parseStringValue(dataLexicalValueString,false,context);


					} else if (datatypeIRI == PREFIX_XML_BOOLEAN_DATATYPE) {
						dataLiteralValue = parseBooleanValue(dataLexicalValueString,context);


					} else if (datatypeIRI == PREFIX_XML_DOUBLE_DATATYPE) {
						dataLiteralValue = parseDoubleValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_FLOAT_DATATYPE) {
						dataLiteralValue = parseFloatValue(dataLexicalValueString,context);


					} else if (datatypeIRI == PREFIX_XML_IRI_DATATYPE) {
						dataLiteralValue = parseIRIValue(dataLexicalValueString,context);

					} else if (datatypeIRI == PREFIX_XML_XML_DATATYPE) {
						dataLiteralValue = parseXMLValue(dataLexicalValueString,context);



					} else if (datatypeIRI == PREFIX_XML_BINARYHEX_DATATYPE) {
						dataLiteralValue = parseBinaryHexValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_BINARYBASE64_DATATYPE) {
						dataLiteralValue = parseBinaryBase64Value(dataLexicalValueString,context);


					} else if (datatypeIRI == PREFIX_XML_DATETIME_DATATYPE) {
						dataLiteralValue = parseDateTimeValue(dataLexicalValueString,context);
					} else if (datatypeIRI == PREFIX_XML_DATETIMESTAMP_DATATYPE) {
						dataLiteralValue = parseDateTimeValue(dataLexicalValueString,context);

					} else {
						LOG(WARNING,"::Konclude::Reasoner::Preprocess::DataLiteralNormaliser",logTr("Unknown or not supported datatype '%1', cannot parse '%2'.").arg(datatypeIRI).arg(dataLexicalValueString),this);
					}

					if (!dataLiteralValue) {
						LOG(WARNING,"::Konclude::Reasoner::Preprocess::DataLiteralNormaliser",logTr("Failed to parse literal '%1' for datatype '%2'.").arg(dataLexicalValueString).arg(datatypeIRI),this);
					}
				}
				return dataLiteralValue;
			}



		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
