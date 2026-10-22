import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.Reference;
import java.util.TreeMap;

public class AttrRefs extends GhidraScript {
    public void run() throws Exception {
        long lo = 0x10253300L, hi = 0x10253600L;
        FunctionManager fm = currentProgram.getFunctionManager();
        TreeMap<Long, String> out = new TreeMap<>();
        for (long a = lo; a < hi; a++) {
            Address addr = toAddr(a);
            for (Reference r : getReferencesTo(addr)) {
                Address from = r.getFromAddress();
                Function f = fm.getFunctionContaining(from);
                String fn = f != null ? f.getName() : "<none>";
                out.put(from.getOffset(), String.format("  %08x -> %08x %s  in %s", from.getOffset(), a, r.getReferenceType(), fn));
            }
        }
        for (String s : out.values()) println(s);
    }
}
