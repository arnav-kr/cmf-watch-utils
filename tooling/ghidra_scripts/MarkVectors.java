// Seed the Cortex-M vector table so Ghidra analyzes the whole image.
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.Memory;

public class MarkVectors extends GhidraScript {
    @Override
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        Address base = toAddr(0x10000200L);
        int seeded = 0;
        for (int i = 1; i <= 80; i++) {
            long v = mem.getInt(base.add(i * 4L)) & 0xFFFFFFFFL;
            if ((v & 1L) == 0 || v < 0x10000000L || v > 0x10260000L) {
                continue;
            }
            Address a = toAddr(v & ~1L);
            if (!mem.contains(a)) {
                continue;
            }
            try {
                if (getFunctionAt(a) == null) {
                    createFunction(a, "vec_" + Integer.toHexString(i));
                }
                disassemble(a);
                seeded++;
            } catch (Exception e) {
                println("vector " + i + " failed: " + e);
            }
        }
        println("WP2_VECTORS seeded=" + seeded);
    }
}
