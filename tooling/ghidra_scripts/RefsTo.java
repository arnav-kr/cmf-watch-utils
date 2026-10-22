import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class RefsTo extends GhidraScript {
    long[] targets = {0x10055720L, 0x1005571cL, 0x10055714L, 0x10055700L,
                      0x1005570cL, 0x10055710L, 0x10055704L, 0x10055708L, 0x10055718L};

    public void run() throws Exception {
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long t : targets) {
            Address addr = toAddr(t);
            Reference[] refs = getReferencesTo(addr);
            println("TARGET " + addr + " refs=" + refs.length);
            for (Reference r : refs) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("    " + r.getReferenceType() + " from " + r.getFromAddress() + " -> " +
                        (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
                if (f != null) funcs.put(f.getEntryPoint().toString(), f);
            }
        }
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (Function f : funcs.values()) {
            println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
            DecompileResults res = di.decompileFunction(f, 120, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed");
        }
    }
}
