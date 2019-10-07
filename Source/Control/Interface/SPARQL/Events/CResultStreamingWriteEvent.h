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

#ifndef KONCLUDE_CONTROL_INTERFACE_SPARQL_EVENTS_CRESULTSTREAMINGWRITEEVENT_H
#define KONCLUDE_CONTROL_INTERFACE_SPARQL_EVENTS_CRESULTSTREAMINGWRITEEVENT_H



// Libraries includes
#include <QByteArray>
#include <QEvent>

// Namespace includes
#include "../SPARQLSettings.h"
#include "../CSPARQLResultBufferWriteData.h"


// Other includes
#include "Concurrent/Events/CCustomEvent.h"


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	using namespace Concurrent::Events;

	namespace Control {

		namespace Interface {

			namespace SPARQL {

				namespace Events {



					/*! 
					 *	\class		CResultStreamingWriteEvent
					 *	\author		Andreas Steigmiller
					 *	\version	0.1
					 *	\brief		
					 */
					class CResultStreamingWriteEvent : public CCustomEvent {
						// public methods
						public:
							//! Constructor
							CResultStreamingWriteEvent(const QList<CSPARQLResultBufferWriteData>& bufferList, bool last);

							//! Destructor
							virtual ~CResultStreamingWriteEvent();

							QList<CSPARQLResultBufferWriteData>* getBufferList();
							bool isLast();


							static const QEvent::Type EVENTTYPE = EVENTRESULTSTREAMINGWRITE;

						// protected methods
						protected:

						// protected variables
						protected:
							QList<CSPARQLResultBufferWriteData> mBufferList;
							bool mLast;


						// private methods
						private:

						// private variables
						private:

					};

				}; // end namespace Events

			}; // end namespace SPARQL

		}; // end namespace Interface

	}; // end namespace Control

}; // end namespace Konclude

#endif // KONCLUDE_CONTROL_INTERFACE_SPARQL_EVENTS_CRESULTSTREAMINGWRITEEVENT_H
