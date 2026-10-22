import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class OtaStrings extends GhidraScript {
    long[] strs = {
        0x102408a9L, // ota_rx thread started
        0x102408d0L, // req_size:%d, rx_info->seg_size:%d, rlen:%d
        0x102408fbL, // cannot read data, offs 0x%x
        0x1024093bL, // ota state <%d> is not OTA_INIT, skip upgrade
        0x10240d92L, // ota_rx_init
        0x10240d9eL, // ota_upgrade_check
        0x10240db0L, // ota_upgrade_init
        0x10240e43L, // OTA upgrade progress ==> %d%%
        0x101fe202L  // OTA_UPG_FLAG
    };

    public void run() throws Exception {
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long sa : strs) {
            Address addr = toAddr(sa);
            Reference[] refs = getReferencesTo(addr);
            println("STR " + addr + " refs=" + refs.length);
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
            DecompileResults res = di.decompileFunction(f, 180, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed");
        }
    }
}
