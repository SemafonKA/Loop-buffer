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

const string adrIfile = "io-files\\input.txt";
const string adrOfile = "io-files\\output.txt";

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

int main() {
	system("chcp 65001"); system("cls");
	
	int queueSize{};
	int commandAmmount{};
	
	fstream input(adrIfile, ios_base::in);
	fstream output(adrOfile, ios_base::out | ios_base::trunc);

	input >> queueSize >> commandAmmount;
	Queue::Queue<int> queue(queueSize);
	cout << queue.capacity() << " " << commandAmmount << endl;

	for (int i{ 0 }; i < commandAmmount; ++i) {
		int command{};
		input >> command;
		if (command > 0) {											// Команда добавления
			for (int k{}; k < command; ++k, ++i) {
				try {
					getFromFile(input, queue);
				}
				catch (logic_error _err) {
					string err = _err.what();
					if (err.find("ERR") != string::npos) {
						int errCode = stoi(err.substr(3,3));
						switch (errCode) {
						case 1:										// Queue is overflow
							putToFile(output, "Memory overflow");
							cout << err << endl;
							int tmp;
							++k;									// Досчитывает оставшиеся числа (одно лишнее уже считано)
							while (k < command) {
								input >> tmp;
								++i; ++k;
							}										// По завершении также выйдет из цикла внутри if (command > 0)

						default: 
							throw _err;
						} // switch (errCode)
					}
					else throw _err;
				}
			}
		}
		else if (command < 0) {										// Команда удаления и среднего арифметического
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
						switch (errCode) {
						case 2:										// Queue is empty
							isErr = true;

							if (count > 1) {
								putToFile(output, sum / count);
								cout << sum / count << endl;
							}
							else {
								putToFile(output, sum);
								cout << sum << endl;
							}
							putToFile(output, "Empty queue");
							cout << err << endl;
							k = -command;							// Выход из цикла внутри if (command < 0)
							break;

						default: 
							throw _err;
						} // switch (errCode)
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