#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
//#define CSV_IO_NO_THREAD
#include "csv.h"
using namespace std;

// func defs
int getTeamSize();
void printTrainer(string tClass, string name, string region, vector<string> mons);

int main(int argc, char* argv[]) {
	srand(time(NULL));
	bool legends = false, types = false, colors = false;
	string uflags, trainerNum;
	cout << "Welcome to Aaron's Trainer Randomizer. The following options are available:" << endl;
	cout << "'-L': Legendary flag, Legendary trainers are now in the pool. This means that ANY trainer\n";
	cout << "      can have up to six legendary Pokemon. Prepare for trouble!\n";
	cout << "'-T': Type flag, trainers will use type themed teams - meaning they will use one type of Pokemon.\n";
	cout << "      There is a natural chance of this already happening, being 1 in 12. This flag adds the \"Specialist\"\n";
	cout << "      into the pool, as well as a handful of other type-specific classes.\n";
	cout << "'-C': Color flag, similar to type flag but with colors instead.\n";
	cout << "      Like types, there's already a natural chance of this happening.\n";
	cout << "Would you like to add any flags? (Type them in now - case sensitive! - and space them out)\n";
	cout << "Your flags (if you want none, type NONE): ";
	getline(cin, uflags);
	cout << "How many trainers would you like to generate? (Numeric characters only (0-9)!): ";
	getline(cin, trainerNum);

	if (uflags.find("-L") != string::npos) {
		cout << "Legendaries added into the fray!\n";
		legends = true;
	}
	if (uflags.find("-T") != string::npos) {
		cout << "Specialists thrown into the mix!\n";
		types = true;
		if (uflags.find("-L") != string::npos) {
			cout << "Legendaries added into the fray!\n";
			legends = true;
		}
	}
	if (uflags.find("-C") != string::npos) {
		cout << "Color maniacs join in for the fun!\n";
		colors = true;
		if (uflags.find("-T") != string::npos) {
			cout << "Specialists thrown into the mix!\n";
			types = true;
			if (uflags.find("-L") != string::npos) {
				cout << "Legendaries added into the fray!\n";
				legends = true;
			}
		}
		else if (uflags.find("-L") != string::npos) {
			cout << "Legendaries added into the fray!\n";
			legends = true;
			if (uflags.find("-T") != string::npos) {
				cout << "Specialists thrown into the mix!\n";
				types = true;
			}
		}
	}
	else if (uflags == "NONE") {
		legends = false;
		types = false;
		colors = false;
	}

	string flag1, flag2, flag3;

	if (argc > 1) { // gross if chain is gross don't look at it if you like having eyes
		flag1 = argv[1];
		if ("-L" == flag1)
			legends = true;
		if ("-T" == flag1)
			types = true;
		if ("-C" == flag1)
			colors = true;
		if (argc > 2) {
			flag2 = argv[2];
			if ("-L" == flag2)
				legends = true;
			if ("-T" == flag2)
				types = true;
			if ("-C" == flag2)
				colors = true;
			if (argc > 3) {
				flag3 = argv[3];
				if ("-L" == flag3)
					legends = true;
				if ("-T" == flag3)
					types = true;
				if ("-C" == flag3)
					colors = true;
			}
		}
	}

	// Here comes a bunch of repeated code oops

	//cout << "Starting names.\n";
	// coin flip to determine if masculine or feminine
	//srand(time(0));
	ifstream opener;
	vector<string> names;
	string line;
	int typeTheme = 0, colorTheme = 0;
	bool gender = rand() % 2; // 0 or 1
	if (!types) 
		typeTheme = rand() % 12;
		//cout << typeTheme << endl;
		if (typeTheme == 11)
			types = true;
	if (!colors)
		colorTheme = rand() % 12;
		//cout << colorTheme << endl;
		if (colorTheme == 11)
			colors = true;
	if (!gender) {
		opener.open("./data/names-f.csv");
		if (opener) {
			while (getline(opener, line)) {
				names.push_back(line + "\n");
			}
			opener.close();
		}
		else {
			cerr << "Error: could not open file - names-f.csv\n";
		}
	}
	else {
		opener.open("./data/names-m.csv");
		if (opener) {
			while (getline(opener, line)) {
				names.push_back(line + "\n");
			}
			opener.close();
		}
		else {
			cerr << "Error: could not open file - names-m.csv\n";
		}
	}

	//cout << "Done!\n";
	//cout << "Testing name: " << names[1940] << endl;

	//cout << "Starting regions.\n";
	vector<string> regions;
	ifstream regionfile("./data/region.csv", ifstream::in);
	if (regionfile) {
		while (getline(regionfile, line)) {
			regions.push_back(line);
		}
		regionfile.close();
	}
	else {
		cerr << "Error: cannot load regions!" << endl;
	}
	//cout << "Done!\n";

	vector<string> monTypes;
	ifstream typefile("./data/type.csv", ifstream::in);
	if (typefile) {
		while (getline(typefile, line)) {
			monTypes.push_back(line);
		}
		typefile.close();
	}
	else 
		cerr << "Error: cannot open file - type.csv\n";

	vector<string> fileColors;
	ifstream colorfile("./data/colors.csv", ifstream::in);
	if (colorfile) {
		while (getline(colorfile, line)) {
			fileColors.push_back(line);
		}
		colorfile.close();
	}
	else 
		cerr << "Error: cannot open file - colors.csv\n";

	string selType;
	if (types) {
		int rnum = rand() % monTypes.size();
		selType = monTypes[rnum];
		cout << "Type theme: " << selType << endl;
	}

	string selColor;
	if (colors) {
		selColor = fileColors[rand() % fileColors.size()];
		cout << "Color theme: " << selColor << endl;
	}

	//cout << "Starting trainers!\n"; 
	vector<string> trainers;
	if (types) {
		io::CSVReader<2> in("./data/trainer-t.csv");
		in.read_header(io::ignore_extra_column, "TRAINER", "PRIMARY");
		string fileTrain, filePrim;
		while (in.read_row(fileTrain, filePrim)) {
			if (filePrim == selType) {
				trainers.push_back(fileTrain);
			}
		}
	}
	else {
		//cout << "Starting trainers.\n";
		ifstream trainerfile("./data/trainer.csv", ifstream::in);
		if (trainerfile) {
			while (getline(trainerfile, line)) {
				//cout << line << endl;
				trainers.push_back(line);
			}
			trainerfile.close();
		}
		else {
			cerr << "Error: cannot open file - trainer.csv\n";
		}

		if (!gender) {
			trainerfile.open("./data/trainer-f.csv");
			if (trainerfile) {
				while (getline(trainerfile, line)) {
					trainers.push_back(line);
				}
				trainerfile.close();
			}
			else {
				cerr << "Error: cannot open file - trainer-f.csv\n";
			}
		}

		else {
			trainerfile.open("./data/trainer-m.csv");
			if (trainerfile) {
				while (getline(trainerfile, line)) {
					trainers.push_back(line);
				}
				trainerfile.close();
			}
			else {
				cerr << "Error: cannot open file - trainer-m.csv\n";
			}
		}
		//cout << "Done!\n";
	}

	//cout << "Starting species.\n";
	vector<string> mons;
	io::CSVReader<6> in("./data/species-update.csv");
	in.read_header(io::ignore_extra_column, "SPECIES", "TYPE 1", "TYPE 2", "COLOR", "EGG GROUP 1", "EGG GROUP 2");
	string species, primary, secondary, color, egg1, egg2;
	while (in.read_row(species, primary, secondary, color, egg1, egg2)) {

		if (types) {
			//cout << "Type time" << endl;
			if (colors) { // types And colors
				//cout << "Color time" << endl;
				if ((selType == primary || selType == secondary) && (selColor == color)) {
					mons.push_back(species);
					/*type1.push_back(primary);
					type2.push_back(secondary);
					monColors.push_back(color);
					eG1.push_back(egg1);
					eG2.push_back(egg2);*/
				}
			}
			else { // just types
				if (selType == primary || selType == secondary) {
					mons.push_back(species);
					/*type1.push_back(primary);
					type2.push_back(secondary);
					monColors.push_back(color);
					eG1.push_back(egg1);
					eG2.push_back(egg2);*/
				}
			}
		}
		else if (colors) { // just colors
			if (selColor == color) {
				mons.push_back(species);
			}
		}
		else { // all mons
			mons.push_back(species);
			/*type1.push_back(primary);
			type2.push_back(secondary);
			monColors.push_back(color);
			eG1.push_back(egg1);
			eG2.push_back(egg2);*/
		}
	}
	//cout << "Done!\n";

	if (legends) {
		//cout << "Adding legendaries into the fray!\n";
		io::CSVReader<6> legends("./data/legendary.csv");
		legends.read_header(io::ignore_extra_column, "SPECIES", "TYPE 1", "TYPE 2", "COLOR", "EGG GROUP 1", "EGG GROUP 2");
		string spe, pri, sec, col, e1, e2;
		while (in.read_row(spe, pri, sec, col, e1, e2)) {
			if (types) {
				if (colors) { // colors and types
					if ((selType == pri || selColor == sec || pri == "ANY" || sec == "ANY") 
						&& (selColor == col)) {
						mons.push_back(spe);
					}
				}
				else { // just types
					if (selType == pri || selColor == sec || pri == "ANY" || sec == "ANY") {
						mons.push_back(spe);
					}
				}
			}
			else if (colors) { // just colors
				if (selColor == col) {
					mons.push_back(spe);
				}
			}
			else { // pushes back All mons
				mons.push_back(spe);
			}
			/*type1.push_back(pri);
			type2.push_back(sec);
			monColors.push_back(col);
			eG1.push_back(e1);
			eG2.push_back(e2);*/
		}
	}

	//cout << "Files loaded. Starting randomization.\n";
	int trainerInt = stoi(trainerNum);
	for (int i = 0; i < trainerInt; i++) {
		int size = getTeamSize();
		//srand(time(0));
		int nameDex = rand() % names.size(); 

		int regionDex = rand() % regions.size();
		//cout << regionDex << endl;

		//cout << "buh\n";
		int trainerDex = rand() % trainers.size();
		//cout << trainerDex << endl;

		vector<string> team;
		bool gmax = false, mega = false; // changes to true if a gmax & mega happens
		for (int loop = 0; loop < size; loop++) {
			//srand(time(NULL));
			if ((mons[rand() % mons.size()]).find("GIGANTAMAX") != string::npos) {
				if (gmax || mega) {
					//cout << "Already have a Gmax\n";
					loop--; // subtracts one from loop to account for a member not being added
				}
				else {
					gmax = true;
					team.push_back(mons[rand() % mons.size()]);
				}

				if (!mega) {
					mega = true;
					team.push_back(mons[rand() % mons.size()]);
				}
			}
			else 
				team.push_back(mons[rand() % mons.size()]);
		}

		string trainerClass = trainers[trainerDex];
		string trainerName = names[nameDex];
		string trainerRegion = regions[regionDex];
		cout << "Trainer " << (i + 1) << ":\n";
		printTrainer(trainerClass, trainerName, trainerRegion, team);
		sleep(1); // this is to let the system clock update for a new rand seed
	}

	//cout << "The before times\n";
	/*for (string nom : names) {
		cout << nom << endl;
	}*/
	/*cout << "Name: "; 
	cout << names[nameDex] << "\n";*/

	cout << "\n";
	cout << "Enjoy your new trainer(s)!\n";
	cout << "Press ENTER to exit.\n";
	string buh;
	//getline(cin, buh);
	getline(cin, buh);
	if (buh == "\n")
		exit(EXIT_SUCCESS);
}

int getTeamSize() {
	//srand(time(NULL)); // random seed
	int size = rand() % 6 + 1; // team size is 1-6
	return size;
}

void printTrainer(string tClass, string name, string region, vector<string> mons) {
	cout << "From " + region + ", you are challenged by " + tClass + " " + name;
	cout << "They have ";
	for (int i = 0; i < mons.size(); i++) {
		if (mons.size() == 1) 
			cout << mons[i] << "!\n";
		else if ((i + 1) == mons.size()) 
			cout << "and " << mons[i] << "!\n";
		else
			cout << mons[i] << ", ";
	}
}