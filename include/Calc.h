#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Table.h"




const Name kWelcomeString= "==========================================\n"
"Welcome to derivative calculator\n"
"Written by Adam Andrzej Kek\xA2w 2023\n"
"==========================================\n"
 ;

void Run(void) {
	/*
	polynomial tmp,tmp2;
	polynomial::Number_power_pair tmp3;
	tmp2.other = "-sin(x)";
	tmp.other = "-cos(x)";
	tmp3.power = 7.0;
	tmp3.number = 1.0;
	tmp.poly.push_back(tmp3);
	tmp3.power = 2.0;
	tmp3.number = 3.0;
	tmp.poly.push_back(tmp3);
	tmp3.power = 0.0;
	tmp3.number = 2.0;
	tmp.poly.push_back(tmp3);
	tmp3.power = 4.0;
	tmp3.number = 1.0;
	tmp2.poly.push_back(tmp3);
	tmp3.power = 3.0;
	tmp3.number = 7.0;
	tmp2.poly.push_back(tmp3);
	std::cout << tmp << std::endl << tmp2 << std::endl;
	tmp = tmp / tmp2;
	std::cout << tmp<<std::endl; */
	std::cout << kWelcomeString;
	while (true)
	{
		Name input;
		std::getline(std::cin,input);
		if(input=="")std::getline(std::cin, input);
		input.erase(remove(input.begin(), input.end(), ' '), input.end());//remove blankspaces
		if (input[0] == '-' || input[0] == '+') input = "0"+input;//fix sign at start
		TreePart one;
		one.content = input;
		one.cut();
		std::cout << one.diriv() <<std::endl;

		



	}
}