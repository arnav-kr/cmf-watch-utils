// Print function context at addresses (env WP2_ADDRS).
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;

public class FuncAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        String env = System.getenv("WP2_ADDRS");
        if (env == null || env.isEmpty()) { println("WP2_FUNCAT none"); return; }
        for (String s : env.split(",")) {
            Address a = toAddr(Long.decode(s.trim()));
            Function at = getFunctionAt(a);
            Function in = getFunctionContaining(a);
            Function before = getFunctionBefore(a);
            println(String.format("%s at=%s in=%s before=%s ins=%s",
                a,
                at==null?"-":at.getName()+"@"+at.getEntryPoint()+" body="+at.getBody().getNumAddresses(),
                in==null?"-":in.getName()+"@"+in.getEntryPoint()+" body="+in.getBody().getNumAddresses(),
                before==null?"-":before.getName()+"@"+before.getEntryPoint(),
                getInstructionAt(a)==null?"-":getInstructionAt(a).toString()));
        }
    }
}