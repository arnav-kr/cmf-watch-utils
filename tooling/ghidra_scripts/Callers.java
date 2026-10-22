import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.Reference;

public class Callers extends GhidraScript {
    public void run() throws Exception {
        long[] targets = { 0x100e9458L, 0x100e8d80L, 0x100e8c4cL, 0x100e96bcL, 0x100e8cdcL };
        FunctionManager fm = currentProgram.getFunctionManager();
        for (long t : targets) {
            println(String.format("==== callers of %08x ====", t));
            for (Reference r : getReferencesTo(toAddr(t))) {
                if (!r.getReferenceType().isCall() && !r.getReferenceType().isData())
                    continue;
                Function f = fm.getFunctionContaining(r.getFromAddress());
                println(String.format("  %s %08x  in %s", r.getReferenceType(),
                        r.getFromAddress().getOffset(), f != null ? f.getName() : "<none>"));
            }
        }
    }
}
