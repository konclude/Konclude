/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#include "CWriteOWL2XMLOntologyCommand.h"


namespace Konclude {

	namespace Control {

		namespace Command {

			namespace Instructions {

				CWriteOWL2XMLOntologyCommand::CWriteOWL2XMLOntologyCommand(COntologyOWL2QtXMLRenderer* renderer, const QString& fileName, CCommand *parentSuperCommand) 
						: CWriteOntologyCommand(parentSuperCommand) {
					mRenderer = renderer;
					mWriteFileName = fileName;
				}


				CWriteOWL2XMLOntologyCommand::~CWriteOWL2XMLOntologyCommand() {
					delete mRenderer;
				}

				QString CWriteOWL2XMLOntologyCommand::getBriefCommandDescription() {
					return QString("Write-OWL-2-XML-Ontology Command");
				}

				qint64 CWriteOWL2XMLOntologyCommand::getCommandTag() {
					return WRITEOWL2XMLONTOLOGYCOMMAND;
				}

				COntologyRenderer* CWriteOWL2XMLOntologyCommand::getRenderer() {
					return mRenderer;
				}

				QString CWriteOWL2XMLOntologyCommand::getWriteFileName() {
					return mWriteFileName;
				}

				CCommand* CWriteOWL2XMLOntologyCommand::getCommand() {
					return this;
				}

				COntologyOWL2QtXMLRenderer* CWriteOWL2XMLOntologyCommand::getOWL2XMLRenderer() {
					return mRenderer;
				}

			}; // end namespace Instructions

		}; // end namespace Command

	}; // end namespace Control

}; // end namespace Konclude
