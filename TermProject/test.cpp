#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class TextBook {
private:
	std::string stringBuffer;
	std::string text_file_name;
public:
	std::vector<std::string> text;

public:

	TextBook(std::string text_file_name) {
		this->text_file_name = text_file_name;
		readFile();
	}
private:
	void readFile() {
		std::ifstream in(text_file_name);

		text.clear();
		if (in.is_open())
		{
			std::string text_temp; //������ ����
			getline(in, text_temp);
			std::istringstream ss(text_temp);

			while (getline(ss, stringBuffer, ' ')) {
				text.push_back(stringBuffer);
			}
		}
		else std::cout << "Unable to open file";
		in.close();
	}

};


class TextEditor {

private:
	TextBook* textbook = nullptr;
	std::vector<int> page_idx;
	std::vector<int> line_idx;
	int now_page_idx;

public:
	TextEditor(TextBook* textbook) :now_page_idx(0) {
		this->textbook = textbook;
		page_idx.push_back(0);
		page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, now_page_idx));
	}

	void run() {
		int execute = 1;
		while (execute) {
			try {
				execute = call();
			}
			catch (std::string& s) {
				execute = call(s);
			}
		}
	}

	int call(std::string consol_msg = "") {
		std::string answer = menu(consol_msg);
		int idx = 0;

		switch (answer.front()) {
		case 'n':
			if (answer.size() > 1) throw std::string("The 'n' keyword cannot be followed by anything word.");
			break;

		case 'p':
			if (answer.size() > 1) throw std::string("The 'p' keyword cannot be followed by anything word.");

			break;
		case 'i':
			//keywordI(answer);
			insertWord(1, 10, "hello");
			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			break;
		case 'd':
			eraseWord(2, 10);
			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			break;
		case 'c':
			changeWord("hello", "bye");
			now_page_idx = 0;
			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			break;
		case 's':
			//std::istringstream ss(answer);
			//std::string stringBuffer;

			//while (getline(ss, stringBuffer, ' ')) {
			//	text.push_back(stringBuffer);
			//}

			if (answer.size() > 78) throw std::string("�ܾ� 75");  //RERE

			idx = searchWord("hello");
			if (idx >= textbook->text.size()) {
				//call("No words are found in the text.");
				throw std::string("No words are found in the text.");
			}
			else {
				now_page_idx = 1;
				page_idx.clear();
				page_idx.push_back(-1);
				page_idx.push_back(idx);
				page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
			}
			break;
		case 't':
			if (answer.size() > 1) throw std::string("The 't' keyword cannot be followed by anything word.");

			fileWrite();
			return 0;
		default:
			break;
		}
		return 1;
	}

	//int call(std::string consol = "") {
	//	std::string answer = menu(consol);
	//	int idx = 0;

	//	switch (answer.front()) {
	//	case 'n':
	//		if (page_idx[now_page_idx + 1] >= textbook->text.size()) {  //RE
	//			call("This is the last page!");
	//		}
	//		else {
	//			now_page_idx++;
	//			if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
	//			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
	//		}
	//		break;

	//	case 'p':
	//		if (page_idx[now_page_idx] == 0) {
	//			call("This is the first page!");
	//		}
	//		else {
	//			now_page_idx--;
	//			print_text(textbook->text, page_idx[now_page_idx]);
	//		}
	//		break;
	//	case 'i':
	//		insertWord(1, 10, "hello");
	//		if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
	//		page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
	//		break;
	//	case 'd':
	//		eraseWord(2, 10);
	//		if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
	//		page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
	//		break;
	//	case 'c':
	//		changeWord("hello", "bye");
	//		now_page_idx = 0;
	//		if (page_idx.size() > now_page_idx + 1) page_idx.erase(page_idx.begin() + now_page_idx + 1);
	//		page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
	//		break;
	//	case 's':
	//		idx = searchWord("hello");
	//		if (idx >= textbook->text.size()) {
	//			call("No words are found in the text.");
	//		}
	//		else {
	//			now_page_idx = 1;
	//			page_idx.clear();
	//			page_idx.push_back(-1);
	//			page_idx.push_back(idx);
	//			page_idx.insert(page_idx.begin() + now_page_idx + 1, print_text(textbook->text, page_idx[now_page_idx]));
	//		}
	//		break;
	//	case 't':
	//		fileWrite();
	//		return 0;
	//	default:
	//		break;
	//	}
	//	return 1;
	//}

	//���� ����
/*
int print_text1(std::vector<std::string> text, int now) {
	std::vector<int> line_idx_temp;
	int page_total = 1;
	int line_char_total = 0;
	int line_num = 0;


	//line_idx_temp.push_back(now);
	//line_char_total = text[now].size();

	//std::cout << ' ' << line_num++ << "| " << text[i++];

	for (int i = now; i < text.size() && line_num <= 20; i++) {

		if (line_char_total==0 || line_char_total + 1 + text[i].size() > 75) { //next line

			if (++line_num > 20) {
				break;
			}
			line_idx_temp.push_back(i);
			if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
			else std::cout << std::endl << line_num << "|";
			line_char_total = text[i].size();
		}
		else {
			line_char_total += 1 + text[i].size();
		}
		std::cout << ' ' << text[i];
	}

	return now;
}
*/

	int print_text(std::vector<std::string> text, int now) {

		int line_num = 0;

		if (now == -1) {
			line_idx = check_prevline_idx(text, page_idx[now_page_idx - 1]);
			if (line_idx[0] == 0) {
				page_idx.erase(page_idx.begin() + now_page_idx);
				now_page_idx = 0;
				now = line_idx[0];
			}
			else {
				page_idx.insert(page_idx.begin() + now_page_idx + 1, line_idx[0]);
				now_page_idx++;
				now = line_idx[0];
			}
		}
		else {
			line_idx = check_nextline_idx(text, now);
		}

		for (int i = line_idx[0]; i < text.size() && line_num <= 20; i++) {

			if (i == line_idx[line_num]) { //next line
				if (++line_num > 20) break;

				if (line_num < 10) std::cout << std::endl << ' ' << line_num << "|";
				else std::cout << std::endl << line_num << "|";
			}
			std::cout << ' ' << text[i];
		}

		std::cout << std::endl << "----------------------------------------------------------------------------------";

		return line_idx[20];
	}

	std::vector<int> check_prevline_idx(std::vector<std::string> text, int now) {
		std::vector<int> line_idx_temp;
		int page_total = 1;
		int line_char_total = 0;
		int i = now;

		for (; i >= 0 && line_idx_temp.size() <= 20; i--) {

			if (line_char_total + 1 + text[i].size() > 75) { //prev line

				if (line_idx_temp.size() == 20) {
					break;
				}

				line_char_total = text[i].size();
				line_idx_temp.insert(line_idx_temp.begin(), i);
			}
			else {
				line_char_total += 1 + text[i].size();
			}

		}

		if (line_idx_temp.size() < 20) {
			return check_nextline_idx(text, 0);
		}
		line_idx_temp.push_back(0);

		return line_idx_temp;
	}

	std::vector<int> check_nextline_idx(std::vector<std::string> text, int now) {
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

			int size = line_idx_temp.size();
			for (int i = 0; i < 20 - size; i++) {
				line_idx_temp.insert(line_idx_temp.begin(), line_idx[19 - i]);
			}
		}
		line_idx_temp.push_back(i);

		return line_idx_temp;
	}

private:

	//void keywordI(std::string answer) {
	//	std::istringstream ss(answer);
	//	std::string stringBuffer;
	//	std::vector<std::string> temp;
	//	std::string add="";

	//	int i = 0;

	//	while (getline(ss, stringBuffer, '(')) {
	//		temp.push_back(stringBuffer);
	//		i++;
	//	}

	//	if (i > 2) {
	//		for (std::vector<std::string>::iterator itr = temp.begin() + 1; itr != temp.end(); itr++)
	//			add += *itr;
	//	}
	//	else add = temp[1];

	//	temp.clear();
	//	ss = std::istringstream(add);
	//	i = 0;

	//	while (getline(ss, stringBuffer, ')')) {
	//		temp.push_back(stringBuffer);
	//		i++;
	//	}

	//	if (i > 2) {
	//		for (std::vector<std::string>::iterator itr = temp.begin(); itr != temp.end() + 1; itr++)
	//			add += *itr;
	//	}
	//	else add = temp[0];

	//	temp.clear();
	//	ss = std::istringstream(add);
	//	i = 0;

	//	while (getline(ss, stringBuffer, ')')) {
	//		temp.push_back(stringBuffer);
	//		i++;
	//	}

	//	if (i > 2) {
	//		throw 
	//	}
	//	else add = temp[1];

	//	temp.clear();
	//}

	void insertWord(int line, int idx, std::string word) {


		textbook->text.insert(textbook->text.begin() + line_idx[line - 1] + (idx - 1), word);

	}

	void eraseWord(int line, int idx) {
		textbook->text.erase(textbook->text.begin() + line_idx[line - 1] + idx - 1);
	}

	void changeWord(std::string beforeWord, std::string afterWord) {
		//������ �������
		for (int i = 0; i < textbook->text.size(); i++) {
			if (textbook->text[i]._Equal(beforeWord)) {
				textbook->text.erase(textbook->text.begin() + i);
				textbook->text.insert(textbook->text.begin() + i, afterWord);
			}
		}
	}

	int searchWord(std::string word) {
		//������ �������
		int i = 0;
		for (; i < textbook->text.size(); i++) {
			if (textbook->text[i]._Equal(word)) {
				break;
			}
		}
		return i;
	}

	void fileWrite() { //����
		std::ofstream out("test2.txt");            //�� ������ ���� ���� // ���� �� �ٽ� ���� RERERER
		//char arr[11] = "BlockDMask";        //���Ͽ� �� ���ڿ�
		if (out.is_open()) {
			//	writeFile.write(arr, 10);    //���Ͽ� ���ڿ� ����
		}
		out.close();
	}

	std::string menu(std::string consol) {
		std::string answer;

		std::cout << std::endl << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "(�ܼ� �޼���) " << consol << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
		std::cout << "�Է�: ";

		std::cin >> answer;
		std::cout << "----------------------------------------------------------------------------------";

		std::cin.get();

		return answer;
	}
};


int main() {

	TextBook* book = new TextBook("test.txt");
	TextEditor* textEditor = new TextEditor(book);

	textEditor->run();

	return 0;
}