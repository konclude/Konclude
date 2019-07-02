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

#include "CClassHierarchyResult.h"


namespace Konclude {

	namespace Reasoner {

		namespace Query {


			CClassHierarchyResult::CClassHierarchyResult() {
				bottomClass = 0;
				topClass = 0;
			}

			CClassHierarchyResult::CClassHierarchyResult(const QString &bottomHierarchyClass, const QString &topHierarchyClass) {
				topClass = getClassSynset(topHierarchyClass);
				bottomClass = getClassSynset(bottomHierarchyClass);
			}

			CClassHierarchyResult::CClassHierarchyResult(const QStringList &bottomHierarchyClasses, const QStringList &topHierarchyClasses) {
				topClass = getClassSynset(topHierarchyClasses);
				bottomClass = getClassSynset(bottomHierarchyClasses);
			}

			CClassHierarchyResult::CClassHierarchyResult(CClassSynsetResult *takeBottomClassSynset, CClassSynsetResult *takeTopClassSynset) {
				topClass = takeTopClassSynset;
				bottomClass = takeBottomClassSynset;
				foreach (QString className, topClass->getEquivalentClassNameList()) {
					classClassSynsetHash.insert(className,topClass);
				}
				classSynsetContainer.append(topClass);
				foreach (QString className, bottomClass->getEquivalentClassNameList()) {
					classClassSynsetHash.insert(className,bottomClass);
				}
				classSynsetContainer.append(bottomClass);
			}


			CClassHierarchyResult::CClassHierarchyResult(CClassSynsetResult *takeBottomClassSynset) {
				topClass = nullptr;
				bottomClass = takeBottomClassSynset;
				foreach (QString className, bottomClass->getEquivalentClassNameList()) {
					classClassSynsetHash.insert(className,bottomClass);
				}
				classSynsetContainer.append(bottomClass);
			}

			CClassHierarchyResult::~CClassHierarchyResult() {
				qDeleteAll(classSynsetContainer);
				qDeleteAll(subClassRelationContainer);
			}

			CClassSynsetResult *CClassHierarchyResult::getBottomClassSynset() {
				return bottomClass;
			}

			CClassSynsetResult *CClassHierarchyResult::getTopClassSynset() {
				return topClass;
			}

			CClassSynsetResult *CClassHierarchyResult::getClassSynset(const QString &className, bool create) {
				CClassSynsetResult *classSynset = classClassSynsetHash.value(className,0);
				if (!classSynset && create) {
					classSynset = new CClassSynsetResult(className);
					classClassSynsetHash.insert(className,classSynset);
					classSynsetContainer.append(classSynset);
				}
				return classSynset;
			}

			CClassSynsetResult *CClassHierarchyResult::getClassSynset(const QStringList &classNames, bool create) {
				CClassSynsetResult *classSynset = 0;
				if (!classNames.isEmpty()) {
					classSynset = classClassSynsetHash.value(classNames.first(),0);
					if (!classSynset && create) {
						classSynset = new CClassSynsetResult(classNames);
						foreach (QString className, classNames) {
							classClassSynsetHash.insert(className,classSynset);
						}
						classSynsetContainer.append(classSynset);
					}
				}
				return classSynset;
			}
			
			
			CClassSynsetResult *CClassHierarchyResult::setTopClassSynset(CClassSynsetResult *classSynset) {
				topClass = classSynset;
				return classSynset;
			}


			CClassSynsetResult *CClassHierarchyResult::addClassSynset(CClassSynsetResult *takeClassSynset) {
				foreach (QString className, takeClassSynset->getEquivalentClassNameList()) {
					classClassSynsetHash.insert(className,takeClassSynset);
				}
				classSynsetContainer.append(takeClassSynset);
				return takeClassSynset;
			} 
 

			CClassSubClassesRelationResult *CClassHierarchyResult::addSubClassRelation(CClassSynsetResult *superClassSynset, CClassSynsetResult *anySubClassSynset) {
				CClassSubClassesRelationResult *subClassRelation = classSynsetSubClassHash.value(superClassSynset,0);
				if (!subClassRelation) {
					subClassRelation = new CClassSubClassesRelationResult(superClassSynset);
					subClassRelationContainer.append(subClassRelation);
					classSynsetSubClassHash.insert(superClassSynset,subClassRelation);
				}
				if (!subClassRelation->hasSubClassesSynset(anySubClassSynset)) {
					subClassRelation->addSubClassesSynset(anySubClassSynset);
				}
				return subClassRelation;
			}

			CClassSubClassesRelationResult *CClassHierarchyResult::getSubClassRelation(CClassSynsetResult *superClassSynset, bool create) {
				CClassSubClassesRelationResult *subClassRelation = classSynsetSubClassHash.value(superClassSynset,0);
				if (!subClassRelation && create) {
					subClassRelation = new CClassSubClassesRelationResult(superClassSynset);
					subClassRelationContainer.append(subClassRelation);
					classSynsetSubClassHash.insert(superClassSynset,subClassRelation);
				}
				return subClassRelation;
			}

			QString CClassHierarchyResult::getQueryResultString() {
				QString string;
				return string;
			}
 
			CTaxonomy *CClassHierarchyResult::fillTaxonomy(CConcreteOntology *ontology, CTaxonomy *taxonomy) {
				bool completeMapping = true;
				CMAPPINGHASH<CStringRefStringHasher, CConcept *> *classConceptHash = ontology->getStringMapping()->getClassNameConceptMappingHash(false);
				if (classConceptHash) {
					QStringList bottomClassList(bottomClass->getEquivalentClassNameList());
					foreach (QString bottomClassString, bottomClassList) {
						CConcept *unsatCon = classConceptHash->value(bottomClassString,0);
						if (unsatCon) {
							if (!taxonomy->isSatisfiable(unsatCon) && !taxonomy->isNotSatisfiable(unsatCon)) {
								taxonomy->setConceptSatisfiable(unsatCon,false);
							}
						} else {
							completeMapping = false;
						}
					}
					QList<CClassSynsetResult *> updateClassSynList;
					QSet<CClassSynsetResult *> updateClassSynSet;
					updateClassSynList.append(topClass);
					while (!updateClassSynList.isEmpty()) {
						CClassSynsetResult *classSynset = updateClassSynList.takeFirst();
						QStringList classNameList(classSynset->getEquivalentClassNameList());
						CConcept *firstSatCon = 0;
						foreach (QString className, classNameList) {
							CConcept *satConcept = classConceptHash->value(className,0);
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
						CClassSubClassesRelationResult *subClassRelation = classSynsetSubClassHash.value(classSynset);
						if (subClassRelation) {
							QSet<CClassSynsetResult *>* subClassSet(subClassRelation->getSubClassesSynsetSet());
							foreach (CClassSynsetResult *subClassSynset, *subClassSet) {
								CConcept *con = 0;
								foreach (QString subclassName, subClassSynset->getEquivalentClassNameList()) {
									con = classConceptHash->value(subclassName,0);
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
									if (!updateClassSynSet.contains(subClassSynset)) {
										updateClassSynSet.insert(subClassSynset);
										updateClassSynList.append(subClassSynset);
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


			void CClassHierarchyResult::compareSubsumersCount(QHash<QString,QString>* subsumHash1, QHash<QString,QString>* subsumHash2) {
				cint64 totalClassDiffCount = 0;				
				cint64 totalClassAddCount = 0;
				cint64 totalClassMissCount = 0;
				cint64 totalAddCount = 0;
				cint64 totalMissCount = 0;
				foreach (const QString& className, subsumHash1->uniqueKeys()) {
					QList<QString> classSuperSynSets1(subsumHash1->values(className));
					QList<QString> classSuperSynSets2(subsumHash2->values(className));

					cint64 missingCount = 0;
					cint64 additionalCount = 0;
					foreach (const QString& supsumerClassName, classSuperSynSets1) {
						if (!classSuperSynSets2.contains(supsumerClassName)) {							
							++missingCount;
						}
					}
					cint64 diffCount = classSuperSynSets2.count() - classSuperSynSets1.count();
					if (diffCount != 0 || missingCount > 0) {
						foreach (const QString& supsumerClassName, classSuperSynSets2) {
							if (!classSuperSynSets1.contains(supsumerClassName)) {							
								++additionalCount;
							}
						}
					}
					if (additionalCount > 0) {
						++totalClassAddCount;
						totalAddCount += additionalCount;
					}
					if (missingCount > 0) {
						++totalClassMissCount;
						totalMissCount += missingCount;
					}
					if (additionalCount > 0 || missingCount > 0) {
						++totalClassDiffCount;
						LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Class '%1' has %2 additional and %3 missing subsumers.").arg(className).arg(additionalCount).arg(missingCount),this);
					}
				}
				if (totalClassDiffCount != 0) {
					LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("%1 classes with additional subsumers, %2 classes with missing subsumers, overall %3 classes different.").arg(totalClassAddCount).arg(totalClassMissCount).arg(totalClassDiffCount),this);
					LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Overall %1 additional subsumers, %2 missing subsumers.").arg(totalAddCount).arg(totalMissCount),this);
				}
			}


			void CClassHierarchyResult::collectSubsumerClassSynsets(CClassHierarchyResult *classHierarchy, QHash<QString,QString>* subsumHash, QSet< QPair<QString,QString> >* subsumSet) {

				QSet< QPair<QString,QString> >* delSubsumSet = nullptr;
				if (!subsumSet) {
					delSubsumSet = new QSet< QPair<QString,QString> >();
					subsumSet = delSubsumSet;
				}

				QList<CClassSynsetResult*> allClassList;
				QSet<CClassSynsetResult*> allClassSet;
				QSet<CClassSynsetResult*> currClassSet;
				CClassSynsetResult* topClassSynset = classHierarchy->getTopClassSynset();
				allClassList.append(topClassSynset);
				allClassSet.insert(topClassSynset);
				currClassSet.insert(topClassSynset);

				while (!allClassList.isEmpty()) {
					CClassSynsetResult* classSynset = allClassList.takeFirst();
					CClassSubClassesRelationResult* subClassSynsetRel = classHierarchy->getSubClassRelation(classSynset,false);

					QList<QString> superClassList;
					foreach (const QString& className1, classSynset->getEquivalentClassNameList()) {
						foreach (const QString& className2, classSynset->getEquivalentClassNameList()) {
							QPair<QString,QString> selfSubSumPair(className1,className2);
							if (!subsumSet->contains(selfSubSumPair)) {
								subsumSet->insert(selfSubSumPair);
								subsumHash->insertMulti(className1,className2);
							}

							QList<QString> tmpSuperClassList(subsumHash->values(className2));
							foreach (const QString& className3, tmpSuperClassList) {
								QPair<QString,QString> subSumPair(className1,className3);
								if (!subsumSet->contains(subSumPair)) {
									subsumSet->insert(subSumPair);
									subsumHash->insertMulti(className1,className3);
								}
							}
						}
						if (superClassList.isEmpty()) {
							superClassList.append(subsumHash->values(className1));
						}
					}

					if (subClassSynsetRel) {
						foreach (CClassSynsetResult* subClassSynset, *subClassSynsetRel->getSubClassesSynsetSet()) {

							QList<QString> subClassList(subClassSynset->getEquivalentClassNameList());

							allClassList.append(subClassSynset);


							foreach (const QString& className1, subClassList) {
								foreach (const QString& className2, subClassList) {
									QPair<QString,QString> selfSubSumPair(className1,className2);
									if (!subsumSet->contains(selfSubSumPair)) {
										subsumSet->insert(selfSubSumPair);
										subsumHash->insertMulti(className1,className2);
									}
								}	
							}

							foreach (const QString& className1, subClassList) {
								foreach (const QString& className2, superClassList) {
									QPair<QString,QString> subSumPair(className1,className2);
									if (!subsumSet->contains(subSumPair)) {
										subsumSet->insert(subSumPair);
										subsumHash->insertMulti(className1,className2);
									}
								}
							}
						}
					}
				}
				delete delSubsumSet;
			}

			bool CClassHierarchyResult::isResultEquivalentTo(CQueryResult *otherQueryResult) {
				if (!otherQueryResult) {
					return false;
				}
				CClassHierarchyResult *otherClassHierarchy = dynamic_cast<CClassHierarchyResult *>(otherQueryResult);
				if (!otherClassHierarchy) {
					return false;
				}
				bool wrongResult = false;
				if (!bottomClass->isResultEquivalentTo(otherClassHierarchy->getBottomClassSynset())) {
					LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Bottom class synset doesn't match."),this);
					wrongResult = true;
				}
				if (topClass && !otherClassHierarchy->getTopClassSynset() || !topClass && otherClassHierarchy->getTopClassSynset()) {
					LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Top class synset doesn't match."),this);
					return false;
				}
				if (!topClass) {
					return !wrongResult;
				}
				if (!topClass->isResultEquivalentTo(otherClassHierarchy->getTopClassSynset())) {
					LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Top class synset doesn't match."),this);
					return false;
				}




#ifndef KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
				//QHash<QString,QString> subsumHash1;
				//QHash<QString,QString> subsumHash2;
				//QSet< QPair<QString,QString> > subsumSet1;
				//QSet< QPair<QString,QString> > subsumSet2;
				//collectSubsumerClassSynsets(this,&subsumHash1,&subsumSet1);
				//collectSubsumerClassSynsets(otherClassHierarchy,&subsumHash2,&subsumSet2);
				//compareSubsumersCount(&subsumHash1,&subsumHash2);
#endif

				CClassSynsetResult* topClassSynset = otherClassHierarchy->getTopClassSynset();
				QList<CClassSynsetResult *> testClassList;
				QList<CClassSynsetResult *> otherClassList;
				QSet<CClassSynsetResult *> testClassSet;
				testClassList.append(topClass);
				otherClassList.append(topClassSynset);
				testClassSet.insert(topClass);
				while (!testClassList.isEmpty()) {
					CClassSynsetResult *classSynset = testClassList.takeFirst();
					CClassSynsetResult *otherClassSynset = otherClassList.takeFirst();
					CClassSubClassesRelationResult *subClassRelation = getSubClassRelation(classSynset,false);
					CClassSubClassesRelationResult *otherSubClassRelation = otherClassHierarchy->getSubClassRelation(otherClassSynset,false);

					if (!subClassRelation && otherSubClassRelation || subClassRelation && !otherSubClassRelation) {
						LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("SubClass relations of '%1' doesn't match.").arg(classSynset->getEquivalentClassNameList().join(", ")),this);
						return false;
					}

					if (subClassRelation) {

						if (subClassRelation->getSubClassesSynsetCount() != otherSubClassRelation->getSubClassesSynsetCount()) {
							wrongResult = true;
							LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("SubClass relations of '%1' doesn't match.").arg(classSynset->getEquivalentClassNameList().join(", ")),this);
						}


						if (subClassRelation->getSubClassesSynsetCount() < 20) {
							QList<CClassSynsetResult *> otherSubClassList(otherSubClassRelation->getSubClassesSynsetList());	
							foreach (CClassSynsetResult *classSynset, subClassRelation->getSubClassesSynsetList()) {
								bool contained = false;					
								CClassSynsetResult *accSubClassSynset = classSynset;

								for (qint64 cnt = otherSubClassList.count(); cnt > 0; --cnt) {
									CClassSynsetResult *accOtherSubClassSynset = otherSubClassList.first();
									if (!accOtherSubClassSynset) {
										wrongResult = true;
										break;
									} else {
										contained |= accOtherSubClassSynset->isResultEquivalentTo(classSynset);
										if (contained) {
											otherSubClassList.takeFirst();

											if (!testClassSet.contains(accSubClassSynset)) {
												testClassSet.insert(accSubClassSynset);
												testClassList.append(accSubClassSynset);
												otherClassList.append(accOtherSubClassSynset);
											}

											break;
										} else {
											otherSubClassList.append(otherSubClassList.takeFirst());
										}
									}
								}
								if (!contained) {
									wrongResult = true;
									LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Couldn't find corresponding synset '%1' of super class synset '%2'.").arg(classSynset->getEquivalentClassNameList().join(", ")).arg(otherClassSynset->getEquivalentClassNameList().join(", ")),this);

								}
							}
						} else {
							QList<CClassSynsetResult *> otherSubClassList(otherSubClassRelation->getSubClassesSynsetList());
							QHash<QString,CClassSynsetResult*> otherSubClassHash;
							foreach (CClassSynsetResult* otherClassSynset, otherSubClassList) {
								foreach (const QString& className ,otherClassSynset->getEquivalentClassNameList()) {
									otherSubClassHash.insert(className,otherClassSynset);
								}
							}

							foreach (CClassSynsetResult *classSynset, subClassRelation->getSubClassesSynsetList()) {
								bool contained = false;					
								CClassSynsetResult *accSubClassSynset = classSynset;

								CClassSynsetResult *accOtherSubClassSynset = otherSubClassHash.value(accSubClassSynset->getEquivalentClassNameList().first());
								if (!accOtherSubClassSynset || !accOtherSubClassSynset->isResultEquivalentTo(classSynset)) {
									wrongResult = true;
									LOG(ERROR,"::Konclude::Reasoner::Query::ClassHierarchyResult",logTr("Couldn't find corresponding synset '%1' of super class synset '%2'.").arg(classSynset->getEquivalentClassNameList().join(", ")).arg(otherClassSynset->getEquivalentClassNameList().join(", ")),this);
								
								} else if (accOtherSubClassSynset) {
									foreach (const QString& className ,accOtherSubClassSynset->getEquivalentClassNameList()) {
										otherSubClassHash.insert(className,nullptr);
									}
									contained = true;
								}

								if (contained) {
									if (!testClassSet.contains(accSubClassSynset)) {
										testClassSet.insert(accSubClassSynset);
										testClassList.append(accSubClassSynset);
										otherClassList.append(accOtherSubClassSynset);
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
