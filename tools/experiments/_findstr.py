import sys

path = sys.argv[1] if len(sys.argv) > 1 else r"D:\upitest\firmware\tree\app.bin"
data = open(path, "rb").read()
print("file", path, "size", len(data), hex(len(data)))
for s in [b"enable no version control", b"ota_rx thread started", b"handle upgrade",
          b"wewear_ota_cmd_d2h_image_data_req", b"wewear_ota_cmd_h2d_start",
          b"unmatched board name", b"upgrade request: param_size", b"ota_upgrade_init"]:
    i = data.find(s)
    print(f"{s.decode():42s} " + (f"off={i:#x} VA={0x10000000+i:#x}" if i >= 0 else "NOT FOUND"))
