#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

using IP = std::vector<unsigned char>;

IP parceIp(const std::string &str, char d)
{
	assert(str.length() <= 15);

	IP r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while (stop != std::string::npos)
	{
		r.push_back((unsigned char)std::stoi(str.substr(start, stop - start)));

		start = stop + 1;
		stop = str.find_first_of(d, start);
	}

	r.push_back((unsigned char)std::stoi(str.substr(start)));

	return r;
}
void printIp(const IP& ip)
{
	for (IP::const_iterator ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
	{
		if (ip_part != ip.cbegin())
			std::cout << ".";

		std::cout << std::to_string(*ip_part);
	}
	std::cout << std::endl;
}


bool _filter(const IP&  ip, int pos)
{
	return true;
}
template<typename T, typename... Args>
bool _filter(const IP&  ip, unsigned int pos, T arg0, Args... args)
{
	static_assert(std::is_integral<T>::value, "wrong type for ip filter");

	if (ip[pos] == arg0)
		return _filter(ip, ++pos, args...);
	return false;
}
template<typename... Args>
bool filter(const IP&  ip, Args... args)
{
	static_assert(sizeof...(args) <= 4, "to mach filter params");
	return _filter(ip, 0, args...);
}


bool filter_any(const IP&  ip)
{
	return true;
}
template<typename T, typename... Args>
bool filter_any(const IP&  ip, T arg0, Args... args)
{
	static_assert(sizeof...(args) <= 3, "to mach filter params");
	static_assert(std::is_integral<T>::value, "wrong type for ip filter");

	bool equal = false;
	for (unsigned int ip_part : ip)
		if (ip_part == arg0)
			equal = true;
	if (equal)
		return filter_any(ip, args...);
	return false;
}


class IPFilter
{
private:
	std::vector<char> params;

public:
	IPFilter(std::initializer_list<char> l) : params{ l }
	{
		assert(params.size() <= 4);
	}

	bool operator()(const IP&  ip)
	{
		for (unsigned int i = 0; i < params.size(); i++)
			if (params[i] != ip[i])
				return false;
		return true;
	}
};

