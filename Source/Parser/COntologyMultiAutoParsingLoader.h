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

#ifndef KONCLUDE_PARSER_CONTOLOGYMULTIAUTOPARSINGLOADER_H
#define KONCLUDE_PARSER_CONTOLOGYMULTIAUTOPARSINGLOADER_H

// Libraries includes


// Namespace includes
#include "COntologyParser.h"
#include "CRDFRedlandRaptorParser.h"
#include "CRDFRedlandRaptorSimpleConcurrentParser.h"
#include "COWL2QtXMLOntologyStableStreamParser.h"
#include "COWL2QtXMLOntologyStreamParser.h"

// Other includes
#include "Parser/FunctionalJAVACC/COWL2FunctionalJAVACCOntologyStreamParser.h"


#include "Config/CConfigDataReader.h"

#include "Control/Command/CCommandRecordRouter.h"
#include "Control/Command/Records/CUnspecifiedMessageErrorRecord.h"

#include "Reasoner/Generator/CConcreteOntologyUpdateCollectorBuilder.h"
#include "Reasoner/Generator/CConcreteOntologyRedlandTriplesDataExpressionMapper.h"

#include "Reasoner/Revision/COntologyConfigurationExtension.h"

#include "Reasoner/Ontology/CConcreteOntology.h"

// Logger includes
#include "Logger/CLogger.h"
#include "Logger/CLogIdentifier.h"

using namespace Konclude::Logger;

namespace Konclude {

	using namespace Config;
	using namespace Control::Command;
	using namespace Reasoner::Generator;
	using namespace Reasoner::Revision;
	using namespace Reasoner::Ontology;

	namespace Parser {

		using namespace FunctionalJAVACC;



		/*! 
		*
		*		\class		COntologyMultiAutoParsingLoader
		*		\author		Andreas Steigmiller
		*		\version	0.1
		*		\brief		TODO
		*
		*/
		class COntologyMultiAutoParsingLoader : public COntologyParser, public CLogIdentifier {
			// public methods
			public:
				//! Constructor
				COntologyMultiAutoParsingLoader(CConcreteOntology *ont = nullptr, COntologyConfigurationExtension *ontConfig = nullptr);

				//! Destructor
				virtual ~COntologyMultiAutoParsingLoader();


				virtual bool parseOntologyFile(const QString& filename);
				virtual bool parseOntologyFile(QIODevice* device);

				bool parseOntology(QIODevice* device, const QString& ontoIRI, const QString& resolvedIRI, CConcreteOntology *ont, COntologyConfigurationExtension *ontConfig, QList<QString>& importOntologiesList, QStringList& parserErrorList, CCommandRecordRouter& commandRecordRouter);
				bool parseOntology(QIODevice* device, const QString& ontoIRI, const QString& resolvedIRI, CConcreteOntology *ont, CConcreteOntologyUpdateCollectorBuilder* builder, COntologyConfigurationExtension *ontConfig, QList<QString>& importOntologiesList, QStringList& parserErrorList, CCommandRecordRouter& commandRecordRouter);


				QStringList getParserOrderFromFileName(const QString& fileName);


			// protected methods
			protected:

#ifdef KONCLUDE_REDLAND_INTEGRATION
				bool parseOntologyWithRaptor(QIODevice* device, CConcreteOntologyUpdateCollectorBuilder *builder, const QString& format, const QString& formatName, const QString& resolvedIRI, QString& parsingTryLogString, QStringList& parserErrorList, CConfiguration* configuration, CCommandRecordRouter& commandRecordRouter);
#endif // !KONCLUDE_REDLAND_INTEGRATION


			// protected variables
			protected:

				CConcreteOntology* mOnt;
				COntologyConfigurationExtension* mOntConfig;

			// private methods
			private:

			// private variables
			private:

		};

	}; // end namespace Parser

}; // end namespace Konclude

#endif // KONCLUDE_PARSER_CONTOLOGYMULTIAUTOPARSINGLOADER_H
