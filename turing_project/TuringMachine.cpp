//
// Created by Retorm on 2020/12/5.
//

#include "TuringMachine.h"
using namespace std;

void TuringMachine::simulate(string &input) {
	// simulate on Turing Machine

	check_input(input);
	put_input(input);
	
	if (verbose) {
		cout << "Input: " << input << endl;
		cout << "==================== RUN ====================" << endl;
	}
	while (step());
	show_tape();
	if (verbose) {
		cout << "==================== END ====================" << endl;
	}
}

void TuringMachine::put_input(string& input) {
	// put input into TM, initialize the tape
	
	// check empty input
	if (input == "") {
		return;
	}
	// pop the initialized Blank
	tapes[0].pop_back();
	for (char ch : input) {
		tapes[0].push_back(ch);
	}
}

int TuringMachine::init_tm(string &file_name) {
    // read description file

	ifstream tm_file;
    tm_file.open(file_name.c_str());

    if (!tm_file) {
        cerr << "No such file." << endl;
        exit(-1);
    }

    // parse the file
	string buf;
    while (getline(tm_file, buf)) {
		if (buf.length() <= 1) continue;
		
		buf = trim(buf, " \n");
		string line = remove_comment(buf);
        if (line.length() <= 1) continue;
        if (line[0] == '#') {
            switch (line[1]) {
                case 'Q': {
					if (line.substr(2, 4) != " = {" || line[line.size()-1] != '}') {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#Q = {a,b,c}\"" << endl;
						}
						exit(-1);
					}
                    string input_q(line, 6, line.length() - 7);
                    this->Q = string_split(input_q);
                    break;
                }
                case 'S': {
					if (line.substr(2, 4) != " = {" || line[line.size() - 1] != '}') {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#S = {a,b,c}\"" << endl;
						}
						exit(-1);
					}
                    string input_s(line, 6, line.length() - 7);
                    this->S = char_split(input_s);
                    break;
                }
                case 'G': {
					if (line.substr(2, 4) != " = {" || line[line.size() - 1] != '}') {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#G = {a,b,c}\"" << endl;
						}
						exit(-1);
					}
                    string input_s(line, 6, line.length() - 7);
                    this->G = char_split(input_s);
                    break;
                }
                case 'F': {
					if (line.substr(2, 4) != " = {" || line[line.size() - 1] != '}') {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#F = {a,b,c}\"" << endl;
						}
						exit(-1);
					}
                    string input_f(line, 6, line.length() - 7);
                    this->F = string_split(input_f);
                    break;
                }
                case 'q': {
                    if (line[2] == '0') {
						if (line.substr(3, 3) != " = ") {
							cerr << "syntex error" << endl;
							if (verbose) {
								cerr << "In line: " << line << endl;
								cerr << "Should be defined like \"#q_0 = a,b,c\"" << endl;
							}
							exit(-1);
						}
                        string input_q0(line, 6, line.length() - 5);
                        this->q_0 = input_q0;
                    } else {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#q_0 = a\"" << endl;
						}
                        exit(-1);
                    }
					break;
                }
                case 'B': {
					if (line.substr(2, 3) != " = " || line.size() != 6) {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#B = a\"" << endl;
						}
						exit(-1);
					}
                    this->B = line[5];
					break;
                }
                case 'N': {
					if (line.substr(2, 3) != " = ") {
						cerr << "syntex error" << endl;
						if (verbose) {
							cerr << "In line: " << line << endl;
							cerr << "Should be defined like \"#N = 2\"" << endl;
						}
						exit(-1);
					}
                    string input_n(line, 5, line.length() - 4);
                    this->N = stoi(input_n);
					break;
                }
            }
        } else {
			build_delta(line);	
        }
    }
	check_describ();
	for (int i = 0; i < N; i++) {
		vector<char> new_tape;
		new_tape.push_back(this->B);
		tapes.push_back(new_tape);
		directions.push_back('*');
		head.push_back(0);
		init_head.push_back(0);
		cur_state = this->q_0;
	}
	tm_file.close();

    return 0;
}

string TuringMachine::trim(const string& str, const string& whitespace)
{
	// strip string
	const size_t strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
		return ""; // no content

	const size_t strEnd = str.find_last_not_of(whitespace);
	const size_t strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

string TuringMachine::remove_comment(const string &line) {
	// remove the comment line and comment behind one line
	
	if (line[0] == '\n') {
        return "";
    }
    char comment_symbol = ';';
    size_t com_index = line.find(comment_symbol, 0);
    if (com_index >= line.length()) {
        return line;
    } else {
        if (com_index == 0) {
            return "";
        }
		size_t i = 1;
		for (; i <= com_index; i++) {
			if (line[com_index - i] != ' ') break;
		}
		if (i == com_index + 1) {
			return "";
		}
		else {
			string new_line(line, 0, com_index - i + 1);
			return new_line;
		}
    }
}

void TuringMachine::build_delta(const string &line) {
    // build map of transition

	vector<string> key;
    vector<string> value;
    string delim(" ");
    // for parse convinience
	string s(line+" ");

    size_t pos = 0;
    string token;
    int count = 0;
    while ((pos = s.find(delim)) != string::npos) {
        count++;
        token = s.substr(0, pos);
        if (count <= 2) {
			if (count == 2) {
				if (token.length() != N) {
					cerr << "syntex error" << endl;
					if (verbose) {
						cerr << "Wrong transition in tape counts: " << line << endl;
					}
					exit(-1);
				}
			}
            key.push_back(token);
        } else {
			if (count == 3 || count == 4) {
				if (token.length() != N) {
					cerr << "syntex error" << endl;
					if (verbose) {
						cerr << "Wrong transition in tape counts: " << line << endl;
					}
					exit(-1);
				}
			}
            value.push_back(token);
        }
        s.erase(0, pos + delim.length());
    }
    if (count > 5) {
		cerr << "syntex error" << endl;
		if (verbose) {
			cerr << "Too much arguments in transition: " << line << endl;
		}
		exit(-1);
	}

	if (Q.size()!=0 && Q.find(key[0]) == Q.end()) {
		cerr << "syntex error" << endl;
		if (verbose) {
			cerr << "There is no such state: " << key[0] << endl;
		}
		exit(-1);
	}

    transitions[key] = value;
	return;
}

void TuringMachine::check_input(string &input) {
	// check whether input is legal
	for (size_t i = 0; i < input.length();i++) {
		if (this->S.find(input[i]) == this->S.end()) {
			if (verbose) {
				cerr << "Input: " << input << endl;
				cerr << "==================== ERR ====================" << endl;
				cerr << "error: '" << input[i] << "' was not declared in the set of input symbols" << endl;
				cerr << "Input: " << input << endl;
				string pointer(7 + i, ' ');
				pointer += "^";
				cerr << pointer << endl;
				cerr << "==================== END ====================" << endl;
				exit(-1);
			}
			else {
				cerr << "illegal input" << endl;
				exit(-1);
			}
		}
	}
}

void TuringMachine::check_describ() {
    // check deficiency in description
	if (this->Q.empty()) {
		cerr << "syntax error" << endl;
        if (verbose) cerr << "Q is empty" << endl;
		exit(-1);
	} else if (this->S.empty()) {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "S is empty" << endl;
		exit(-1);
	} else if (this->G.empty()) {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "G is empty" << endl;
		exit(-1);
	} else if (this->F.empty()) {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "F is empty" << endl;
		exit(-1);
	} else if (this->q_0.empty()) {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "q_0 is empty" << endl;
		exit(-1);
	} else if (this->B == '\0') {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "B is empty" << endl;
		exit(-1);
	} else if (this->N == 0) {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "N is empty" << endl;
		exit(-1);
	} else if (this->transitions.empty()) {
		cerr << "syntax error" << endl;
		if (verbose) cerr << "transition is empty" << endl;
		exit(-1);
	}
}

int TuringMachine::step() {
	// simulation step

	if (verbose) {
		show_step();
	}
	global_step++;

	string heads = "";
	for (int i = 0; i < N; i++) {
		heads += get_bit(i);
	}
	vector<string> cur_trans;
	cur_trans.push_back(cur_state);
	cur_trans.push_back(heads);

	// no transition -> halt
	if (transitions.find(cur_trans) == transitions.end()) {
		if (verbose) {
			show_step();
		}
		return 0;
	}
	vector<string> next_trans = transitions[cur_trans];
	string next_heads = next_trans[0];
	string next_directs = next_trans[1];
	string next_state = next_trans[2];

	for (int i = 0; i < N; i++) {
		// write to tape
		tapes[i][head[i]] = next_heads[i];
		directions[i] = next_directs[i];

		if (next_directs[i] == 'r') {
			head[i] += 1;
			if (head[i] >= tapes[i].size()) {
				tapes[i].push_back(B);
			}
		}
		else if (next_directs[i] == 'l') {
			head[i] -= 1;
			if (head[i] < 0) {
				// keep head[i]>=0
				head[i] += 1;
				tapes[i].insert(tapes[i].begin(), B);
				// move init_head pointer to point the init input position
				init_head[i] += 1;
			}
		}
	}
	cur_state = next_state;
	// final state
	if (this->F.find(cur_state) != this->F.end()) {
		if (verbose) {
			show_step();
		}
		return 0;
	}
    return 1;
}

char TuringMachine::get_bit(int i) {
	// read a bit on tape
	return tapes[i][head[i]];
}

void TuringMachine::show_step() {
	/*
	 Step   : 0
	 Index0 : 0 1 2 3 4 5 6
	 Tape0  : 1 0 0 1 0 0 1
	 Head0  : ^
	 Index1 : 0
	 Tape1  : _
	 Head1  : ^
	 State  : 0
	 ---------------------------------------------
	 */
	cout << "Step \t: " << global_step << endl;
	for (int i = 0; i < N; i++) {
		cout << "Index" << i <<" \t: ";
		// to count blank to print
		vector<int> output_count;
		int begin = first_index(tapes[i], head[i]);
		int end = last_index(tapes[i], head[i]);
		for (int j = begin; j <= end; j++) {
			int index = j - init_head[i];
			index = index < 0 ? -index : index;
			cout << index << " ";
			output_count.push_back(get_length(index));
		}
		cout << endl;

		cout << "Tape" << i << "\t: ";
		for (int j = begin; j <= end; j++) {
			string blank = string(output_count[j - begin], ' ');
			cout << tapes[i][j] << blank;
		}
		cout << endl;

		cout << "Head" << i << "\t: ";
		for (int j = 0; j < head[i] - begin; j++) {
			string blank = string(output_count[j], ' ');
			cout << " " + blank;
		}
		cout << "^" << endl;
	}
	cout << "State \t: " << cur_state << endl;
	cout << "---------------------------------------------" << endl;
}

void TuringMachine::show_tape() {
	// show symbols on tape0
	int begin = first_index(tapes[0], head[0]);
	int end = last_index(tapes[0], head[0]);
	string result = "";
	for (int i = begin; i <= end; i++) {
		result += tapes[0][i];
	}
	cout << result << endl;
}


int TuringMachine::first_index(const vector<char> tape, const int head) {
	// find first non-blank symbol
	int begin = 0;
	for (; begin < tape.size() && tape[begin] == this->B && begin < head; begin++);
	return begin;
}

int TuringMachine::last_index(const vector<char> tape, const int head) {
	// find last non-blank symbol
	int end = tape.size() - 1;
	for (; end >= 0 && tape[end] == this->B && end > head; end--);
	return end;
}

set<string> TuringMachine::string_split(const string &str) {
	// split string describe into string variable
	if (str.find(" ") != string::npos) {
		cerr << "syntex error" << endl;
		if (verbose) {
			cerr << "extra blank space in set description." << endl;
		}
		exit(-1);
	}

	set<string> res;
    string delim(",");
    string s(str+",");

    size_t pos = 0;
    string token;
    while ((pos = s.find(delim)) != string::npos) {
        token = s.substr(0, pos);
        res.insert(token);
        s.erase(0, pos + delim.length());
    }
    return res;
};

set<char> TuringMachine::char_split(const string& str) {
	// split string describe into char variable
	if (str.find(" ") != string::npos) {
		cerr << "syntex error" << endl;
		if (verbose) {
			cerr << "extra blank space in set description." << endl;
		}
		exit(-1);
	}

	set<char> res;
	string delim(",");
	string s(str + ",");

	size_t pos = 0;
	string token;
	while ((pos = s.find(delim)) != string::npos) {
		token = s.substr(0, pos);
		res.insert(token[0]);
		s.erase(0, pos + delim.length());
	}
	return res;
};

int TuringMachine::get_length(const int x)
{
	// get int length
	int leng = 0;
	int input;
	if (x == 0) {
		return 1;
	}
	if (x < 0) {
		input = -x;
	}
	else {
		input = x;
	}
	while (input)
	{
		input /= 10;
		leng++;
	}
	return leng;
}
