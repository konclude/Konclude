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

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CDEPENDENCYNODE_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CDEPENDENCYNODE_H

// Libraries includes


// Namespace includes
#include "DependencySettings.h"
#include "CDependency.h"
#include "CDependencyTrackPoint.h"
#include "CDependencyIterator.h"


// Other includes
#include "Reasoner/Kernel/Process/Marker/CMarker.h"
#include "Reasoner/Kernel/Process/CProcessingTag.h"

#include "Reasoner/Kernel/Process/CConceptDescriptor.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		using namespace Ontology;

		namespace Kernel {

			namespace Process {

				using namespace Marker;

				namespace Dependency {

					/*! 
					 *
					 *		\class		CDependencyNode
					 *		\author		Andreas Steigmiller
					 *		\version	0.1
					 *		\brief		TODO
					 *
					 */
					class CDependencyNode : public CProcessingTag {
						// public methods
						public:
							//! Constructor
							CDependencyNode(CProcessContext* processContext = nullptr);


							enum DEPENDENCNODEYTYPE {	DNTUNDEFINED, DNTINDEPENDENTBASE, DNTANDDEPENDENCY, DNTAUTOMATCHOOSEDEPENDENCY, DNTORDEPENDENCY, 
														DNTALLDEPENDENCY, DNTAUTOMATTRANSACTIONDEPENDENCY, DNTSOMEDEPENDENCY, DNTSELFDEPENDENCY, DNTVALUEDEPENDENCY, 
														DNTNEGVALUEDEPENDENCY, DNTDISTINCTDEPENDENCY, DNTATLEASTDEPENDENCY, DNTMERGEDCONCEPT, DNTMERGEDLINK, DNTMERGEDINDIVIDUAL, 
														DNTMERGEDEPENDENCY, DNTATMOSTDEPENDENCY, DNTQUALIFYDEPENDENCY, DNTFUNCTIONALDEPENDENCY, DNTNOMINALDEPENDENCY,
														DNTIMPLICATIONDEPENDENCY, DNTEXPANDEDDEPENDENCY, DNTCONNECTIONDEPENDENCY, DNTREUSEINDIVIDUALDEPENDENCY, 
														DNTREUSECONCEPTSDEPENDENCY, DNTREUSECOMPLETIONGRAPHDEPENDENCY, DNTDATATYPETRIGGERDEPENDENCY, DNTDATATYPECONNECTIONDEPENDENCY,
														DNTROLEASSERTIONDEPENDENCY, DNTDATAASSERTIONDEPENDENCY, DNTMERGEPOSSIBLEINSTANCEINDIVIDUALDEPENDENCY, DNTSAMEINDIVIDUALSMERGEDEPENDENCY,
														DNTORONLYOPTIONDEPENDENCY,

														DNTBINDVARIABLEDEPENDENCY, DNTPROPAGATEBINDINGDEPENDENCY, DNTBINDPROPAGATEAND, DNTBINDPROPAGATEIMPLICATION,
														DNTPROPAGATEBINDINGSUCCESSORDEPENDENCY, DNTBINDPROPAGATEALL, DNTBINDPROPAGATECYCLE, DNTPROPAGATECONNECTIONDEPENDENCY,
														DNTPROPAGATECONNECTIONAWAYDEPENDENCY, DNTBINDPROPAGATEGROUNDINGDEPENDENCY,

														DNTVARBINDPROPAGATEAND, DNTPROPAGATEVARIABLEBINDINGDEPENDENCY, DNTVARBINDPROPAGATEALL, DNTPROPAGATEVARIABLEBINDINGSUCCESSORDEPENDENCY,
														DNTVARBINDVARIABLEDEPENDENCY, DNTVARBINDPROPAGATEJOIN, DNTVARBINDPROPAGATEGROUNDINGDEPENDENCY, DNTPROPAGATEVARIABLECONNECTIONDEPENDENCY,
														DNTVARBINDPROPAGATEIMPLICATION,

														DNTRESOLVEREPRESENTATIVE, DNTREPRESENTATIVEAND, DNTREPRESENTATIVEALL, DNTREPRESENTATIVEIMPLICATION, DNTREPRESENTATIVEBINDVARIABLE, 
														DNTREPRESENTATIVEJOIN, DNTREPRESENTATIVEGROUNDING
							};


							CDependencyNode* initDependencyNode(DEPENDENCNODEYTYPE depType, CConceptDescriptor* conceptDescriptor = nullptr);
							CDependencyNode* initDependencyNode(DEPENDENCNODEYTYPE depType, CIndividualProcessNode* individualNode, CConceptDescriptor* conceptDescriptor = nullptr);
							
							CConceptDescriptor* getConceptDescriptor();
							CDependencyNode* setConceptDescriptor(CConceptDescriptor* conceptDescriptor);

							CDependencyTrackPoint* getPreviousDependencyTrackPoint();

							virtual CDependencyTrackPoint* getContinueDependencyTrackPoint() = 0;

							virtual bool isDeterministiDependencyNode() = 0;
							bool isNonDeterministiDependencyNode();


							//CMarker* getMarker();
							//CDependencyNode* setMarker(CMarker* depMarker);
							//CDependencyNode* addMarker(CMarker* depMarker);


							CDependencyIterator getAdditionalDependencyIterator(bool aheadDependencies = true, bool afterDependencies = true);
							//CDependency* getAdditionalAheadDependencies();
							CDependency* getAdditionalAfterDependencies();
							cint64 getAdditionalDependencyCount();
							bool hasAdditionalDependencies();

							//bool hasAdditionalAheadDependencies();
							bool hasAdditionalAfterDependencies();

							bool hasDependencies();
							cint64 getDependencyCount();

							cint64 getDependedBranchingTag();
							cint64 getDependedBranchingLevel();

							DEPENDENCNODEYTYPE getDependencyType();
							bool isDependencyType(DEPENDENCNODEYTYPE depType);
							bool isIndependentBaseDependencyType();
							bool isUndefinedDependencyType();

							CIndividualProcessNode* getAppropriateIndividualNode();
							bool hasAppropriateIndividualNode();
							CDependencyNode* setAppropriateIndividualNode(CIndividualProcessNode* indiNode);

							virtual bool isRepresentativeResolveDependencyNode();
							virtual bool isRepresentativeSelectDependencyNode();

						// protected methods
						protected:
							//CDependencyNode* addAheadDependency(CDependency* conceptDependency);
							CDependencyNode* addAfterDependency(CDependency* linkDependency);

							bool updateDependencyTrackPointBranchingTag(CDependencyTrackPoint* depTrackPoint, cint64 branchinLevelTag);

						// protected variables
						protected:
							CProcessContext* mProcessContext;
							CConceptDescriptor* mConceptDescriptor;
							CIndividualProcessNode* mIndividualNode;
							DEPENDENCNODEYTYPE mDepNodeType;

							//CMarker* mMarker;

							CDependencyTrackPoint* mDepTrackPoint;
							//CDependency* mAdditionalAheadDepLinker;
							CDependency* mAdditionalAfterDepLinker;


						// private methods
						private:

						// private variables
						private:


					};

				}; // end namespace Dependency

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_DEPENDENCY_CDEPENDENCYNODE_H
