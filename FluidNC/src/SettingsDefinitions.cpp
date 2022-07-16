#include "SettingsDefinitions.h"
#include "Logging.h"

StringSetting* config_filename;

StringSetting* build_info;

StringSetting* start_message;

IntSetting* status_mask;

EnumSetting* message_level;

enum_opt_t messageLevels = {
    // clang-format off
    { "None", MsgLevelNone },
    { "Error", MsgLevelError },
    { "Warning", MsgLevelWarning },
    { "Info", MsgLevelInfo },
    { "Debug", MsgLevelDebug },
    { "Verbose", MsgLevelVerbose },
    // clang-format on
};

enum_opt_t onoffOptions = { { "OFF", 0 }, { "ON", 1 } };

void make_coordinate(CoordIndex index, const char* name) {
    float coord_data[MAX_N_AXIS] = { 0.0 };
    auto  coord                  = new Coordinates(name);
    coords[index]                = coord;
    if (!coord->load()) {
        coords[index]->setDefault();
    }
}

void make_settings() {
    Setting::init();

    // Propagate old coordinate system data to the new format if necessary.
    // G54 - G59 work coordinate systems, G28, G30 reference positions, etc
    make_coordinate(CoordIndex::G54, "G54");
    make_coordinate(CoordIndex::G55, "G55");
    make_coordinate(CoordIndex::G56, "G56");
    make_coordinate(CoordIndex::G57, "G57");
    make_coordinate(CoordIndex::G58, "G58");
    make_coordinate(CoordIndex::G59, "G59");
    make_coordinate(CoordIndex::G28, "G28");
    make_coordinate(CoordIndex::G30, "G30");

    message_level = new EnumSetting("Which Messages", EXTENDED, WG, NULL, "Message/Level", MsgLevelInfo, &messageLevels, NULL);

    config_filename = new StringSetting("Name of Configuration File", EXTENDED, WG, NULL, "Config/Filename", "config.yaml", 1, 50, NULL);

    // GRBL Numbered Settings
    new IntSetting("Step pulse time, microseconds", GRBL, WG, "0", "Grbl/0", 4, 1, 10, NULL);
    new IntSetting("Step idle delay, milliseconds", GRBL, WG, "1", "Grbl/1", 255, 0, 1023, NULL);
    new IntSetting("Step pulse invert, mask", GRBL, WG, "2", "Grbl/2", 0, 0, 0xFFFF, NULL);
    new IntSetting("Step direction invert, mask", GRBL, WG, "3", "Grbl/3", 0, 0, 0xFFFF, NULL);
    new IntSetting("Invert step enable pin, boolean", GRBL, WG, "4", "Grbl/4", 0, 0, 1, NULL);
    new IntSetting("Invert limit pins, boolean", GRBL, WG, "5", "Grbl/5", 0, 0, 1, NULL);
    new IntSetting("Invert probe pin, boolean", GRBL, WG, "6", "Grbl/6", 0, 0, 1, NULL);
    status_mask = new IntSetting("What to include in status report", GRBL, WG, "10", "Report/Status", 1, 0, 3, NULL);
    new StringSetting("Junction deviation, millimeters", GRBL, WG, "11", "Grbl/11", "0.010", 1, 50, NULL);
    new StringSetting("Arc tolerance, millimeters", GRBL, WG, "12", "Grbl/12", "0.02", 1, 50, NULL);
    // new IntSetting("Report in inches, boolean", GRBL, WG, "13", "Grbl/13", 0, 0, 1, NULL);
    new IntSetting("Soft limits enable, boolean", GRBL, WG, "20", "Grbl/20", 1, 0, 1, NULL);
    new IntSetting("Hard limits enable, boolean", GRBL, WG, "21", "Grbl/21", 1, 0, 1, NULL);
    new IntSetting("Homing cycle enable, boolean", GRBL, WG, "22", "Grbl/22", 1, 0, 1, NULL);
    new IntSetting("Homing direction invert, mask", GRBL, WG, "23", "Grbl/23", 3, 0, 0xFFFF, NULL);
    new StringSetting("Homing locate feed rate, mm/min", GRBL, WG, "24", "Grbl/24", "100", 1, 50, NULL);
    new StringSetting("Homing search seek rate, mm/min", GRBL, WG, "25", "Grbl/25", "1200", 1, 50, NULL);
    new IntSetting("Homing switch debounce delay, milliseconds", GRBL, WG, "26", "Grbl/26", 500, 0, 100000, NULL);
    new StringSetting("Homing switch pull-off distance, millimeters", GRBL, WG, "27", "Grbl/27", "2", 1, 50, NULL);
    new StringSetting("Maximum spindle speed, RPM", GRBL, WG, "30", "Grbl/30", "1000", 1, 50, NULL);
    new StringSetting("Minimum spindle speed, RPM", GRBL, WG, "31", "Grbl/31", "0", 1, 50, NULL);
    //"$32 = 1    (Laser-mode enable, boolean)" << '\n';
    new StringSetting("X-axis travel resolution, step/mm", GRBL, WG, "100", "Grbl/100", "53.3333", 1, 50, NULL);
    new StringSetting("Y-axis travel resolution, step/mm", GRBL, WG, "101", "Grbl/101", "53.3333", 1, 50, NULL);
    new StringSetting("Z-axis travel resolution, step/mm", GRBL, WG, "102", "Grbl/102", "100", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "103", "Grbl/103", "100.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "104", "Grbl/104", "100.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "105", "Grbl/105", "100.00", 1, 50, NULL);
    new StringSetting("X-axis maximum rate, mm/min", GRBL, WG, "110", "Grbl/110", "4800.000", 1, 50, NULL);
    new StringSetting("Y-axis maximum rate, mm/min", GRBL, WG, "111", "Grbl/111", "4800.000", 1, 50, NULL);
    new StringSetting("Z-axis maximum rate, mm/min", GRBL, WG, "112", "Grbl/112", "3000.000", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "113", "Grbl/113", "1000.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "114", "Grbl/114", "1000.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "115", "Grbl/115", "1000.00", 1, 50, NULL);
    new StringSetting("X-axis acceleration, mm/sec^2", GRBL, WG, "120", "Grbl/120", "200.00", 1, 50, NULL);
    new StringSetting("Y-axis acceleration, mm/sec^2", GRBL, WG, "121", "Grbl/121", "200.00", 1, 50, NULL);
    new StringSetting("Z-axis acceleration, mm/sec^2", GRBL, WG, "122", "Grbl/122", "100.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "123", "Grbl/123", "200.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "124", "Grbl/124", "200.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "125", "Grbl/125", "200.00", 1, 50, NULL);
    new StringSetting("X-axis maximum travel, millimeters", GRBL, WG, "130", "Grbl/130", "655.00", 1, 50, NULL);
    new StringSetting("Y-axis maximum travel, millimeters", GRBL, WG, "131", "Grbl/131", "625.00", 1, 50, NULL);
    new StringSetting("Z-axis maximum travel, millimeters", GRBL, WG, "132", "Grbl/132", "225.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "133", "Grbl/133", "300.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "134", "Grbl/134", "300.00", 1, 50, NULL);
    new StringSetting("", GRBL, WG, "135", "Grbl/135", "300.00", 1, 50, NULL);

    build_info = new StringSetting("OEM build info for $I command", EXTENDED, WG, NULL, "Firmware/Build", "", 0, 20, NULL);

    start_message =
        new StringSetting("Message issued at startup", EXTENDED, WG, NULL, "Start/Message", "Grbl \\V [FluidNC \\B (\\R) \\H]", 0, 40, NULL);
}
