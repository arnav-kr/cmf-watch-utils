import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.scalar.Scalar;

public class FindImm extends GhidraScript {
    public void run() throws Exception {
        long want = 0x1dL;
        Listing listing = currentProgram.getListing();
        FunctionManager fm = currentProgram.getFunctionManager();
        int n = 0;
        for (Instruction ins : listing.getInstructions(true)) {
            for (int i = 0; i < ins.getNumOperands(); i++) {
                for (Object o : ins.getOpObjects(i)) {
                    if (o instanceof Scalar) {
                        Scalar s = (Scalar) o;
                        if (s.getUnsignedValue() == want) {
                            Function f = fm.getFunctionContaining(ins.getAddress());
                            println(String.format("%08x  %-28s  in %s", ins.getAddress().getOffset(),
                                    ins.toString(), f != null ? f.getName() : "<none>"));
                            n++;
                        }
                    }
                }
            }
        }
        println("total " + n);
    }
}
