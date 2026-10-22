import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class Ota55 extends GhidraScript {
    void decomp(long a, DecompInterface di) throws Exception {
        Function f = getFunctionContaining(toAddr(a));
        if (f == null) { println("no func @ " + Long.toHexString(a)); return; }
        println("===== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " =====");
        DecompileResults res = di.decompileFunction(f, 180, monitor);
        if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
        else println("decompile failed");
    }

    public void run() throws Exception {
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (long a : new long[]{0x10055724L, 0x10055588L, 0x1005565cL, 0x100558b4L,
                                 0x100558dcL, 0x10055974L, 0x1005540cL, 0x10052308L,
                                 0x100522a8L, 0x100523d0L}) {
            decomp(a, di);
        }
        // who calls FUN_10055724
        Function tf = getFunctionContaining(toAddr(0x10055724L));
        if (tf != null) {
            println("CALLERS of FUN_10055724:");
            for (Reference r : getReferencesTo(tf.getEntryPoint())) {
                Function cf = getFunctionContaining(r.getFromAddress());
                println("   from " + r.getFromAddress() + " in " +
                        (cf == null ? "<none>" : cf.getName() + "@" + cf.getEntryPoint()));
            }
        }
    }
}
