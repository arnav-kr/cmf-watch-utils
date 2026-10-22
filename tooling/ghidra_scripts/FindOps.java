import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class FindOps extends GhidraScript {
    long[] ops = {0x9052L, 0x9040L, 0x9041L, 0x9042L, 0x905fL, 0x905eL};
    LinkedHashMap<String, Function> allFuncs = new LinkedHashMap<>();

    void dumpFuncs(String label, Collection<Function> fs) throws Exception {
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (Function f : fs) {
            println("===== " + label + " DECOMP " + f.getName() + " @" + f.getEntryPoint() + " =====");
            DecompileResults res = di.decompileFunction(f, 120, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed");
        }
    }

    void searchString(String s) {
        StringBuilder hex = new StringBuilder();
        for (byte b : s.getBytes()) hex.append(String.format("%02x", b & 0xFF));
        Address[] hits = findBytes(toAddr(0x10000000L), hex.toString(), 16);
        for (Address found : hits) {
            println("FOUND '" + s + "' @ " + found);
            for (Reference r : getReferencesTo(found)) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("    from " + r.getFromAddress() + " -> " +
                        (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
                if (f != null) allFuncs.put(f.getEntryPoint().toString(), f);
            }
        }
    }

    public void run() throws Exception {
        for (String s : new String[]{"upgrade request: param_size", "request upgrade cmd repeated",
                                     "OTA_UPG_FLAG", "ota_upgrade_init", "ota_upgrade_check"}) {
            searchString(s);
        }

        println("---- immediate opcode scan ----");
        LinkedHashMap<String, Function> ofuncs = new LinkedHashMap<>();
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
                                        " in " + (f == null ? "<none>" : f.getName()));
                                if (f != null) ofuncs.put(f.getEntryPoint().toString(), f);
                            }
                        }
                    }
                }
            }
        }
        dumpFuncs("STRING", allFuncs.values());
        dumpFuncs("OPCODE", ofuncs.values());
    }
}
