import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class FindStrXrefs extends GhidraScript {
    long[] strs = {
        0x10240A71L, 0x10240B95L, 0x10241434L, 0x102413CEL, 0x10241175L,
        0x102411A0L, 0x102411CBL, 0x102413ADL, 0x1024141CL, 0x10240322L,
        0x10228AF9L, 0x102290F1L, 0x10240883L, 0x10240861L, 0x1024073FL,
        0x10240756L, 0x10240428L, 0x10241404L
    };

    public void run() throws Exception {
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long sa : strs) {
            Address addr = toAddr(sa);
            Reference[] refs = getReferencesTo(addr);
            if (refs.length == 0) continue;
            println("STR " + addr + " refs=" + refs.length);
            for (Reference r : refs) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("    from " + r.getFromAddress() + " -> " +
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
            else println("decompile failed: " + res.getErrorMessage());
        }
    }
}
