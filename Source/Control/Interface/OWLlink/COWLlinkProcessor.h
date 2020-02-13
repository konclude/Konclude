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

#ifndef KONCLUDE_CONTROL_INTERFACE_OWLLINK_COWLLINKPROCESSOR_H
#define KONCLUDE_CONTROL_INTERFACE_OWLLINK_COWLLINKPROCESSOR_H

// Libraries includes
#include <QString>
#include <QRegExp>
#include <QTime>
#include <QTemporaryFile>
#include <QBuffer>

// Namespace includes


// Other includes
#include "Reasoner/Preprocess/CLexicalNormalisationPreProcess.h"
#include "Reasoner/Preprocess/CNegationTransformationPreProcess.h"
#include "Reasoner/Preprocess/CSubroleTransformationPreProcess.h"
#include "Reasoner/Preprocess/COntologyAssignmentTransformationPreProcess.h"
#include "Reasoner/Preprocess/COntologyConfigDependedPreProcesser.h"


#include "Concurrent/CThread.h"

#include "Utilities/CSingletonProvider.hpp"

#include "Control/Command/CCommanderManager.h"
#include "Control/Command/CCommanderManagerConfigType.h"
#include "Control/Command/CCommandDelegater.h"
#include "Control/Command/CPreconditionSynchronizer.h"
#include "Control/Command/CConfigManagerReader.h"

#include "Control/Command/Records/CUnspecifiedMessageErrorRecord.h"

#include "Control/Command/Events/CRealizeCommandEvent.h"
#include "Control/Command/Events/CCommandProcessedCallbackEvent.h"
#include "Control/Command/Events/CInitializeEvent.h"

#include "Control/Command/Instructions/CInitializeConfigurationCommand.h"
#include "Control/Command/Instructions/CInitializeReasonerCommand.h"

#include "Control/Command/Instructions/CCreateKnowledgeBaseRevisionUpdateCommand.h"
#include "Control/Command/Instructions/CParseOWL2XMLOntologyCommand.h"
#include "Control/Command/Instructions/CSAXParseOWL2XMLOntologyCommand.h"
#include "Control/Command/Instructions/CStreamParseOWL2XMLOntologyCommand.h"
#include "Control/Command/Instructions/CInstallKnowledgeBaseRevisionUpdateCommand.h"
#include "Control/Command/Instructions/CIsSatisfiableQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CParseOWL2XMLQueryCommand.h"
#include "Control/Command/Instructions/CCalculateQueryCommand.h"
#include "Control/Command/Instructions/CGetCurrentKnowledgeBaseRevisionCommand.h"
#include "Control/Command/Instructions/CParseTestsuiteCommandsCommand.h"
#include "Control/Command/Instructions/CParseTestsuiteXMLNodeCommand.h"
#include "Control/Command/Instructions/CTestTestsuiteCommand.h"
#include "Control/Command/Instructions/CInsertResponseCommentCommand.h"
#include "Control/Command/Instructions/CConstructSubClassHirarchyQueryCommand.h"
#include "Control/Command/Instructions/CGetSubClassHierarchyQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CReadTextCommand.h"
#include "Control/Command/Instructions/CReadXMLCommand.h"
#include "Control/Command/Instructions/CParseKRSSTestConceptOntologyCommand.h"
#include "Control/Command/Instructions/CWriteOWL2XMLOntologyCommand.h"
#include "Control/Command/Instructions/CExtractToldExpressionsOntologyCommand.h"
#include "Control/Command/Instructions/CWriteOWLlinkOWL2XMLSatisfiableTestCommand.h"
#include "Control/Command/Instructions/CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseOWLXMLOntologyCommand.h"
#include "Control/Command/Instructions/CChooseParseInstallKnowledgeBaseOWLXMLOntologyCommand.h"
#include "Control/Command/Instructions/CPreprocessKnowledgeBaseRequirementsForQueryCommand.h"
#include "Control/Command/Instructions/CGenerateQueryCommand.h"
#include "Control/Command/Instructions/CPrepareKnowledgeBaseForRevisionCommand.h"
#include "Control/Command/Instructions/CConstructFlattenedTypesQueryCommand.h"
#include "Control/Command/Instructions/CGetFlattenedTypesOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CProcessQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CEvaluateTestsuiteCommand.h"
#include "Control/Command/Instructions/CConstructWriteFunctionalSubClassHirarchyQueryCommand.h"
#include "Control/Command/Instructions/CWriteFunctionalSubClassHierarchyQueryCommand.h"
#include "Control/Command/Instructions/CConstructIsConsistentQueryCommand.h"
#include "Control/Command/Instructions/CIsConsistentQueryOWL2XMLNodeCommand.h"
#include "Control/Command/Instructions/CConstructClassSatisfiableQueryCommand.h"
#include "Control/Command/Instructions/CProcessClassNameSatisfiableQueryCommand.h"
#include "Control/Command/Instructions/CConstructClassifyQueryCommand.h"
#include "Control/Command/Instructions/CClassifyQueryCommand.h"
#include "Control/Command/Instructions/CConstructRealizeQueryCommand.h"
#include "Control/Command/Instructions/CRealizeQueryCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseEntitiesCommand.h"
#include "Control/Command/Instructions/CExtractKnowledgeBaseEntitiesCommand.h"
#include "Control/Command/Instructions/CGetKnowledgeBaseLanguageCommand.h"
#include "Control/Command/Instructions/CExtractKnowledgeBaseLanguageCommand.h"
#include "Control/Command/Instructions/CWriteXMLSubClassHierarchyQueryCommand.h"
#include "Control/Command/Instructions/CConstructWriteXMLSubClassHirarchyQueryCommand.h"
#include "Control/Command/Instructions/CWriteXMLIndividualTypesQueryCommand.h"
#include "Control/Command/Instructions/CConstructWriteXMLIndividualTypesQueryCommand.h"
#include "Control/Command/Instructions/CStreamParseOWL2FunctionalOntologyCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseOWLFunctionalOntologyCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseOWLAutoOntologyCommand.h"
#include "Control/Command/Instructions/CChooseParseInstallKnowledgeBaseOWLAutoOntologyCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseStartDownloadCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseFinishDownloadCommand.h"
#include "Control/Command/Instructions/CLoadKnowledgeBaseAllDownloadedContinuer.h"
#include "Control/Command/Instructions/CImportKnowledgeBaseOWLAutoOntologyCommand.h"
#include "Control/Command/Instructions/CWriteFunctionalIndividualTypesQueryCommand.h"
#include "Control/Command/Instructions/CConstructWriteFunctionalIndividualTypesQueryCommand.h"
#include "Control/Command/Instructions/CIsTriviallyConsistentQueryCommand.h"
#include "Control/Command/Instructions/CConstructIsTriviallyConsistentQueryCommand.h"
#include "Control/Command/Instructions/CConstructWriteCustomQueryCommand.h"
#include "Control/Command/Instructions/CWriteCustomQueryCommand.h"
#include "Control/Command/Instructions/CPrepareKnowledgeBaseForQueryCommand.h"
#include "Control/Command/Instructions/CParseProcessSPARQLTextCommand.h"
#include "Control/Command/Instructions/CParseSPARQLQueryCommand.h"
#include "Control/Command/Instructions/CCalculateQueriesCommand.h"
#include "Control/Command/Instructions/CParseProcessSPARQLUpdateTextCommand.h"
#include "Control/Command/Instructions/CParseProcessSPARQLManageTextCommand.h"
#include "Control/Command/Instructions/CGetQueryDependentKnowledgeBaseRevisionUpdatesCommand.h"
#include "Control/Command/Instructions/CInstallQueryDependentKnowledgeBaseRevisionUpdatesCommand.h"
#include "Control/Command/Instructions/CConnectKnowledgeBaseExternalTriplesDSNCommand.h"
#include "Control/Command/Instructions/CParseInstallKnowledgeBaseExternalTriplesDSNConnectionCommand.h"


#include "Reasoner/Query/CWriteOREFunctionalClassSubsumptionsHierarchyQuery.h"
#include "Reasoner/Query/CWriteSerializerClassSubsumptionsHierarchyQuery.h"
#include "Reasoner/Query/CWriteSerializerPropertySubsumptionsHierarchyQuery.h"
#include "Reasoner/Query/CWriteSerializerIndividualFlattenedTypesQuery.h"
#include "Reasoner/Query/CIsConsistentQuery.h"
#include "Reasoner/Query/CIsTriviallyConsistentQuery.h"
#include "Reasoner/Query/CRealizeQuery.h"
#include "Reasoner/Query/CClassifyQuery.h"

#include "Parser/COWLlinkQtXMLCommandParser.h"
#include "Parser/COWL2QtXMLOntologyParser.h"
#include "Parser/COWLlinkQueryParser.h"
#include "Parser/CXMLTestsuiteCommandParser.h"
#include "Parser/COWLlinkQueryExtensionParser.h"
#include "Parser/COWLlinkQtXMLComplexQueryParser.h"
#include "Parser/COWLlinkQtXMLSimpleQueryParser.h"
#include "Parser/CKRSSOntologyParser.h"
#include "Parser/COWL2QtXMLOntologySAXParser.h"
#include "Parser/COWL2QtXMLOntologyStreamParser.h"
#include "Parser/COWL2QtXMLOntologyStableStreamParser.h"
#include "Parser/CSPARQLSimpleQueryParser.h"
#include "Parser/CSPARQLKnowledgeBaseSplittingOperationParser.h"
#include "Parser/CSPARQLSimpleUpdateParser.h"
#include "Parser/CSPARQLSimpleManagementParser.h"
#include "Parser/COWL2RDFTurtleAssertionsSimpleParser.h"
#include "Parser/CRDFRedlandRaptorParser.h"
#include "Parser/CSPARQLRedlandRasqalQueryParser.h"
#include "Parser/CRDFRedlandExternalTriplesDSNConnectingParser.h"

#include "Parser/FunctionalJAVACC/COWL2FunctionalJAVACCOntologyStreamParser.h"

#include "Renderer/CRenderWriteManager.h"
#include "Renderer/COntologyExtractionManager.h"

#include "Network/HTTP/CQtHttpTransactionManager.h"



#include "Reasoner/Generator/CConcreteOntologyUpdateBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyQueryExtendedBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyQuerySimpleBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyUpdateSeparateHashingCollectorBuilder.h"

#ifdef KONCLUDE_REDLAND_INTEGRATION
#include "Reasoner/Generator/CConcreteOntologyRedlandTriplesDataExpressionMapper.h"
#endif // !KONCLUDE_REDLAND_INTEGRATION

#include "Test/CReasonerTestsuiteTester.h"

#include "Test/Evaluation/CReasonerEvaluationManagerThread.h"

#include "Config/CConfiguration.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"


namespace Konclude {

	using namespace Logger;
	using namespace Concurrent;
	using namespace Utilities;
	using namespace Parser;
	using namespace FunctionalJAVACC;
	using namespace Reasoner;
	using namespace Generator;
	using namespace Preprocess;
	using namespace Config;
	using namespace Renderer;
	using namespace Query;
	using namespace Network;
	using namespace HTTP;
	//using namespace Test::Evaluation;

	namespace Control {

		using namespace Command;
		using namespace Instructions;
		using namespace Events;
		using namespace Records;

		namespace Interface {

			namespace OWLlink {

				/*! 
				 *
				 *		\class		COWLlinkProcessor
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class COWLlinkProcessor : public CThread, public CCommandDelegater, public CLogIdentifier {
					// public methods
					public:
						//! Constructor
						COWLlinkProcessor(bool immediatelyStartThread = true);

						//! Destructor
						virtual ~COWLlinkProcessor();

						virtual COWLlinkProcessor *startProcessing();

						virtual CCommandDelegater *delegateCommand(CCommand *command);



					// protected methods
					protected:
						bool processCustomsEvents(QEvent::Type type, CCustomEvent *event);

						virtual COWLlinkProcessor *initializeOWLlinkContent() = 0;
						virtual COWLlinkProcessor *concludeOWLlinkContent() = 0;

						virtual CConfiguration *getConfiguration() = 0;

						QStringList getParserOrderFromFileName(const QString& fileName);

						CQtHttpTransactionManager* getNetworkTransactionManager();
						
						bool createDownloadParseCommands(const QString& knowledgeBaseNameString, CCommand* parentCommand, CKnowledgeBaseRevisionCommandProvider* kbProviderCommand, const QMap<QString,QString>& ontoIRIMapping, const QStringList& ontoIRIStringList, CCommandRecordRouter& commandRecordRouter);

						bool parseOntology(QIODevice* device, const QString& ontoIRI, const QString& resolvedIRI, CConcreteOntology *ont, COntologyConfigurationExtension *ontConfig, QList<QString>& importOntologiesList, QStringList& parserErrorList, CCommandRecordRouter& commandRecordRouter);

#ifdef KONCLUDE_REDLAND_INTEGRATION
						bool parseOntologyWithRaptor(QIODevice* device, CConcreteOntologyUpdateCollectorBuilder *builder, const QString& format, const QString& formatName, const QString& resolvedIRI, QString& parsingTryLogString, QStringList& parserErrorList, CConfiguration* configuration, CCommandRecordRouter& commandRecordRouter);
#endif // !KONCLUDE_REDLAND_INTEGRATION


					// protected variables
					protected:
						bool mConfLoadOntologiesSAXParsing;
						bool mConfLoadOntologiesStreamParsing;
						CPreconditionSynchronizer *preSynchronizer;

						QByteArray fileContent;

						CCommanderManager *reasonerCommander;

						CCommandDelegater *defaultCommandDelegater;

						bool mConfLogProcessingTimes;

						CQtHttpTransactionManager* mNetworkManager;


					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace OWLlink 

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_OWLLINK_COWLLINKPROCESSOR_H
