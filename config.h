/* output buffer size */
const size_t maxlen = 100;
/* refresh every this many milliseconds */
const msec refresh = 15000;

Modules modules[] = {
	/* module, param */
	{ &datetime, "%F %R  " },
	{ &file, "/proc/loadavg" },
 	{ &str, "  " },
	{ &file, "/sys/devices/platform/thinkpad_hwmon/hwmon/hwmon4/fan1_input" },
	{ &str, " RPM  " },
	{ &file, "/sys/devices/virtual/hwmon/hwmon0/temp1_input" },
	{ &trun, "3" },
	{ &str, "°C  " },
	{ &battime, "/sys/class/power_supply/BAT0" },
	{ &str, " "},
	{ &baticon, "/sys/class/power_supply/BAT0" },
};
