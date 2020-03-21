/*
	ERR001: queue is overflow
	ERR002: Queue is empty
	ERR003: out of range
	ERR004: empty input file
	ERR005: file is not open
*/

#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include "Queue.h"

using namespace std;
const int sourceFileAmount = 10000;

const string adrIfile      = "io-files\\input.txt";
const string adrOfile      = "io-files\\output.txt";
const string adrSourceFile = "source\\source.txt";

void getFromFile(fstream& file, Queue::Queue<int>& queue, int size = 1) {
	if (!file) throw logic_error("ERR005: file is not open");

	for (int i{ 0 }, num{ 0 }; i < size; ++i) {
		if (file >> num) {
			queue.push(num);
		}
		else throw logic_error("ERR004: empty input file");
	}
}

void putToFile(fstream& file, int num) {
	if (!file) throw logic_error("ERR005: file is not open");

	file << num << endl;
}

void putToFile(fstream& file, const string& str) {
	if (!file) throw logic_error("ERR005: file is not open");

	file << str << endl;
}

void fileFill(fstream& file, int amount = sourceFileAmount) {
	if (!file) throw logic_error("ERR005: file is not open");
	file.seekp(ios_base::beg);

	random_device rd;
	for (int i = 0; i < amount; ++i) {
		file << static_cast<int>(rd() % 20001) - 10000 << ' ';
	}
	file.seekg(ios_base::beg);
}

int main() {
	system("chcp 65001"); system("cls");
	
	char tmpch{};
	int queueSize{};
	int commandAmmount{};
	
	fstream input(adrIfile, ios_base::in);
	fstream output(adrOfile, ios_base::out | ios_base::trunc);
	fstream source(adrSourceFile, ios_base::in | ios_base::out | ios_base::trunc);
	fileFill(source);

	input >> queueSize >> commandAmmount;
	Queue::Queue<int> queue(queueSize);
	cout << queue.capacity() << " " << commandAmmount << endl;

	for (int i{ 0 }; i < commandAmmount; ++i) {
		int command{};
		input >> command;
		if (command > 0 && !queue.isFull()) {
			for (int k{}; k < command; ++k) {
				try {
					getFromFile(source, queue);
				}
				catch (logic_error _err) {
					string err = _err.what();
					if (err.find("ERR") != string::npos) {
						int errCode = stoi(err.substr(3,3));
						switch (errCode) {
						case 1: 
							putToFile(output, "Memory overflow");
							cout << err << endl;
							k = command;
							break;
						case 4: 
							fileFill(source);
							break;
						}
					}
					else throw _err;
				}
			}
		}
		else if (command < 0 && !queue.isEmpty()) {
			bool isErr{ false };
			int sum{};
			int count{};
			for (int k{}; k < -command; ++k) {
				try {
					sum += queue.pop();   ++count;
				}
				catch (logic_error _err) {
					string err = _err.what();
					if (err.find("ERR") != string::npos) {
						int errCode = stoi(err.substr(3, 3));
						if (errCode == 2) {
							isErr = true;

							putToFile(output, sum / count);
							cout << sum / count << endl;
							putToFile(output, "Empty queue");
							cout << err << endl;
							break;
						}
					}
					else throw _err;
				}
			}
			if (!isErr) {
				putToFile(output, sum / count);
				cout << sum / count << endl;
			}
		}
	}

	cout << "Для завершения нажмите ввод";
	cin.get();
	return 0;
}