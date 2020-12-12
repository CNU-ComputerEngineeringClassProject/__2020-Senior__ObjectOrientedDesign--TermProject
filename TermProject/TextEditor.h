#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class TextEditor {

private:
	std::vector<int> page_idx;
	std::vector<int> line_idx;
	int now_page_idx = 0;

public:
	TextEditor(std::string text_file_name);
	void run();
	int call(std::string consol_msg);
	void set_page_and_line(std::vector<std::string> text, int now);
	std::vector<int> check_prevline_idx(std::vector<std::string> text, int prev_line_before_change, int now_line_before_change, int next_line_after_change);
	std::vector<int> check_nextline_idx(std::vector<std::string> text, int now);

private:
	int print_text(std::vector<std::string> text, int now);
	std::string getInvalidInputMSG(bool follow, char keyword);
	std::vector<std::string> keyword_check(std::string answer);
	void insertWord(int line, int idx, std::string word);
	void eraseWord(int line, int idx);
	void changeWord(std::string beforeWord, std::string afterWord);
	int searchWord(std::string word);
	void fileWrite();
	std::string menu(std::string consol);
};
