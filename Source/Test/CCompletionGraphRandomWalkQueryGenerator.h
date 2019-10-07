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

#ifndef KONCLUDE_TEST_CCOMPLETIONGRAPHRANDOMWALKQUERYGENERATOR_H
#define KONCLUDE_TEST_CCOMPLETIONGRAPHRANDOMWALKQUERYGENERATOR_H

// Libraries includes
#include <QString>
#include <QFile>
#include <QDir>

// Namespace includes


// Other includes
#include "Reasoner/Kernel/Task/CSatisfiableCalculationTask.h"

#include "Reasoner/Ontology/CConcreteOntology.h"


// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"



namespace Konclude {

	using namespace Logger;

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Algorithm {

				using namespace Process;
				using namespace Task;

				/*!
					*
					*		\class		CCompletionGraphRandomWalkQueryGenerator
					*		\author		Andreas Steigmiller
					*		\version	0.1
					*		\brief		TODO
					*
					*/
				class CCompletionGraphRandomWalkQueryGenerator : public CLogIdentifier {

					// public methods
				public:
					//! Constructor
					CCompletionGraphRandomWalkQueryGenerator();

					//! Destructor
					virtual ~CCompletionGraphRandomWalkQueryGenerator();



					void generateQueries(CSatisfiableCalculationTask* satTask);



					QList<cint64> getShortestPathBetweenNodes(cint64 nodeId1, cint64 nodeId2, CSatisfiableCalculationTask* satTask);
					QList<cint64> getRandomizedPathBetweenNodes(QList<cint64> pathList, CSatisfiableCalculationTask* satTask);

					bool generateTreeFoldedCycleQuery(CSatisfiableCalculationTask* satTask);
					bool generateRealCycleQuery(CSatisfiableCalculationTask* satTask);


					void generateConceptTerms(CIndividualProcessNode* indiNode, QString indiVar, QSet<QString>& queryParameters, QSet<QString>& queryTermSet, QStringList& queryTerms, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satTask);
					bool generateRoleTerm(CIndividualProcessNode* indiNode1, QString indiVar1, CIndividualProcessNode* indiNode2, QString indiVar2, QSet<QString>& queryParameters, QSet<QString>& queryTermSet, QStringList& queryTerms, QHash<QString, cint64>& varQueryUseCount, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satTask);


					void generateTreeExpansion(CIndividualProcessNode* indiNode, QString indiVar, QSet<QString>& queryParameters, QSet<QString>& queryTermSet, QStringList& queryTerms, QHash<QString, cint64>& statisticsHash, cint64& nextTreeExpansionNr, CSatisfiableCalculationTask* satTask);


					bool writeQuery(const QStringList& queryTerms, const QString& queryTypeString, QList<QString> selectableVariables, const QList<QString>& queryParameters, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satTask);
					bool writeQueryStatistics(const QString& ontologyFileName, const QString& queryFileName, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satCalcTask);


					QString getIndividualNodeQueryVariable(cint64 indiId, cint64 varId, QHash<QPair<cint64, cint64>, QString>& indiVarIdNameHash, QHash<QString, cint64>& statisticsHash, CSatisfiableCalculationTask* satTask);
					QString getIndividualNodeQueryVariable(cint64 indiId, cint64 varId, QHash<QPair<cint64, cint64>, QString>& indiVarIdNameHash, QHash<QString, cint64>& statisticsHash, bool createDistinguishedVariableIfPossible, const QString& varPrefixString, CSatisfiableCalculationTask* satTask);


					CIndividualProcessNode* getUpToDateIndividual(cint64 indiId, CSatisfiableCalculationTask* satTask);
					cint64 getQueryForOntologyCount(CSatisfiableCalculationTask* satTask);

					QHash<QString, cint64> getInitialStatisticsHash(CSatisfiableCalculationTask* satTask);


					// protected methods
				protected:

					// protected variables
				protected:

					cint64 mQueryId = 0;


					cint64 mTreeFoldedCycleQueryGenerationCount = 0;
					cint64 mTreeFoldedCycleQueryGenerationTryCount = 200;
					cint64 mTreeFoldedCycleQueryFailedExtractionCount = 0;

					cint64 mRealCycleQueryGenerationCount = 0;
					cint64 mRealCycleQueryGenerationTryCount = 20000;
					cint64 mRealCycleQueryFailedExtractionCount = 0;


					// private methods
				private:

					// private variables
				private:

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_TEST_CCOMPLETIONGRAPHRANDOMWALKQUERYGENERATOR_H
