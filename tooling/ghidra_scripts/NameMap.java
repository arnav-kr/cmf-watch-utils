// Name functions from embedded source paths / keywords and export a subsystem map.
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class NameMap extends GhidraScript {
    private static final String[][] SUBSYS = {
        {"ota", "ota"}, {"upgrade", "ota"}, {"update", "ota"}, {"firmware", "ota"}, {"dfu", "ota"},
        {"bt_", "bt"}, {"ble", "bt"}, {"gatt", "bt"}, {"hfp", "bt"}, {"a2dp", "bt"}, {"spp", "bt"},
        {"at+", "bt"}, {"pair", "bt"}, {"adv", "bt"},
        {"lvgl", "ui"}, {"display", "ui"}, {"screen", "ui"}, {"watchface", "ui"}, {"watch_face", "ui"},
        {"ui_", "ui"}, {"msgbox", "ui"}, {"layout", "ui"},
        {"audio", "audio"}, {"voice", "audio"}, {"mic", "audio"}, {"speaker", "audio"},
        {"sensor", "sensor"}, {"accel", "sensor"}, {"ppg", "sensor"}, {"heart", "sensor"}, {"step", "sensor"},
        {"sdfs", "fs"}, {"flash", "fs"}, {"ef_", "fs"}, {"file", "fs"},
        {"power", "power"}, {"battery", "power"}, {"sleep", "power"},
        {"zephyr", "kernel"}, {"k_", "kernel"}, {"thread", "kernel"}, {"timer", "kernel"}
    };

    @Override
    public void run() throws Exception {
        String outDir = System.getenv("WP2_OUT");
        if (outDir == null) outDir = "out";
        File dd = new File(outDir);

        Map<Address, String> strs = new HashMap<>();
        DataIterator it = currentProgram.getListing().getDefinedData(true);
        while (it.hasNext()) {
            Data d = it.next();
            if (d == null || d.getValue() == null) continue;
            String tn = d.getDataType().getName().toLowerCase();
            if (tn.contains("string") || tn.contains("unicode"))
                strs.put(d.getAddress(), d.getValue().toString());
        }
        println("WP2_NAMEMAP strings=" + strs.size());

        Map<Function, List<String>> ev = new LinkedHashMap<>();
        ReferenceIterator refs = currentProgram.getReferenceManager()
                .getReferenceIterator(currentProgram.getMinAddress());
        while (refs.hasNext()) {
            Reference r = refs.next();
            String s = strs.get(r.getToAddress());
            if (s == null) continue;
            Function fn = getFunctionContaining(r.getFromAddress());
            if (fn == null) continue;
            ev.computeIfAbsent(fn, k -> new ArrayList<>()).add(s);
        }
        println("WP2_NAMEMAP funcs_with_strings=" + ev.size());

        Set<String> used = new HashSet<>();
        for (Function fn : currentProgram.getFunctionManager().getFunctions(true))
            used.add(fn.getName());

        int renamed = 0, rows = 0;
        File tsv = new File(dd, "function_map.tsv");
        try (PrintWriter pw = new PrintWriter(tsv, "UTF-8")) {
            pw.println("entry\tsize\tname\tsubsystems\tsource_files\tevidence");
            for (Function fn : currentProgram.getFunctionManager().getFunctions(true)) {
                List<String> e = ev.getOrDefault(fn, Collections.emptyList());
                String src = null, evidence = null;
                List<String> srcs = new ArrayList<>();
                for (String s : e) {
                    if (s.contains("WEST_TOPDIR/") && s.endsWith(".c")) srcs.add(s);
                    if (evidence == null && s.length() > 3 && s.length() < 90) evidence = s;
                }
                if (!srcs.isEmpty()) src = srcs.get(0).substring(srcs.get(0).lastIndexOf('/') + 1);
                TreeSet<String> sys = new TreeSet<>();
                for (String s : e) {
                    String l = s.toLowerCase();
                    for (String[] kv : SUBSYS)
                        if (l.contains(kv[0])) sys.add(kv[1]);
                }
                if (!fn.getName().startsWith("vec_") && src != null) {
                    String base = src.replace(".c", "").replaceAll("[^A-Za-z0-9_]", "_");
                    if (!Character.isLetter(base.charAt(0))) base = "f_" + base;
                    String cand = base;
                    int k = 1;
                    while (used.contains(cand)) cand = base + "_" + (k++);
                    try {
                        fn.setName(cand, SourceType.USER_DEFINED);
                        used.add(cand);
                        renamed++;
                    } catch (Exception ex) { }
                }
                pw.println(String.format("0x%x\t%d\t%s\t%s\t%s\t%s",
                        fn.getEntryPoint().getOffset(), fn.getBody().getNumAddresses(),
                        fn.getName(), String.join(",", sys),
                        String.join("|", srcs),
                        evidence == null ? "" : evidence.replace("\t", " ").replace("\n", "\\n")));
                rows++;
            }
        }
        println("WP2_NAMEMAP renamed=" + renamed + " rows=" + rows);
    }
}
