import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.*;
import ghidra.app.cmd.function.CreateFunctionCmd;
import java.util.*;

public class DecompRange extends GhidraScript {
    long RANGE_S = 0x10152000L, RANGE_E = 0x10157000L;
    long[] targets = { 0x101562e8L, 0x10155fbcL, 0x10156270L, 0x10153b60L };

    public void run() throws Exception {
        AddressSet set = new AddressSet(toAddr(RANGE_S), toAddr(RANGE_E));

        Listing lst = currentProgram.getListing();
        List<Address> orphan = new ArrayList<>();
        InstructionIterator it = lst.getInstructions(set, true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            if (lst.getFunctionContaining(ins.getAddress()) == null)
                orphan.add(ins.getAddress());
        }
        println("orphan instrs in range: " + orphan.size());
        int made = 0;
        for (Address a : orphan) {
            if (lst.getFunctionContaining(a) != null) continue;
            CreateFunctionCmd cmd = new CreateFunctionCmd(a);
            if (cmd.applyTo(currentProgram)) made++;
        }
        println("functions created: " + made);

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long a : targets) {
            Function f = getFunctionContaining(toAddr(a));
            println("target " + toAddr(a) + " -> " + (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
            if (f != null) funcs.put(f.getEntryPoint().toString(), f);
        }
        for (Function f : funcs.values()) {
            println("========== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " ==========");
            DecompileResults res = di.decompileFunction(f, 90, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed: " + res.getErrorMessage());
        }
    }
}
