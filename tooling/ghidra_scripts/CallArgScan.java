import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;

public class CallArgScan extends GhidraScript {
    public void run() throws Exception {
        long[] targets = { 0x10142c28L, 0x10145188L, 0x10142d04L, 0x10145188L };
        long want = 0x1dL;
        Listing listing = currentProgram.getListing();
        ReferenceManager rm = currentProgram.getReferenceManager();
        Register r0 = currentProgram.getRegister("r0");
        for (long t : targets) {
            println(String.format("==== calls to %08x with r0=0x1d ====", t));
            Address a = toAddr(t);
            ReferenceIterator it = rm.getReferencesTo(a);
            while (it.hasNext()) {
                Reference r = it.next();
                if (!r.getReferenceType().isCall()) continue;
                Address callAddr = r.getFromAddress();
                Instruction call = listing.getInstructionAt(callAddr);
                if (call == null) continue;
                // walk backward
                Instruction cur = call.getPrevious();
                int steps = 0;
                boolean found = false;
                while (cur != null && steps < 14) {
                    String m = cur.getMnemonicString();
                    if ((m.equals("mov") || m.equals("movs") || m.equals("movw")) &&
                        cur.getNumOperands() >= 1 &&
                        cur.getRegister(0) != null && cur.getRegister(0).equals(r0)) {
                        for (Object o : cur.getOpObjects(1)) {
                            if (o instanceof Scalar) {
                                Scalar s = (Scalar) o;
                                if ((s.getUnsignedValue() & 0xffffffffL) == want) found = true;
                            }
                        }
                        break;
                    }
                    cur = cur.getPrevious();
                    steps++;
                }
                if (found) {
                    Function f = getFunctionContaining(callAddr);
                    println(String.format("  %08x  in %s", callAddr.getOffset(),
                            f != null ? f.getName() : "<none>"));
                }
            }
        }
    }
}
