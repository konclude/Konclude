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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYUPDATEBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYUPDATEBUILDER_H


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
			 *		\class		CConcreteOntologyUpdateBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyUpdateBuilder : public CConcreteOntologyBuildDataUpdater {
				// public methods
				public:
					CConcreteOntologyUpdateBuilder(CConcreteOntology* updateConcreteOntology);
					virtual ~CConcreteOntologyUpdateBuilder();

					virtual bool initializeBuilding();
					virtual bool completeBuilding();

					virtual bool tellOntologyAxiom(CAxiomExpression* axiom);
					virtual bool retractOntologyAxiom(CAxiomExpression* axiom);


				// protected functions
				protected:
					bool setConceptOperands(CConcept* concept, CConcept* operandConcept, bool negate = false);
					bool setConceptOperandsFromClassTerms(CConcept* concept, CEXPRESSIONLIST<CClassTermExpression*>* classTermList, bool negate = false);
					bool setConceptOperandsFromClassTerms(CConcept* concept, CClassTermExpression* classTermExp, bool negate = false);
					bool setConceptOperandsFromDataRangeTerms(CConcept* concept, CDataRangeTermExpression* dataRangeTermExp, bool negate = false);
					bool setConceptOperandsFromDataRangeTerms(CConcept* concept, CEXPRESSIONLIST<CDataRangeTermExpression*>* dataRangeTermExpList, bool negate = false);
					bool setConceptRoleFromObjectPropertyTerm(CConcept* concept, CObjectPropertyTermExpression* objectPropertyTermExp);
					bool setConceptRoleFromDataPropertyTerm(CConcept* concept, CDataPropertyTermExpression* dataPropertyTermExp);
					bool setConceptIndividualFromIndividualTerm(CConcept* concept, CIndividualTermExpression* indiTermExp);
					bool setConceptNominalIndividualFromIndividualTerm(CConcept* concept, CIndividualTermExpression* indiTermExp);
					bool setIndividualAssertionConceptFromClassTerm(CIndividual* individual, CClassTermExpression* classTermExp, bool negate = false);
					bool setIndividualAssertionNominalFromClassTerm(CIndividual* individual, CClassTermExpression* classTermExp, bool negate = false);
					bool setIndividualAssertionRoleFromObjectPropertyTerm(CIndividual* individual, CObjectPropertyTermExpression* objPropTermExp, CIndividualTermExpression* destIndiTermExp);

					bool buildClassConcept(CClassTermExpression* classTermExp);
					bool buildConceptSubClassInclusion(CClassTermExpression* subClassExp, CClassTermExpression* inclusionClassExp, bool negate = false);
					bool buildConceptEquivalentClass(CClassTermExpression* classExp1, CClassTermExpression* classExp2);
					bool buildPermutableConceptEquivalentClass(CClassTermExpression* classExp1, CClassTermExpression* classExp2);
					bool buildPermutableConceptEquivalentClass(CClassTermExpression* eqCallClassExp, const CEXPRESSIONLIST<CClassTermExpression*>& eqClassExpList);
					bool buildPermutableConceptDisjointClass(CClassTermExpression* disCallClassExp, const CEXPRESSIONLIST<CClassTermExpression*>& disClassExpList);
					bool buildDisjointUnionConceptClass(CClassTermExpression* disCallClassExp, const CEXPRESSIONLIST<CClassTermExpression*>& disClassExpList);
					bool buildGeneralConceptInclusionClassExpression(CClassTermExpression* gciClassTermExp);

					bool buildObjectPropertyRole(CObjectPropertyTermExpression* objPropTermExp);

					bool buildDataPropertyRole(CDataPropertyTermExpression* dataPropTermExp);
					bool buildDatatype(CDatatypeExpression* datatypeExp);

					
					bool buildIndividualIndi(CIndividualTermExpression* indiTermExp);

					CRoleChain* buildRoleChainForSubObjectPropertyOfExpression(CSubObjectPropertyOfExpression* subObPropExp);
					CRoleChain* buildRoleChainForTransetiveObjectPropertyExpression(CTransetiveObjectPropertyExpression* objPropAxiomExp);
					
					CConcept* getConceptForClassTerm(CClassTermExpression* classTermExp, bool forceLocalisation = false);
					CRole* getRoleForObjectPropertyTerm(CObjectPropertyTermExpression* objPropTermExp, bool forceLocalisation = false);
					CRole* getRoleForDataPropertyTerm(CDataPropertyTermExpression* dataPropTermExp, bool forceLocalisation = false);
					CIndividual* getIndividualForIndividualTerm(CIndividualTermExpression* indiTermExp, bool forceLocalisation = false);
					
					CConcept* getConceptForDataRangeTerm(CDataRangeTermExpression* dataRangeExp, bool forceLocalisation = false);
					CDatatype* getDatatypeForDatatypeExpression(CDatatypeExpression* datatypeExp, bool forceLocalisation = false);
					CDataLiteral* getDataLiteralForLiteralExpression(CDataLiteralExpression* dataLiteralExp, bool forceLocalisation = false);

					bool updateName(CNamedItem* item, const QString& name);

					bool resortAndInstallConceptsAndRolesAndIndividuals();
					CSortedNegLinker<CConcept*>* resortConceptLinkersByTagging(CSortedNegLinker<CConcept*>* conceptLinkers, cint64 linkerCount = -1);
					CSortedNegLinker<CRole*>* resortRoleLinkersByTagging(CSortedNegLinker<CRole*>* roleLinkers);

					bool buildConceptTags();
					bool buildRoleTags();
					bool buildRoleChainTags();
					bool buildIndividualIDs();

					cint64 getIndividualVariableID(CConcept* concept, CObjectIndividualVariableExpression* objectIndVarExp);

					// protected variables
				protected:
					COntologyDataBoxes* mOntoData;

					COntologyBuildConstructFlags* mConstructFlags;

					CTBox* tBox;
					CABox* aBox;
					CRBox* rBox;

					bool mInitialBuild;

					// contains also previous ontology data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeAxiomList;

					// contains only updated data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellUpdatedAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractUpdatedAxiomSet;

					CONTOLOGYAXIOMLIST< QPair<CAxiomExpression*,bool> >* mChangeUpdatedAxiomList;

					CBUILDHASH<CClassAxiomExpression*,bool>* mUpdateClassAxiomHash;
					CBUILDHASH<CObjectPropertyAxiomExpression*,bool>* mUpdateObjectPropertyAxiomHash;
					CBUILDHASH<CDataPropertyAxiomExpression*,bool>* mUpdateDataPropertyAxiomHash;
					CBUILDHASH<CAssertionAxiomExpression*,bool>* mUpdateAssertionAxiomHash;


					// contains also previous ontology data
					CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >* mClassTermClassAxiomSet;
					CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* mClassTermClassAxiomHash;


					CBUILDHASH<CDatatypeExpression*,CDatatype*>* mDatatypeExpDatatypeHash;
					CBUILDHASH<CDatatype*,CDatatypeExpression*>* mDatatypeDatatypeExpHash;


					CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >* mObjPropTermObjPropAxiomSet;
					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* mObjPropTermObjPropAxiomHash;


					CBUILDSET< QPair<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*> >* mDataPropTermDataPropAxiomSet;
					CBUILDHASH<CDataPropertyTermExpression*,CDataPropertyAxiomExpression*>* mDataPropTermDataPropAxiomHash;



					CBUILDSET<CClassTermExpression*>* mTopRebuildClassTermExpressionsSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mTopRebuildObjectPropertyTermExpressionsSet;

					CBUILDSET< QPair<CIndividualTermExpression*,CAssertionAxiomExpression*> >* mIndiTermAssertionAxiomSet;
					CBUILDHASH<CIndividualTermExpression*,CAssertionAxiomExpression*>* mIndiTermAssertionAxiomHash;

					CBUILDHASH<CClassTermExpression*,CConcept*>* mClassTermConceptHash;
					CBUILDHASH<CConcept*,CClassTermExpression*>* mConceptClassTermHash;

					CBUILDHASH<CDataRangeTermExpression*,CConcept*>* mDataRangeTermConceptHash;
					CBUILDHASH<CConcept*,CDataRangeTermExpression*>* mConceptDataRangeTermHash;

					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* mObjPropTermRoleHash;
					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* mRoleObjPropTermHash;
					CBUILDHASH<CDataPropertyTermExpression*,CRole*>* mDataPropTermRoleHash;
					CBUILDHASH<CRole*,CDataPropertyTermExpression*>* mRoleDataPropTermHash;


					CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>* mObjPropTermRoleChainHash;
					CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>* mRoleChainObjPropTermHash;

					CBUILDHASH<CIndividualTermExpression*,CIndividual*>* mIndividulTermIndiHash;
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* mIndiIndividulTermHash;



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




					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mLocInverseObjectPropertyHash;
					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mLocExpressionBuildHash;
					CBUILDLIST<CBuildExpression*>* mLocExpressionBuildContainerList;


					CBUILDSET<CClassTermExpression*>* mInitialBuildConceptSet;

					CBUILDSET<CClassTermExpression*>* mLocBuildConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mLocBuildObjectRoleSet;
					CBUILDSET<CDataPropertyTermExpression*>* mLocBuildDataRoleSet;
					CBUILDSET<CDataRangeTermExpression*>* mLocBuildDataRangeSet;
					CBUILDSET<CDatatypeExpression*>* mLocBuildDatatypeSet;
					CBUILDSET<CIndividualTermExpression*>* mLocBuildIndividualSet;

					CBUILDLIST<CIndividualTermExpression*>* mLocBuildIndividualList;
					CBUILDLIST<CObjectPropertyTermExpression*>* mLocBuildObjectRoleList;
					CBUILDLIST<CDataPropertyTermExpression*>* mLocBuildDataRoleList;
					CBUILDLIST<CDataRangeTermExpression*>* mLocBuildDataRangeList;
					CBUILDLIST<CDatatypeExpression*>* mLocBuildDatatypeList;
					CBUILDLIST<CClassTermExpression*>* mLocBuildConceptList;




					// only local builder data
					CBUILDSET<CClassTermExpression*> mClassGCIExpressionSet;
					bool mRebuildTopConcept;
					CBUILDSET<CBuildExpression*> mLocalisationSet;

					CBUILDSET<CClassTermExpression*> mBuildingConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*> mBuildingObjectRoleSet;
					CBUILDSET<CDataPropertyTermExpression*> mBuildingDataRoleSet;
					CBUILDSET<CDataRangeTermExpression*> mBuildingDataRangeSet;
					CBUILDSET<CDatatypeExpression*> mBuildingDatatypeSet;
					CBUILDSET<CIndividualTermExpression*> mBuildingIndividualSet;

					CBUILDSET<CClassTermExpression*> mNewBuildedConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*> mNewBuildedObjectRoleSet;
					CBUILDSET<CDataPropertyTermExpression*> mNewBuildedDataRoleSet;
					CBUILDSET<CDataRangeTermExpression*> mNewBuildedDataRangeSet;
					CBUILDSET<CDatatypeExpression*> mNewBuildedDatatypeSet;
					CBUILDSET<CIndividualTermExpression*> mNewBuildedIndividualSet;


					CBUILDLIST<CConcept*> mTaggingConceptList;
					CBUILDLIST<CConcept*> mInstallConceptList;

					CBUILDSET<CRole*> mTaggingRoleSet;
					CBUILDLIST<CRole*> mInstallRoleList;

					CBUILDLIST<CIndividual*> mTaggingIndividualSet;
					CBUILDLIST<CIndividual*> mInstallIndividualList;

					CBUILDSET<CRoleChain*> mTaggingRoleChainSet;
					CBUILDLIST<CRoleChain*> mInstallRoleChainList;

					CBUILDSET<CClassTermExpression*> mNewBuildConceptSet;
					CBUILDSET<CObjectPropertyTermExpression*> mNewBuildObjectRoleSet;
					CBUILDSET<CDataPropertyTermExpression*> mNewBuildDataRoleSet;
					CBUILDSET<CDataRangeTermExpression*> mNewBuildDataRangeSet;
					CBUILDSET<CDatatypeExpression*> mNewBuildDatatypeSet;
					CBUILDSET<CIndividualTermExpression*> mNewBuildIndividualSet;



					CBUILDSET<CBuildExpression*> mELExceptionExpressionSet;

					QList<CIndividualTermExpression*> mLateIndiNominalConceptCreationList;
					bool mNominalConceptCreationRequired;


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYUPDATEBUILDER_H
