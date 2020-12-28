//
// Created by Retorm on 2020/12/5.
//

#ifndef TM_TURINGMACHINE_H
#define TM_TURINGMACHINE_H

#include <set>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class TuringMachine {
public:
    TuringMachine(string &file_name, bool verbose=true) {
        global_step = 0;
		this->verbose = verbose;
        init_tm(file_name);
    }

    void simulate(string &input);

private:
	bool verbose;
	set<string> Q;
    set<char> S;
    set<char> G;
    set<string> F;
    int N = 0;
    string q_0;
	char B = '\0';
    map<vector<string>,vector<string>> transitions;

    vector<char> directions;
    vector<vector<char>> tapes;
	vector<int> head;
	vector<int> init_head;
	string cur_state;

    int global_step;

    int init_tm(string &file_name);

    void build_delta(const string &line);

    string remove_comment(const string &line);

    void check_input(string &input);

    void check_describ();

	void put_input(string& input);

    int step();

	char get_bit(int i);

	void show_step();

	void show_tape();

    set<string> string_split(const string &str);
	
	set<char> char_split(const string &str);

	int first_index(const vector<char> tape, const int head);
	int last_index(const vector<char> tape, const int head);

	string trim(const string& str, const string& whitespace = " \t\n");

	int get_length(const int x);
};

#endif //TM_TURINGMACHINE_H
