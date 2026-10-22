// Decompile an explicit list of function entry addresses (env WP2_ADDRS, comma-separated).
// @category WP2
import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import java.io.*;

public class DecompileAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        String env = System.getenv("WP2_ADDRS");
        if (env == null || env.isEmpty()) { println("WP2_DECOMPAT no addresses"); return; }
        String outDir = System.getenv("WP2_OUT");
        if (outDir == null) outDir = "out";
        File dd = new File(outDir, "decomp");
        dd.mkdirs();
        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        int n = 0;
        for (String s : env.split(",")) {
            long a = Long.decode(s.trim());
            Function fn = getFunctionAt(toAddr(a));
            if (fn == null) { println("WP2_DECOMPAT missing " + s); continue; }
            DecompileResults res = ifc.decompileFunction(fn, 120, monitor);
            if (res == null || !res.decompileCompleted()) { println("WP2_DECOMPAT fail " + s); continue; }
            String fname = String.format("%08x_%s.c", a, fn.getName());
            try (PrintWriter pw = new PrintWriter(new File(dd, fname), "UTF-8")) {
                pw.println("// entry 0x" + Long.toHexString(a) + "  " + fn.getName());
                pw.println(res.getDecompiledFunction().getC());
            }
            n++;
        }
        println("WP2_DECOMPAT wrote=" + n);
    }
}
