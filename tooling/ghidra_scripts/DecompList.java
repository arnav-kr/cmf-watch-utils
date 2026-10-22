import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;

public class DecompList extends GhidraScript {
    public void run() throws Exception {
        long[] addrs = {
            0x100e96bcL, 0x100e9458L, 0x100e8d80L, 0x100e8c4cL, 0x100e8cdcL,
            0x100e8cdcL
        };
        FunctionManager fm = currentProgram.getFunctionManager();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (long a : addrs) {
            Function f = fm.getFunctionAt(toAddr(a));
            println("\n===== " + (f != null ? f.getName() : String.format("%08x", a)) + " =====");
            if (f == null) { println("no function"); continue; }
            DecompileResults res = di.decompileFunction(f, 60, monitor);
            if (res != null && res.getDecompiledFunction() != null) {
                println(res.getDecompiledFunction().getC());
            } else {
                println("decompile failed");
            }
        }
    }
}
