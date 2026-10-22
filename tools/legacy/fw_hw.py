import sys

TOKENS = [
    "ATS30", "ATS3", "AG3352", "AG33", "Cortex-M", "nRF5", "Apollo", "GNSS", "GPS",
    "BME280", "BME68", "LIS2", "BMI1", "BMI2", "EM7028", "ST7789", "AMOLED", "LCD",
    "FreeRTOS", "Zephyr", "RT-Thread", "NTC", "PPG", "HRV", "SpO2", "SPO2", "accel",
    "AIROHA", "Actions", "hisilicon", "sensor", "battery", "charger", "BLE", "Bluetooth",
    "openocd", "J-Link", "CMSIS", "HAL_", "SC7A20", "QMA", "MPU6", "VR5500", "ATS3089",
]


def main():
    p = sys.argv[1]
    b = open(p, "rb").read()
    print("size", len(b))
    for t in TOKENS:
        idx = b.find(t.encode())
        n = b.count(t.encode())
        if idx >= 0:
            print(f"  {t:<12} count={n:<5} first@0x{idx:x}")
    for pat in [b"CMF", b"WatchPro", b"watch_pro", b"jx402", b"3089"]:
        print(f"  {pat!r} count={b.count(pat)}")


if __name__ == "__main__":
    main()
