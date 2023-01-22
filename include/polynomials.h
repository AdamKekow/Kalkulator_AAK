#pragma once
#include <sstream>
#include <cmath>
class polynomial {
public:
	class Number_power_pair {
	public:
		double number{};
		double power{};
		Number_power_pair() {}//defoult constructor
		//		bool operator() (Number_power_pair i, Number_power_pair j) { return (i.power < j.power); }
	};
	std::vector<Number_power_pair> poly{};
	Name other{};
};
bool first_smaller_power(polynomial::Number_power_pair i, polynomial::Number_power_pair j) { return (i.power < j.power); }
//bool operator== (const polynomial::Number_power_pair& i,const polynomial::Number_power_pair& j) { return (i.power == j.power); }
polynomial operator+(const polynomial& in1, const polynomial& in2) {
	polynomial out;
	if (in1.other.size() > 0 && in2.other.size() > 0)
	{
		out.other = in1.other + "+" + in2.other;
	}
	else 
	{
		out.other = in1.other + in2.other;
	}
	auto size1 = in1.poly.size();
	auto size2 = in2.poly.size();
	if (size1 == 0) {
		out.poly = in2.poly;
		return out;
	}
	if (size2 == 0) {
		out.poly = in1.poly;
		return out;
	}
	std::vector <double> found{};//avible powers
	for (auto i = 0; i < size1; i++) { //becouse algorithm/find are not working even when comply to contract
		found.push_back(in1.poly[i].power);
	}
	for (auto i = 0; i < size2; i++) { //becouse algorithm/find are not working even when comply to contract
		auto it = std::find(found.begin(), found.end(), in2.poly[i].power);
		if (it == found.end())found.push_back(in2.poly[i].power);
	}
	for (auto i = 0; i < found.size(); i++)
	{
		polynomial::Number_power_pair tmp;
		tmp.power = found[i];
		for (auto ii = 0; ii < size1; ii++)
		{
			if (found[i] == in1.poly[ii].power)
			{
				tmp.number += in1.poly[ii].number;
				break;
			}
		}
		for (auto ii = 0; ii < size2; ii++)
		{
			if (found[i] == in2.poly[ii].power)
			{
				tmp.number += in2.poly[ii].number;
				break;
			}
		}
		if(tmp.number!=0.0)out.poly.push_back(tmp);//remove zeros from polynomial
	}

	std::sort(out.poly.begin(), out.poly.end(), first_smaller_power);

	return out;
}
std::ostream& operator<< (std::ostream& out, const polynomial& data) {
	auto size = data.poly.size();
	if (size == 0)
	{
		out << data.other;
		if (data.other.length() == 0)out << "0";
		return out;
	}
	for (auto i = 0; i < data.poly.size(); i++) {
		if ((data.poly[i].number > 0.0)&&(i!=0)) {
			out << "+" << data.poly[i].number;
		}
		else
		{
			out << data.poly[i].number;
		}
		if (data.poly[i].power == 1.0)
		{
			out << "x";
		}
		else if (data.poly[i].power != 0.0) {
			out << "x^" << data.poly[i].power;
		}
		
	}
	out << data.other;
	return out;
}
polynomial operator-(const polynomial& in1,polynomial in2) {
	auto size = in2.poly.size();
	for (auto i = 0; i < size; i++)
	{
		in2.poly[i].number = -in2.poly[i].number;
	}
	if (in2.other[0] == '+')in2.other[0] = '-';//replace sign
	else if (in2.other[0] == '-')in2.other[0] = '+';
	return in1 + in2;

}
polynomial operator*(const polynomial& in1, polynomial in2) {
	polynomial out;
	if ((in1.other.length()>0)&&(in2.other.length()==0))
	{
		std::stringstream ss;
		ss << "("<<in2<<")*("<<in1.other<<")";
		out.other = ss.str();
	}
	else if ((in1.other.length() > 0) && (in2.other.length() > 0))
	{
		std::stringstream ss;
		ss << "(" << in1 << ")*(" << in2 << ")";
		out.other = out.other + ss.str();
	}
	else if ((in1.other.length() == 0) && (in2.other.length() > 0)) {
		if ((in1.poly[0].power == 0.0) && (in1.poly[0].number == 1.0) && (in1.poly.size() == 1)) 
		{
			out = in2;
		}
		else
		{
			std::stringstream ss;
			ss << "+(" << in1 << ")*(" << in2.other << ")";
			out.other = out.other + ss.str();
		}
	}
	auto size1 = in1.poly.size();
	auto size2 = in2.poly.size();
	for (auto i = 0; i < size1; i++)
	{
		for (auto ii = 0; ii < size2; ii++)
		{
			polynomial ptmp;
			polynomial::Number_power_pair tmp;
			tmp.power = in1.poly[i].power + in2.poly[ii].power;
			tmp.number = in1.poly[i].number * in2.poly[ii].number;
			ptmp.poly.push_back(tmp);
			out = out + ptmp;
		}
	}

	std::sort(out.poly.begin(), out.poly.end(), first_smaller_power);

	return out;
}
polynomial operator/(const polynomial& in1, polynomial in2) {
	polynomial out;
	if ((in1.other.length() > 0) || (in2.other.length() > 0))
	{
	std::stringstream ss;
	ss << "(" << in1 << ")/(" << in2 << ")";
	out.other = ss.str();
	return out;
	}
/*
	if ((in1.other.length() > 0) && (in2.other.length() == 0))
	{
		std::stringstream ss;
		ss << "" << in1.other << "/(" << in2 << ")";
		out.other = ss.str();
	}
	else if ((in1.other.length() > 0) && (in2.other.length() > 0))
	{
		std::stringstream ss;
		ss << "(" << in1 << ")/(" << in2 << ")";
		out.other = out.other + ss.str();
		return out;
	}
	else if ((in1.other.length() == 0) && (in2.other.length() > 0)) {
		std::stringstream ss;
		ss << "+(" << in1 << ")/" << in2.other << "";
		out.other = out.other + ss.str();
	}*/
	auto size1 = in1.poly.size();
	auto size2 = in2.poly.size();
	for (auto i = 0; i < size1; i++)
	{
		for (auto ii = 0; ii < size2; ii++)
		{
			polynomial ptmp;
			polynomial::Number_power_pair tmp;
			tmp.power = in1.poly[i].power - in2.poly[ii].power;
			tmp.number = in1.poly[i].number / in2.poly[ii].number;
			ptmp.poly.push_back(tmp);
			out = out + ptmp;
		}
	}

	std::sort(out.poly.begin(), out.poly.end(), first_smaller_power);

	return out;
}
polynomial operator^(const polynomial& in1, polynomial in2) {
	polynomial out;
	if ((in1.other.length() > 0) || (in2.other.length() > 0))
	{
		std::stringstream ss;
		ss << "(" << in1 << ")^(" << in2 << ")";
		out.other = ss.str();
		return out;
	}
	auto size1 = in1.poly.size();
	auto size2 = in2.poly.size();
	if (size2 == 0) return in1;
	if ((size2 > 1)||(in2.poly[0].power!=0.0)) {
		std::stringstream ss;
		ss << "(" << in1 << ")^(" << in2 << ")";
		out.other = ss.str();
		return out;
	}
	for (auto i = 0; i < size1; i++)
	{
			polynomial::Number_power_pair tmp;
			tmp.power = in1.poly[i].power * in2.poly[0].number;
			tmp.number =std::pow(in1.poly[i].number, in2.poly[0].number);
			out.poly.push_back(tmp);
	}

	std::sort(out.poly.begin(), out.poly.end(), first_smaller_power);

	return out;
}
Name printName(polynomial input) {
	std::stringstream ss;
	ss << input;
	return ss.str();
}