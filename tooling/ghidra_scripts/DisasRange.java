// Force-disassemble a Thumb code range that analysis skipped.
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;

public class DisasRange extends GhidraScript {
    @Override
    public void run() throws Exception {
        String s = System.getenv("WP2_DISAS");
        long start = 0x100E88AEL, end = 0x100EAC84L;
        if (s != null && s.contains("-")) {
            String[] p = s.split("-");
            start = Long.decode(p[0]);
            end = Long.decode(p[1]);
        }
        Address a = toAddr(start);
        Address last = toAddr(end);
        int n = 0;
        while (a.compareTo(last) < 0) {
            if (getInstructionAt(a) == null) {
                try { disassemble(a); } catch (Exception e) { }
            }
            a = a.add(2);
            n++;
            if ((n & 0x3FF) == 0) println("WP2_DISAS at " + a);
        }
        println("WP2_DISAS done " + toAddr(start) + ".." + last);
    }
}
