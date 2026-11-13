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

#ifndef KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYEXPRESSIONBUILDINGLOADER_H
#define KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYEXPRESSIONBUILDINGLOADER_H

// Libraries includes
#include <QList>

// Namespace includes
#include "JNISettings.h"
#include "CJNIAxiomExpressionVisitingLoader.h"

// Other includes
#include "Reasoner/Generator/CExpressionHasher.h"

#include "Parser/Expressions/CBuildExpression.h"


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Reasoner::Generator;
	using namespace Reasoner::Ontology;
	using namespace Parser::Expression;

	namespace Control {

		namespace Interface {

			namespace JNI {

				/*!
				 *
				 *		\class		CJNIQueryExpressionBuildingLoader
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		Builds the class expressions that the querying bridge is asked
				 *					about, the anonymous ones of the OWL API.
				 *
				 *		The expressions are built into a revision of the installed ontology that is
				 *		never installed, so that nothing is written into the installed ontology. The
				 *		build data of such a revision is a copy-on-write copy of the installed one, so
				 *		an expression that is only asked about is kept in a hash of its own, as
				 *		CConcreteOntologyUpdateSeparateHashingCollectorBuilder does for the OWLlink
				 *		queries, otherwise the first query would detach a copy of the expression hash
				 *		of the whole ontology. The expressions found that way are owned here and
				 *		deleted with the loader, the ontology does not delete the expressions of its
				 *		hashes.
				 *
				 */
				class CJNIQueryExpressionBuildingLoader : public CJNIAxiomExpressionVisitingLoader {

					// public methods
					public:
						//! Constructor
						CJNIQueryExpressionBuildingLoader(CJNIInstanceManager* jniInstanceManager, CJNIOntologyRevisionData* ontRevData, CConcreteOntology* expressionOntology);

						//! Destructor
						virtual ~CJNIQueryExpressionBuildingLoader();

						CConcreteOntology* getExpressionOntology();

					// protected methods
					protected:
						virtual CConcreteOntologyBuildDataUpdater* addHashedExpression(CBuildExpression* expression);
						virtual CBuildExpression* getHashedExpression(CBuildExpression* expression);

					// protected variables
					protected:
						CConcreteOntology* mExpressionOntology;
						CBUILDHASH<CExpressionHasher,CBuildExpression*> mSeparateExpressionBuildHash;
						QList<CBuildExpression*> mOwnedExpressionList;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace JNI

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_JNI_CJNIQUERYEXPRESSIONBUILDINGLOADER_H
