import java.io.*;
import java.nio.file.*;
import java.util.*;
import org.semanticweb.owlapi.apibinding.OWLManager;
import org.semanticweb.owlapi.formats.OWLXMLDocumentFormat;
import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.reasoner.*;
import org.semanticweb.owlapi.reasoner.structural.StructuralReasonerFactory;
import uk.ac.manchester.cs.owlapi.modularity.ModuleType;
import uk.ac.manchester.cs.owlapi.modularity.SyntacticLocalityModuleExtractor;

/**
 * Checks a reasoner's class hierarchy against OWL 2 DL reference reasoners on a locality module.
 *
 * Written for issue #90, where the saturation and the absorber learnt to handle the data literals of
 * SNOMED CT's drug model; the trusted path then answers that part of the ontology without the
 * tableau, so it was checked against HermiT and FaCT++, which are complete for OWL 2 DL, on a
 * module small enough for them. The hierarchy file holds one line per (class, ancestor) pair,
 * "sub super", plus "equiv a b" lines, sorted, owl:Thing left out; two such files are compared
 * with diff (or comm, for large ones).
 *
 * Modes:
 *   module <ontology> <signature file> <module out .owl> <hierarchy out> [reasoner factory class]
 *       extracts the STAR locality module of the classes listed in the signature file (one IRI per
 *       line), saves it as OWL/XML, classifies it with the reasoner (HermiT by default) and writes
 *       the hierarchy
 *   classify <ontology> <hierarchy out> <reasoner factory class>
 *       classifies a saved module with another reasoner, e.g. FaCT++:
 *       uk.ac.manchester.cs.factplusplus.owlapiv3.FaCTPlusPlusReasonerFactory
 *   asserted <ontology> <hierarchy out>
 *       writes the hierarchy of an ontology that asserts its inferred hierarchy, such as the output
 *       of Konclude's command line classification, via the structural reasoner
 *
 * Class path: the OWL API 4.5 (HermiT 1.4.3.456 and the FaCT++ binding are built against it; with
 * the OWL API 5 both fail with NoSuchMethodError). HermiT comes from Maven
 * (net.sourceforge.owlapi:org.semanticweb.hermit); FaCT++ needs its jar and its native library on
 * java.library.path. run-dhv-check.sh resolves all this.
 *
 * Sizes that worked: a module around 40 random classes of SNOMED CT International defined with a
 * DataHasValue had 1 058 classes; HermiT took 139 s, FaCT++ 5 s, Konclude 2 ms. A module around
 * 300 such classes had 5 601 classes and HermiT had not finished after 14 minutes at 14 GB.
 */
public class DhvCheck {
	public static void main(String[] a) throws Exception {
		OWLOntologyManager m = OWLManager.createOWLOntologyManager();
		OWLDataFactory df = m.getOWLDataFactory();
		if (a[0].equals("module")) {
			long t0 = System.nanoTime();
			OWLOntology o = m.loadOntologyFromOntologyDocument(new File(a[1]));
			System.out.printf("== loaded %d axioms in %.1f s%n", o.getAxiomCount(), (System.nanoTime() - t0) / 1e9);
			Set<OWLEntity> sig = new HashSet<>();
			for (String line : Files.readAllLines(Paths.get(a[2]))) {
				line = line.trim();
				if (!line.isEmpty()) sig.add(df.getOWLClass(IRI.create(line)));
			}
			t0 = System.nanoTime();
			SyntacticLocalityModuleExtractor ex = new SyntacticLocalityModuleExtractor(m, o, ModuleType.STAR);
			Set<OWLAxiom> module = ex.extract(sig);
			OWLOntology mo = m.createOntology(module, IRI.create("http://konclude.com/test/dhvmodule"));
			m.saveOntology(mo, new OWLXMLDocumentFormat(), new FileOutputStream(a[3]));
			System.out.printf("== module of %d axioms, %d classes, in %.1f s, saved %s%n", module.size(), mo.getClassesInSignature().size(), (System.nanoTime() - t0) / 1e9, a[3]);
			classify(mo, a.length > 5 ? a[5] : "org.semanticweb.HermiT.ReasonerFactory", a[4]);
		} else if (a[0].equals("classify")) {
			// classify <ontology> <hierarchy out> <reasoner factory class>
			OWLOntology o = m.loadOntologyFromOntologyDocument(new File(a[1]));
			classify(o, a[3], a[2]);
		} else if (a[0].equals("asserted")) {
			OWLOntology o = m.loadOntologyFromOntologyDocument(new File(a[1]));
			OWLReasoner r = new StructuralReasonerFactory().createReasoner(o);
			r.precomputeInferences(InferenceType.CLASS_HIERARCHY);
			writeHierarchy(o, r, a[2]);
			r.dispose();
		} else {
			throw new IllegalArgumentException("mode must be module, classify or asserted");
		}
	}

	static void classify(OWLOntology o, String factoryClass, String out) throws Exception {
		OWLReasonerFactory f = (OWLReasonerFactory) Class.forName(factoryClass).getDeclaredConstructor().newInstance();
		long t0 = System.nanoTime();
		OWLReasoner r = f.createReasoner(o);
		r.precomputeInferences(InferenceType.CLASS_HIERARCHY);
		System.out.printf("== %s classified in %.1f s%n", r.getReasonerName(), (System.nanoTime() - t0) / 1e9);
		writeHierarchy(o, r, out);
		r.dispose();
	}

	static void writeHierarchy(OWLOntology o, OWLReasoner r, String out) throws Exception {
		TreeSet<String> lines = new TreeSet<>();
		for (OWLClass c : o.getClassesInSignature()) {
			if (c.isOWLThing() || c.isOWLNothing()) continue;
			String cn = name(c);
			for (OWLClass s : r.getSuperClasses(c, false).getFlattened()) {
				if (!s.isOWLThing()) lines.add(cn + " " + name(s));
			}
			for (OWLClass e : r.getEquivalentClasses(c).getEntities()) {
				if (!e.equals(c)) lines.add("equiv " + (cn.compareTo(name(e)) < 0 ? cn + " " + name(e) : name(e) + " " + cn));
			}
		}
		try (PrintWriter w = new PrintWriter(new FileWriter(out))) {
			for (String l : lines) w.println(l);
		}
		System.out.printf("== %d hierarchy lines written to %s%n", lines.size(), out);
	}

	static String name(OWLClass c) {
		return c.getIRI().toString().replaceAll(".*[/#]", "");
	}
}
