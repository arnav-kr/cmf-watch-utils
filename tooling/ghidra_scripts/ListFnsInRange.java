import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;

public class ListFnsInRange extends GhidraScript {
    public void run() throws Exception {
        FunctionManager fm = currentProgram.getFunctionManager();
        long[][] ranges = {
            {0x10195a00L, 0x10195f00L}
        };
        for (long[] r : ranges) {
            println(String.format("---- %08x .. %08x ----", r[0], r[1]));
            Address lo = toAddr(r[0]);
            Address hi = toAddr(r[1]);
            FunctionIterator it = fm.getFunctions(lo, true);
            while (it.hasNext()) {
                Function f = it.next();
                long a = f.getEntryPoint().getOffset();
                if (a > r[1]) break;
                println(String.format("%08x  %-24s  body=%d", a, f.getName(), f.getBody().getNumAddresses()));
            }
        }
    }
}
