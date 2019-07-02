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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDDATA_H
#define KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDDATA_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"
#include "CConcept.h"
#include "CRole.h"
#include "CIndividual.h"
#include "COntologyImportData.h"
#include "COntologyBuildExpressionCounter.h"


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
#include "Parser/Expressions/CDataPropertyTermExpression.h"
#include "Parser/Expressions/CDataRangeTermExpression.h"
#include "Parser/Expressions/CDatatypeExpression.h"
#include "Parser/Expressions/CDataLexicalValueExpression.h"
#include "Parser/Expressions/CDataPropertyExpression.h"
#include "Parser/Expressions/CDataFacetExpression.h"

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

					CDataRangeTermExpression* getTopDataRangeExpression();
					CDataRangeTermExpression* getBottomDataRangeExpression();


					CBUILDSET<CDeclarationAxiomExpression*>* getDeclarationAxiomSet();


					CObjectPropertyTermExpression* getTopObjectPropertyExpression();
					CObjectPropertyTermExpression* getBottomObjectPropertyExpression();

					CDataPropertyTermExpression* getTopDataPropertyExpression();
					CDataPropertyTermExpression* getBottomDataPropertyExpression();

					COntologyBuildData* setTopClassExpression(CClassTermExpression* topClassExp);
					COntologyBuildData* setBottomClassExpression(CClassTermExpression* bottomClassExp);

					COntologyBuildData* setTopDataRangeExpression(CDataRangeTermExpression* dataRangeExp);
					COntologyBuildData* setBottomDataRangeExpression(CDataRangeTermExpression* dataRangeExp);

					COntologyBuildData* setTopObjectPropertyExpression(CObjectPropertyTermExpression* topObjectPropertyExp);
					COntologyBuildData* setBottomObjectPropertyExpression(CObjectPropertyTermExpression* bottomObjectPropertyExp);

					COntologyBuildData* setTopDataPropertyExpression(CDataPropertyTermExpression* dataPropertyExp);
					COntologyBuildData* setBottomDataPropertyExpression(CDataPropertyTermExpression* dataPropertyExp);


					CBUILDHASH<CExpressionHasher,CBuildExpression*>* getStructuralExpressionBuildHash();
					CBUILDLIST<CBuildExpression*>* getExpressionBuildListContainer();


					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* getClassEntityBuildHash();
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* getObjectPropertyEntityBuildHash();
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* getIndividualEntityBuildHash();
					CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>* getAnonymousIndividualBuildHash();
					CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>* getIndividualVariableBuildHash();
					CBUILDHASH<CStringRefStringHasher,CDatatypeExpression*>* getDatatypeIRIBuildHash();
					CBUILDHASH<CStringRefStringHasher,CDataFacetExpression*>* getFacetIRIBuildHash();
					CBUILDHASH<CStringRefStringHasher,CDataPropertyExpression*>* getDataPropertyEntityBuildHash();
					CBUILDHASH<CStringRefStringHasher,CDataLexicalValueExpression*>* getDataLexicalValueBuildHash();



					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* getInverseObjectPropertyHash();
					CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >* getInverseObjectPropertyList();


					CBUILDHASH<CObjectIndividualVariableExpression*,cint64>* getIndividualVariableIDHash();


					CBUILDSET<CClassTermExpression*>* getBuildConceptSet();
					CBUILDSET<CObjectPropertyTermExpression*>* getBuildObjectRoleSet();
					CBUILDSET<CIndividualTermExpression*>* getBuildIndividualSet();
					CBUILDSET<CDataPropertyTermExpression*>* getBuildDataRoleSet();
					CBUILDSET<CDataRangeTermExpression*>* getBuildDataRangeSet();
					CBUILDSET<CDatatypeExpression*>* getBuildDatatypeSet();


					CBUILDLIST<CClassTermExpression*>* getBuildConceptList();
					CBUILDLIST<CObjectPropertyTermExpression*>* getBuildObjectRoleList();
					CBUILDLIST<CDataPropertyTermExpression*>* getBuildDataRoleList();
					CBUILDLIST<CIndividualTermExpression*>* getBuildIndividualList();
					CBUILDLIST<CDataRangeTermExpression*>* getBuildDataRangeList();
					CBUILDLIST<CDatatypeExpression*>* getBuildDatatypeList();

					cint64 getNextAxiomNumber(bool moveNext = false);	
					COntologyBuildData* setNextAxiomNumber(cint64 axiomNumber);

					cint64 getNextEntityNumber(bool moveNext = false);	
					COntologyBuildData* setNextEntityNumber(cint64 entityNumber);



					CBUILDHASH<CStringRefStringHasher,COntologyImportData*>* getImportDataHash();

					COntologyBuildExpressionCounter* getBuildExpressionCounter();


				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;


					COntologyBuildExpressionCounter mExpressionCounter;

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

					CDataRangeTermExpression* mTopDataRangeExpression;
					CDataRangeTermExpression* mBottomDataRangeExpression;

					CDataPropertyTermExpression* mTopDataPropExpression;
					CDataPropertyTermExpression* mBottomDataPropExpression;

					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mExpressionBuildHash;

					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* mClassBuildHash;
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* mObjectPropertyBuildHash;
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* mIndividualBuildHash;
					CBUILDHASH<QPair<CStringRefStringHasher,CStringRefStringHasher>,CAnonymousIndividualExpression*>* mAnoIndividualBuildHash;
					CBUILDHASH<QPair<CStringRefStringHasher,cint64>,CObjectIndividualVariableExpression*>* mIndividualVariableBuildHash;
					CBUILDHASH<CStringRefStringHasher,CDatatypeExpression*>* mDatatypeIRIDatatypeBuildHash;
					CBUILDHASH<CStringRefStringHasher,CDataPropertyExpression*>* mDataPropertyBuildHash;
					CBUILDHASH<CStringRefStringHasher,CDataLexicalValueExpression*>* mDataLexicalValueBuildHash;
					CBUILDHASH<CStringRefStringHasher,CDataFacetExpression*>* mFacetIRIFacetBuildHash;

					
					
					CBUILDHASH<CStringRefStringHasher,COntologyImportData*>* mImportDataHash;

					// mapping hashes and container
					CBUILDLIST<CBuildExpression*>* mExpressionBuildListContainer;


					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mInverseObjectPropertyHash;
					CBUILDLIST< QPair<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*> >* mInverseObjectPropertyList;


					CBUILDSET<CClassTermExpression*>* mBuildConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mBuildObjectRoleSet;
					CBUILDSET<CDataPropertyTermExpression*>* mBuildDataRoleSet;
					CBUILDSET<CIndividualTermExpression*>* mBuildIndividualSet;
					CBUILDSET<CDatatypeExpression*>* mBuildDatatypeSet;
					CBUILDSET<CDataRangeTermExpression*>* mBuildDataRangesSet;

					CBUILDLIST<CIndividualTermExpression*>* mBuildIndividualList;
					CBUILDLIST<CObjectPropertyTermExpression*>* mBuildObjectRoleList;
					CBUILDLIST<CDataPropertyTermExpression*>* mBuildDataRoleList;
					CBUILDLIST<CClassTermExpression*>* mBuildConceptList;
					CBUILDLIST<CDatatypeExpression*>* mBuildDatatypeList;
					CBUILDLIST<CDataRangeTermExpression*>* mBuildDataRangesList;

				// private methods
				private:

				// private variables
				private:

			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CONTOLOGYBUILDDATA_H
