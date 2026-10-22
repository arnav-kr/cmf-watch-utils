import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.util.*;

public class OtaStrRefs extends GhidraScript {
    public void run() throws Exception {
        LinkedHashMap<String, Function> all = new LinkedHashMap<>();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        String[] strs = {
            "wewear_ota_cmd_d2h_image_data_req",
            "wewear_ota_cmd_h2d_start",
            "wewear_ota_cmd_d2h_end",
            "ota_backend_bt_init",
            "ota_storage_write_sd_nand",
            "ota_storage_erase_sd_nand",
            "ota_rx thread started",
            "handle upgrade",
            "ota_upgrade_init",
            "enable no version control",
        };
        for (String s : strs) {
            StringBuilder hex = new StringBuilder();
            for (byte b : s.getBytes()) hex.append(String.format("%02x", b & 0xFF));
            println("STRING '" + s + "' len=" + s.length());
            for (ghidra.program.model.mem.MemoryBlock blk : currentProgram.getMemory().getBlocks()) {
                Address[] hits = findBytes(blk.getStart(), hex.toString(), 64);
                for (Address found : hits) {
                    println("  @ " + found + " (" + blk.getName() + ")");
                    for (Reference r : getReferencesTo(found)) {
                        Function f = getFunctionContaining(r.getFromAddress());
                        println("    from " + r.getFromAddress() + " -> " +
                                (f == null ? "<none>" : f.getName() + "@" + f.getEntryPoint()));
                        if (f != null) all.put(f.getEntryPoint().toString(), f);
                    }
                }
            }
        }
        for (Function f : all.values()) {
            println("===== DECOMP " + f.getName() + " @" + f.getEntryPoint() + " =====");
            DecompileResults res = di.decompileFunction(f, 120, monitor);
            if (res.decompileCompleted()) println(res.getDecompiledFunction().getC());
            else println("decompile failed");
        }
    }
}
