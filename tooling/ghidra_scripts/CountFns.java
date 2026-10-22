import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;

public class CountFns extends GhidraScript {
    public void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator it = fm.getFunctions(true);
        int total = 0, thunks = 0, named = 0, external = 0;
        while (it.hasNext()) {
            Function f = it.next();
            total++;
            if (f.isThunk()) thunks++;
            if (f.isExternal()) external++;
            if (f.getName().startsWith("FUN_")) { } else named++;
        }
        println("total functions: " + total);
        println("thunks: " + thunks);
        println("external: " + external);
        println("non-FUN_ names: " + named);
        println("functions with body: " + (total - external));
    }
}
