import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import java.util.*;

public class Ops42 extends GhidraScript {
    public void run() throws Exception {
        long[] ops = {0xa040L, 0xa041L, 0xa042L, 0xa043L, 0xa044L, 0xa052L,
                      0x9040L, 0x9041L, 0x9042L, 0x9052L, 0x10000L};
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        InstructionIterator it = currentProgram.getListing().getInstructions(true);
        while (it.hasNext()) {
            Instruction ins = it.next();
            for (int op = 0; op < ins.getNumOperands(); op++) {
                for (Object o : ins.getOpObjects(op)) {
                    if (o instanceof Scalar) {
                        long v = ((Scalar) o).getUnsignedValue();
                        for (long t : ops) {
                            if (v == t) {
                                Function f = getFunctionContaining(ins.getAddress());
                                println("IMM " + Long.toHexString(v) + " @" + ins.getAddress() +
                                        " in " + (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
                                if (f != null) funcs.put(f.getEntryPoint().toString(), f);
                            }
                        }
                    }
                }
            }
        }
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (Function f : funcs.values()) {
            println("===== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " =====");
            DecompileResults res = di.decompileFunction(f, 180, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed");
        }
    }
}
