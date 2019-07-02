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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"
#include "CConcept.h"
#include "CRole.h"
#include "CIndividual.h"


// Other includes
#include "Parser/Expressions/CAxiomExpression.h"
#include "Parser/Expressions/CClassAxiomExpression.h"
#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CClassExpression.h"
#include "Parser/Expressions/CObjectPropertyExpression.h"
#include "Parser/Expressions/CObjectPropertyExpression.h"
#include "Parser/Expressions/CIndividualTermExpression.h"
#include "Parser/Expressions/CNamedIndividualExpression.h"
#include "Parser/Expressions/CAnonymousIndividualExpression.h"
#include "Parser/Expressions/CObjectPropertyAxiomExpression.h"
#include "Parser/Expressions/CDeclarationAxiomExpression.h"
#include "Parser/Expressions/CAssertionAxiomExpression.h"
#include "Parser/Expressions/CObjectIndividualVariableExpression.h"

#include "Reasoner/Generator/CExpressionHasher.h"

#include "Reasoner/Generator/CStringRefStringHasher.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Generator;

		namespace Ontology {

			/*! 
			 *
			 *		\class		COntologyBuildData
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class COntologyBuildData {
				// public methods
				public:
					//! Constructor
					COntologyBuildData(COntologyContext* ontoContext = nullptr);

					//! Destructor
					virtual ~COntologyBuildData();

					COntologyBuildData* referenceBuildData(COntologyBuildData* buildData);


					CONTOLOGYAXIOMSET<CAxiomExpression*>* getTellAxiomSet();
					CONTOLOGYAXIOMSET<CAxiomExpression*>* getRetractAxiomSet();

					CONTOLOGYAXIOMSET<CAxiomExpression*>* getUpdatedTellAxiomSet();
					CONTOLOGYAXIOMSET<CAxiomExpression*>* getUpdatedRetractAxiomSet();


					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* getChangeAxiomList();
					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* getUpdatedChangeAxiomList();


					CClassTermExpression* getTopClassExpression();
					CClassTermExpression* getBottomClassExpression();


					CBUILDSET<CDeclarationAxiomExpression*>* getDeclarationAxiomSet();


					CObjectPropertyTermExpression* getTopObjectPropertyExpression();
					CObjectPropertyTermExpression* getBottomObjectPropertyExpression();


					COntologyBuildData* setTopClassExpression(CClassTermExpression* topClassExp);
					COntologyBuildData* setBottomClassExpression(CClassTermExpression* bottomClassExp);


					COntologyBuildData* setTopObjectPropertyExpression(CObjectPropertyTermExpression* topObjectPropertyExp);
					COntologyBuildData* setBottomObjectPropertyExpression(CObjectPropertyTermExpression* bottomObjectPropertyExp);


					CBUILDHASH<CExpressionHasher,CBuildExpression*>* getStructuralExpressionBuildHash();
					CBUILDLIST<CBuildExpression*>* getExpressionBuildListContainer();


					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* getClassEntityBuildHash();
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* getObjectPropertyEntityBuildHash();
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* getIndividualEntityBuildHash();
					CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>* getAnonymousIndividualBuildHash();
					CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>* getIndividualVariableBuildHash();



					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* getInverseObjectPropertyHash();
					CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >* getInverseObjectPropertyList();


					CBUILDHASH<CObjectIndividualVariableExpression*,cint64>* getIndividualVariableIDHash();


					CBUILDSET<CClassTermExpression*>* getBuildConceptSet();
					CBUILDSET<CObjectPropertyTermExpression*>* getBuildRoleSet();
					CBUILDSET<CIndividualTermExpression*>* getBuildIndividualSet();


					CBUILDLIST<CClassTermExpression*>* getBuildConceptList();
					CBUILDLIST<CObjectPropertyTermExpression*>* getBuildRoleList();
					CBUILDLIST<CIndividualTermExpression*>* getBuildIndividualList();

					cint64 getNextAxiomNumber(bool moveNext = false);	
					COntologyBuildData* setNextAxiomNumber(cint64 axiomNumber);

					cint64 getNextEntityNumber(bool moveNext = false);	
					COntologyBuildData* setNextEntityNumber(cint64 entityNumber);

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;

					// contains also previous ontology data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeAxiomList;


					// contains only updated data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellUpdatedAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractUpdatedAxiomSet;


					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeUpdatedAxiomList;

					cint64 mAxiomNumber;				
					cint64 mEntityNumber;				


					CBUILDSET<CDeclarationAxiomExpression*>* mDeclarationAxiomSet;

					CBUILDHASH<CObjectIndividualVariableExpression*,cint64>* mIndividualVariableIDHash;


					// contains also previous ontology data,
					// exact build expression identification
					CClassTermExpression* mTopClassExpression;
					CClassTermExpression* mBottomClassExpression;

					CObjectPropertyTermExpression* mTopObjPropExpression;
					CObjectPropertyTermExpression* mBottomObjPropExpression;


					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mExpressionBuildHash;

					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* mClassBuildHash;
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* mObjectPropertyBuildHash;
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* mIndividualBuildHash;
					CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>* mAnoIndividualBuildHash;
					CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>* mIndividualVariableBuildHash;

					// mapping hashes and container
					CBUILDLIST<CBuildExpression*>* mExpressionBuildListContainer;


					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mInverseObjectPropertyHash;
					CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >* mInverseObjectPropertyList;


					CBUILDSET<CClassTermExpression*>* mBuildConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mBuildRoleSet;
					CBUILDSET<CIndividualTermExpression*>* mBuildIndividualSet;

					CBUILDLIST<CIndividualTermExpression*>* mBuildIndividualList;
					CBUILDLIST<CObjectPropertyTermExpression*>* mBuildRoleList;
					CBUILDLIST<CClassTermExpression*>* mBuildConceptList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDDATA_H
