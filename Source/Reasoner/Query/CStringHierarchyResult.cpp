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
#ifndef KONCLUDE_REASONER_QUERY_CSTRINGHIERARCHYRESULT_CPP
#define KONCLUDE_REASONER_QUERY_CSTRINGHIERARCHYRESULT_CPP


#include "CStringHierarchyResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			template<class T, class R, class E>
			CStringHierarchyResult<T,R,E>::CStringHierarchyResult() {
				mBottomString = 0;
				mTopString = 0;
			}

			template<class T, class R, class E>
			CStringHierarchyResult<T,R,E>::CStringHierarchyResult(const QString &bottomHierarchyString, const QString &topHierarchyString) {
				mTopString = getStringSynset(topHierarchyString);
				mBottomString = getStringSynset(bottomHierarchyString);
			}

			template<class T, class R, class E>
			CStringHierarchyResult<T,R,E>::CStringHierarchyResult(const QStringList &bottomHierarchyStrings, const QStringList &topHierarchyStrings) {
				mTopString = getStringSynset(topHierarchyStrings);
				mBottomString = getStringSynset(bottomHierarchyStrings);
			}

			template<class T, class R, class E>
			CStringHierarchyResult<T,R,E>::CStringHierarchyResult(E *takeBottomStringSynset, E *takeTopStringSynset) {
				mTopString = takeTopStringSynset;
				mBottomString = takeBottomStringSynset;
				foreach (QString stringName, mTopString->getEquivalentStringNameList()) {
					mStringStringSynsetHash.insert(stringName,mTopString);
				}
				mStringSynsetContainer.append(mTopString);
				foreach (QString stringName, mBottomString->getEquivalentStringNameList()) {
					mStringStringSynsetHash.insert(stringName,mBottomString);
				}
				mStringSynsetContainer.append(mBottomString);
			}


			template<class T, class R, class E>
			CStringHierarchyResult<T,R,E>::CStringHierarchyResult(E *takeBottomStringSynset) {
				mTopString = nullptr;
				mBottomString = takeBottomStringSynset;
				foreach (QString stringName, mBottomString->getEquivalentStringNameList()) {
					mStringStringSynsetHash.insert(stringName,mBottomString);
				}
				mStringSynsetContainer.append(mBottomString);
			}

			template<class T, class R, class E>
			CStringHierarchyResult<T,R,E>::~CStringHierarchyResult() {
				qDeleteAll(mStringSynsetContainer);
				qDeleteAll(mSubStringRelationContainer);
			}

			template<class T, class R, class E>
			E *CStringHierarchyResult<T,R,E>::getBottomStringSynset() {
				return mBottomString;
			}

			template<class T, class R, class E>
			E *CStringHierarchyResult<T,R,E>::getTopStringSynset() {
				return mTopString;
			}

			template<class T, class R, class E>
			E *CStringHierarchyResult<T,R,E>::getStringSynset(const QString &stringName, bool create) {
				E *stringSynset = mStringStringSynsetHash.value(stringName,0);
				if (!stringSynset && create) {
					stringSynset = new E(stringName);
					mStringStringSynsetHash.insert(stringName,stringSynset);
					mStringSynsetContainer.append(stringSynset);
				}
				return stringSynset;
			}

			template<class T, class R, class E>
			E *CStringHierarchyResult<T,R,E>::getStringSynset(const QStringList &stringNames, bool create) {
				E *stringSynset = 0;
				if (!stringNames.isEmpty()) {
					stringSynset = mStringStringSynsetHash.value(stringNames.first(),0);
					if (!stringSynset && create) {
						stringSynset = new E(stringNames);
						foreach (QString stringName, stringNames) {
							mStringStringSynsetHash.insert(stringName,stringSynset);
						}
						mStringSynsetContainer.append(stringSynset);
					}
				}
				return stringSynset;
			}
			
			
			template<class T, class R, class E>
			E *CStringHierarchyResult<T,R,E>::setTopStringSynset(E *stringSynset) {
				mTopString = stringSynset;
				return stringSynset;
			}


			template<class T, class R, class E>
			E *CStringHierarchyResult<T,R,E>::addStringSynset(E *takeStringSynset) {
				foreach (QString stringName, takeStringSynset->getEquivalentStringNameList()) {
					mStringStringSynsetHash.insert(stringName,takeStringSynset);
				}
				mStringSynsetContainer.append(takeStringSynset);
				return takeStringSynset;
			} 
 

			template<class T, class R, class E>
			R *CStringHierarchyResult<T,R,E>::addSubStringRelation(E *superStringSynset, E *anySubStringSynset) {
				R *subStringRelation = mStringSynsetSubStringHash.value(superStringSynset,0);
				if (!subStringRelation) {
					subStringRelation = new R(superStringSynset);
					mSubStringRelationContainer.append(subStringRelation);
					mStringSynsetSubStringHash.insert(superStringSynset,subStringRelation);
				}
				if (!subStringRelation->hasSubStringsSynset(anySubStringSynset)) {
					subStringRelation->addSubStringsSynset(anySubStringSynset);
				}
				return subStringRelation;
			}

			template<class T, class R, class E>
			R *CStringHierarchyResult<T,R,E>::getSubStringRelation(E *superStringSynset, bool create) {
				R *subStringRelation = mStringSynsetSubStringHash.value(superStringSynset,0);
				if (!subStringRelation && create) {
					subStringRelation = new R(superStringSynset);
					mSubStringRelationContainer.append(subStringRelation);
					mStringSynsetSubStringHash.insert(superStringSynset,subStringRelation);
				}
				return subStringRelation;
			}

			template<class T, class R, class E>
			QString CStringHierarchyResult<T,R,E>::getQueryResultString() {
				QString string;
				return string;
			}
 
			template<class T, class R, class E>
			CTaxonomy *CStringHierarchyResult<T,R,E>::fillTaxonomy(CConcreteOntology *ontology, CTaxonomy *taxonomy) {
				bool completeMapping = true;
				CMAPPINGHASH<CStringRefStringHasher, CConcept *> *stringConceptHash = ontology->getStringMapping()->getClassNameConceptMappingHash(false);
				if (stringConceptHash) {
					QStringList bottomStringList(mBottomString->getEquivalentStringNameList());
					foreach (QString bottomStringString, bottomStringList) {
						CConcept *unsatCon = stringConceptHash->value(bottomStringString,0);
						if (unsatCon) {
							if (!taxonomy->isSatisfiable(unsatCon) && !taxonomy->isNotSatisfiable(unsatCon)) {
								taxonomy->setConceptSatisfiable(unsatCon,false);
							}
						} else {
							completeMapping = false;
						}
					}
					QList<E *> updateStringSynList;
					QSet<E *> updateStringSynSet;
					updateStringSynList.append(mTopString);
					while (!updateStringSynList.isEmpty()) {
						E *stringSynset = updateStringSynList.takeFirst();
						QStringList stringNameList(stringSynset->getEquivalentStringNameList());
						CConcept *firstSatCon = 0;
						foreach (QString stringName, stringNameList) {
							CConcept *satConcept = stringConceptHash->value(stringName,0);
							if (satConcept) {
								if (!taxonomy->isSatisfiable(satConcept) && !taxonomy->isNotSatisfiable(satConcept)) {
									taxonomy->setConceptSatisfiable(satConcept,true);
								}
								if (firstSatCon) {
									if (!taxonomy->isSubsumption(firstSatCon,satConcept) && !taxonomy->isNotSubsumption(firstSatCon,satConcept)) {
										taxonomy->setConceptSubsumption(firstSatCon,satConcept);
									}
									if (!taxonomy->isSubsumption(satConcept,firstSatCon) && !taxonomy->isNotSubsumption(satConcept,firstSatCon)) {
										taxonomy->setConceptSubsumption(satConcept,firstSatCon);
									}
								}
								if (!firstSatCon) {
									firstSatCon = satConcept;
								}
							} else {
								completeMapping = false;
							}
						}
						R *subStringRelation = mStringSynsetSubStringHash.value(stringSynset);
						if (subStringRelation) {
							QSet<E *>* subStringSet(subStringRelation->getSubStringsSynsetSet());
							foreach (E *subStringSynset, *subStringSet) {
								CConcept *con = 0;
								foreach (QString substringName, subStringSynset->getEquivalentStringNameList()) {
									con = stringConceptHash->value(substringName,0);
									if (con) {
										if (!taxonomy->isSatisfiable(con) && !taxonomy->isNotSatisfiable(con)) {
											taxonomy->setConceptSatisfiable(con,true);
										}
										break;
									}
								}
								if (con) {
									if (!taxonomy->isSubsumption(con,firstSatCon) && !taxonomy->isNotSubsumption(con,firstSatCon)) {
										taxonomy->setConceptSubsumption(con,firstSatCon);
									}
									if (!updateStringSynSet.contains(subStringSynset)) {
										updateStringSynSet.insert(subStringSynset);
										updateStringSynList.append(subStringSynset);
									}
								} else {
									completeMapping = false;
								}
							}
						}
					}
				}
				return taxonomy;
			}


			template<class T, class R, class E>
			void CStringHierarchyResult<T,R,E>::compareSubsumersCount(QHash<QString,QString>* subsumHash1, QHash<QString,QString>* subsumHash2) {
				cint64 totalStringDiffCount = 0;				
				cint64 totalStringAddCount = 0;
				cint64 totalStringMissCount = 0;
				cint64 totalAddCount = 0;
				cint64 totalMissCount = 0;
				foreach (const QString& stringName, subsumHash1->uniqueKeys()) {
					QList<QString> stringSuperSynSets1(subsumHash1->values(stringName));
					QList<QString> stringSuperSynSets2(subsumHash2->values(stringName));

					cint64 missingCount = 0;
					cint64 additionalCount = 0;
					foreach (const QString& supsumerStringName, stringSuperSynSets1) {
						if (!stringSuperSynSets2.contains(supsumerStringName)) {							
							++missingCount;
						}
					}
					cint64 diffCount = stringSuperSynSets2.count() - stringSuperSynSets1.count();
					if (diffCount != 0 || missingCount > 0) {
						foreach (const QString& supsumerStringName, stringSuperSynSets2) {
							if (!stringSuperSynSets1.contains(supsumerStringName)) {							
								++additionalCount;
							}
						}
					}
					if (additionalCount > 0) {
						++totalStringAddCount;
						totalAddCount += additionalCount;
					}
					if (missingCount > 0) {
						++totalStringMissCount;
						totalMissCount += missingCount;
					}
					if (additionalCount > 0 || missingCount > 0) {
						++totalStringDiffCount;
						LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("'%1' has %2 additional and %3 missing subsumers.").arg(stringName).arg(additionalCount).arg(missingCount),this);
					}
				}
				if (totalStringDiffCount != 0) {
					LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("%1 with additional subsumers, %2 with missing subsumers, overall %3 different.").arg(totalStringAddCount).arg(totalStringMissCount).arg(totalStringDiffCount),this);
					LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Overall %1 additional subsumers, %2 missing subsumers.").arg(totalAddCount).arg(totalMissCount),this);
				}
			}


			template<class T, class R, class E>
			void CStringHierarchyResult<T,R,E>::collectSubsumerStringSynsets(CStringHierarchyResult *stringHierarchy, QHash<QString,QString>* subsumHash, QSet< QPair<QString,QString> >* subsumSet) {

				QSet< QPair<QString,QString> >* delSubsumSet = nullptr;
				if (!subsumSet) {
					delSubsumSet = new QSet< QPair<QString,QString> >();
					subsumSet = delSubsumSet;
				}

				QList<E*> allStringList;
				QSet<E*> allStringSet;
				QSet<E*> currStringSet;
				E* topStringSynset = stringHierarchy->getTopStringSynset();
				allStringList.append(topStringSynset);
				allStringSet.insert(topStringSynset);
				currStringSet.insert(topStringSynset);

				while (!allStringList.isEmpty()) {
					E* stringSynset = allStringList.takeFirst();
					R* subStringSynsetRel = stringHierarchy->getSubStringRelation(stringSynset,false);

					QList<QString> superStringList;
					foreach (const QString& stringName1, stringSynset->getEquivalentStringNameList()) {
						foreach (const QString& stringName2, stringSynset->getEquivalentStringNameList()) {
							QPair<QString,QString> selfSubSumPair(stringName1,stringName2);
							if (!subsumSet->contains(selfSubSumPair)) {
								subsumSet->insert(selfSubSumPair);
								subsumHash->insertMulti(stringName1,stringName2);
							}

							QList<QString> tmpSuperStringList(subsumHash->values(stringName2));
							foreach (const QString& stringName3, tmpSuperStringList) {
								QPair<QString,QString> subSumPair(stringName1,stringName3);
								if (!subsumSet->contains(subSumPair)) {
									subsumSet->insert(subSumPair);
									subsumHash->insertMulti(stringName1,stringName3);
								}
							}
						}
						if (superStringList.isEmpty()) {
							superStringList.append(subsumHash->values(stringName1));
						}
					}

					if (subStringSynsetRel) {
						foreach (E* subStringSynset, *subStringSynsetRel->getSubStringsSynsetSet()) {

							QList<QString> subStringList(subStringSynset->getEquivalentStringNameList());

							allStringList.append(subStringSynset);


							foreach (const QString& stringName1, subStringList) {
								foreach (const QString& stringName2, subStringList) {
									QPair<QString,QString> selfSubSumPair(stringName1,stringName2);
									if (!subsumSet->contains(selfSubSumPair)) {
										subsumSet->insert(selfSubSumPair);
										subsumHash->insertMulti(stringName1,stringName2);
									}
								}	
							}

							foreach (const QString& stringName1, subStringList) {
								foreach (const QString& stringName2, superStringList) {
									QPair<QString,QString> subSumPair(stringName1,stringName2);
									if (!subsumSet->contains(subSumPair)) {
										subsumSet->insert(subSumPair);
										subsumHash->insertMulti(stringName1,stringName2);
									}
								}
							}
						}
					}
				}
				delete delSubsumSet;
			}

			template<class T, class R, class E>
			bool CStringHierarchyResult<T,R,E>::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				T *otherStringHierarchy = dynamic_cast<T *>(otherQueryResult);
				if (!otherStringHierarchy) {
					return false;
				}
				bool wrongResult = false;
				if (!mBottomString->isResultEquivalentTo(otherStringHierarchy->getBottomStringSynset())) {
					LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Bottom synset does not match."),this);
					wrongResult = true;
				}
				if (mTopString && !otherStringHierarchy->getTopStringSynset() || !mTopString && otherStringHierarchy->getTopStringSynset()) {
					LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Top synset does not match."),this);
					return false;
				}
				if (!mTopString) {
					return !wrongResult;
				}
				if (!mTopString->isResultEquivalentTo(otherStringHierarchy->getTopStringSynset())) {
					LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Top synset does not match."),this);
					return false;
				}




#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
				//QHash<QString,QString> subsumHash1;
				//QHash<QString,QString> subsumHash2;
				//QSet< QPair<QString,QString> > subsumSet1;
				//QSet< QPair<QString,QString> > subsumSet2;
				//collectSubsumerStringSynsets(this,&subsumHash1,&subsumSet1);
				//collectSubsumerStringSynsets(otherStringHierarchy,&subsumHash2,&subsumSet2);
				//compareSubsumersCount(&subsumHash1,&subsumHash2);
#endif

				E* topStringSynset = otherStringHierarchy->getTopStringSynset();
				QList<E *> testStringList;
				QList<E *> otherStringList;
				QSet<E *> testStringSet;
				testStringList.append(mTopString);
				otherStringList.append(topStringSynset);
				testStringSet.insert(mTopString);
				while (!testStringList.isEmpty()) {
					E *stringSynset = testStringList.takeFirst();
					E *otherStringSynset = otherStringList.takeFirst();
					R *subStringRelation = getSubStringRelation(stringSynset,false);
					R *otherSubStringRelation = otherStringHierarchy->getSubStringRelation(otherStringSynset,false);

					if (!subStringRelation && otherSubStringRelation || subStringRelation && !otherSubStringRelation) {
						LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Sub relations of '%1' do not match.").arg(stringSynset->getEquivalentStringNameList().join(", ")),this);
						return false;
					}

					if (subStringRelation) {

						if (subStringRelation->getSubStringsSynsetCount() != otherSubStringRelation->getSubStringsSynsetCount()) {
							wrongResult = true;
							LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Sub relations of '%1' do not match.").arg(stringSynset->getEquivalentStringNameList().join(", ")),this);
						}


						if (subStringRelation->getSubStringsSynsetCount() < 20) {
							QList<E *> otherSubStringList(otherSubStringRelation->getSubStringsSynsetList());	
							foreach (E *stringSynset, subStringRelation->getSubStringsSynsetList()) {
								bool contained = false;					
								E *accSubStringSynset = stringSynset;

								for (qint64 cnt = otherSubStringList.count(); cnt > 0; --cnt) {
									E *accOtherSubStringSynset = otherSubStringList.first();
									if (!accOtherSubStringSynset) {
										wrongResult = true;
										break;
									} else {
										contained |= accOtherSubStringSynset->isResultEquivalentTo(stringSynset);
										if (contained) {
											otherSubStringList.takeFirst();

											if (!testStringSet.contains(accSubStringSynset)) {
												testStringSet.insert(accSubStringSynset);
												testStringList.append(accSubStringSynset);
												otherStringList.append(accOtherSubStringSynset);
											}

											break;
										} else {
											otherSubStringList.append(otherSubStringList.takeFirst());
										}
									}
								}
								if (!contained) {
									wrongResult = true;
									LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Could not find corresponding sub synset '%1' of super synset '%2'.").arg(stringSynset->getEquivalentStringNameList().join(", ")).arg(otherStringSynset->getEquivalentStringNameList().join(", ")),this);

								}
							}
						} else {
							QList<E *> otherSubStringList(otherSubStringRelation->getSubStringsSynsetList());
							QHash<QString,E*> otherSubStringHash;
							foreach (E* otherStringSynset, otherSubStringList) {
								foreach (const QString& stringName ,otherStringSynset->getEquivalentStringNameList()) {
									otherSubStringHash.insert(stringName,otherStringSynset);
								}
							}

							foreach (E *stringSynset, subStringRelation->getSubStringsSynsetList()) {
								bool contained = false;					
								E *accSubStringSynset = stringSynset;

								E *accOtherSubStringSynset = otherSubStringHash.value(accSubStringSynset->getEquivalentStringNameList().first());
								if (!accOtherSubStringSynset || !accOtherSubStringSynset->isResultEquivalentTo(stringSynset)) {
									wrongResult = true;
									LOG(ERROR,"::Konclude::Reasoner::Query::StringHierarchyResult",logTr("Could not find corresponding sub synset '%1' of super synset '%2'.").arg(stringSynset->getEquivalentStringNameList().join(", ")).arg(otherStringSynset->getEquivalentStringNameList().join(", ")),this);
								
								} else if (accOtherSubStringSynset) {
									foreach (const QString& stringName ,accOtherSubStringSynset->getEquivalentStringNameList()) {
										otherSubStringHash.insert(stringName,nullptr);
									}
									contained = true;
								}

								if (contained) {
									if (!testStringSet.contains(accSubStringSynset)) {
										testStringSet.insert(accSubStringSynset);
										testStringList.append(accSubStringSynset);
										otherStringList.append(accOtherSubStringSynset);
									}
								}

							}
						}
					}
				}
				return !wrongResult;
			}


		}; // end namespace Query

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_QUERY_CSTRINGHIERARCHYRESULT_CPP
