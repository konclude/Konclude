import com.konclude.owlapi.KoncludeReasonerFactory;
import org.semanticweb.owlapi.apibinding.OWLManager;
import org.semanticweb.owlapi.model.*;
import org.semanticweb.owlapi.reasoner.*;
import java.io.*;

// the dispose probe of issue #45: create, classify and dispose a reasoner repeatedly, printing the process
// size and thread count after each dispose; with a pause file, waits before exiting
public class DisposeProbe {
	static String run(String... cmd) throws Exception {
		Process p = new ProcessBuilder(cmd).redirectErrorStream(true).start();
		BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
		StringBuilder sb = new StringBuilder(); String l;
		while ((l = r.readLine()) != null) sb.append(l).append('\n');
		p.waitFor();
		return sb.toString();
	}
	static String state() throws Exception {
		String pid = Long.toString(ProcessHandle.current().pid());
		String rss = "0"; for (String tok : run("ps", "-o", "rss=", "-p", pid).trim().split("\\s+")) if (tok.matches("\\d+")) rss = tok;
		int threads = run("ps", "-M", "-p", pid).split("\n").length - 1;
		String fp = run("footprint", "-p", pid);
		String phys = "";
		for (String line : fp.split("\n")) if (line.contains("phys_footprint:")) phys = line.trim();
		return String.format("rss=%.2fGB threads=%d %s", Long.parseLong(rss) / 1048576.0, threads, phys);
	}
	public static void main(String[] args) throws Exception {
		File file = new File(args[0]);
		int cycles = Integer.parseInt(args[1]);
		OWLOntologyManager man = OWLManager.createOWLOntologyManager();
		OWLOntology ont = man.loadOntologyFromOntologyDocument(file);
		System.out.println("loaded " + ont.getClassesInSignature().size() + " classes; " + state());
		KoncludeReasonerFactory factory = new KoncludeReasonerFactory();
		for (int c = 1; c <= cycles; c++) {
			long t = System.currentTimeMillis();
			OWLReasoner reasoner = factory.createReasoner(ont);
			reasoner.precomputeInferences(InferenceType.CLASS_HIERARCHY);
			int n = reasoner.getSubClasses(man.getOWLDataFactory().getOWLThing(), false).getFlattened().size();
			String before = state();
			reasoner.dispose();
			System.gc();
			System.out.println("cycle " + c + " (" + (System.currentTimeMillis() - t) + " ms, " + n + " subclasses of Thing) before dispose: " + before + "; after: " + state());
		}
		if (args.length > 2) {
			System.out.println("paused, pid " + ProcessHandle.current().pid());
			File pause = new File(args[2]);
			while (pause.exists()) Thread.sleep(1000);
		}
	}
}
