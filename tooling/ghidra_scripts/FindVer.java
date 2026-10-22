import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import java.util.*;

public class FindVer extends GhidraScript {
    long RANGE_S = 0x10153000L, RANGE_E = 0x10157800L;
    long[] poolAddrs = {
        0x10154DE8L, 0x10154E30L, 0x101562E8L, 0x10156270L,
        0x101562E4L, 0x10155FBCL, 0x10155FC4L, 0x10153B60L,
        0x101541D8L, 0x101541DCL
    };

    public void run() throws Exception {
        AddressSet set = new AddressSet(toAddr(RANGE_S), toAddr(RANGE_E));
        int n0 = 0;
        InstructionIterator it0 = currentProgram.getListing().getInstructions(set, true);
        while (it0.hasNext()) { it0.next(); n0++; }
        println("instrs before clear=" + n0);
        clearListing(toAddr(RANGE_S), toAddr(RANGE_E));
        new DisassembleCommand(toAddr(RANGE_S), set, true).applyTo(currentProgram);
        println("disassembled region");

        Listing lst = currentProgram.getListing();
        List<Address> orphan = new ArrayList<>();
        InstructionIterator it = lst.getInstructions(set, true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            if (lst.getFunctionContaining(ins.getAddress()) == null)
                orphan.add(ins.getAddress());
        }
        int made = 0;
        for (Address a : orphan) {
            if (lst.getFunctionContaining(a) != null) continue;
            if (new CreateFunctionCmd(a).applyTo(currentProgram)) made++;
        }
        println("orphan=" + orphan.size() + " created=" + made);

        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long pa : poolAddrs) {
            Reference[] refs = getReferencesTo(toAddr(pa));
            if (refs.length == 0) continue;
            println("pool " + toAddr(pa) + " refs=" + refs.length);
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
}
