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
	bool legends = false, types = false, colors = false, eggs = false;
	string uflags, trainerNum;
	cout << "Welcome to Aaron's Trainer Randomizer. The following options are available:" << endl;
	cout << "'-L': Legendary flag, Legendary trainers are now in the pool. This means that ANY trainer\n";
	cout << "      can have up to six legendary Pokemon, and are GUARANTEED at least one.\n"; 
	cout<<  "      Prepare for trouble!\n";
	cout << "'-T': Type flag, trainers will use type themed teams - meaning they will use one type of Pokemon.\n";
	cout << "      There is a natural chance of this already happening, being 1 in 12. This flag adds the \"Specialist\"\n";
	cout << "      into the pool, as well as a handful of other type-specific classes.\n";
	cout << "'-C': Color flag, similar to type flag but with colors instead.\n";
	cout << "      Like types, there's already a natural chance of this happening.\n";
	cout << "'-E': Egg group flag, much like colors and types. Already a natural chance of occurring.\n";
	cout << "Would you like to add any flags? (Type them in now - case sensitive! - and space them out)\n";
	cout << "Your flags (if you want none, type NONE): ";
	getline(cin, uflags);
	cout << "How many trainers would you like to generate? (Numeric characters only (0-9)!): ";
	getline(cin, trainerNum);

	int position = 0;
	while (position < uflags.length()) {
		if (uflags.find("-L") != string::npos) {
			cout << "Legendaries added into the fray!\n";
			legends = true;
			position += 2;
		}
		if (uflags.find("-T") != string::npos) {
			cout << "Type specialists are now available trainers!\n";
			types = true;
			position += 2;
		}
		if (uflags.find("-C") != string::npos) {
			cout << "Color collectors are now available trainers!\n";
			colors = true;
			position += 2;
		}
		if (uflags.find("-E") != string::npos) {
			cout << "Egg group breeders are now available trainers!\n";
			eggs = true;
			position += 2;
		}
		else if (position == uflags.length() || position > uflags.length())
			break;

		else {
			legends = false;
			types = false;
			colors = false;
			break;
		}
	}

	/*if (types || colors || eggs) {
		cout << "All trainers will have the same types, colors, or eggs by default.\n";
		cout << "This can, however, be altered.\n";
		cout << "Type 'YES' if you'd like all trainers to follow a type, color, or egg\n";
		cout << "theme but not share the same theme: ";
		string themePrompt;
		getline(cin, themePrompt);
		if (themePrompt == "YES") {
			cout << "Themes will differ across trainers.\n";
		}
	}*/

	// Here comes a bunch of repeated code oops

	//cout << "Starting names.\n";
	// coin flip to determine if masculine or feminine
	//srand(time(0));
	ifstream opener;
	vector<string> names;
	string line;
	int typeTheme = 0, colorTheme = 0, eggTheme = 0;
	bool gender = rand() % 2; // 0 or 1
	if (!types) {
		typeTheme = rand() % 12;
		//cout << typeTheme << endl;
		if (typeTheme == 11)
			types = true;
	}
	if (!colors) {
		colorTheme = rand() % 12;
		//cout << colorTheme << endl;
		if (colorTheme == 11)
			colors = true;
	}
	if (!eggs) {
		eggTheme = rand() % 12;
		if (eggTheme == 11)
			eggs = true;
	}
	if (!gender) {
		cout << "Feminine\n";
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
		cout << "Masculine\n";
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

	vector<string> fileEggs;
	ifstream eggfile("./data/eggs.csv", ifstream::in);
	if (eggfile) {
		while (getline(eggfile, line))
			fileEggs.push_back(line);
		eggfile.close();
	}

	//cout << "Themes print here." << endl;

	int trainerInt = stoi(trainerNum);

	string selPrim, selSec;
	bool typeCheck = false; // if typeCheck is false, there's only one type instead of two
	if (types) {
		if ((rand() % 2) == 1) { 
			typeCheck = true;
		}
		if (typeCheck) {
			int rnum = rand() % monTypes.size();
			selPrim = monTypes[rnum];
			sleep(1);
			rnum = rand() % monTypes.size();
			selSec = monTypes[rnum];
			cout << "Type Theme: " << selPrim << " & " << selSec << endl; 
		}
		else {
			int rnum = rand() % monTypes.size();
			selPrim = monTypes[rnum];
			selSec = selPrim;
			cout << "Type Theme: " << selPrim << endl;
		}
	}

	string selColor;
	if (colors) {
		selColor = fileColors[rand() % fileColors.size()];
		cout << "Color Theme: " << selColor << endl;
	}

	string selEgg;
	if (eggs) {
		selEgg = fileEggs[rand() % fileEggs.size()];
		cout << "Egg Group Theme: " << selEgg << endl;
	}

	//cout << "Starting trainers!\n"; 
	vector<string> trainers;
	// Note:
	// trainer-t.csv is now the DEFAULT file. It will ALWAYS be loaded.
	// The only difference now is that EVERY trainer class has a primary
	// and secondary type. Whenever !types, all trainer classes will be
	// loaded. This means that if !types, Dragon Tamers could have a team
	// of 3 Fire Types and an Electric type for instance.
	// Does this make a ton of sense? Not really! But in order to keep
	// things truly random, I believe this is the best approach.
	// This now includes the type "ANY", which means that if types are
	// true these classes will be always added into the pool since they
	// can use any type.

	io::CSVReader<3> trainType("./data/trainer-t.csv");
	trainType.read_header(io::ignore_extra_column, "TRAINER", "PRIMARY", "SECONDARY");
	string fileTrain, filePrim, fileSec;
	while (trainType.read_row(fileTrain, filePrim, fileSec)) {
		if (types) {
			if (filePrim == selPrim || fileSec == selPrim || filePrim == "ANY" || fileSec == "ANY"
				|| filePrim == selSec || fileSec == selSec) {
				trainers.push_back(fileTrain);
			}
		}
		else {
			trainers.push_back(fileTrain);
		}
	}

	if (colors) {
		io::CSVReader<2> in("./data/trainer-c.csv");
		in.read_header(io::ignore_extra_column, "TRAINER", "COLOR");
		string fileTrain, filePrim;
		while (in.read_row(fileTrain, filePrim)) {
			if (filePrim == selColor) {
				trainers.push_back(fileTrain);
			}
		}
	}

	if (!gender) {
		io::CSVReader<3> in("./data/trainer-f.csv");
		in.read_header(io::ignore_extra_column, "TRAINER", "PRIMARY", "SECONDARY");
		string fileTrain, filePrim, fileSec;
		while (in.read_row(fileTrain, filePrim, fileSec)) {
			if (types) {
				if (filePrim == selPrim || fileSec == selPrim || filePrim == "ANY" || fileSec == "ANY"
					|| filePrim == selSec || fileSec == selSec) {
					trainers.push_back(fileTrain);
				}
			}
			else 
				trainers.push_back(fileTrain);
			}
		}

	else {
		io::CSVReader<3> in("./data/trainer-m.csv");
		in.read_header(io::ignore_extra_column, "TRAINER", "PRIMARY", "SECONDARY");
		string fileTrain, filePrim, fileSec;
		while (in.read_row(fileTrain, filePrim, fileSec)) {
			if (types) {
				if (filePrim == selPrim || fileSec == selPrim || filePrim == "ANY" || fileSec == "ANY"
					|| filePrim == selSec || fileSec == selSec) {
					trainers.push_back(fileTrain);
				}
			}
			else
				trainers.push_back(fileTrain);
		}

	}
		//cout << "Done!\n";

	//cout << "Starting species.\n";
	vector<string> mons, legendaries;
	io::CSVReader<6> in("./data/species-update.csv");
	in.read_header(io::ignore_extra_column, "SPECIES", "TYPE 1", "TYPE 2", "COLOR", "EGG GROUP 1", "EGG GROUP 2");
	string species, primary, secondary, color, egg1, egg2;
	while (in.read_row(species, primary, secondary, color, egg1, egg2)) {

		if (types) {
			//cout << "Type time" << endl;
			if (colors && (eggs != true)) { // types And colors
				//cout << "Color time" << endl;
				if ((selPrim == primary || selPrim == secondary || selSec == primary || selSec == secondary) 
					 && (selColor == color)) {
					mons.push_back(species);
				}
			}
			else if (colors && eggs) { // types, colors, and eggs
				if ((selPrim == primary || selPrim == secondary || selSec == primary || selSec == secondary) 
					 && (selColor == color) && (selEgg == egg1 || selEgg == egg2)) {
					mons.push_back(species);
				}
			}
			else if (eggs && (colors != true)) { // types and eggs
				if ((selPrim == primary || selPrim == secondary || selSec == primary || selSec == secondary) 
					 && (selEgg == egg1 || selEgg == egg2)) {
					mons.push_back(species);
				}
			}
			else { // just types
				if (selPrim == primary || selPrim == secondary || selSec == primary || selSec == secondary) {
					mons.push_back(species);
				}
			}
		}
		else if (colors) { // just colors
			if (eggs) { // colors and eggs
				if ((selColor == color) && (selEgg == egg1 || selEgg == egg2)) {
					mons.push_back(species);
				}
			}
			else {
				if (selColor == color) {
					mons.push_back(species);
				}
			}
		}
		else if (eggs) { // just eggs
			if (selEgg == egg1 || selEgg == egg2)
				mons.push_back(species);
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
		while (legends.read_row(spe, pri, sec, col, e1, e2)) {
			if (types) {
				if (colors && eggs) { // eggs, colors, and types
					if ((selPrim == pri || selColor == sec || pri == "ANY" || sec == "ANY") 
						&& (selColor == col) && (selEgg == e1 || selEgg == e2)) {
						legendaries.push_back(spe);
					}
				}
				else if (colors && (eggs != true)) { // colors and types
					if ((selPrim == pri || selColor == sec || pri == "ANY" || sec == "ANY") 
						&& (selColor == col)) {
						legendaries.push_back(spe);
					}
				}
				else if (eggs && (colors != true)) { // eggs and types
					if ((selPrim == pri || selColor == sec || pri == "ANY" || sec == "ANY") 
						&& (selEgg == e1 || selEgg == e2)) {
						legendaries.push_back(spe);
					}
				}
				else { // just types
					if (selPrim == pri || selColor == sec || pri == "ANY" || sec == "ANY") {
						legendaries.push_back(spe);
					}
				}
			}
			else if (colors) { // just colors
				if (eggs) { // colors and eggs
					if ((selColor == col) && (selEgg == e1 || selEgg == e2)) {
						legendaries.push_back(spe);
					}
				}
				else {
					if (selColor == col) {
						legendaries.push_back(spe);
					}
				}
			}
			else if (eggs) { // just eggs
				if (selEgg == e1 || selEgg == e2) {
					legendaries.push_back(spe);
				}
			}
			else { // pushes back All mons
				legendaries.push_back(spe);
			}
		}
	}

	//cout << "Files loaded. Starting randomization.\n";
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
		bool flipLegend = false;
		bool gmax = false, mega = false; // changes to true if a gmax & mega happens
		for (int loop = 0; loop < size; loop++) {
			int flipRand = rand() % 6;
			if (flipRand == 5) {
				if (legends)
					flipLegend = true;
			}

			if (flipLegend) {
				cout << "A legend comes to life!\n";
				team.push_back(legendaries[rand() % legendaries.size()]);
			}

			else {
				if (((mons[rand() % mons.size()]).find("GIGANTAMAX") != string::npos) || ((mons[rand() % mons.size()]).find("MEGA") != string::npos)) {
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

			flipLegend = false; // resets flipLegend
		}

		string trainerClass = trainers[trainerDex];
		string trainerName = names[nameDex];
		string trainerRegion = regions[regionDex];
		cout << "Trainer " << (i + 1) << ":\n";
		printTrainer(trainerClass, trainerName, trainerRegion, team);
		sleep(1); // this is to let the system clock update for a new rand seed
	}

	cout << "\n";
	if (trainerInt == 1) {
		cout << "Enjoy your new trainer!\n";
	}
	else {
		cout << "Enjoy your new trainers!\n";
	}
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