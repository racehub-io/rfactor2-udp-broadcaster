#pragma once

using namespace std;

const string ToString(const unsigned char& sc)
{
	return std::string(reinterpret_cast<const char*>(&sc), 1);
}

const string ToString(const signed char& sc)
{
	return std::string(reinterpret_cast<const char*>(&sc), 1);
}

const string ToString(const unsigned char sc[], int size)
{
	return std::string(reinterpret_cast<const char*>(sc), size);
	//return reinterpret_cast<const char*>(&sc);
}

const string ToString(const signed char sc[], int size)
{
	return std::string(reinterpret_cast<const char*>(sc), size);
	//return reinterpret_cast<const char*>(&sc);
}