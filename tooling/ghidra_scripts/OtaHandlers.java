import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class OtaHandlers extends GhidraScript {
    void decomp(Function f, DecompInterface di) throws Exception {
        if (f == null) { println("  <null function>"); return; }
        println("===== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " =====");
        DecompileResults res = di.decompileFunction(f, 180, monitor);
        if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
        else println("decompile failed");
    }

    public void run() throws Exception {
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        // function containing the h2d_start string ref
        Address a = toAddr(0x1015238cL);
        Function f = getFunctionContaining(a);
        println("function containing 0x1015238c: " + (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
        decomp(f, di);

        // callers of the transmit helper used by the finish handler
        for (long tgt : new long[]{0x10152184L, 0x1015240cL, 0x10152af0L}) {
            Function tf = getFunctionContaining(toAddr(tgt));
            println("CALLERS of " + Long.toHexString(tgt) + " (" + (tf == null ? "?" : tf.getName()) + ")");
            if (tf == null) continue;
            for (Reference r : getReferencesTo(tf.getEntryPoint())) {
                Function cf = getFunctionContaining(r.getFromAddress());
                println("   from " + r.getFromAddress() + " in " +
                        (cf == null ? "<none>" : cf.getName() + "@" + cf.getEntryPoint()));
            }
        }
        // decompile the transmit helper itself
        decomp(getFunctionContaining(toAddr(0x10152184L)), di);
    }
}
