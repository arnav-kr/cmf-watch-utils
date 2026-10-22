// Export functions, strings and memory blocks from the analyzed WP2 app.bin
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class DumpApp extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = System.getenv("WP2_OUT");
        if (outDir == null) outDir = "D:\\upitest\\fwsdk\\out";
        File fdir = new File(outDir);
        fdir.mkdirs();

        try (PrintWriter pw = new PrintWriter(new File(fdir, "blocks.txt"), "UTF-8")) {
            for (MemoryBlock b : currentProgram.getMemory().getBlocks())
                pw.println(b.getName() + " " + b.getStart() + "-" + b.getEnd()
                        + " r=" + b.isRead() + " w=" + b.isWrite() + " x=" + b.isExecute()
                        + " init=" + b.isInitialized());
        }

        FunctionManager fm = currentProgram.getFunctionManager();
        int n = 0;
        try (PrintWriter pw = new PrintWriter(new File(fdir, "functions.txt"), "UTF-8")) {
            for (Function fn : fm.getFunctions(true)) {
                pw.println(fn.getEntryPoint() + "\t" + fn.getBody().getNumAddresses() + "\t" + fn.getName());
                n++;
            }
        }

        int ns = 0;
        try (PrintWriter pw = new PrintWriter(new File(fdir, "strings.txt"), "UTF-8")) {
            DataIterator it = currentProgram.getListing().getDefinedData(true);
            while (it.hasNext()) {
                Data d = it.next();
                if (d == null || d.getValue() == null) continue;
                String tn = d.getDataType().getName().toLowerCase();
                if (tn.contains("string") || tn.contains("unicode")) {
                    pw.println(d.getAddress() + "\t" + d.getValue().toString().replace("\n", "\\n"));
                    ns++;
                }
            }
        }
        println("WP2_DUMP functions=" + n + " strings=" + ns);
    }
}
