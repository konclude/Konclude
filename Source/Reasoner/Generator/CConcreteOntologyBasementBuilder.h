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

#ifndef KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYBASEMENTBUILDER_H
#define KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYBASEMENTBUILDER_H


// Libraries includes
#include <QListIterator>
#include <QHash>
#include <QString>
#include <QObject>

// Namespace includes
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
			 *		\class		CConcreteOntologyBasementBuilder
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CConcreteOntologyBasementBuilder {
				public:
					CConcreteOntologyBasementBuilder(CConcreteOntology* basementBuildConcreteOntology);
					virtual ~CConcreteOntologyBasementBuilder();

					virtual bool initializeBuilding();
					virtual bool completeBuilding();

					virtual bool buildOntologyBasement();

					bool addNameAbbreviation(const QString &name, const QString &abbreviatedName);

				// protected functions
				protected:
					bool updateName(CNamedItem* item, const QString& name);

					bool createDatatype(const QString& datatypeIRI);

				// protected variables
				protected:
					CConcreteOntology* mOnto;
					COntologyStringMapping* mOntoStrings;
					COntologyDataBoxes* mOntoData;
					COntologyBuildData* mOntoBuild;

					CTBox* tBox;
					CABox* aBox;
					CRBox* rBox;

					CMemoryAllocationManager* mMemManager;


					CActiveEntityCountVector* mActiveEntityCountVector;

					// contains also previous ontology data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractAxiomSet;

					// contains only updated data
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mTellUpdatedAxiomSet;
					CONTOLOGYAXIOMSET<CAxiomExpression*>* mRetractUpdatedAxiomSet;

					CBUILDHASH<CClassAxiomExpression*,bool>* mUpdateClassAxiomHash;
					CBUILDHASH<CObjectPropertyAxiomExpression*,bool>* mUpdateObjectPropertyAxiomHash;

					// contains also previous ontology data
					CBUILDSET< QPair<CClassTermExpression*,CClassAxiomExpression*> >* mClassTermClassAxiomSet;
					CBUILDHASH<CClassTermExpression*,CClassAxiomExpression*>* mClassTermClassAxiomHash;

					CBUILDSET< QPair<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*> >* mObjPropTermObjPropAxiomSet;
					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyAxiomExpression*>* mObjPropTermObjPropAxiomHash;

					CBUILDSET<CClassTermExpression*>* mTopRebuildClassTermExpressionsSet;
					CBUILDSET<CObjectPropertyTermExpression*>* mTopRebuildObjectPropertyTermExpressionsSet;


					CBUILDHASH<CStringRefStringHasher,CDatatypeExpression*>* mDatatypeBuildHash;

					CClassTermExpression* mTopClassExpression;
					CClassTermExpression* mBottomClassExpression;

					CObjectPropertyTermExpression* mTopObjPropExpression;
					CObjectPropertyTermExpression* mBottomObjPropExpression;

					CDataPropertyTermExpression* mTopDataPropExpression;
					CDataPropertyTermExpression* mBottomDataPropExpression;



					CBUILDLIST<CBuildExpression*>* mExpressionBuildContainerList;


					CBUILDHASH<CClassTermExpression*,CConcept*>* mClassTermConceptHash;
					CBUILDHASH<CConcept*,CClassTermExpression*>* mConceptClassTermHash;

					CBUILDHASH<CDataRangeTermExpression*, CConcept*>* mDataRangeTermConceptHash;
					CBUILDHASH<CConcept*, CDataRangeTermExpression*>* mConceptDataRangeTermHash;

					CBUILDHASH<CObjectPropertyTermExpression*,CRole*>* mObjPropTermRoleHash;
					CBUILDHASH<CRole*,CObjectPropertyTermExpression*>* mRoleObjPropTermHash;

					CBUILDHASH<CDataPropertyTermExpression*,CRole*>* mDataPropTermRoleHash;
					CBUILDHASH<CRole*,CDataPropertyTermExpression*>* mRoleDataPropTermHash;

					CBUILDHASH<CDatatypeExpression*,CDatatype*>* mDatatypeExpDatatypeHash;
					CBUILDHASH<CDatatype*,CDatatypeExpression*>* mDatatypeDatatypeExpHash;


					CBUILDHASH<CObjectPropertyAxiomExpression*,CRoleChain*>* mObjPropTermRoleChainHash;
					CBUILDHASH<CRoleChain*,CObjectPropertyAxiomExpression*>* mRoleChainObjPropTermHash;

					CBUILDHASH<CIndividualTermExpression*,CIndividual*>* mIndividulTermIndiHash;
					CBUILDHASH<CIndividual*,CIndividualTermExpression*>* mIndiIndividulTermHash;

					CBUILDHASH<CObjectPropertyTermExpression*,CObjectPropertyTermExpression*>* mInverseObjectPropertyHash;


					// contains also previous ontology data,
					// exact build expression identification
					CBUILDHASH<CExpressionHasher,CBuildExpression*>* mExpressionBuildHash;

					CBUILDHASH<CStringRefStringHasher,CClassExpression*>* mClassBuildHash;
					CBUILDHASH<CStringRefStringHasher,CObjectPropertyExpression*>* mObjectPropertyBuildHash;
					CBUILDHASH<CStringRefStringHasher,CNamedIndividualExpression*>* mIndividualBuildHash;
					CBUILDHASH<CStringRefStringHasher,CDataPropertyExpression*>* mDataPropertyBuildHash;


					// contains also previous ontology data,
					// name prefixes
					CMAPPINGHASH<QString,CNamePrefix*>* mAbbreviatedNamePrefixMapHash;
					CMAPPINGHASH<QString,CNamePrefix*>* mNamePrefixMapHash;


			};

		}; // end namespace Generator

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // end KONCLUDE_REASONER_GENERATOR_CCONCRETEONTOLOGYBASEMENTBUILDER_H
