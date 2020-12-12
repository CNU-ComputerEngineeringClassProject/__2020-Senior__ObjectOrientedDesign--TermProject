#include "TextEditor.h"
#include "WordVector.h"


TextEditor::TextEditor(std::string file_name) {
	text_file_name = file_name;
	book = new TextBook();
	WordVector::instance()->splitWord(book->loadFile(text_file_name));

	now_page_idx = 0;
	page_idx.push_back(0);
	if (WordVector::instance()->get_text_list().size() > 0) {
		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), now_page_idx);
		print_text(WordVector::instance()->get_text_list(), now_page_idx);
	}
}

void TextEditor::run() {
	int execute = 1;
	std::string consol_msg = "";
	while (execute) {
		try {
			if (WordVector::instance()->get_text_list().size() == 0) consol_msg = "�ؽ�Ʈ�� �������";
			execute = call(consol_msg);
			consol_msg = "";
		}
		catch (std::string s) {
			consol_msg = s;
		}
		catch (...) {
			consol_msg = "Error";
		}
	}
}

int TextEditor::call(std::string consol_msg = "") {
	std::string answer = menu(consol_msg);
	if (answer.size() < 1) throw std::string("Invalid input");

	int idx = 0, line_num = 0, sequence_num = 0;
	std::vector<std::string> answer_split = keyword_check(answer);

	switch (answer.front())
	{
	case 'n':
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string("����ִ� �ؽ�Ʈ�����Դϴ�.");
		if (page_idx[now_page_idx + 1] >= WordVector::instance()->get_text_list().size()) throw std::string("This is the last page!");
		if (page_idx[now_page_idx] == 0) now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		now_page_idx++;		
		idx = print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		if (page_idx[now_page_idx + 1] >= WordVector::instance()->get_text_list().size()) {
			page_idx.erase(page_idx.begin()+ now_page_idx);
			page_idx.insert(page_idx.begin()+ now_page_idx,idx);
		}
		break; 

	case 'p':
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string("����ִ� �ؽ�Ʈ�����Դϴ�.");
		if (page_idx[now_page_idx] == 0) throw std::string("This is the first page!");
		if (page_idx[now_page_idx + 1] >= WordVector::instance()->get_text_list().size()) now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		now_page_idx--;
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);		
		break; 

	case 't':
		book->writeFile(text_file_name, WordVector::instance()->get_text_list());
		return 0;

	case 's': //ù�常 ���� ��� ���� ����

		if (WordVector::instance()->get_text_list().size() == 0) throw std::string("����ִ� �ؽ�Ʈ�����Դϴ�.");
		if (answer_split[0].size() > 75) throw std::string("�ܾ� 75");

		idx = searchWord(answer_split[0]);

		if (idx >= WordVector::instance()->get_text_list().size()) {
			throw std::string("No words are found in the text.");
		}
		else {
			now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), idx);
			print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		}

		break; 

	case 'd':

		if (WordVector::instance()->get_text_list().size() == 0) throw std::string("����ִ� �ؽ�Ʈ�����Դϴ�.");

		line_num = std::stoi(answer_split[0]);
		sequence_num = std::stoi(answer_split[1]);

		if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string("���� 20");  //RERE
		if (1 > sequence_num || line_idx[line_num] - line_idx[line_num - 1] < sequence_num) throw std::string("�ܾ� �ε��� �ʰ�");  //RERE

		eraseWord(line_num, sequence_num);

		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		break;
	
	case 'c': 
	
		if (WordVector::instance()->get_text_list().size() == 0) throw std::string("����ִ� �ؽ�Ʈ�����Դϴ�.");
		if (answer_split[0].size() > 75 || answer_split[1].size() > 75) throw std::string("�ܾ� 75");  //RERE

		idx = searchWord(answer_split[0]);
		if (idx >= WordVector::instance()->get_text_list().size()) {
			throw std::string("No words are found in the text.");
		}

		changeWord(answer_split[0], answer_split[1]);
		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), 0);
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		break;

	case 'i':

		line_num = std::stoi(answer_split[0]);
		sequence_num = std::stoi(answer_split[1]);
		if (line_idx.size() == 0) {
			if (line_num != 1 || sequence_num != 1) {
				throw std::string("���� Ȯ��");
			}
		}
		else {
			if (line_num < 1 || line_num > line_idx.size() - 1) throw std::string("���� 20");  //RERE
			if (1 > sequence_num || line_idx[line_num] - line_idx[ line_num - 1] + 1 < sequence_num) throw std::string("�ܾ� �ε��� �ʰ�");  //RERE
			if (answer_split[2].size() > 75)throw std::string("�ܾ� 75");  //RERE
		}
		insertWord(line_num, sequence_num, answer_split[2]);
		now_page_idx = set_page_and_line(WordVector::instance()->get_text_list(), now_page_idx);
		print_text(WordVector::instance()->get_text_list(), page_idx[now_page_idx]);

		break;

	default:
		break;
	}
	return 1;
}

int TextEditor::set_page_and_line(std::vector<std::string> text, int now) {
	int idx_temp=now;
	std::vector<int> line_idx_temp;
	int temp_now_page_idx = 0;

	page_idx.clear();
	page_idx.push_back(now);

	//����������
	while (idx_temp > 0) {
		temp_now_page_idx++;
		idx_temp = check_prevline_idx(text, idx_temp)[0];
		page_idx.insert(page_idx.begin(),idx_temp);
	}


	idx_temp = now;
	//����������
	while (idx_temp < WordVector::instance()->get_text_list().size()) {
		line_idx_temp = check_nextline_idx(text, idx_temp);
		if (idx_temp == now) line_idx = line_idx_temp;
		idx_temp = line_idx_temp[line_idx_temp.size()-1];
		if (idx_temp == WordVector::instance()->get_text_list().size()) {
			page_idx.pop_back();
			page_idx.push_back(line_idx_temp[0]);
		}
		page_idx.push_back(idx_temp);
	}

	return temp_now_page_idx;
}
std::vector<int> TextEditor::check_prevline_idx(std::vector<std::string> text, int next) {

	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int i = next-1;

	for (; i >= 0 && line_idx_temp.size() < 20; i--) {

		if (line_char_total + 1 + text[i].size() > 75) { //next line

			if(line_char_total + text[i].size() == 75) {
				line_char_total = 0;
				line_idx_temp.insert(line_idx_temp.begin(), i);
			}
			else {
				line_char_total = text[i].size();
				line_idx_temp.insert(line_idx_temp.begin(), i+1);
			}
		}
		else {
			line_char_total += 1 + text[i].size();
		}

	}

	if (i < 0) {
		line_idx_temp = check_nextline_idx(text, 0);
	}
	else {
		line_idx_temp.push_back(next);
	}

	return line_idx_temp;
}
std::vector<int> TextEditor::check_nextline_idx(std::vector<std::string> text, int now) {
	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int i = now;


	for (; i < text.size() && line_idx_temp.size() <= 20; i++) {

		if (line_char_total == 0 || line_char_total + 1 + text[i].size() > 75) { //next line

			if (line_idx_temp.size() == 20) {
				break;
			}

			line_char_total = text[i].size();
			line_idx_temp.push_back(i);
		}
		else {
			line_char_total += 1 + text[i].size();
		}

	}

	if (line_idx_temp.size() < 20) {
		
		std::vector<int> prev_line_idx_temp = check_nextline_idx(text, page_idx[page_idx.size()-2]);

		int target = prev_line_idx_temp.size() - (1 + 20 - line_idx_temp.size());
		for (int j = prev_line_idx_temp.size() - 2; j >= target;j--) {
			line_idx_temp.insert(line_idx_temp.begin(), prev_line_idx_temp[j]);
		}
	}
	line_idx_temp.push_back(i);

	return line_idx_temp;
}

int TextEditor::print_text(std::vector<std::string> text, int now) {

	if (WordVector::instance()->get_text_list().size() == 0) return -1;
	line_idx =check_nextline_idx(text, now);

	std::cout << std::endl<< now << " line_idx: ";
	for (auto v : line_idx) {
		std::cout << v << " ";
	}
	std::cout << std::endl << " ";

	for (int i = line_idx[0], line_num = 0; i < text.size() && line_num <= 20; i++) {

		if (i == line_idx[line_num]) { //next line
			if (++line_num > 20) break;

			if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
			else std::cout << std::endl << line_num << "|";
		}
		std::cout << ' ' << text[i];
	}

	std::cout << std::endl << "----------------------------------------------------------------------------------";


	return line_idx[0];
}


std::string TextEditor::getInvalidInputMSG(bool follow, char keyword) {
	std::string msg = "Invalid input: The '";
	msg += keyword;

	if (follow) msg += "' keyword can be followed by anything word.";
	else msg += "' keyword cannot be followed by anything word.";

	return msg;
}

std::vector<std::string> TextEditor::keyword_check(std::string answer) {

	char keyword(answer.front());
	std::string message = "";
	std::vector<std::string> answer_split;
	std::string word = "";
	bool front_parentheses = false, back_parentheses = false;

	switch (keyword)
	{
	case ' ': throw std::string("Ű���� �Է� �� ������ ���� �ȵ˴ϴ�.");
	case 'n':
	case 'p':
	case 't':

		if (answer.size() > 1) throw std::string(getInvalidInputMSG(false, keyword));

		answer_split.push_back("" + keyword);
		return answer_split;

	case 's':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
			}
			else if (answer.at(i) == '(') {
				if (front_parentheses) word += '(';
				front_parentheses = true;
			}
			else if (answer.at(i) == ')') {
				if (back_parentheses) word += ')';
				back_parentheses = true;
			}
			else {
				if (back_parentheses) word += ')';
				back_parentheses = false;
				word += answer.at(i);
			}
		}
		if (!back_parentheses) throw std::string("Invalid input");

		answer_split.push_back(word);
		return answer_split;

	case 'c':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));


		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
			}
			else if (answer.at(i) == ',') {
				answer_split.push_back(word);
				word = "";
				if (answer_split.size() > 1) throw std::string("Invalid input");
			}
			else {
				if (answer.at(i) == '(') {
					if (front_parentheses) word += '(';
					front_parentheses = true;
				}
				else if (answer.at(i) == ')') {
					if (back_parentheses) word += ')';
					back_parentheses = true;
				}
				else {
					if (back_parentheses) word += ')';
					back_parentheses = false;
					word += answer.at(i);
				}
			}
		}
		answer_split.push_back(word);
		if (!back_parentheses || answer_split.size() < 2) throw std::string("Invalid input");
		return answer_split;

	case 'd':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
			}
			else if (answer.at(i) == ',') {
				answer_split.push_back(word);
				word = "";
				if (answer_split.size() > 1) throw std::string("Invalid input");
			}
			else {
				if (answer.at(i) == '(') {
					if (front_parentheses) throw std::string("Invalid input");
					front_parentheses = true;
				}
				else if (answer.at(i) == ')') {
					if (back_parentheses) throw std::string("Invalid input");
					back_parentheses = true;
				}
				else {
					if (back_parentheses) throw std::string("Invalid input");
					if (answer.at(i) < 48 || answer.at(i) > 57) throw std::string("Invalid input: Keyword 'd' must only enter numbers for the argument.");
					word += answer.at(i);
				}
			}
		}
		answer_split.push_back(word);
		if (!back_parentheses || answer_split.size() < 2) throw std::string("Invalid input");
		return answer_split;

	case 'i':
		if (answer.size() == 1) throw std::string(getInvalidInputMSG(true, keyword));

		for (int i = 1; i < answer.size(); i++) {
			if (answer.at(i) == ' ') {
				if (front_parentheses && !back_parentheses) throw std::string("Invalid input: There should be no spaces in parentheses.");
			}
			else if (answer.at(i) == ',') {
				answer_split.push_back(word);
				word = "";
				if (answer_split.size() > 2) throw std::string("Invalid input");
			}
			else {
				if (answer.at(i) == '(') {
					if (front_parentheses) throw std::string("Invalid input");
					front_parentheses = true;
				}
				else if (answer.at(i) == ')') {
					if (back_parentheses) throw std::string("Invalid input");
					back_parentheses = true;
				}
				else {
					if (back_parentheses) throw std::string("Invalid input");
					if (answer_split.size() < 2 && (answer.at(i) < 48 || answer.at(i) > 57)) throw std::string("Invalid input: Keyword 'i'�� ù��° �ι�° ���ڴ� ���ڿ��� �մϴ�.");
					word += answer.at(i);
				}
			}
		}
		answer_split.push_back(word);
		if (!back_parentheses || answer_split.size() < 3) throw std::string("Invalid input");
		return answer_split;
	default:
		throw std::string("Invalid input");
	}
}

void TextEditor::insertWord(int line, int idx, std::string word) {
	if (line_idx.size() == 0) WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin(), word);
	else WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin() + line_idx[ line - 1] + ( idx - 1), word);
}

void TextEditor::eraseWord(int line, int idx) {
	WordVector::instance()->get_text_list().erase(WordVector::instance()->get_text_list().begin() + line_idx[ line - 1] + idx - 1);
}

void TextEditor::changeWord(std::string beforeWord, std::string afterWord) {
	for (int i = 0; i < WordVector::instance()->get_text_list().size(); i++) {
		if (WordVector::instance()->get_text_list()[i]._Equal(beforeWord)) {
			WordVector::instance()->get_text_list().erase(WordVector::instance()->get_text_list().begin() + i);
			WordVector::instance()->get_text_list().insert(WordVector::instance()->get_text_list().begin() + i, afterWord);
		}
	}
}

int TextEditor::searchWord(std::string word) {
	int i = 0;
	for (; i < WordVector::instance()->get_text_list().size(); i++) {
		if (WordVector::instance()->get_text_list()[i]._Equal(word)) {
			break;
		}
	}
	return i;
}

std::string TextEditor::menu(std::string consol) {
	std::string answer;

	std::cout << std::endl << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;
	std::cout << "(�ܼ� �޼���) " << consol << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;
	std::cout << "�Է�: ";

	//std::cin >> answer;
	getline(std::cin, answer);
	std::cout << "----------------------------------------------------------------------------------";


	return answer;
}