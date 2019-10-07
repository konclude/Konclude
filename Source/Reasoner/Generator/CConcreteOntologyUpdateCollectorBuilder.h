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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYUPDATECOLLECTORBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYUPDATECOLLECTORBUILDER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>

// Namespace includes
#include "CConcreteOntologyBuildDataUpdater.h"
#include "CExpressionSplitter.h"
#include "CExpressionHasher.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"
#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CTBox.h"
#include "Reasoner/Ontology/CABox.h"
#include "Reasoner/Ontology/CRBox.h"
#include "Reasoner/Ontology/CAbbreviatedIRIName.h"
#include "Reasoner/Ontology/CIRIName.h"


#include "Utilities/CTrible.hpp"
#include "Utilities/CNegLinker.hpp"
#include "Utilities/CHashableIntConvertedNTuble.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/Memory/CObjectAllocator.h"

#include "Parser/COntologyBuilder.h"

// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Parser;

	namespace Reasoner {

		using namespace Ontology;	

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyUpdateCollectorBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyUpdateCollectorBuilder : public CConcreteOntologyBuildDataUpdater {
				// public methods
				public:
					CConcreteOntologyUpdateCollectorBuilder(CConcreteOntology* updateConcreteOntology);
					virtual ~CConcreteOntologyUpdateCollectorBuilder();

					virtual bool initializeBuilding();
					virtual bool completeBuilding();

					virtual bool tellOntologyAxiom(CAxiomExpression* axiom);
					virtual bool retractOntologyAxiom(CAxiomExpression* axiom);

					virtual bool addOntologyImport(const QStringRef& name);
					virtual bool addOntologyImport(const QString& name);


					QList<QString> takeAddedImportOntologyList();


				// protected functions
				protected:

					// protected variables
				protected:

					// contains also previous ontology data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeAxiomList;

					// contains only updated data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellUpdatedAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractUpdatedAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeUpdatedAxiomList;




					QSet<CAxiomExpression*> mLocTellUpdatedAxiomSet;
					QSet<CAxiomExpression*> mLocRetractUpdatedAxiomSet;

					QList<QString> mLocAddedImportOntologies;

			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYUPDATECOLLECTORBUILDER_H
