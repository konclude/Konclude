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

package com.konclude.protege;

import com.konclude.owlapi.KoncludeReasoner;
import com.konclude.owlapi.KoncludeReasonerFactory;

import org.protege.editor.owl.model.inference.AbstractProtegeOWLReasonerInfo;
import org.semanticweb.owlapi.reasoner.BufferingMode;
import org.semanticweb.owlapi.reasoner.OWLReasonerFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Registers Konclude with the reasoner menu of Protege, see plugin.xml, and hands it the
 * KoncludeReasonerFactory of the OWL API wrapper.
 *
 * Protege creates the reasoner on its classification thread and asks it from the event thread
 * and others, which KoncludeReasoner allows for by running every native call on a thread of
 * its own. The shared library of Konclude is carried by the bundle of this plug-in and is
 * unpacked by OSGi, see Bundle-NativeCode in the pom, so that System.loadLibrary finds it.
 */
public class KoncludeProtegeReasonerInfo extends AbstractProtegeOWLReasonerInfo {

	private static final Logger LOGGER = LoggerFactory.getLogger(KoncludeProtegeReasonerInfo.class);

	private final KoncludeReasonerFactory mFactory = new KoncludeReasonerFactory();

	/**
	 * Loads the shared library, which reports a platform the plug-in has no library for here,
	 * in the log of Protege, instead of at the first start of the reasoner. The reasoner is
	 * listed either way, its start then fails with the same UnsatisfiedLinkError.
	 */
	@Override
	public void initialise() throws Exception {
		try {
			KoncludeReasoner.loadNativeLibrary();
			LOGGER.info("Konclude: the shared library of Konclude is loaded, processor count {} (set with -D{}){}",
					KoncludeReasoner.PROCESSOR_COUNT, KoncludeReasoner.PROCESSOR_COUNT_PROPERTY,
					KoncludeReasoner.ADDITIONAL_CONFIGURATION.isEmpty() ? ""
							: ", additional settings " + KoncludeReasoner.ADDITIONAL_CONFIGURATION.trim());
		} catch (UnsatisfiedLinkError error) {
			LOGGER.error("Konclude: the shared library of Konclude could not be loaded on "
					+ System.getProperty("os.name") + " " + System.getProperty("os.arch")
					+ ". The plug-in carries the library for macOS on arm64 only. " + error.getMessage());
		}
	}

	@Override
	public void dispose() throws Exception {
	}

	/**
	 * The bridge cannot update an installed ontology, every change means translating the
	 * ontology again, so the changes are better collected until Protege synchronises.
	 */
	@Override
	public BufferingMode getRecommendedBuffering() {
		return BufferingMode.BUFFERING;
	}

	@Override
	public OWLReasonerFactory getReasonerFactory() {
		return mFactory;
	}
}
