import ghidra.app.script.GhidraScript;
import ghidra.program.model.lang.Register;
import java.math.BigInteger;

public class SetThumb extends GhidraScript {
    public void run() throws Exception {
        Register r = currentProgram.getProgramContext().getRegister("TMode");
        if (r == null) {
            println("no TMode register found!");
            return;
        }
        currentProgram.getProgramContext().setValue(r,
            toAddr(0x10000000L), toAddr(0x10260000L), BigInteger.ONE);
        println("TMode set to THUMB over 0x10000000-0x10260000");
    }
}
