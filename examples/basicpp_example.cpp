#include <string>
#include <iostream>
#include <exception>

#include"basicpp/inifile.h"

/**
 * @brief Main program Create an ini file, add values to it and read it.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char **argv){
	int res = 1;
	IniFile inifile("test.ini");

	std::string sect_vin = "Some wine";
	std::string sect_pain = "The bread";
	std::string sect_fromage = "Fromage";

	std::vector<std::string> sections;

	
	try {
		inifile.add_key(sect_pain, "baguette", "1 euro");		
		inifile.add_key(sect_pain, "pain de campagne", "can be expensive");

		inifile.add_key(sect_vin, "bordeau", "always apreciated");
		inifile.add_key(sect_vin, "bourgone", "more heady, prety good");
		inifile.add_key(sect_vin, "chateau neuf du pape", "white is better than red");

		inifile.add_key(sect_fromage, "camember", "soft and strong, hopefully");
		inifile.add_key(sect_fromage, "beaufort", "one of the best");
		inifile.add_key(sect_fromage, "gruyere", "swiss but still good");

		inifile.get_sections(&sections);


		for (int i =0; i < sections.size(); i++){
			std::vector<std::string> keys;

			inifile.get_keys(sections.at(i), &keys);
			for (int j = 0; j < keys.size(); j++){
				string value = "";
				inifile.get_value(sections.at(i), keys.at(j), &value);
				std::cout << "section: " << sections[i] << " key: " << keys[j] << " value: " << value << std::endl;
			}
		}

		res = 0;

	}
	catch (exception &e) {
		std::cout << "Unhandled Exception: " << e.what() << std::endl;
		res = -1;
	}

	return res;
}
