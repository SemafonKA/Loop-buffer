#pragma once
#include <iostream>

namespace Queue{

template<typename T>
inline void swap(T& first, T& second) {
	T tmp = first;
	first = second;
	second = tmp;
}

enum class Mode {
	LIST = 0,
	LIST_ADRESS = 1,
	LIST_ADR_FULL = 2,
	LIST_NUMBER = 3,
	LIST_NUM_ADR = 4,
	LIST_NUM_ADR_FULL = 5,
	NO_OUT = 6
};

template<typename T>
class Queue {
private:
	class Node {
	public:
		T data{};
		Node* next{};
		Node* prev{};
	};

	Node* m_front{};
	Node* m_back{};

	int m_listSize{};
	int m_listCapacity{};

	void ListCheck() const {
		Node* member = m_front;
		for (int i = 0; i < m_listSize; ++i) {
			if (member == nullptr) {
				std::cerr << "ERR: error in resize Queue";
				throw std::logic_error("ERR: error in resize Queue");
			}
			member = member->next;
		}
	}

	Node* ListSearch(int pos) const {
		if (pos >= m_listSize) {
			std::cerr << "ERR: out of range";
			throw std::logic_error("ERR: out of range");
			return nullptr;
		}
		if (pos < 0)				return nullptr;
		if (pos == 0)				return m_front;
		if (pos == m_listSize - 1)	return m_back;

		Node* member{};
		if (pos <= m_listSize / 2) {
			member = m_front;

			for (int i = 0; i < pos; ++i) {
				member = member->next;
			}
		}
		else {
			member = m_back;

			for (int i = m_listSize - 1; i > pos; --i) {
				member = member->prev;
			}
		}

		return member;
	}

	/* Удаляет все члены списка */
	void clear() {
		if (!isEmpty()) {
			Node* member = m_front;
			Node* nextMember;
			for (int i = 0; i < m_listSize; ++i) {
				nextMember = member->next;
				delete member;
				member = nextMember;
			}
			m_front = nullptr;
			m_back = nullptr;
			m_listSize = 0;
		}
	}

	/* Вставить новый член списка после позиции pos */
	Queue& _push(int pos, T data = 0) {
		if (m_listSize == m_listCapacity) throw std::logic_error("ERR: queue is overflow");

		Node tmpNode{ T{}, m_front };								// Звено, предшествующее первому звену 
		Node* member = (pos == -1) ? &tmpNode : ListSearch(pos);	// Звено, предшествующее новому звену	
		Node* nextMember = member->next;

		member->next = new Node;
		member->next->prev = (pos == -1) ? (nullptr) : (member);
		member->next->data = data;

		member->next->next = nextMember;
		if (nextMember == nullptr) { m_back = member->next; }
		if (pos < 0) { m_front = member->next; }
		++m_listSize;

		return (*this);
	}

	/* Удалить член списка из заданной позиции pos */
	T _remove(int pos) {
		if (isEmpty()) {
			throw std::logic_error("ERR: Queue is empty!");
			return 0;
		}
		Node* prevMember = ListSearch(pos - 1);
		if (prevMember != nullptr) {				// Если нужное звено находится не в начале
			Node* member = prevMember->next;
			T data = member->data;

			prevMember->next = member->next;
			if (pos != m_listSize - 1) member->next->prev = member->prev;
			delete member;

			if (pos == m_listSize - 1) m_back = prevMember;
			--m_listSize;

			return data;
		}
		else {										//	if (prevMember != nullptr)									
			T data = m_front->data;
			Node* nextMember = m_front->next;
			delete m_front;

			m_front = nextMember;
			if (m_listSize > 1) m_front->prev = nullptr;
			--m_listSize;

			return data;
		}
	}

public:
	Queue(int _capacity) : m_listCapacity{ _capacity } {

	}
	~Queue() {
		clear();
	}
	
	/* Меняет максимальную ёмкость очереди */
	int resize(unsigned int _capacity, bool isExcessRemove = false) {
		if (m_listSize > _capacity) {
			if (!isExcessRemove) return 0;
			else {
				while (m_listSize > _capacity) {
					_remove(--m_listSize);
				}
				m_listCapacity = _capacity;
				return 2;
			}
		}
		m_listCapacity = _capacity;
		return 1;
	}

	/* Возвращает размер очереди */
	unsigned int size()		const { return m_listSize; }
	/* Возвращает ёмкость очереди */
	unsigned int capacity() const { return m_listCapacity; }

	/* Возвращает, пустой ли список или нет */
	bool isEmpty() const { return m_listSize == 0; }

	/* Добавляет новое звено в начало списка */
	Queue& push(T data = 0) { return _push(-1, data); }

	///* Возвращает значение в последнем звене списка (ссылку) */
	//T& back() { return at(m_listSize - 1); }

	/* Удаляет последнее звено списка */
	T pop() { return _remove(m_listSize - 1); }
};

}			// namespace dList