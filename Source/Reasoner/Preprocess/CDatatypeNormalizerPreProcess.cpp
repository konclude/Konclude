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

#include "CDatatypeNormalizerPreProcess.h"


namespace Konclude {

	namespace Reasoner {

		namespace Preprocess {


			CDatatypeNormalizerPreProcess::CDatatypeNormalizerPreProcess() {
			}


			CDatatypeNormalizerPreProcess::~CDatatypeNormalizerPreProcess() {
			}


			CConcreteOntology *CDatatypeNormalizerPreProcess::preprocess(CConcreteOntology *ontology, CPreProcessContext* context) {
				if (ontology) {
					mTBox = ontology->getDataBoxes()->getTBox();
					mABox = ontology->getDataBoxes()->getABox();
					mRBox = ontology->getDataBoxes()->getRBox();
					mMBox = ontology->getDataBoxes()->getMBox();


					mIntegerRestrictionConcept = nullptr;
					mStringRestrictionConcept = nullptr;
					mNormalizedStringRestrictionConcept = nullptr;
					mTokenRestrictionConcept = nullptr;
					mNMTokenRestrictionConcept = nullptr;
					mNameRestrictionConcept = nullptr;
					mNCNameRestrictionConcept = nullptr;
					mLanguageRestrictionConcept = nullptr;
					mDatetimeStampRestrictionConcept = nullptr;
					mTopDataRangeConcept = mTBox->getTopDataRangeConcept();

					mRolesVector = mRBox->getRoleVector();
					mConceptVector = mTBox->getConceptVector();
					mIndiVec = mABox->getIndividualVector();

					mDataRangeConceptHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getDataRangeTermConceptMappingHash();
					mConceptDataRangeHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getConceptDataRangeTermMappingHash();

					mDatatypeExpressionDatatpyeHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getDatatypeExpressionDatatypeHash();
					mDatatpyeDatatypeExpressionHash = ontology->getDataBoxes()->getExpressionDataBoxMapping()->getDatatypeDatatypeExpessionHash();

					mMemMan = ontology->getDataBoxes()->getBoxContext()->getMemoryAllocationManager();
					mOntology = ontology;

					mValueSpaceTypes = ontology->getDataBoxes()->getDatatypeValueSpaceTypes();
					mDatatypeVector = mTBox->getDatatypeVector(false);
					

					QList<CDatatype*> datatypeNormalisationList;


					if (mDatatypeVector) {
						cint64 datatypeCount = mDatatypeVector->getItemCount();
						for (cint64 i = 0; i < datatypeCount; ++i) {
							CDatatype* datatype = mDatatypeVector->getData(i);
							if (datatype) {
								QString datatypeIRI = datatype->getDatatypeIRI();
								mIRIDatatypeHash.insert(datatypeIRI,datatype);
							}
						}

						for (cint64 i = 0; i < datatypeCount; ++i) {
							CDatatype* datatype = mDatatypeVector->getLocalData(i);
							if (datatype) {
								if (!datatype->hasDatatypeConcept()) {
									CConcept* datatypeConcept = nullptr;
									CDatatypeExpression* datatypeExpression = mDatatpyeDatatypeExpressionHash->value(datatype);
									if (datatypeExpression) {
										datatypeConcept = mDataRangeConceptHash->value(datatypeExpression);
									}
									if (!datatypeConcept) {
										datatypeConcept = createDatatypeConcept(datatype);
										if (datatypeExpression) {
											mDataRangeConceptHash->insert(datatypeExpression,datatypeConcept);
											mConceptDataRangeHash->insert(datatypeConcept,datatypeExpression);
										}
										datatypeConcept->setDatatype(datatype);
									}
									datatype->setDatatypeConcept(datatypeConcept);
								}
								if (!datatype->isNormalized()) {
									datatypeNormalisationList.append(datatype);
								}
							}
						}

						foreach (CDatatype* datatype, datatypeNormalisationList) {
							normalizeDatatype(datatype,context);
							datatype->setNormalized(true);
						}

					}


				}

				return ontology;
			}


			CConcept* CDatatypeNormalizerPreProcess::createConcept() {
				CConcept* concept = CObjectAllocator<CConcept>::allocateAndConstruct(mMemMan);
				cint64 nextConTag = mConceptVector->getItemCount();
				concept->initConcept();
				concept->setTag(nextConTag);
				mConceptVector->setData(nextConTag,concept);
				return concept;
			}

			CConcept* CDatatypeNormalizerPreProcess::createDatatypeConcept(CDatatype* datatype) {
				CConcept* datatypeConcept = createConcept();
				datatypeConcept->setOperatorCode(CCDATATYPE);
				return datatypeConcept;
			}


			CConcept* CDatatypeNormalizerPreProcess::addConceptOperand(CConcept* concept, CConcept* operand, bool negated) {
				CSortedNegLinker<CConcept*>* opConLinker = CObjectAllocator< CSortedNegLinker<CConcept*> >::allocateAndConstruct(mMemMan);
				opConLinker->init(operand,negated);
				concept->addOperandLinker(opConLinker);
				concept->incOperandCount();
				return concept;
			}


			CConcept* CDatatypeNormalizerPreProcess::getNumberRestrictionConcept(cint64 restrictionCode, const QString& value) {
				CConcept* restrictionConcept = mNumberRestrictionConceptHash.value(QPair<cint64,QString>(restrictionCode,value));
				if (!restrictionConcept) {
					restrictionConcept = createConcept();
					restrictionConcept->setParameter(restrictionCode);
					restrictionConcept->setOperatorCode(CCDATARESTRICTION);
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);
					restrictionConcept->setDatatype(realDatatype);
					CDataLiteral* dataLiteral = CObjectAllocator<CDataLiteral>::allocateAndConstruct(mMemMan);
					dataLiteral->initDataLiteral(value,realDatatype);
					restrictionConcept->setDataLiteral(dataLiteral);
					mNumberRestrictionConceptHash.insert(QPair<cint64,QString>(restrictionCode,value),restrictionConcept);
				}
				return restrictionConcept;
			}



			CDatatype* CDatatypeNormalizerPreProcess::getDatatype(const QString& datatypeIRI) {
				return mIRIDatatypeHash.value(datatypeIRI,nullptr);
			}



			CConcept* CDatatypeNormalizerPreProcess::getIntegerRestrictionConcept() {
				if (!mIntegerRestrictionConcept) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);
					mIntegerRestrictionConcept = createConcept();
					mIntegerRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mIntegerRestrictionConcept->setDatatype(realDatatype);
					mIntegerRestrictionConcept->setParameter(CDFC_INTEGER);
				}
				return mIntegerRestrictionConcept;
			}


			CConcept* CDatatypeNormalizerPreProcess::getStringRestrictionConcept() {
				if (!mStringRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mStringRestrictionConcept = createConcept();
					mStringRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mStringRestrictionConcept->setDatatype(plainLiteralDatatype);
					mStringRestrictionConcept->setParameter(CDFC_STRING);
				}
				return mStringRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getNormalizedStringRestrictionConcept() {
				if (!mNormalizedStringRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mNormalizedStringRestrictionConcept = createConcept();
					mNormalizedStringRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mNormalizedStringRestrictionConcept->setDatatype(plainLiteralDatatype);
					mNormalizedStringRestrictionConcept->setParameter(CDFC_NORMALIZED_STRING);
				}
				return mNormalizedStringRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getTokenRestrictionConcept() {
				if (!mTokenRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mTokenRestrictionConcept = createConcept();
					mTokenRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mTokenRestrictionConcept->setDatatype(plainLiteralDatatype);
					mTokenRestrictionConcept->setParameter(CDFC_TOKEN);
				}
				return mTokenRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getNMTokenRestrictionConcept() {
				if (!mNMTokenRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mNMTokenRestrictionConcept = createConcept();
					mNMTokenRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mNMTokenRestrictionConcept->setDatatype(plainLiteralDatatype);
					mNMTokenRestrictionConcept->setParameter(CDFC_NMTOKEN);
				}
				return mNMTokenRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getNameRestrictionConcept() {
				if (!mNameRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mNameRestrictionConcept = createConcept();
					mNameRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mNameRestrictionConcept->setDatatype(plainLiteralDatatype);
					mNameRestrictionConcept->setParameter(CDFC_NAME);
				}
				return mNameRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getNCNameRestrictionConcept() {
				if (!mNCNameRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mNCNameRestrictionConcept = createConcept();
					mNCNameRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mNCNameRestrictionConcept->setDatatype(plainLiteralDatatype);
					mNCNameRestrictionConcept->setParameter(CDFC_NCNAME);
				}
				return mNCNameRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getLanguageRestrictionConcept() {
				if (!mLanguageRestrictionConcept) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					mLanguageRestrictionConcept = createConcept();
					mLanguageRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mLanguageRestrictionConcept->setDatatype(plainLiteralDatatype);
					mLanguageRestrictionConcept->setParameter(CDFC_LANGUAGE);
				}
				return mLanguageRestrictionConcept;
			}

			CConcept* CDatatypeNormalizerPreProcess::getDateTimeStampRestrictionConcept() {
				if (!mDatetimeStampRestrictionConcept) {
					CDatatype* dateTimeDatatype = getDatatype(PREFIX_XML_DATETIMESTAMP_DATATYPE);
					mDatetimeStampRestrictionConcept = createConcept();
					mDatetimeStampRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					mDatetimeStampRestrictionConcept->setDatatype(dateTimeDatatype);
					mDatetimeStampRestrictionConcept->setParameter(CDFC_DATETIMESTAMP);
				}
				return mDatetimeStampRestrictionConcept;
			}


			CDatatypeNormalizerPreProcess* CDatatypeNormalizerPreProcess::normalizeDatatype(CDatatype* datatype, CPreProcessContext* context) {
				QString datatypeIRI = datatype->getDatatypeIRI();
				CConcept* datatypeConcept = datatype->getDatatypeConcept();


				if (datatypeIRI == PREFIX_XML_INTEGER_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);


				} else if (datatypeIRI == PREFIX_XML_DECIMAL_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* decimalRestrictionConcept = createConcept();
					decimalRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					decimalRestrictionConcept->setDatatype(realDatatype);
					decimalRestrictionConcept->setParameter(CDFC_DECIMAL);
					addConceptOperand(datatypeConcept,decimalRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);


				} else if (datatypeIRI == PREFIX_OWL_RATIONAL_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* rationalRestrictionConcept = createConcept();
					rationalRestrictionConcept->setOperatorCode(CCDATARESTRICTION);
					rationalRestrictionConcept->setDatatype(realDatatype);
					rationalRestrictionConcept->setParameter(CDFC_RATIONAL);
					addConceptOperand(datatypeConcept,rationalRestrictionConcept,false);


					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);


				} else if (datatypeIRI == PREFIX_OWL_REAL_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);


				} else if (datatypeIRI == PREFIX_OWL_TOP_DATATYPE) {
					if (mTopDataRangeConcept) {
						addConceptOperand(datatypeConcept,mTopDataRangeConcept,false);
					}

				} else if (datatypeIRI == PREFIX_OWL_BOTTOM_DATATYPE) {
					if (mTopDataRangeConcept) {
						addConceptOperand(datatypeConcept,mTopDataRangeConcept,true);
					}




				} else if (datatypeIRI == PREFIX_XML_NONNEGATIVEINTEGER_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"0");
					addConceptOperand(datatypeConcept,minInclusiveZeroRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);



				} else if (datatypeIRI == PREFIX_XML_POSITIVEINTEGER_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minExclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_EXCLUSIVE,"0");
					addConceptOperand(datatypeConcept,minExclusiveZeroRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);



				} else if (datatypeIRI == PREFIX_XML_NONPOSITIVEINTEGER_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* maxInclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"0");
					addConceptOperand(datatypeConcept,maxInclusiveZeroRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);


				} else if (datatypeIRI == PREFIX_XML_NEGATIVEINTEGER_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* maxExclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_EXCLUSIVE,"0");
					addConceptOperand(datatypeConcept,maxExclusiveZeroRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);



				} else if (datatypeIRI == PREFIX_XML_LONG_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveLongRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"-9223372036854775808");
					addConceptOperand(datatypeConcept,minInclusiveLongRestrictionConcept,false);
					CConcept* maxInclusiveLongRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"9223372036854775807");
					addConceptOperand(datatypeConcept,maxInclusiveLongRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);




				} else if (datatypeIRI == PREFIX_XML_INT_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveIntRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"-2147483647");
					addConceptOperand(datatypeConcept,minInclusiveIntRestrictionConcept,false);
					CConcept* maxInclusiveIntRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"2147483647");
					addConceptOperand(datatypeConcept,maxInclusiveIntRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);




				} else if (datatypeIRI == PREFIX_XML_SHORT_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveShortRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"-32767");
					addConceptOperand(datatypeConcept,minInclusiveShortRestrictionConcept,false);
					CConcept* maxInclusiveShortRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"32767");
					addConceptOperand(datatypeConcept,maxInclusiveShortRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);




				} else if (datatypeIRI == PREFIX_XML_BYTE_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveByteRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"-127");
					addConceptOperand(datatypeConcept,minInclusiveByteRestrictionConcept,false);
					CConcept* maxInclusiveByteRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"127");
					addConceptOperand(datatypeConcept,maxInclusiveByteRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);




				} else if (datatypeIRI == PREFIX_XML_UNSIGNEDLONG_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"0");
					addConceptOperand(datatypeConcept,minInclusiveZeroRestrictionConcept,false);
					CConcept* maxInclusiveULongRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"18446744073709551615");
					addConceptOperand(datatypeConcept,maxInclusiveULongRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);



				} else if (datatypeIRI == PREFIX_XML_UNSIGNEDINT_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"0");
					addConceptOperand(datatypeConcept,minInclusiveZeroRestrictionConcept,false);
					CConcept* maxInclusiveUIntRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"4294967295");
					addConceptOperand(datatypeConcept,maxInclusiveUIntRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);



				} else if (datatypeIRI == PREFIX_XML_UNSIGNEDSHORT_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"0");
					addConceptOperand(datatypeConcept,minInclusiveZeroRestrictionConcept,false);
					CConcept* maxInclusiveUShortRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"65535");
					addConceptOperand(datatypeConcept,maxInclusiveUShortRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);


				} else if (datatypeIRI == PREFIX_XML_UNSIGNEDBYTE_DATATYPE) {
					CDatatype* realDatatype = getDatatype(PREFIX_OWL_REAL_DATATYPE);

					CConcept* integerRestrictionConcept = getIntegerRestrictionConcept();
					addConceptOperand(datatypeConcept,integerRestrictionConcept,false);

					CConcept* minInclusiveZeroRestrictionConcept = getNumberRestrictionConcept(CDFC_MIN_INCLUSIVE,"0");
					addConceptOperand(datatypeConcept,minInclusiveZeroRestrictionConcept,false);
					CConcept* maxInclusiveUByteRestrictionConcept = getNumberRestrictionConcept(CDFC_MAX_INCLUSIVE,"255");
					addConceptOperand(datatypeConcept,maxInclusiveUByteRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceRealType());
					datatype->setBasicDatatype(realDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_REAL);









				} else if (datatypeIRI == PREFIX_XML_PLAINLITERAL_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);


				} else if (datatypeIRI == PREFIX_XML_STRING_DATATYPE) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);

					addConceptOperand(datatypeConcept,plainLiteralDatatype->getDatatypeConcept(),false);


					CConcept* stringRestrictionConcept = getStringRestrictionConcept();
					addConceptOperand(datatypeConcept,stringRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);


				} else if (datatypeIRI == PREFIX_XML_NORMALIZEDSTRING_DATATYPE) {
					CDatatype* stringDatatype = getDatatype(PREFIX_XML_STRING_DATATYPE);
					addConceptOperand(datatypeConcept,stringDatatype->getDatatypeConcept(),false);

					CConcept* normalizedStringRestrictionConcept = getStringRestrictionConcept();
					addConceptOperand(datatypeConcept,normalizedStringRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);



				} else if (datatypeIRI == PREFIX_XML_TOKEN_DATATYPE) {
					CDatatype* normalizedStringDatatype = getDatatype(PREFIX_XML_NORMALIZEDSTRING_DATATYPE);
					addConceptOperand(datatypeConcept,normalizedStringDatatype->getDatatypeConcept(),false);

					CConcept* tokenRestrictionConcept = getTokenRestrictionConcept();
					addConceptOperand(datatypeConcept,tokenRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);



				} else if (datatypeIRI == PREFIX_XML_NAME_DATATYPE) {
					CDatatype* tokenDatatype = getDatatype(PREFIX_XML_TOKEN_DATATYPE);
					addConceptOperand(datatypeConcept,tokenDatatype->getDatatypeConcept(),false);

					CConcept* nameRestrictionConcept = getNameRestrictionConcept();
					addConceptOperand(datatypeConcept,nameRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);


				} else if (datatypeIRI == PREFIX_XML_NCNAME_DATATYPE) {
					CDatatype* nameDatatype = getDatatype(PREFIX_XML_NAME_DATATYPE);
					addConceptOperand(datatypeConcept,nameDatatype->getDatatypeConcept(),false);

					CConcept* ncnameRestrictionConcept = getNCNameRestrictionConcept();
					addConceptOperand(datatypeConcept,ncnameRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);


				} else if (datatypeIRI == PREFIX_XML_NMTOKEN_DATATYPE) {
					CDatatype* tokenDatatype = getDatatype(PREFIX_XML_TOKEN_DATATYPE);
					addConceptOperand(datatypeConcept,tokenDatatype->getDatatypeConcept(),false);

					CConcept* nmTokenRestrictionConcept = getNMTokenRestrictionConcept();
					addConceptOperand(datatypeConcept,nmTokenRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);


				} else if (datatypeIRI == PREFIX_XML_LANGUAGE_DATATYPE) {
					CDatatype* plainLiteralDatatype = getDatatype(PREFIX_XML_PLAINLITERAL_DATATYPE);
					addConceptOperand(datatypeConcept,plainLiteralDatatype->getDatatypeConcept(),false);

					CConcept* languageRestrictionConcept = getLanguageRestrictionConcept();
					addConceptOperand(datatypeConcept,languageRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceStringType());
					datatype->setBasicDatatype(plainLiteralDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_STRING);










				} else if (datatypeIRI == PREFIX_XML_BOOLEAN_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceBooleanType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_BOOLEAN);







				} else if (datatypeIRI == PREFIX_XML_DOUBLE_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceDoubleType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_DOUBLE);


				} else if (datatypeIRI == PREFIX_XML_FLOAT_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceFloatType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_FLOAT);







				} else if (datatypeIRI == PREFIX_XML_IRI_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceIRIType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_IRI);




				} else if (datatypeIRI == PREFIX_XML_XML_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceXMLType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_XML);






				} else if (datatypeIRI == PREFIX_XML_BINARYHEX_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceBinaryHexDataType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_HEXBINARY);


				} else if (datatypeIRI == PREFIX_XML_BINARYBASE64_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceBinaryBase64DataType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_BASE64BINARY);







				} else if (datatypeIRI == PREFIX_XML_DATETIME_DATATYPE) {
					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceDateTimeType());
					datatype->setBasicDatatype(datatype);
					datatype->setBasicDatatype(true);
					datatype->setBasicDatatypeType(CDatatype::DBT_DATETIME);


				} else if (datatypeIRI == PREFIX_XML_DATETIMESTAMP_DATATYPE) {
					CDatatype* dateTimeDatatype = getDatatype(PREFIX_XML_DATETIME_DATATYPE);
					addConceptOperand(datatypeConcept,dateTimeDatatype->getDatatypeConcept(),false);

					CConcept* dateTimeStampRestrictionConcept = getDateTimeStampRestrictionConcept();
					addConceptOperand(datatypeConcept,dateTimeStampRestrictionConcept,false);

					datatype->setValueSpaceType(mValueSpaceTypes->getValueSpaceDateTimeType());
					datatype->setBasicDatatype(dateTimeDatatype);
					datatype->setBasicDatatypeType(CDatatype::DBT_DATETIME);



				}
				return this;
			}


		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude
