// Create (if needed) and decompile functions at explicit addresses (env WP2_ADDRS).
// @category WP2
import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.io.*;

public class DecompileForce extends GhidraScript {
    @Override
    public void run() throws Exception {
        String env = System.getenv("WP2_ADDRS");
        if (env == null || env.isEmpty()) { println("WP2_FORCE no addresses"); return; }
        String outDir = System.getenv("WP2_OUT");
        if (outDir == null) outDir = "out";
        File dd = new File(outDir, "decomp");
        dd.mkdirs();
        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        int n = 0;
        for (String s : env.split(",")) {
            long a = Long.decode(s.trim());
            Address addr = toAddr(a);
            Function fn = getFunctionAt(addr);
            if (fn != null && fn.getBody().getNumAddresses() <= 2) {
                removeFunction(fn);
                fn = null;
            }
            if (fn == null) fn = getFunctionContaining(addr);
            if (fn == null) {
                try {
                    fn = createFunction(addr, null);
                } catch (Exception e) {
                    println("WP2_FORCE create fail " + s + " " + e);
                    fn = getFunctionContaining(addr);
                    if (fn == null) fn = getFunctionBefore(addr);
                }
            }
            if (fn == null) { println("WP2_FORCE missing " + s); continue; }
            DecompileResults res = ifc.decompileFunction(fn, 180, monitor);
            if (res == null || !res.decompileCompleted()) { println("WP2_FORCE fail " + s); continue; }
            String fname = String.format("%08x_%s.c", a, fn.getName());
            try (PrintWriter pw = new PrintWriter(new File(dd, fname), "UTF-8")) {
                pw.println("// entry 0x" + Long.toHexString(a) + "  " + fn.getName()
                        + "  (" + fn.getEntryPoint() + ")");
                pw.println(res.getDecompiledFunction().getC());
            }
            println("WP2_FORCE wrote " + fname + " body=" + fn.getBody().getNumAddresses());
            n++;
        }
        println("WP2_FORCE wrote=" + n);
    }
}
