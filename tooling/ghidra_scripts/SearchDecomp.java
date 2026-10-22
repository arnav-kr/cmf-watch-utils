import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;

public class SearchDecomp extends GhidraScript {
    public void run() throws Exception {
        String[] toks = { "9042", "9040", "9041", "905f", "9063", "9064", "a042",
                          "a040", "a041", "a05f", "a063", "a064", "0x9052", "0xa052" };
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator it = fm.getFunctions(true);
        int total = 0, hits = 0;
        while (it.hasNext()) {
            Function f = it.next();
            total++;
            DecompileResults res = di.decompileFunction(f, 60, monitor);
            if (res == null || !res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            String low = c.toLowerCase();
            boolean hit = false;
            for (String t : toks) { if (low.contains(t)) { hit = true; break; } }
            if (hit) {
                hits++;
                println("########## HIT " + f.getName() + " @ " + f.getEntryPoint() + " ##########");
                println(c);
            }
        }
        println("scanned " + total + " functions, " + hits + " hits");
    }
}
