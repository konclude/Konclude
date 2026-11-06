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

import org.semanticweb.owlapi.model.*;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Looks for an ABox that forces two individuals to be merged, which Konclude does not
 * terminate on unless the completion graph is built, see the comment of KoncludeReasoner and
 * Java/Readme.md.
 *
 * The check is syntactic and deliberately errs towards reporting a risk: it reports a subject
 * that has two or more different asserted values of one object property whose number of
 * values is restricted somewhere in the ontology, by a functionality axiom or by a maximum or
 * exact cardinality. The same is done for the subjects of an inverse functional property.
 *
 * This finds the cases in which the merge follows directly from the assertions. A merge that
 * only follows after further reasoning is not found, which is why KoncludeReasoner also
 * offers the time out of the OWL API as a second line of defence.
 *
 * A reported risk costs the completion graph construction, which is slower, and never a wrong
 * answer, whereas a missed one costs a reasoner that does not answer, so the trade is made in
 * that direction.
 */
public class KoncludeIndividualMergeInspector {

	/** the assertions from which the merge follows, empty if none was found */
	private final Set<OWLObjectPropertyAssertionAxiom> mMergingAssertions =
			new HashSet<OWLObjectPropertyAssertionAxiom>();

	private OWLObjectPropertyExpression mMergingProperty = null;
	private OWLIndividual mMergingIndividual = null;

	public KoncludeIndividualMergeInspector(OWLOntology ontology) {
		inspect(ontology);
	}

	/** whether the ABox forces two individuals to be merged, as far as that is visible here */
	public boolean isMergeForced() {
		return !mMergingAssertions.isEmpty();
	}

	public Set<OWLObjectPropertyAssertionAxiom> getMergingAssertions() {
		return mMergingAssertions;
	}

	/** a sentence for a message, empty if no merge was found */
	public String getDescription() {
		if (!isMergeForced()) {
			return "";
		}
		return "'" + mMergingIndividual + "' has " + mMergingAssertions.size() + " different values of "
				+ "the restricted property '" + mMergingProperty + "', which forces them to be the same "
				+ "individual";
	}


	// ---------------------------------------------------------------- the check

	private void inspect(OWLOntology ontology) {
		Set<OWLObjectPropertyExpression> restricted = new HashSet<OWLObjectPropertyExpression>();
		Set<OWLObjectPropertyExpression> inverseRestricted = new HashSet<OWLObjectPropertyExpression>();
		collectRestrictedProperties(ontology, restricted, inverseRestricted);
		if (restricted.isEmpty() && inverseRestricted.isEmpty()) {
			return;
		}

		// subject and property to the asserted values, and the other way round for the
		// inverse functional case
		Map<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>> valuesOfSubject =
				new HashMap<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>>();
		Map<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>> subjectsOfValue =
				new HashMap<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>>();

		for (OWLOntology imported : ontology.getImportsClosure()) {
			for (OWLObjectPropertyAssertionAxiom axiom
					: imported.getAxioms(AxiomType.OBJECT_PROPERTY_ASSERTION)) {
				OWLObjectPropertyExpression property = axiom.getProperty();
				if (restricted.contains(property)) {
					add(valuesOfSubject, axiom.getSubject(), property, axiom.getObject());
				}
				if (inverseRestricted.contains(property)) {
					add(subjectsOfValue, axiom.getObject(), property, axiom.getSubject());
				}
			}
		}

		if (!findMerge(ontology, valuesOfSubject, false)) {
			findMerge(ontology, subjectsOfValue, true);
		}
	}

	/**
	 * The properties whose number of values is restricted, and separately the ones whose
	 * number of subjects is restricted. A maximum or exact cardinality of 0 cannot force a
	 * merge, it makes the class unsatisfiable instead, so only a restriction to at least one
	 * value is of interest here.
	 */
	private void collectRestrictedProperties(OWLOntology ontology,
			Set<OWLObjectPropertyExpression> restricted,
			Set<OWLObjectPropertyExpression> inverseRestricted) {

		for (OWLOntology imported : ontology.getImportsClosure()) {
			for (OWLFunctionalObjectPropertyAxiom axiom
					: imported.getAxioms(AxiomType.FUNCTIONAL_OBJECT_PROPERTY)) {
				addRestricted(axiom.getProperty(), restricted, inverseRestricted);
			}
			for (OWLInverseFunctionalObjectPropertyAxiom axiom
					: imported.getAxioms(AxiomType.INVERSE_FUNCTIONAL_OBJECT_PROPERTY)) {
				addRestricted(axiom.getProperty(), inverseRestricted, restricted);
			}
			for (OWLAxiom axiom : imported.getAxioms()) {
				for (OWLClassExpression nested : axiom.getNestedClassExpressions()) {
					OWLObjectPropertyExpression property = null;
					if (nested instanceof OWLObjectMaxCardinality) {
						OWLObjectMaxCardinality restriction = (OWLObjectMaxCardinality) nested;
						if (restriction.getCardinality() > 0) {
							property = restriction.getProperty();
						}
					} else if (nested instanceof OWLObjectExactCardinality) {
						OWLObjectExactCardinality restriction = (OWLObjectExactCardinality) nested;
						if (restriction.getCardinality() > 0) {
							property = restriction.getProperty();
						}
					}
					if (property != null) {
						addRestricted(property, restricted, inverseRestricted);
					}
				}
			}
		}
	}

	/** an inverse property restricts the other direction, so the two sets are swapped for it */
	private void addRestricted(OWLObjectPropertyExpression property,
			Set<OWLObjectPropertyExpression> forward, Set<OWLObjectPropertyExpression> backward) {
		if (property.isAnonymous()) {
			backward.add(property.getNamedProperty());
		} else {
			forward.add(property.getNamedProperty());
		}
	}

	private void add(Map<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>> map,
			OWLIndividual key, OWLObjectPropertyExpression property, OWLIndividual value) {
		Map<OWLObjectPropertyExpression, Set<OWLIndividual>> byProperty = map.get(key);
		if (byProperty == null) {
			byProperty = new HashMap<OWLObjectPropertyExpression, Set<OWLIndividual>>();
			map.put(key, byProperty);
		}
		Set<OWLIndividual> values = byProperty.get(property);
		if (values == null) {
			values = new HashSet<OWLIndividual>();
			byProperty.put(property, values);
		}
		values.add(value);
	}

	private boolean findMerge(OWLOntology ontology,
			Map<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>> map,
			boolean inverse) {

		for (Map.Entry<OWLIndividual, Map<OWLObjectPropertyExpression, Set<OWLIndividual>>> entry
				: map.entrySet()) {
			for (Map.Entry<OWLObjectPropertyExpression, Set<OWLIndividual>> byProperty
					: entry.getValue().entrySet()) {
				if (byProperty.getValue().size() < 2) {
					continue;
				}
				// an asserted sameness of the values means the merge is already known, which is
				// not the case that Konclude does not terminate on
				if (allAssertedSame(ontology, byProperty.getValue())) {
					continue;
				}
				mMergingIndividual = entry.getKey();
				mMergingProperty = byProperty.getKey();
				collectAssertions(ontology, entry.getKey(), byProperty.getKey(), inverse);
				return true;
			}
		}
		return false;
	}

	private boolean allAssertedSame(OWLOntology ontology, Set<OWLIndividual> individuals) {
		for (OWLOntology imported : ontology.getImportsClosure()) {
			for (OWLSameIndividualAxiom axiom : imported.getAxioms(AxiomType.SAME_INDIVIDUAL)) {
				if (axiom.getIndividuals().containsAll(individuals)) {
					return true;
				}
			}
		}
		return false;
	}

	private void collectAssertions(OWLOntology ontology, OWLIndividual individual,
			OWLObjectPropertyExpression property, boolean inverse) {
		for (OWLOntology imported : ontology.getImportsClosure()) {
			for (OWLObjectPropertyAssertionAxiom axiom
					: imported.getAxioms(AxiomType.OBJECT_PROPERTY_ASSERTION)) {
				if (!axiom.getProperty().equals(property)) {
					continue;
				}
				OWLIndividual key = inverse ? axiom.getObject() : axiom.getSubject();
				if (key.equals(individual)) {
					mMergingAssertions.add(axiom);
				}
			}
		}
	}
}
