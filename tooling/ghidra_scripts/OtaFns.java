import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.util.*;

public class OtaFns extends GhidraScript {
    long S = 0x10153E00L, E = 0x10157400L;
    String[] kw = { "verify", "version", "checksum", "part ", "lzma", "ota", "OTA", "file " };

    public void run() throws Exception {
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        FunctionManager fm = currentProgram.getFunctionManager();
        int total = 0, printed = 0;
        for (Function f : fm.getFunctions(true)) {
            long ep = f.getEntryPoint().getOffset();
            if (ep < S || ep > E) continue;
            total++;
            DecompileResults res = di.decompileFunction(f, 120, monitor);
            if (!res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            boolean hit = false;
            for (String k : kw) if (c.contains(k)) { hit = true; break; }
            if (true) {
                printed++;
                println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
                println(c);
            }
        }
        println("total fns in range=" + total + " printed=" + printed);
    }
}
