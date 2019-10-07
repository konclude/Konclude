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

#ifndef KONCLUDE_REASONER_ONTOLOGY_CEXPRESSIONDATABOXMAPPING_H
#define KONCLUDE_REASONER_ONTOLOGY_CEXPRESSIONDATABOXMAPPING_H

// Libraries includes


// Namespace includes
#include "OntologySettings.h"
#include "COntologyContext.h"
#include "CConcept.h"
#include "CIndividual.h"
#include "CRole.h"
#include "CDatatype.h"
#include "COntologyBuildConstructFlags.h"
#include "CActiveEntityCountVector.h"


// Other includes
#include "Utilities/CSortedNegLinker.hpp"
#include "Utilities/CMemoryManager.hpp"
#include "Utilities/CLinker.hpp"


#include "Parser/Expressions/CClassTermExpression.h"
#include "Parser/Expressions/CIndividualTermExpression.h"
#include "Parser/Expressions/CObjectPropertyTermExpression.h"
#include "Parser/Expressions/CClassAxiomExpression.h"
#include "Parser/Expressions/CObjectPropertyAxiomExpression.h"
#include "Parser/Expressions/CAssertionAxiomExpression.h"
#include "Parser/Expressions/CDataPropertyTermExpression.h"
#include "Parser/Expressions/CDataRangeTermExpression.h"
#include "Parser/Expressions/CDatatypeExpression.h"
#include "Parser/Expressions/CDataPropertyAxiomExpression.h"
#include "Parser/Expressions/CDataLexicalValueExpression.h"

#include "Reasoner/Generator/CExpressionHasher.h"

#include "Utilities/Container/CLinker.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	using namespace Utilities;
	using namespace Utilities::Container;

	using namespace Parser::Expression;

	namespace Reasoner {

		using namespace Generator;

		namespace Ontology {

			/*! 
			 *
			 *		\class		CExpressionDataBoxMapping
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CExpressionDataBoxMapping {
				// public methods
				public:
					//! Constructor
					CExpressionDataBoxMapping(COntologyContext* ontoContext);
					virtual ~CExpressionDataBoxMapping();

					CExpressionDataBoxMapping* referenceDataBoxMapping(CExpressionDataBoxMapping* dataBoxMapping);

					COntologyBuildConstructFlags* getBuildConstructFlags();

					CActiveEntityCountVector* getActiveEntityVector();

					CBUILDHASH<CClassTermExpression*,CConcept*>* getClassTermConceptMappingHash();
					CBUILDHASH<CConcept*,CClassTermExpression*>* getConceptClassTermMappingHash();

					CBUILDHASH<CDataRangeTermExpression*,CConcept*>* getDataRangeTermConceptMappingHash();
					CBUILDHASH<CConcept*,CDataRangeTermExpression*>* getConceptDataRangeTermMappingHash();

					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* getObjectPropertyTermRoleMappingHash();
					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* getRoleObjectPropertyTermMappingHash();

					CBUILDHASH<CDataPropertyTermExpression*,CRole*>* getDataPropertyTermRoleMappingHash();
					CBUILDHASH<CRole*,CDataPropertyTermExpression*>* getRoleDataPropertyTermMappingHash();

					CBUILDHASH<CIndividualTermExpression*,CIndividual*>* getIndividulTermIndiMappingHash();
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* getIndiIndividulTermMappingHash();


					CBUILDHASH<CDatatypeExpression*,CDatatype*>* getDatatypeExpressionDatatypeHash();
					CBUILDHASH<CDatatype*,CDatatypeExpression*>* getDatatypeDatatypeExpessionHash();

					CBUILDHASH<QPair<CDataLexicalValueExpression*, CDatatype*>, CDataLiteral*>* getDataLexicalValueExpressionDatatypePairDataLiteralHash();

					CBUILDHASH<CClassAxiomExpression*,bool>* getUpdatedClassAxiomTellOrRetractHash();
					CBUILDHASH<CObjectPropertyAxiomExpression*,bool>* getUpdateObjectPropertyAxiomHash();
					CBUILDHASH<CDataPropertyAxiomExpression*,bool>* getUpdateDataPropertyAxiomHash();
					CBUILDHASH<CAssertionAxiomExpression*,bool>* getUpdateAssertionAxiomHash();

					CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >* getClassTermExpressionClassAxiomExpressionSet();
					CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* getClassTermExpressionClassAxiomExpressionHash();

					CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >* getObjectPropertyTermObjectPropertyAxiomSet();
					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* getObjectPropertyTermObjectPropertyAxiomHash();

					CBUILDSET< QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*> >* getDataPropertyTermDataPropertyAxiomSet();
					CBUILDHASH<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>* getDataPropertyTermDataPropertyAxiomHash();


					CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >* getIndividualTermAssertionAxiomSet();
					CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>* getIndividualTermAssertionAxiomHash();


					CBUILDSET<CClassTermExpression*>* getTopRebuildClassTermExpressionsSet();
					CBUILDSET<CObjectPropertyTermExpression*>* getTopRebuildObjectPropertyTermExpressionsSet();

					CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>* getObjectPropertyTermRoleChainHash();
					CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>* getRoleChainObjectPropertyTermHash();

					CBUILDSET<CClassTermExpression*>* getBuildedConceptSet();
					CBUILDSET<CObjectPropertyTermExpression*>* getBuildedObjectRoleSet();
					CBUILDSET<CDataPropertyTermExpression*>* getBuildedDataRoleSet();
					CBUILDSET<CDataRangeTermExpression*>* getBuildedDataRangeSet();
					CBUILDSET<CDatatypeExpression*>* getBuildedDatatypeSet();
					CBUILDSET<CIndividualTermExpression*>* getBuildedIndividualSet();

					CBUILDLIST<CClassTermExpression*>* getBuildedConceptList();
					CBUILDLIST<CObjectPropertyTermExpression*>* getBuildedObjectRoleList();
					CBUILDLIST<CDataPropertyTermExpression*>* getBuildedDataRoleList();
					CBUILDLIST<CDataRangeTermExpression*>* getBuildedDataRangeList();
					CBUILDLIST<CDatatypeExpression*>* getBuildedDatatypeList();
					CBUILDLIST<CIndividualTermExpression*>* getBuildedIndividualList();


					cint64 getLastActiveCountedEntityAxiom();
					CExpressionDataBoxMapping* setLastActiveCountedEntityAxiom(cint64 lastCounted);


					cint64 getLastProcessedChangedAxiom();
					cint64 getLastProcessedBuildIndividual();
					cint64 getLastProcessedBuildConcept();
					cint64 getLastProcessedBuildObjectRole();
					cint64 getLastProcessedBuildDataRole();
					cint64 getLastProcessedBuildDataRange();
					cint64 getLastProcessedBuildDatatype();

					cint64 getLastProcessedInverseProperty();
					cint64 getLastProcessedExpression();

					cint64 getLastBuildedIndividual();
					cint64 getLastBuildedConcept();
					cint64 getLastBuildedObjectRole();
					cint64 getLastBuildedDataRole();
					cint64 getLastBuildedDataRange();
					cint64 getLastBuildedDatatype();

					CExpressionDataBoxMapping* setLastProcessedChangedAxiom(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedBuildIndividual(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedBuildConcept(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedBuildObjectRole(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedBuildDataRole(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedBuildDataRange(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedBuildDatatype(cint64 lastBuilded);

					CExpressionDataBoxMapping* setLastProcessedInverseProperty(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastProcessedExpression(cint64 lastBuilded);

					CExpressionDataBoxMapping* setLastBuildedIndividual(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastBuildedConcept(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastBuildedObjectRole(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastBuildedDataRole(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastBuildedDataRange(cint64 lastBuilded);
					CExpressionDataBoxMapping* setLastBuildedDatatype(cint64 lastBuilded);


					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* getInverseObjectPropertyHash();
					CBUILDHASH<CExpressionHasher,CBuildExpression*>* getStructuralExpressionBuildHash();
					CBUILDLIST<CBuildExpression*>* getExpressionBuildListContainer();

				// protected methods
				protected:

				// protected variables
				protected:
					COntologyContext* mOntoContext;

					CActiveEntityCountVector* mActiveEntityCountVector;

					COntologyBuildConstructFlags mConstructFlags;

					CBUILDHASH<CClassTermExpression*,CConcept*>* mClassTermConceptHash;
					CBUILDHASH<CConcept*,CClassTermExpression*>* mConceptClassTermHash;
					CBUILDHASH<CDataRangeTermExpression*,CConcept*>* mDataRangeTermConceptHash;
					CBUILDHASH<CConcept*,CDataRangeTermExpression*>* mConceptDataRangeTermHash;
					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* mObjPropTermRoleHash;
					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* mRoleObjPropTermHash;

					CBUILDHASH<CDataPropertyTermExpression*,CRole*>* mDataPropTermRoleHash;
					CBUILDHASH<CRole*,CDataPropertyTermExpression*>* mRoleDataPropTermHash;


					CBUILDHASH<CIndividualTermExpression*,CIndividual*>* mIndividulTermIndiHash;
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* mIndiIndividulTermHash;

					CBUILDHASH<CDatatypeExpression*,CDatatype*>* mDatatypeExpDatatypeHash;
					CBUILDHASH<CDatatype*,CDatatypeExpression*>* mDatatypeDatatypeExpHash;

					CBUILDHASH<QPair<CDataLexicalValueExpression*, CDatatype*>, CDataLiteral*>* mLexicalDataExpDatatypePairDataLiteralHash;

					CBUILDHASH<CClassAxiomExpression*,bool>* mUpdateClassAxiomHash;
					CBUILDHASH<CObjectPropertyAxiomExpression*,bool>* mUpdateObjectPropertyAxiomHash;
					CBUILDHASH<CDataPropertyAxiomExpression*,bool>* mUpdateDataPropertyAxiomHash;
					CBUILDHASH<CAssertionAxiomExpression*,bool>* mUpdateAssertionAxiomHash;


					// contains also previous ontology data
					CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >* mClassTermClassAxiomSet;
					CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* mClassTermClassAxiomHash;

					CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >* mObjPropTermObjPropAxiomSet;
					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* mObjPropTermObjPropAxiomHash;

					CBUILDSET< QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*> >* mDataPropTermDataPropAxiomSet;
					CBUILDHASH<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>* mDataPropTermDataPropAxiomHash;


					CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >* mIndiTermAssertionAxiomSet;
					CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>* mIndiTermAssertionAxiomHash;


					CBUILDSET<CClassTermExpression*>* mTopRebuildClassTermExpressionsSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mTopRebuildObjectPropertyTermExpressionsSet;


					CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>* mObjPropTermRoleChainHash;
					CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>* mRoleChainObjPropTermHash;

					cint64 mLastActiveCountedEntityAxiom;

					cint64 mLastProcessedChangedAxiom;
					cint64 mLastProcessedBuildIndividual;
					cint64 mLastProcessedBuildConcept;
					cint64 mLastProcessedBuildObjectRole;
					cint64 mLastProcessedBuildDataRole;
					cint64 mLastProcessedBuildDataRange;
					cint64 mLastProcessedBuildDatatype;

					cint64 mLastProcessedInverseProperty;
					cint64 mLastProcessedExpression;

					cint64 mLastBuildedIndividual;
					cint64 mLastBuildedConcept;
					cint64 mLastBuildedObjectRole;
					cint64 mLastBuildedDataRole;
					cint64 mLastBuildedDataRange;
					cint64 mLastBuildedDatatype;

					CBUILDSET<CIndividualTermExpression*>* mBuildedIndividualSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mBuildedObjectRoleSet;
					CBUILDSET<CDataPropertyTermExpression*>* mBuildedDataRoleSet;
					CBUILDSET<CDataRangeTermExpression*>* mBuildedDataRangeSet;
					CBUILDSET<CDatatypeExpression*>* mBuildedDatatypeSet;
					CBUILDSET<CClassTermExpression*>* mBuildedConceptSet;

					CBUILDLIST<CIndividualTermExpression*>* mBuildedIndividualList;
					CBUILDLIST<CObjectPropertyTermExpression*>* mBuildedObjectRoleList;
					CBUILDLIST<CDataPropertyTermExpression*>* mBuildedDataRoleList;
					CBUILDLIST<CDataRangeTermExpression*>* mBuildedDataRangeList;
					CBUILDLIST<CDatatypeExpression*>* mBuildedDatatypeList;
					CBUILDLIST<CClassTermExpression*>* mBuildedConceptList;

					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mInverseObjectPropertyHash;
					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mExpressionBuildHash;
					CBUILDLIST<CBuildExpression*>* mExpressionBuildListContainer;


				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Ontology

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_ONTOLOGY_CEXPRESSIONDATABOXMAPPING_H
