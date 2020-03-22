/*
	ERR001: queue is overflow
	ERR002: Queue is empty
	ERR003: out of range
*/

#pragma once
#include <iostream>

namespace Queue{

template<typename T>
inline void swap(T& first, T& second) {
	T tmp = first;
	first = second;
	second = tmp;
}

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

	Node* listSearch(int pos) const {
		if (pos >= m_listSize)		throw std::logic_error("ERR003: out of range");
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
		if (m_listSize == m_listCapacity) throw std::logic_error("ERR001: queue is overflow");

		Node tmpNode{ T{}, m_front };								// Звено, предшествующее первому звену 
		Node* member = (pos == -1) ? &tmpNode : listSearch(pos);	// Звено, предшествующее новому звену	
		Node* nextMember = member->next;

		member->next = new Node;
		member->next->prev = (pos == -1) ? (nullptr) : (member);
		member->next->data = data;

		member->next->next = nextMember;
		if (nextMember == nullptr) { m_back = member->next; }
		else if (pos == -1) { nextMember->prev = member->next; }
		if (pos == -1)		{ m_front = member->next; }
		++m_listSize;

		return (*this);
	}

	/* Удалить член списка из заданной позиции pos */
	T _remove(int pos) {
		if (isEmpty()) {
			throw std::logic_error("ERR002: Queue is empty");
			return 0;
		}
		Node* prevMember = listSearch(pos - 1);
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
	bool isFull()  const { return m_listCapacity == m_listSize; };

	/* Добавляет новое звено в начало списка */
	Queue& push(T data = 0) { return _push(-1, data); }

	T& at(int _pos) {
		Node* member = listSearch(_pos);
		return member->data;
	}

	/* Удаляет последнее звено списка */
	T pop() { return _remove(m_listSize - 1); }
};

}	// namespace Queue