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

#include "CJNIQueryProcessor.h"


namespace Konclude {

	namespace Control {

		namespace Interface {

			namespace JNI {


				CJNIQueryProcessor::CJNIQueryProcessor(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData) {
					mOntRevData = ontRevData;
					mJNIInstanceManager = jniInstanceManager;
					mJNICommandProcessor = jniInstanceManager->getJNICommandProcessor();
				}

				CJNIQueryProcessor::~CJNIQueryProcessor() {					
				}


				bool CJNIQueryProcessor::checkIsOntologyConsistent() {
					bool consistent = false;
					CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
					CIsConsistentQuery* consistencyQuery = new CIsConsistentQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig);
					mJNICommandProcessor->calculateOntologyQuery(consistencyQuery);
					CQueryResult* result = consistencyQuery->getQueryResult();
					if (result) {
						CBooleanQueryResult* boolQueryResult = dynamic_cast<CBooleanQueryResult*>(result);
						consistent = boolQueryResult->getResult();
					}
					delete consistencyQuery;
					return consistent;
				}






				bool CJNIQueryProcessor::queryOntologySubClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className,true,false,direct);
						mJNICommandProcessor->calculateOntologyQuery(subSuperClassQuery);
						CQueryResult* result = subSuperClassQuery->getQueryResult();
						if (result) {
							subSuperClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete subSuperClassQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologySuperClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperClassesResultVisitCallbackQuery* subSuperClassQuery = new CSubSuperClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className,false,true,direct);
						mJNICommandProcessor->calculateOntologyQuery(subSuperClassQuery);
						CQueryResult* result = subSuperClassQuery->getQueryResult();
						if (result) {
							subSuperClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete subSuperClassQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologyEquivalentClasses(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CEquivalentClassesResultVisitCallbackQuery* equivClassQuery = new CEquivalentClassesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,className);
						mJNICommandProcessor->calculateOntologyQuery(equivClassQuery);
						CQueryResult* result = equivClassQuery->getQueryResult();
						if (result) {
							equivClassQuery->callbackVisitingClasses(visitingCallback);
						}
						delete equivClassQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve concept for '%1'.").arg(className),0);
					}
					return false;
				}










				bool CJNIQueryProcessor::queryOntologySubObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperPropertiesResultVisitCallbackQuery* subSuperPropertiesQuery = new CSubSuperPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,propertyName,true,false,false,direct);
						mJNICommandProcessor->calculateOntologyQuery(subSuperPropertiesQuery);
						CQueryResult* result = subSuperPropertiesQuery->getQueryResult();
						if (result) {
							subSuperPropertiesQuery->callbackVisitingProperties(visitingCallback);
						}
						delete subSuperPropertiesQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologySuperObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSubSuperPropertiesResultVisitCallbackQuery* subSuperPropertiesQuery = new CSubSuperPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,propertyName,false,true,false,direct);
						mJNICommandProcessor->calculateOntologyQuery(subSuperPropertiesQuery);
						CQueryResult* result = subSuperPropertiesQuery->getQueryResult();
						if (result) {
							subSuperPropertiesQuery->callbackVisitingProperties(visitingCallback);
						}
						delete subSuperPropertiesQuery;
						return result != nullptr;
					}
					return false;
				}


				bool CJNIQueryProcessor::queryOntologyEquivalentObjectProperties(const QString& propertyName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CEquivalentPropertiesResultVisitCallbackQuery* equivPropQuery = new CEquivalentPropertiesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,role,false,propertyName);
						mJNICommandProcessor->calculateOntologyQuery(equivPropQuery);
						CQueryResult* result = equivPropQuery->getQueryResult();
						if (result) {
							equivPropQuery->callbackVisitingProperties(visitingCallback);
						}
						delete equivPropQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve role for '%1'.").arg(propertyName),0);
					}
					return false;
				}







				bool CJNIQueryProcessor::queryOntologyInstances(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CConcept* concept = ont->getStringMapping()->getConceptFromName(className);
					if (concept) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CInstancesResultVisitCallbackQuery* instancesQuery = new CInstancesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,concept,direct,className);
						mJNICommandProcessor->calculateOntologyQuery(instancesQuery);
						CQueryResult* result = instancesQuery->getQueryResult();
						if (result) {
							instancesQuery->callbackVisitingInstances(visitingCallback);
						}
						delete instancesQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve concept for '%1'.").arg(className),0);
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyTypes(const QString& indiName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					if (indi) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CTypesResultVisitCallbackQuery* typesQuery = new CTypesResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,direct,indiName);
						mJNICommandProcessor->calculateOntologyQuery(typesQuery);
						CQueryResult* result = typesQuery->getQueryResult();
						if (result) {
							typesQuery->callbackVisitingTypes(visitingCallback);
						}
						delete typesQuery;
						return result != nullptr;
					} else {
						LOG(INFO,"::Konclude::JNIQueryProcessor",logTr("Could not resolve individual for '%1'.").arg(indiName),0);
					}
					return false;
				}



				bool CJNIQueryProcessor::queryOntologySameIndividuals(const QString& indiName, CEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					if (indi) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CSameIndividualsResultVisitCallbackQuery* sameIndiQuery = new CSameIndividualsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,indiName);
						mJNICommandProcessor->calculateOntologyQuery(sameIndiQuery);
						CQueryResult* result = sameIndiQuery->getQueryResult();
						if (result) {
							sameIndiQuery->callbackVisitingInstances(visitingCallback);
						}
						delete sameIndiQuery;
						return result != nullptr;
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyObjectPropertySources(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (indi && role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CObjectPropertySourcesTargetsResultVisitCallbackQuery* propertySouTarQuery = new CObjectPropertySourcesTargetsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,role,true,false);
						mJNICommandProcessor->calculateOntologyQuery(propertySouTarQuery);
						CQueryResult* result = propertySouTarQuery->getQueryResult();
						if (result) {
							propertySouTarQuery->callbackVisitingIndividuals(visitingCallback);
						}
						delete propertySouTarQuery;
						return result != nullptr;
					}
					return false;
				}




				bool CJNIQueryProcessor::queryOntologyObjectPropertyTargets(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback) {
					CConcreteOntology* ont = mOntRevData->getOntologyRevision()->getOntology();
					CIndividual* indi = ont->getStringMapping()->getIndividualFromName(indiName);
					CRole* role = ont->getStringMapping()->getRoleFromName(propertyName);
					if (indi && role) {
						CCalculationConfigurationExtension* calcConfig = new CCalculationConfigurationExtension(mOntRevData->getOntologyRevision()->getOntologyConfiguration());
						CObjectPropertySourcesTargetsResultVisitCallbackQuery* propertySouTarQuery = new CObjectPropertySourcesTargetsResultVisitCallbackQuery(mOntRevData->getOntologyRevision()->getOntology(),calcConfig,indi,role,false,true);
						mJNICommandProcessor->calculateOntologyQuery(propertySouTarQuery);
						CQueryResult* result = propertySouTarQuery->getQueryResult();
						if (result) {
							propertySouTarQuery->callbackVisitingIndividuals(visitingCallback);
						}
						delete propertySouTarQuery;
						return result != nullptr;
					}
					return false;
				}






			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude
