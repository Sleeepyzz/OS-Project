#include <kernel/rtc.h>
#include <stdio.h>
#include <kernel/port.h>

#define CURRENT_YEAR        2017
 
int century_register = 0x00;                                // Set by ACPI table parsing code if possible
 
unsigned char second;
unsigned char minute;
unsigned char hour;
unsigned char day;
unsigned char month;
unsigned int year;
 
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

void rtc_install() {
	asm ("cli");
	outb(cmos_address, 0x8B);
	char prev=inb(cmos_data);
	outb(cmos_address, 0x8B);
	outb(cmos_data, prev | 0x40);
}
 
int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}
 
unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

unsigned char bcd_to_bin(int value){
	return (value & 0xF) + 10 * ((value >> 4) & 0xF);
}
 
void init_unix_time(){
	outb(CMOS_CMD, RTC_REG_B);
	int flags = inb(CMOS_DATA);
	int second, minute, hour, day, month, year;

	second	= get_RTC_register(RTC_SECOND);
	minute	= get_RTC_register(RTC_MINUTE);
	hour	= get_RTC_register(RTC_HOUR);
	day	= get_RTC_register(RTC_DAY);
	month	= get_RTC_register(RTC_MONTH);
	year	= get_RTC_register(RTC_YEAR);
	
	if(!(flags & RTC_MODE_BINARY)){
		second	= bcd_to_bin(second);
		minute	= bcd_to_bin(minute);
		hour	= bcd_to_bin(hour);
		day	= bcd_to_bin(day);
		month	= bcd_to_bin(month);
		year	= bcd_to_bin(year);
	}
	
	if(!(flags & RTC_TIME_24)){
		hour = get_RTC_register(RTC_HOUR);
		if(hour & 0x80)
			hour = 12 + bcd_to_bin(hour & 70);
		else
			hour = bcd_to_bin(hour);
	}
	
	year += 2000;

	printf("%d:%d:%d %d-%d-%d\n", hour, minute, second, month, day, year);
	unix_time = ((year - 1970) * 365 * 24 * 60 * 60)\
	+ (30 * month * 24 * 60 * 60)\
	+ (24 * day * 60 * 60)\
	+ (60 * 60 * hour)\
	+ (60 * minute)\
	+ second;

	printf(unix_time);
	printf("Virtix date: %d:%d:%d %d-%d-%d\n", get_hour(), get_minute(), get_seconds(), get_month(), get_day(), get_year() + 1970);
}
// Translate month number

void print_date() {
	printf(month);
	printf(" ");
	printf(day);
	printf(", ");
	printf(year);
	printf(" ");
	printf(hour);
	printf(":");
	printf(minute);
	printf(":");
	printf(second);
}
