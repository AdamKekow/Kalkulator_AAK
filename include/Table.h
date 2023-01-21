#pragma once

using Name = std::string; //Mo¿e polski nie powinien byæ wykluczany z programowania? wstring czeka na lepsze czasy
#include "polynomials.h"

Name mapa[] = { "-","+","/","*","^","sin","cos","ln" };
class TreePart {
public:
	enum class action {  minus, plus,  divide, multiply, power, sinus, cosinus, ln,  TreeEnd,Bracket };
	
	Name content{};
	action ac { action::TreeEnd};
	std::vector<TreePart> branch{};

	void cut() {
		TreePart wy,wy2;
		for (auto i = 0; i < (int)(action::TreeEnd); i++) {
			std::size_t found = content.find(mapa[i]);
			if (found != std::string::npos) {
				int brackets = 0;
				for (int ii = 0; ii < found+1; ii++) {
					if (content[ii] == '(')brackets++;
					if (content[ii] == ')')brackets--;
				}
				if (brackets == 0) {
					switch (i)
					{
					case (int)(action::minus):
						ac = action::minus;
						wy.content.insert(0, content.substr(0, found));
						wy2.content.insert(0, content.substr(found + 1, content.length()));
						break;
					case (int)(action::plus) :
						ac = action::plus;
						wy.content.insert(0, content.substr(0, found));
						wy2.content.insert(0, content.substr(found + 1, content.length()));
						break;
					case (int)(action::divide) :
						ac = action::divide;
						wy.content.insert(0, content.substr(0, found));
						wy2.content.insert(0, content.substr(found + 1, content.length()));
						break;
					case (int)(action::multiply):
						ac = action::multiply;
						wy.content.insert(0, content.substr(0, found));
						wy2.content.insert(0, content.substr(found + 1, content.length()));
						break;
					case (int)(action::sinus):
						ac = action::sinus;
						wy.content.insert(0, content.substr(3, content.length()));
						break;
					case (int)(action::cosinus):
						ac = action::cosinus;
						wy.content.insert(0, content.substr(3, content.length()));
						break;
					case (int)(action::ln):
						ac = action::ln;
						wy.content.insert(0, content.substr(2, content.length()));
						break;
					case (int)(action::power):
						ac = action::power;
						wy.content.insert(0, content.substr(0, found));
						wy2.content.insert(0, content.substr(found + 1, content.length()));
						break;
					default:
						break;
					}
					break;
				}
			}
		}
		if (ac == action::TreeEnd)
		{
			if ((content[0] == '(') && (content[content.length() - 1] == ')')) {
				content = content.substr(1, content.length() - 2);//remove brackets
				TreePart tmp;
				tmp.content = content;
				tmp.cut();
				ac = tmp.ac;
				branch = tmp.branch;
			}
		}
		if (wy.content.length() > 0) {
			std::size_t found = wy.content.find_first_not_of("x0123456789.");
			if (found != std::string::npos) {
				wy.cut();
			}
			branch.push_back(wy);
		}
		if (wy2.content.length() > 0) {
			std::size_t found = wy2.content.find_first_not_of("x0123456789.");
			if (found != std::string::npos) {
				wy2.cut();
			}
			branch.push_back(wy2);
		}
		
	}

	polynomial diriv() {
		polynomial wy{};
		switch (ac)
		{
		case (action::TreeEnd):
			if (content.find_first_of("x") != std::string::npos) {
				std::string tmp;
				polynomial::Number_power_pair tmp2;
				tmp = content;
				tmp.erase(remove(tmp.begin(), tmp.end(), 'x'), tmp.end());//remove x
				tmp2.power = 0.0;
				tmp2.number = 1.0;
				if (tmp.size() != 0) tmp2.number = stod(tmp);
				wy.poly.push_back(tmp2);
				return wy;
			}
			else {
				return wy;
			}
			break;
		case (action::plus) :
			wy = branch[0].diriv()+ branch[1].diriv();
			return wy;
			break;
		case (action::minus):
			wy = branch[0].diriv() - branch[1].diriv();
			return wy;
			break;
		case (action::multiply):
			wy = branch[0].diriv() * branch[1].algebralic() + branch[0].algebralic() * branch[1].diriv();
			return wy;
			break;
		case (action::divide):
			wy = (branch[0].diriv() * branch[1].algebralic() - branch[0].algebralic() * branch[1].diriv())/(branch[1].algebralic() * branch[1].algebralic());
			return wy;
			break;
		case (action::power):
			if (branch[0].content.size()==0)
			{
				if (true)
				{

				}
			}
			break;
		case (action::sinus):
			{
			polynomial tmp;
			tmp.other = "+cos(" + branch[0].content + ")";
			wy = (branch[0].diriv() * tmp);
			return wy;
			}
		case (action::cosinus):
		{
			polynomial tmp;
			tmp.other = "-sin(" + branch[0].content + ")";
			wy = (branch[0].diriv() * tmp);
			return wy;
		}
			break;
		default:
			break;
		}

		return wy;
	
	
	}
	polynomial algebralic() {
		polynomial wy{};
		switch (ac)
		{
		case (action::TreeEnd):
			if (content.find_first_of("x") != Name::npos) {
				Name tmp;
				polynomial::Number_power_pair tmp2;
				tmp = content;
				tmp.erase(remove(tmp.begin(), tmp.end(), 'x'), tmp.end());//remove x
				tmp2.power = 1.0;
				tmp2.number = 1.0;
				if (tmp.size() != 0) tmp2.number = stod(tmp);
				wy.poly.push_back(tmp2);
				return wy;
			}
			else {
				polynomial::Number_power_pair tmp2;
				tmp2.power = 0.0;
				tmp2.number = 0.0;
				if (content.size() != 0) tmp2.number = stod(content);
				wy.poly.push_back(tmp2);
				return wy;
			}
			break;
		case (action::plus):
			wy = branch[0].algebralic() + branch[1].algebralic();
			return wy;
			break;
		case (action::minus):
			wy = branch[0].algebralic() - branch[1].algebralic();
			return wy;
			break;
		case (action::multiply):
			wy = branch[0].algebralic() * branch[1].algebralic();
			return wy;
		case (action::divide):
			wy = branch[0].algebralic() / branch[1].algebralic();
			return wy;
			break;
		case (action::power):
			wy = branch[0].algebralic() ^ branch[1].algebralic();
			return wy;
		case (action::sinus):
			wy.other=content;
			return wy;
			break;
		case (action::cosinus):
			wy.other = content;
			return wy;
			break;
		default:
			break;
		}

		return wy;
	
	}

};
/*
std::vector<Name> Cut(Name in){
	std::vector<Name> wy;
	int nawiasy=0;
	for (int i = 0; i < in.length(); i++) {
	
	2*x+sin(2x+1)
	}
	return wy;
}
*/

