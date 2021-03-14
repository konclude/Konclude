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
#ifdef KONCLUDE_REDLAND_INTEGRATION

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREDLANDTRIPLESDATAQUERYINGEXPRESSIONMAPPER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREDLANDTRIPLESDATAQUERYINGEXPRESSIONMAPPER_H


// Libraries includes
#include <QHash>
#include <QStack>


// Namespace includes
#include "CConcreteOntologyRedlandTriplesDataExpressionMapper.h"


// Other includes
#include "Reasoner/Ontology/CConcreteOntology.h"

#include "Reasoner/Triples/CRedlandStoredTriplesData.h"
#include "Reasoner/Triples/CRedlandNodeHasher.h"

#include "Utilities/Container/CLinker.h"


// Logger includes
#include "Logger/CLogger.h"





namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Memory;
	using namespace Utilities::Container;
	using namespace Parser;

	namespace Reasoner {

		using namespace Ontology;
		using namespace Triples;

		namespace Generator {

			/*! 
			 *
			 *		\class		CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper : public CConcreteOntologyRedlandTriplesDataExpressionMapper {
				// public methods
				public:
					CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper(COntologyBuilder* ontologyBuilder);
					virtual ~CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper();



					QList<CAxiomExpression*> takeQueryingCollectedAxiomExpressions();

					CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper* useEntitiesVariableExpressionMapping(const QHash<QString, CBuildExpression*>& freshEntityUseVariableExpressionHash, CTriplesData* tripleData);

				// protected functions
				protected:
					virtual CConcreteOntologyRedlandTriplesDataExpressionMapper* handleParsedOntologyAxiomExpression(CAxiomExpression* axiomExpression);

					CConcreteOntologyRedlandTriplesDataQueryingExpressionMapper* insertEntityUsingVariableExpression(CBuildExpression* expression, const QString& entityUri, librdf_world* world, QHash<CRedlandNodeHasher, CRedlandNodeProcessingData*>& nodeIdentifierDataHash, QList<CRedlandNodeProcessingData*>& nodeHandlingList);

				// protected variables
				protected:
					QList<CAxiomExpression*> mQueryingCollectedAxiomExpList;


			};


		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYREDLANDTRIPLESDATAQUERYINGEXPRESSIONMAPPER_H

#endif // !KONCLUDE_REDLAND_INTEGRATION
