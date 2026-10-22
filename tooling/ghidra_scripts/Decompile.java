// Decompile functions that reference interesting strings.
// @category WP2
import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class Decompile extends GhidraScript {
    private static final String[] KEYS = {
        "system_app_launch", "ota_fg", "res_chk", "ver_chk", "ota type",
        "Booting Zephyr", "watchface", "watch_face", "wf_", "AT+GETVERSION",
        "firmware", "upgrade", "update", "version"
    };

    @Override
    public void run() throws Exception {
        String outDir = System.getenv("WP2_OUT");
        if (outDir == null) outDir = "D:\\upitest\\fwsdk\\out";
        File dd = new File(outDir, "decomp");
        dd.mkdirs();

        List<String> keys = Arrays.asList(KEYS);
        Map<Long, String> targets = new LinkedHashMap<>();
        Listing listing = currentProgram.getListing();
        DataIterator it = listing.getDefinedData(true);
        while (it.hasNext()) {
            Data d = it.next();
            if (d == null || d.getValue() == null) continue;
            String tn = d.getDataType().getName().toLowerCase();
            if (!tn.contains("string") && !tn.contains("unicode")) continue;
            String v = d.getValue().toString();
            for (String k : keys) {
                if (v.toLowerCase().contains(k.toLowerCase())) {
                    for (Reference r : getReferencesTo(d.getAddress())) {
                        Function fn = getFunctionContaining(r.getFromAddress());
                        if (fn != null && !targets.containsKey(fn.getEntryPoint().getOffset())) {
                            targets.put(fn.getEntryPoint().getOffset(),
                                    String.format("0x%x %s :: %s", d.getAddress().getOffset(),
                                            fn.getName(), v.replace("\n", "\\n")));
                        }
                    }
                    break;
                }
            }
        }
        println("WP2_DECOMP targets=" + targets.size());

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        int n = 0;
        for (Map.Entry<Long, String> e : targets.entrySet()) {
            if (n >= 60) break;
            Function fn = getFunctionAt(toAddr(e.getKey()));
            if (fn == null) continue;
            DecompileResults res = ifc.decompileFunction(fn, 90, monitor);
            if (res == null || !res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            String fname = String.format("%08x_%s.c", e.getKey(), fn.getName());
            try (PrintWriter pw = new PrintWriter(new File(dd, fname), "UTF-8")) {
                pw.println("// " + e.getValue());
                pw.println("// entry 0x" + Long.toHexString(e.getKey()) + "  " + fn.getName());
                pw.println(c);
            }
            n++;
        }
        println("WP2_DECOMP wrote=" + n);
    }
}
