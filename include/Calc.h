#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Table.h"




const Name kWelcomeName= "==========================================\n"
"Welcome to derivative calculator\n"
"Written by Adam Andrzej Kek\xA2w 2023\n"
"==========================================\n"
 ;

void Run(void) {

	std::cout << kWelcomeName;
	while (true)
	{
		Name input;
		std::getline(std::cin,input);
		if(input=="")std::getline(std::cin, input);
		input.erase(remove(input.begin(), input.end(), ' '), input.end());//remove blankspaces
		TreePart one;
		one.content = input;
		one.cut();
		std::cout << one.diriv() <<std::endl;

		



	}
}