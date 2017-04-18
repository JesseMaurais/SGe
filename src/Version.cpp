#include "Version.hpp"
#include <cstring>
#include <cstdio>
#include <cassert>

static unsigned GetBuildVersion()
{
	char month[4];
	int day, year;
	std::sscanf(__DATE__, "%s %d %d", month, &day, &year);
	int hour, minute, second;
	std::sscanf(__TIME__, "%d::%d::%d", &hour, &minute, &second);

	unsigned build = year * 10;
	if (not std::strcmp(month, "Jan")) build += 1;
	else
	if (not std::strcmp(month, "Feb")) build += 2;
	else
	if (not std::strcmp(month, "Mar")) build += 3;
	else
	if (not std::strcmp(month, "Apr")) build += 4;
	else
	if (not std::strcmp(month, "May")) build += 5;
	else
	if (not std::strcmp(month, "Jun")) build += 6;
	else
	if (not std::strcmp(month, "Jul")) build += 7;
	else
	if (not std::strcmp(month, "Aug")) build += 8;
	else
	if (not std::strcmp(month, "Sep")) build += 9;
	else
	if (not std::strcmp(month, "Oct")) build += 10;
	else
	if (not std::strcmp(month, "Nov")) build += 11;
	else
	if (not std::strcmp(month, "Dec")) build += 12;
	else
		assert(false and "Month not recognized");

	build *= 10;
	build += day;

	build *= 10;
	build += hour;
	build *= 10;
	build += minute;
	build *= 10;
	build += second;
	return build;
}

unsigned ABIMajorVersion = APIMajorVersion;
unsigned ABIMinorVersion = ABIMinorVersion;
unsigned ABIBuildVersion = GetBuildVersion();

