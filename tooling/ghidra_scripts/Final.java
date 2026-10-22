import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import ghidra.app.plugin.core.analysis.AutoAnalysisManager;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import java.util.*;

public class Final extends GhidraScript {
    long S = 0x10153C00L, E = 0x10157000L;
    long[] entries = { 0x10155ED4L, 0x10155FD8L };
    long[] targets = {
        0x10154DE8L, 0x10154E30L, 0x101541D4L, 0x101541D8L, 0x101541DCL,
        0x101545C4L, 0x101545D0L, 0x101545BCL, 0x101545CCL, 0x101545E8L,
        0x10240A71L, 0x10240B95L, 0x1024073FL, 0x10240756L, 0x10240883L,
        0x10240861L, 0x10241434L, 0x102413CEL, 0x1024140BL
    };

    public void run() throws Exception {
        AddressSet set = new AddressSet(toAddr(S), toAddr(E));
        new DisassembleCommand(toAddr(S), set, true).applyTo(currentProgram);
        AutoAnalysisManager aam = AutoAnalysisManager.getAnalysisManager(currentProgram);
        aam.reAnalyzeAll(set);
        aam.startAnalysis(monitor);
        println("re-analyzed region");

        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long t : targets) {
            Reference[] refs = getReferencesTo(toAddr(t));
            if (refs.length == 0) continue;
            println("TGT " + toAddr(t) + " refs=" + refs.length);
            for (Reference r : refs) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("    from " + r.getFromAddress() + " -> " +
                        (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
                if (f != null) funcs.put(f.getEntryPoint().toString(), f);
            }
        }

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        Listing lst = currentProgram.getListing();
        for (long e : entries) {
            Function f = getFunctionContaining(toAddr(e));
            if (f == null) {
                new DisassembleCommand(toAddr(e), new AddressSet(toAddr(e), toAddr(e).add(0x400)), true)
                        .applyTo(currentProgram);
                f = createFunction(toAddr(e), null);
            }
            if (f != null) funcs.put(f.getEntryPoint().toString(), f);
        }
        for (Function f : funcs.values()) {
            println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
            DecompileResults res = di.decompileFunction(f, 150, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed: " + res.getErrorMessage());
        }
    }
}
