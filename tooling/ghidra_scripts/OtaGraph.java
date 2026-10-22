import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.RefType;
import java.util.TreeSet;

public class OtaGraph extends GhidraScript {
    public void run() throws Exception {
        long lo = 0x10152000L, hi = 0x10156000L;
        AddressSet set = new AddressSet(toAddr(lo), toAddr(hi));
        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator it = fm.getFunctions(set, true);
        while (it.hasNext()) {
            Function f = it.next();
            StringBuilder sb = new StringBuilder();
            sb.append(String.format("%s @%08x size=%d calls:", f.getName(),
                    f.getEntryPoint().getOffset(), f.getBody().getNumAddresses()));
            TreeSet<String> callees = new TreeSet<>();
            InstructionIterator ii = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (ii.hasNext()) {
                Instruction ins = ii.next();
                for (Reference r : ins.getReferencesFrom()) {
                    RefType rt = r.getReferenceType();
                    if (rt.isCall()) {
                        Function g = fm.getFunctionAt(r.getToAddress());
                        if (g != null) callees.add(g.getName());
                        else callees.add(String.format("%08x", r.getToAddress().getOffset()));
                    }
                }
            }
            for (String c : callees) sb.append(" ").append(c);
            println(sb.toString());
        }
        println("==== callers of 101545e8 (ota_rx) ====");
        for (Reference r : getReferencesTo(toAddr(0x101545e8L))) {
            println(String.format("  %s from %08x", r.getReferenceType(),
                    r.getFromAddress().getOffset()));
        }
        println("==== callers of 10154790 (handle upgrade) ====");
        for (Reference r : getReferencesTo(toAddr(0x10154790L))) {
            println(String.format("  %s from %08x", r.getReferenceType(),
                    r.getFromAddress().getOffset()));
        }
    }
}
