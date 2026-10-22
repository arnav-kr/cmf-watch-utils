import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class OtaReq extends GhidraScript {
    long[] strs = {
        0x1023fc0eL, // upgrade request: param_size %d
        0x1023fc2dL, // request upgrade cmd repeated.
        0x10240164L, // upgrade pending, skip erase parts
        0x1024092cL  // handle upgrade
    };

    public void run() throws Exception {
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long sa : strs) {
            Address addr = toAddr(sa);
            Reference[] refs = getReferencesTo(addr);
            println("STR " + addr + " '" + getString(addr) + "' refs=" + refs.length);
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

    String getString(Address a) {
        try {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < 64; i++) {
                byte b = getByte(a.add(i));
                if (b == 0) break;
                sb.append((char) (b & 0xFF));
            }
            return sb.toString();
        } catch (Exception e) { return "?"; }
    }
}
