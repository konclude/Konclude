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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H

// Libraries includes
#include <QStack>

// Namespace includes
#include "JNISettings.h"
#include "CJNIOntologyRevisionData.h"
#include "CJNICommandProcessor.h"
#include "CJNIInstanceManager.h"
#include "CJNIQueryExpressionBuildingLoader.h"

// Other includes
#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"

#include "Parser/Expressions/CBuildExpression.h"
#include "Parser/Expressions/CClassTermExpression.h"

#include "Reasoner/Revision/COntologyRevision.h"

#include "Reasoner/Query/CSetOfEntityExpressionSetResultVisitingCallback.h"
#include "Reasoner/Query/CComplexConceptAnsweringQuery.h"
#include "Reasoner/Query/CComplexSatisfiabilityAnsweringQuery.h"
#include "Reasoner/Query/CComplexSubClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexSuperClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexEquivalentClassesAnsweringQuery.h"
#include "Reasoner/Query/CComplexIndividualInstancesAnsweringQuery.h"
#include "Reasoner/Query/CBooleanQueryResult.h"
#include "Reasoner/Query/CTaxonomyPremisingQuery.h"
#include "Reasoner/Ontology/COntologyProcessingStepVector.h"
#include "Reasoner/Query/CClassSynsetsResult.h"
#include "Reasoner/Query/CClassSetResult.h"
#include "Reasoner/Query/CIndividualSynsetsResult.h"
#include "Reasoner/Query/CEquivalentClassesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSubSuperClassesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CEquivalentPropertiesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSubSuperPropertiesResultVisitCallbackQuery.h"

#include "Reasoner/Query/CInstancesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CSameIndividualsResultVisitCallbackQuery.h"
#include "Reasoner/Query/CTypesResultVisitCallbackQuery.h"
#include "Reasoner/Query/CObjectPropertySourcesTargetsResultVisitCallbackQuery.h"

#include "Control/Command/CConfigManagerReader.h"
#include "Reasoner/Kernel/Manager/CReasonerManagerThread.h"
#include "Reasoner/Kernel/Manager/CRealizationManager.h"
#include "Reasoner/Kernel/Calculation/CConcurrentTaskCalculationEnvironment.h"
#include "Reasoner/Classifier/CClassificationManager.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner::Generator;
	using namespace Reasoner::Query;
	using namespace Parser::Expression;
	using namespace Reasoner::Kernel::Manager;
	using namespace Reasoner::Kernel::Calculation;
	using namespace Reasoner::Classifier;
	using namespace Control::Command;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*! 
				 *
				 *		\class		CJNIQueryProcessor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CJNIQueryProcessor {

					// public methods
					public:
						//! Constructor
						CJNIQueryProcessor(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData);

						//! Destructor
						virtual ~CJNIQueryProcessor();


						bool checkIsOntologyConsistent();


						bool queryOntologySubClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySuperClasses(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						// answers a query about a named class from the class taxonomy directly, on the calling thread, when the
						// classification is complete and the class is in it, otherwise through the reasoner (see the .cpp)
						void calculateNamedClassQuery(CTaxonomyPremisingQuery* query, CConcept* concept);
						CTaxonomy* getCompletedClassTaxonomy(CConcreteOntology* ontology);
						bool queryOntologyEquivalentClasses(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback);
						// the whole class taxonomy at once, if the classification is complete: one entity set per hierarchy
						// node, the top node first, and the edges as pairs of the nodes' positions in that order, parent first
						bool queryOntologyClassHierarchy(CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QVector<qint32>& edgeVector);


						bool queryOntologySubObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySuperObjectProperties(const QString& propertyName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyEquivalentObjectProperties(const QString& propertyName, CEntityExpressionSetResultVisitingCallback* visitingCallback);



						bool queryOntologyInstances(const QString& className, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyTypes(const QString& indiName, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologySameIndividuals(const QString& indiName, CEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyObjectPropertySources(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);
						bool queryOntologyObjectPropertyTargets(const QString& indiName, const QString& propertyName, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback);


						/**
						 * The builder for the class expressions that the queries below are asked
						 * about. It builds into a revision of the installed ontology that is never
						 * installed, created on first use and kept for the lifetime of this processor,
						 * so the expressions stay valid as long as the querying bridge is open.
						 * Returns null if no revision could be created.
						 */
						CJNIAxiomExpressionVisitingLoader* getClassExpressionBuilder();

						/**
						 * The queries about a class expression, the anonymous class expressions of the
						 * OWL API. They are answered by the complex answering queries that the OWLlink
						 * interface uses, whose results name the entities, so the names are resolved
						 * to the entity expressions of the installed ontology and reported through the
						 * same callbacks as the named queries above. Each returns false if the query
						 * was not answered and says why in the error message.
						 */
						bool checkIsClassExpressionSatisfiable(CBuildExpression* classExpression, bool& satisfiable, QString& errorMessage);
						bool queryClassExpressionSubClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);
						bool queryClassExpressionSuperClasses(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);
						bool queryClassExpressionEquivalentClasses(CBuildExpression* classExpression, CEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);
						bool queryClassExpressionInstances(CBuildExpression* classExpression, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);

						/**
						 * The progress of the calculations of the reasoner, the numbers that the command
						 * line prints with '-a', see CAnalyseReasonerManager::logQueryProgressUpdates.
						 * Fills PROGRESS_VALUE_COUNT values: the processed and the approximated remaining
						 * tasks of the calculation, then the percentage, the tested and the total tests
						 * and the approximated remaining milliseconds of the classification, the same
						 * four of the realization, and the initialised and the existing saturation nodes
						 * of the ontology's precomputation, which '-a' does not print. A value that is
						 * not known is 0.
						 *
						 * Unlike the queries this only reads counters of the managers and the ontology,
						 * so it may be called from another thread while a query is calculated, which is
						 * what it is for. The managers are looked up once, through the configuration of
						 * the reasoner commander, which answers that while it calculates.
						 */
						bool queryReasoningProgress(QVector<double>& values);

						static const cint64 PROGRESS_VALUE_COUNT = 12;


					// protected methods
					protected:
						CClassTermExpression* getQueryClassTermExpression(CBuildExpression* classExpression, QString& errorMessage);
						CQueryResult* calculateClassExpressionQuery(CComplexConceptAnsweringQuery* query, QString& errorMessage);

						bool computeClassExpressionEquivalentClassNames(CClassTermExpression* classTermExp, QStringList& classNames, QString& errorMessage);
						bool queryClassExpressionSubSuperClasses(CBuildExpression* classExpression, bool subClasses, bool superClasses, bool direct, CSetOfEntityExpressionSetResultVisitingCallback* visitingCallback, QString& errorMessage);

						void visitClassName(const QString& className, CEntityExpressionSetResultVisitingCallback* visitingCallback);
						void visitIndividualName(const QString& individualName, CEntityExpressionSetResultVisitingCallback* visitingCallback);

					// protected variables
					protected:
						CJNIInstanceManager* mJNIInstanceManager;
						CJNICommandProcessor* mJNICommandProcessor;
						CJNIOntologyRevisionData* mOntRevData;

						COntologyRevision* mExpressionOntRev;
						CJNIQueryExpressionBuildingLoader* mExpressionBuilder;

						bool mProgressManagersLookedUp;
						CReasonerManager* mReasonerManager;
						CClassificationManager* mClassificationManager;



					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYPROCESSOR_H
