#pragma once
#define max_depth 128 //defines max depth off tree structure
using Name = std::string; //Mo¿e polski nie powinien byæ wykluczany z programowania? wstring czeka na lepsze czasy
#include "polynomials.h"

Name mapa[] = { "-","+","/","*","^","sin","cos","ln" };
class TreePart {
public:
	enum class action {  minus, plus,  divide, multiply, power, sinus, cosinus, ln,  TreeEnd,Bracket };
	
	Name content{};
	action ac { action::TreeEnd};
	std::vector<TreePart> branch{};
	/////////////////////////////////////////////
	//Input Name (string), user input preprocesed to remove undefined behaviour
	/////////////////////////////////////////////
	//Output tree structure with action information
	/////////////////////////////////////////////
	void cut() {
		if (content[0] == '-' || content[0] == '+') content = "0" + content;//fix sign at start
		TreePart wy,wy2;
		for (auto i = 0; i < (int)(action::TreeEnd); i++) {
			std::size_t found = content.find(mapa[i]);//maybe revers find?
			if (found != Name::npos) {
				if (i == (int)action::divide)//order fix
				{
					std::size_t found2 = content.find(mapa[(int)action::multiply]);
					int brackets = 0;
					for (int ii = 0; ii < found2 + 1; ii++) {
						if (content[ii] == '(')brackets++;
						if (content[ii] == ')')brackets--;
					}
					if ((found2 > found) && (brackets == 0)&&(found2!=Name::npos))
					{
						found = found2;
						i = (int)action::multiply;
					}
				}
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
						if(wy2.content==wy.content) //remove sin(x)/sin(x)=1
						{
							ac = action::TreeEnd;
							content = 1;
						}
						break;
					case (int)(action::multiply):
						ac = action::multiply;
						wy.content.insert(0, content.substr(0, found));
						wy2.content.insert(0, content.substr(found + 1, content.length()));
						if ((wy2.content == "0") || (wy.content == "0"))ac = action::TreeEnd;
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
			if (found != Name::npos) {
				wy.cut();
			}
			branch.push_back(wy);
		}
		if (wy2.content.length() > 0) {
			std::size_t found = wy2.content.find_first_not_of("x0123456789.");
			if (found != Name::npos) {
				wy2.cut();
			}
			branch.push_back(wy2);
		}
		
	}
	/////////////////////////////////////////////
	//Input TreePart tree structure
	/////////////////////////////////////////////
	//Output direvetive assemby of mathematic input
	/////////////////////////////////////////////
	polynomial diriv() {
		polynomial wy{};
		switch (ac)
		{
		case (action::TreeEnd):
			if(content.find_first_not_of("x0123456789.")== Name::npos){
				if (content.find_first_of("x") != Name::npos) {
					Name tmp;
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
			}
			else 
			{
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
			if (branch[0].algebralic().other.length() == 0)
			{
				if (branch[1].algebralic().other.length() == 0)
				{
					polynomial tmp{};
					polynomial::Number_power_pair tmp2{};
					tmp= branch[0].algebralic() ^ branch[1].algebralic();
					for (auto i = 0; i < tmp.poly.size(); i++) 
					{
						tmp2.power = tmp.poly[i].power - 1;
						tmp2.number = tmp.poly[i].number * tmp.poly[i].power;
						wy.poly.push_back(tmp2);
					
					}
					return wy;
				}
				else 
				{
					return wy;
				}
			}
			else
			{
				return wy;
			}
			break;
		case (action::sinus):
			{
			polynomial tmp{};
			tmp.other = "+cos(" + branch[0].content + ")";
			wy = (branch[0].diriv() * tmp);
			return wy;
			}
		case (action::cosinus):
		{
			polynomial tmp{};
			tmp.other = "-sin(" + branch[0].content + ")";
			wy = (branch[0].diriv() * tmp);
			return wy;
		}
			break;
		case (action::ln):
		{
			polynomial tmp{};
			if (branch[0].algebralic().other.length() > 0) { 
			
			}
			wy = (branch[0].diriv() / branch[0].algebralic());
			return wy;
		}
		break;
		default:
			break;
		}

		return wy;
	
	
	}
	/////////////////////////////////////////////
	//Input TreePart tree structure
	/////////////////////////////////////////////
	//Output Algebraic assemby of mathematic input
	/////////////////////////////////////////////
	polynomial algebralic() {
		polynomial wy{};
		switch (ac)
		{
		case (action::TreeEnd):
			if (content.find_first_not_of("x0123456789.") == Name::npos) {
				if (content.find_first_of("x") != Name::npos) {
					Name tmp;
					polynomial::Number_power_pair tmp2;
					tmp = content;
					tmp.erase(remove(tmp.begin(), tmp.end(), 'x'), tmp.end());//remove x
					tmp.erase(remove(tmp.begin(), tmp.end(), '+'), tmp.end());
					tmp2.power = 1.0;
					tmp2.number = 1.0;
					if (tmp.size() != 0) tmp2.number = stod(tmp);
					wy.poly.push_back(tmp2);
					return wy;
				}
				else {
					Name tmp;
					polynomial::Number_power_pair tmp2;
					tmp = content;
					tmp2.power = 0.0;
					tmp2.number = 0.0;
					tmp.erase(remove(tmp.begin(), tmp.end(), '+'), tmp.end());
					if (tmp.size() != 0) tmp2.number = stod(tmp);
					wy.poly.push_back(tmp2);
					return wy;
				}
			}
			else 
			{
				wy.other = content;
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
		{
			std::stringstream ss;
			ss << "+sin(" << branch[0].algebralic() << ")";
			wy.other = ss.str();
			return wy;
		}
			break;
		case (action::cosinus):
		{
			std::stringstream ss;
			ss << "+cos(" << branch[0].algebralic() << ")";
			wy.other = ss.str();
			return wy;
		}
			break;
		case (action::ln):
			{
			std::stringstream ss;
			ss << "+ln(" << branch[0].algebralic() << ")";
			wy.other = ss.str();
			return wy;
			}
			break;
		default:
			break;
		}

		return wy;
	
	}
	/////////////////////////////////////////////
	//Input tree with exess information
	/////////////////////////////////////////////
	//Output tree structure without clutter
	/////////////////////////////////////////////
	void optimize() {
	
	
	
	
	}
};

