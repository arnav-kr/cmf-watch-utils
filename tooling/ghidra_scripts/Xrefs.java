// Dump all references to addresses in env WP2_XREFS (comma-separated).
// Prints: target, from-address, from-function, type, and the instruction text.
// Also prints the 8 bytes at the target (useful for tables).
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.mem.Memory;

public class Xrefs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String env = System.getenv("WP2_XREFS");
        if (env == null || env.isEmpty()) { println("WP2_XREFS none"); return; }
        Memory mem = currentProgram.getMemory();
        for (String s : env.split(",")) {
            long a = Long.decode(s.trim());
            Address t = toAddr(a);
            StringBuilder hex = new StringBuilder();
            for (int i = 0; i < 8; i++) {
                try { hex.append(String.format("%02x", mem.getByte(t.add(i)) & 0xff)); }
                catch (Exception e) { break; }
            }
            println(String.format("TARGET %08x bytes=%s", a, hex));
            ReferenceIterator it = currentProgram.getReferenceManager().getReferencesTo(t);
            int n = 0;
            while (it.hasNext()) {
                Reference r = it.next();
                Address from = r.getFromAddress();
                Function fn = getFunctionContaining(from);
                Instruction ins = getInstructionAt(from);
                println(String.format("  REF from=%s fn=%s type=%s ins=%s",
                        from, fn == null ? "-" : fn.getName() + "@" + fn.getEntryPoint(),
                        r.getReferenceType(), ins == null ? "-" : ins.toString()));
                n++;
            }
            println("  count=" + n);
        }
    }
}
