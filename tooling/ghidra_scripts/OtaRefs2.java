import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class OtaRefs2 extends GhidraScript {
    public void run() throws Exception {
        long[] targets = {
            0x1023fc0eL, // upgrade request: param_size %d
            0x1023fc2dL, // request upgrade cmd repeated.
            0x1023fd70L, // wewear_ota_cmd_d2h_image_data_req
            0x1023feaeL, // wewear_ota_cmd_h2d_start
            0x1023fec7L, // wewear_ota_cmd_d2h_end
            0x1023fedeL, // ota_backend_bt_init
            0x1023ff90L, // ota_storage_erase_sd_nand
            0x1023ffaaL, // ota_storage_write_sd_nand
            0x102408a9L, // ota_rx thread started
            0x1024092cL, // handle upgrade
            0x1024093bL, // ota state not OTA_INIT
            0x10240c8dL, // ota status -> OTA_INIT
            0x10240db0L, // ota_upgrade_init
            0x10240d23L, // enable no version control
        };
        LinkedHashMap<String, Function> funcs = new LinkedHashMap<>();
        for (long t : targets) {
            Address addr = toAddr(t);
            Reference[] refs = getReferencesTo(addr);
            println("TARGET " + addr + " refs=" + refs.length);
            for (Reference r : refs) {
                Function f = getFunctionContaining(r.getFromAddress());
                println("    " + r.getReferenceType() + " from " + r.getFromAddress() + " -> " +
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
            else println("decompile failed");
        }
    }
}
