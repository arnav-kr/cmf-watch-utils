import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;

public class RefsToAddr extends GhidraScript {
    public void run() throws Exception {
        long[] targets = { 0x100fb660L, 0x100fb57cL, 0x100fb2b0L, 0x100fb2a4L };
        ReferenceManager rm = currentProgram.getReferenceManager();
        for (long t : targets) {
            println(String.format("==== refs to %08x ====", t));
            Address a = toAddr(t);
            ReferenceIterator refs = rm.getReferencesTo(a);
            while (refs.hasNext()) {
                Reference r = refs.next();
                Address from = r.getFromAddress();
                Function f = getFunctionContaining(from);
                println(String.format("  %08x  %-24s  %s", from.getOffset(),
                        f != null ? f.getName() : "<none>", r.getReferenceType()));
            }
        }
    }
}
