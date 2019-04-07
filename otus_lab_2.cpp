// otus_lab_2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <fstream>
#include <sstream>


using IP = std::vector<unsigned char>;

std::vector<std::string> split(const std::string &str, char d)
{
	std::vector<std::string> r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while (stop != std::string::npos)
	{
		r.push_back(str.substr(start, stop - start));

		start = stop + 1;
		stop = str.find_first_of(d, start);
	}

	r.push_back(str.substr(start));

	return r;
}


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

	if (pos >= ip.size())
		return true;
	if (ip[pos] == arg0)
		return _filter(ip, ++pos, args...);
	return false;
}
template<typename... Args>
bool filter(const IP&  ip, Args... args)
{
	return _filter(ip, 0, args...);
}

bool _filter_any(const IP&  ip)
{
	return true;
}
template<typename T, typename... Args>
bool filter_any(const IP&  ip, T arg0, Args... args)
{
	static_assert(std::is_integral<T>::value, "wrong type for ip filter");

	bool equal = false;
	for (unsigned int ip_part : ip)
		if (ip_part == arg0)
			equal = true;
	if (equal)
		return _filter_any(ip, args...);
	return false;
}


int main()
{
	try
	{
		// just for comfortable testing
		std::stringstream stream;
		std::string line;
		std::ifstream in("E://otus/02-ip_filter-example/ip_filter.tsv");
		if (in.is_open()) {
			while (getline(in, line))
				stream << line + '\n';
		}
		in.close();

		std::vector<IP> ip_pool;

		for (std::string line; std::getline(std::cin, line);)//std::cin //stream
		{
			std::vector<std::string> v = split(line, '\t');
			ip_pool.push_back(parceIp(v.at(0), '.'));
		}

		// TODO reverse lexicographically sort
		std::sort(ip_pool.rbegin(), ip_pool.rend());

		for (std::vector<IP >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
			printIp(*ip);

		for (std::vector<IP >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
		{
			if (filter(*ip, 1))
				printIp(*ip);
		}

		for (std::vector<IP >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
		{
			if (filter(*ip, 46, 70))
				printIp(*ip);
		}

		for (std::vector<IP >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
		{
			if (filter_any(*ip, 46))
				printIp(*ip);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}


