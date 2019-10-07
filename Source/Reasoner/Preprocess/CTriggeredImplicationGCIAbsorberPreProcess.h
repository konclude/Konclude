/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_REASONER_PREPROCESS_CTRIGGEREDIMPLICATIONGCIABSORBERPREPROCESS_H
#define KONCLUDE_REASONER_PREPROCESS_CTRIGGEREDIMPLICATIONGCIABSORBERPREPROCESS_H

// Libraries includes
#include <QSet>

// Namespace includes
#include "CConcreteOntologyPreProcess.h"
#include "CConceptRoleIndividualLocator.h"


// Other includes
#include "Utilities/CDynamicExpandingMemoryManager.hpp"
#include "Utilities/CSortedNegLinker.hpp"

#include "Config/CConfigDataReader.h"

#include "Reasoner/Generator/CConcreteOntologyInverseRoleBuilder.h"

#include "Reasoner/Ontology/CConceptTextFormater.h"

// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Config;

	namespace Reasoner {

		using namespace Generator;
		using namespace Ontology;

		namespace Preprocess {

			/*! 
			 *
			 *		\class		CTriggeredImplicationGCIAbsorberPreProcess
			 *		\author		Andreas Steigmiller
			 *		\version	0.1
			 *		\brief		TODO
			 *
			 */
			class CTriggeredImplicationGCIAbsorberPreProcess : public CConcreteOntologyPreProcess {
				// public methods
				public:
					//! Constructor
					CTriggeredImplicationGCIAbsorberPreProcess();

					//! Destructor
					virtual ~CTriggeredImplicationGCIAbsorberPreProcess();

					virtual CConcreteOntology *preprocess(CConcreteOntology *ontology, CPreProcessContext* context);

				// protected methods
				protected:
					bool addConceptToImplied(CConcept* impliedConcept, CConcept* concept, bool negated);
					bool addSubClassConcept(CConcept* subclassConcept, CConcept* addedConcept, bool negated);
					bool addRoleDomainConcept(CRole* role, CConcept* addedConcept, bool negated);
					CConcept* createImpliedConcept();
					CConcept* createImplicationConcept(CConcept* impliedConcept, bool negated);
					CConcept* createImplicationTriggerConcept(CConcept* impConcept, bool negated);
					bool addImplicationTrigger(CConcept* implConcept, CConcept* triggerConcept, bool negated);
					CConcept* createTriggerPropagationConcept(CConcept* destConcept, CRole* backwardPropRole);

					bool isEquivalenceConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount = nullptr);
					bool isGCIConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated, cint64* notAbsorbableOperandsCount = nullptr);
					bool isConceptImplicationTriggerable(CConcept* concept, bool negated);
					bool isConceptSimpleImplicationTriggerable(CConcept* concept, bool negated);
					CConcept* createEquivalenceAbsorbedTriggeredImplication(CConcept* eqConcept, bool negated);
					CConcept* createDisjunctionAbsorbedTriggeredImplication(CConcept* orConcept, bool negated);
					

					cint64 createImplicationAddedToTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated);
					cint64 createTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated, cint64* impAddTriggerCount, CConcept** impAddTrigger);


					CRole* getInverseRole(CRole* role);

					bool isDisjunctionConceptTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated);
					bool isDisjunctionConceptSimpleTriggeredImplicationAbsorbable(CConcept* orConcept, bool negated);

					bool isConceptPartialImplicationTriggerable(CConcept* concept, bool negated);
					

					
					bool absorbGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated);

					CConcept* createGCIAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, QList< QPair<CConcept*,bool> >& absorbList, QList< QPair<CConcept*,bool> >& candList);


					cint64 createPartialImplicationAddedToTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated);
					cint64 createPartialTrigger(CConcept* implicationConcept, CConcept* triggerConcept, CConcept* concept, bool negated, cint64* impAddTriggerCount, CConcept** impAddTrigger);

					bool absorbPartialGCIConceptsToTriggeredImplications(CConcept* orConcept, bool negated);
					CConcept* createGCIPartialAbsorbedTriggeredImplication(CConcept* orConcept, bool negated, const QList< QPair<CConcept*,bool> >& absorbList);
					CConcept* createPartialExtendedGCIAbsorbedTriggeredImplication(CConcept* implicationConcept, const QList< QPair<CConcept*,bool> >& absorbList);

					bool addAbsorbableDisjunctCandidates(CConcept* orConcept, bool negated, QList< QPair<CConcept*,bool> >& list);
					QList< QPair<CConcept*,bool> > splitAbsorbableDisjuncts(QList< QPair<CConcept*,bool> >& list);
					QList< QPair<CConcept*,bool> > getPartialAbsorbableDisjuncts(QList< QPair<CConcept*,bool> >& list);


					QString generateDebugGCIConceptString(CConcept* concept, bool conceptNegation);
					QString generateDebugGeneratedAbsorbedImplicationString(cint64 minNewConceptTag, cint64 lastNewConceptTag);

				// protected variables
				protected:
					CMemoryAllocationManager* mMemMan;
					QHash< QPair<CConcept*,bool> ,CConcept*> mConceptTriggerHash;
					QHash<CConcept*,cint64> mTriggerCountHash;
					CConceptVector* mConceptVec;
					CConcept* mBottomConcept;
					CConcreteOntology* mOnto;

					CBOXHASH<CConcept*,CConcept*>* mTriggerImplHash;

					bool mConfRequiresExistingInverseRole;
					bool mConfAbsorbEqClassDefinitions;
					bool mConfPartialGCIAbsorption;
					bool mConfAbsorbDisjunctions;
					bool mConfAbsorbSimpleDisjunctions;

					cint64 mStatInvRolesGenerated;
					cint64 mStatGCIAbsorbed;
					cint64 mStatGCINormalAbsorbed;
					cint64 mStatGCIPartialAbsorbed;
					cint64 mStatGCIAbsorptionPartialExtended;
					cint64 mStatGCITotal;
					cint64 mStatEqAbsorbed;
					cint64 mStatEqTotal;
					cint64 mStatImplConceptsGenerated;

					QString mDebugGCIString;
					QString mDebugAbsorbedString;

					QString mDebugLastGCIString;
					QString mDebugLastAbsorbedGCIConceptString;
					QString mDebugLastAbsorbedGCITechniqueString;
					QString mDebugLastAbsorbedGCIString;
					QString mDebugTotalAbsorbedGCIString;
					cint64 mLastNewConceptTag;
					cint64 mFirstNewConceptTag;

				// private methods
				private:

				// private variables
				private:


			};

		}; // end namespace Preprocess

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_PREPROCESS_CTRIGGEREDIMPLICATIONGCIABSORBERPREPROCESS_H
