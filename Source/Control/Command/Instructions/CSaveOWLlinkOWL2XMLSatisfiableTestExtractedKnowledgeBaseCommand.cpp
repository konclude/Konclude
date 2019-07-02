/*
 *		Copyright (C) 2013, 2014, 2015 by the Konclude Developer Team.
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

#include "CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand(const QString &knowledgeBaseNameString, const QString& loadIRI, const QStringList& extractEntityNames, const QString& testKBName, CCommand *parentSuperCommand) 
						: CSaveKnowledgeBaseCommand(knowledgeBaseNameString,parentSuperCommand) {
					mEntityNames = extractEntityNames;
					mSaveIRI = loadIRI;
					mTestKBName = testKBName;
				}


				CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::~CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand() {
				}


				qint64 CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::getCommandTag() {
					return SAVEOWLLINKOWL2XMLSATTESTKBCOMMAND;
				}




				QString CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::getBriefCommandDescription() {
					return QString("Save-'%1'-Extracted-Knowledge-Base-OWLlink-OWL-2-XML-Satisfiable-Test Command").arg(kbString);
				}


				QString CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::getSaveIRIString() {
					return mSaveIRI;
				}


				QStringList CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::getExtractEntitiyList() {
					return mEntityNames;
				}


				QString CSaveOWLlinkOWL2XMLSatisfiableTestExtractedKnowledgeBaseCommand::getTestKBName() {
					return mTestKBName;
				}


			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
