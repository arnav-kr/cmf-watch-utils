import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.util.*;

public class DecompAt extends GhidraScript {
    long[] addrs = {0x100523d0L, 0x10155354L};

    public void run() throws Exception {
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long a : addrs) {
            Address addr = toAddr(a);
            Function f = getFunctionContaining(addr);
            if (f == null) {
                for (ghidra.program.model.symbol.Reference r : getReferencesTo(addr)) {
                    Function g = getFunctionContaining(r.getFromAddress());
                    println("LIT " + addr + " loaded at " + r.getFromAddress() + " -> " +
                            (g == null ? "<none>" : g.getName() + "@" + g.getEntryPoint()));
                    if (g != null) funcs.put(g.getEntryPoint().toString(), g);
                }
            } else {
                println("ADDR " + Long.toHexString(a) + " in " + f.getName() + "@" + f.getEntryPoint());
                funcs.put(f.getEntryPoint().toString(), f);
            }
        }
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (Function f : funcs.values()) {
            println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
            DecompileResults res = di.decompileFunction(f, 180, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed");
        }
    }
}
