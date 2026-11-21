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

#ifndef KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPLETIONGRAPHSNAPSHOTWRITER_H
#define KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPLETIONGRAPHSNAPSHOTWRITER_H

// Libraries includes
#include <QString>
#include <QJsonObject>
#include <QJsonArray>

// Namespace includes
#include "AlgorithmSettings.h"

// Other includes
#include "Reasoner/Kernel/Process/CIndividualProcessNodeVector.h"
#include "Reasoner/Kernel/Process/CIndividualProcessNode.h"

#include "Reasoner/Ontology/CConcept.h"
#include "Reasoner/Ontology/CRole.h"

// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			using namespace Process;
			using namespace Dependency;

			namespace Algorithm {

				/*!
				 *
				 *		\class		CCompletionGraphSnapshotWriter
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		Writes a completion graph (all nodes of a node vector) as one JSON document.
				 *
				 *		The snapshot is meant for tools that draw completion graphs and compare the
				 *		deterministic graph (Gd), the cached non-deterministic model (Gn) and the graph of a test.
				 *		Every label entry, every link and every merge carries a flag that tells whether it depends on a
				 *		non-deterministic choice (together with the number of the branch), so that the
				 *		non-deterministic parts can be highlighted without comparing graphs.
				 *		Concepts are written in a short readable notation (e.g. a disjunction of two nominals or an at-least restriction). The notation
				 *		is the only place that has to change if another concept format is wanted.
				 *
				 */
				class CCompletionGraphSnapshotWriter {
					// public methods
					public:
						//! Describes the snapshot that is written to a file.
						struct CSnapshotDescription {
							QString mGraphName;				//!< "gd", "gn" or "test"
							QString mPhase;					//!< e.g. "before-invalidation", "after-invalidation", "end"
							QString mNote;					//!< free text, e.g. which node was invalidated
							cint64 mMaxCachedNodeID = -1;	//!< largest node id that is covered by the cached graph
						};

						//! Writes all nodes of the vector as JSON. Returns false if the file could not be written.
						static bool writeSnapshot(const QString& fileName, const CSnapshotDescription& description, CIndividualProcessNodeVector* nodeVector);

						//! Short notation of a concept in which a named class is written as its local name and a nominal as {name}.
						static QString getShortConceptString(CConcept* concept, bool negated, cint64 depth = 0);

					// protected methods
					protected:
						static QJsonObject getNodeJson(CIndividualProcessNode* node);
						static QJsonArray getLabelJson(CIndividualProcessNode* node);
						static QJsonArray getPendingQueueJson(CIndividualProcessNode* node);
						static QJsonArray getLinksJson(CIndividualProcessNode* node);
						static QJsonArray getBlockedNodesJson(CIndividualProcessNode* node);
						static QJsonArray getFlagsJson(CIndividualProcessNode* node);

						static QString getConceptKind(CConcept* concept);
						static QString getShortName(const QString& iriName);
						static QString getShortRoleString(CRole* role);
						static QString getShortOperandsString(CConcept* concept, const QString& separator, bool negated, cint64 depth);

						static bool isNonDeterministic(CDependencyTrackPoint* depTrackPoint);
						static cint64 getBranchTag(CDependencyTrackPoint* depTrackPoint);

				};

			}; // end namespace Algorithm

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_ALGORITHM_CCOMPLETIONGRAPHSNAPSHOTWRITER_H
