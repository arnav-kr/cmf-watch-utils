import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.listing.Function;
import java.util.*;

public class FindOta extends GhidraScript {
    long[] addrs = {
        0x101fd5d8L, 0x101fd5c1L, 0x101fe1ecL, 0x101fd047L, 0x101fe1cfL,
        0x101fe236L, 0x101fe25eL, 0x101fe2eaL, 0x101fe361L, 0x101fe202L,
        0x10240883L, 0x1024073fL, 0x10240756L, 0x10240861L, 0x10240a71L,
        0x10240b95L, 0x10241434L, 0x10241175L, 0x102411a0L, 0x102411cbL,
        0x102413adL, 0x102413ceL, 0x1024141cL, 0x10240322L, 0x10228af9L,
        0x10228bbdL, 0x1023fd2bL, 0x102290f1L
    };

    public void run() throws Exception {
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        LinkedHashMap<Long, Function> funcs = new LinkedHashMap<>();
        for (long a : addrs) {
            Address addr = toAddr(a);
            Reference[] refs = getReferencesTo(addr);
            if (refs.length == 0) continue;
            println("REFS -> " + addr + "  (" + refs.length + ")");
            for (Reference r : refs) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("    from " + r.getFromAddress() + "  func=" +
                        (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
                if (f != null) funcs.put(f.getEntryPoint().getOffset(), f);
            }
        }
        for (Function f : funcs.values()) {
            println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
            DecompileResults res = di.decompileFunction(f, 90, monitor);
            if (res.decompileCompleted())
                println(res.getDecompiledFunction().getC());
            else
                println("decompile failed: " + res.getErrorMessage());
        }
    }
}
