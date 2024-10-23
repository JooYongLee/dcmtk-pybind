#pragma once
#include <plog/Log.h>
#include <Eigen>
#include <iostream>
namespace plog {\
template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();

	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();

	return out;
}

//
template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Vector<T, Eigen::Dynamic> &X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();

	return out;
}
//
template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::RowVector<T, Eigen::Dynamic> &X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

//
template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Vector<T, 3>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();

	return out;
}
//
template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::RowVector<T, 3>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}
//
template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::RowVector<T, Eigen::Dynamic>> &X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::Vector<T, Eigen::Dynamic>> &X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::RowVector<T, 2>>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::Vector<T, 2>>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::RowVector<T, 3>>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::Vector<T, 3>>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::RowVector<T, 4>>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Transpose<Eigen::Vector<T, 4>>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();
	return out;
}

}

template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& vec)
{
	for (const auto& k : vec)
		os << k << ",";
	return os;
}

template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Matrix<T, 4, 4, Eigen::RowMajor>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();

	return out;
}


template<typename T>
plog::Record& operator<<(plog::Record& out, const Eigen::Matrix<T, 4, 4>& X)
{
	std::ostringstream le_msg;
	le_msg << X;
	out << le_msg.str();

	return out;
}