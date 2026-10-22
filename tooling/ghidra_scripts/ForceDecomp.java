import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.*;
import ghidra.app.cmd.disassemble.DisassembleCommand;

public class ForceDecomp extends GhidraScript {
    long[] entries = { 0x10156280L, 0x101562F4L, 0x10156100L, 0x10155E00L };

    public void run() throws Exception {
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (long e : entries) {
            Address a = toAddr(e);
            AddressSet set = new AddressSet(a, a.add(0x800));
            new DisassembleCommand(a, set, true).applyTo(currentProgram);
            Function f = getFunctionContaining(a);
            if (f == null) f = createFunction(a, null);
            if (f == null) { println("NO FUNC at " + a); continue; }
            println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
            DecompileResults res = di.decompileFunction(f, 120, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed: " + res.getErrorMessage());
        }
    }
}
