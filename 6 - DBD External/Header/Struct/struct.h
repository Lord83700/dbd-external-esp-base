#pragma once

struct Vec3 {
	double x, y, z;
};

struct Vec2 {
	double x, y;
};

struct Vector4 {
	double x, y, z, a;
};

struct BonePair {
	DWORD from;
	DWORD to;
};

struct FindWindowData
{
    unsigned long pid;
    std::string class_name;
    std::string window_name;
    HWND hwnd;
};
