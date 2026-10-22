// Precise string -> referencing function map for selected keywords.
// @category WP2
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class FindStrings extends GhidraScript {
    private static final String[] WANT = {
        "WFManager_SetWatchFaceListForBle", "WFManager_ReplaceWatchFaceForBle",
        "sync_watch_face_info", "wewear_ota_cmd_d2h_end", "wewear_d2h_file_end",
        "wewear_new_h2d_wf_start", "wewear_yun_h2d_wf_start", "wewear_new_d2h_wf_end",
        "GETVERSION", "GETGPSVERSION", "REQ_VERSION",
        "OTA_UPG_FLAG", "OTA_UPG_TYPE", "REC_OTA_FLAG", "OTA_INFO",
        "ota_rx thread", "handle upgrade", "ota_backend_bt_init",
        "ota file_cnt", "OTA image progress",
        "system_app_launch", "ota state:", "ota type",
        "Firmware Version", "res Version", "System Version",
        "JX APP Firmware Version", "Booting Zephyr",
        "head_id", "ability_id", "cmd_id",
        "album_wf_layout_create", "wf_idx", "wf_lock_flg", "ReplaceWatchFace",
        "ui_view_creater", "jx_msgbox_creater", "lvgl_virtual_display_create",
        "AT+BRSF", "BT_HFP"
    };

    @Override
    public void run() throws Exception {
        String outDir = System.getenv("WP2_OUT");
        if (outDir == null) outDir = "out";
        File out = new File(outDir, "string_xrefs.txt");
        List<String> want = Arrays.asList(WANT);
        try (PrintWriter pw = new PrintWriter(out, "UTF-8")) {
            DataIterator it = currentProgram.getListing().getDefinedData(true);
            while (it.hasNext()) {
                Data d = it.next();
                if (d == null || d.getValue() == null) continue;
                String tn = d.getDataType().getName().toLowerCase();
                if (!tn.contains("string") && !tn.contains("unicode")) continue;
                String v = d.getValue().toString();
                boolean hit = false;
                for (String w : want) if (v.contains(w)) { hit = true; break; }
                if (!hit) continue;
                pw.println(String.format("%s\t%s", d.getAddress(), v.replace("\n", "\\n")));
                for (Reference r : getReferencesTo(d.getAddress())) {
                    Function fn = getFunctionContaining(r.getFromAddress());
                    if (fn != null)
                        pw.println(String.format("\tREF %s\t%s\t%s", r.getFromAddress(),
                                fn.getEntryPoint(), fn.getName()));
                }
            }
        }
        println("WP2_FINDSTR -> " + out);
    }
}
