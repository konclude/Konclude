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

package com.konclude.owlapi;

import org.semanticweb.owlapi.model.OWLOntology;
import org.semanticweb.owlapi.reasoner.BufferingMode;
import org.semanticweb.owlapi.reasoner.OWLReasoner;
import org.semanticweb.owlapi.reasoner.OWLReasonerConfiguration;
import org.semanticweb.owlapi.reasoner.OWLReasonerFactory;
import org.semanticweb.owlapi.reasoner.SimpleConfiguration;

/**
 * Creates a KoncludeReasoner, the entry point that the OWL API and a Protege plug-in expect.
 *
 * Every created reasoner holds a library instance of Konclude and has to be closed with
 * dispose().
 */
public class KoncludeReasonerFactory implements OWLReasonerFactory {

	private final String mLoadingConfiguration;
	private final KoncludeReasoner.MergeSafety mMergeSafety;

	public KoncludeReasonerFactory() {
		this("", KoncludeReasoner.MergeSafety.OFF);
	}

	/**
	 * The loading configuration is handed to initKoncludeLibraryInstance of every created
	 * reasoner, see KoncludeReasoner.
	 */
	public KoncludeReasonerFactory(String loadingConfiguration) {
		this(loadingConfiguration, KoncludeReasoner.MergeSafety.OFF);
	}

	public KoncludeReasonerFactory(KoncludeReasoner.MergeSafety mergeSafety) {
		this("", mergeSafety);
	}

	/**
	 * The merge safety says what to do about an ABox that forces two individuals to be merged,
	 * which only matters against a shared library that predates the fix for it, see
	 * KoncludeReasoner.MergeSafety.
	 */
	public KoncludeReasonerFactory(String loadingConfiguration, KoncludeReasoner.MergeSafety mergeSafety) {
		mLoadingConfiguration = loadingConfiguration != null ? loadingConfiguration : "";
		mMergeSafety = mergeSafety != null ? mergeSafety : KoncludeReasoner.MergeSafety.OFF;
	}

	@Override
	public String getReasonerName() {
		return "Konclude";
	}

	@Override
	public OWLReasoner createReasoner(OWLOntology ontology) {
		return createReasoner(ontology, new SimpleConfiguration());
	}

	@Override
	public OWLReasoner createReasoner(OWLOntology ontology, OWLReasonerConfiguration configuration) {
		return new KoncludeReasoner(ontology, configuration, BufferingMode.BUFFERING,
				mLoadingConfiguration, mMergeSafety);
	}

	@Override
	public OWLReasoner createNonBufferingReasoner(OWLOntology ontology) {
		return createNonBufferingReasoner(ontology, new SimpleConfiguration());
	}

	@Override
	public OWLReasoner createNonBufferingReasoner(OWLOntology ontology, OWLReasonerConfiguration configuration) {
		return new KoncludeReasoner(ontology, configuration, BufferingMode.NON_BUFFERING,
				mLoadingConfiguration, mMergeSafety);
	}
}
