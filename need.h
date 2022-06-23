#pragma once
#define _CRT_NONSTDC_NO_WARNINGS
#include<iostream>
#include<ctime>
#include<windows.h>
#define INF 99999
#include<vld.h>
using namespace std;
struct Spot
{
	int x;
	int y;
	float g;
	double f;
	bool iswall;
	int nnabs;
	float vh;
	Spot** Nabs;
	Spot* prev;
	COLOUR c;
	Spot() {
		x = y = 0;
		f = INF;
		g = INF;
		iswall = false;
		nnabs = 0;
		c = FG_BLUE;
		Nabs = NULL;
		prev = NULL;
		vh = 0;
	}
	Spot(int x = 0, int y = 0) :x(x), y(y) {
		f = INF;
		g = INF;
		iswall = false;
		/*if (rand() % 10 < 1) {
			f = 999;
			iswall = true;
		}*/
		prev = NULL;
		Nabs = NULL;
		nnabs = 0;
		c = FG_BLUE;
		vh = 0;
	}

	Spot(Spot& c) {
		x = c.x;
		y = c.y;
		f = c.f;
		g = c.g;
		this->c = c.c;
		iswall = c.iswall;
		prev = c.prev;
		vh = c.vh;
		nnabs = c.nnabs;
		if (nnabs) {
			Nabs = new Spot * [nnabs];
			for (int i = 0; i < nnabs; i++) {
				Nabs[i] = c.Nabs[i];
			}
		}
		else
			Nabs = NULL;
	}
	Spot& operator=(float d) {
		g = d;
	}
	bool operator==(int n) {
		return f == n;
	}
	bool operator>(Spot& s) {
		bool ret = false;
		if (f > s.f)
			ret = true;
		else if (f == s.f && g > s.g)
			ret = true;
		else if (f == s.f && g == s.g && vh > s.vh)
			ret = true;
		return ret;
	}
	void nab(Spot* s) {
		Spot** temp = new Spot * [nnabs + 1];
		for (int i = 0; i < nnabs; i++) {
			temp[i] = Nabs[i];
		}
		if (Nabs)
			delete[] Nabs;
		Nabs = temp;
		Nabs[nnabs] = s;
		nnabs++;
	}
	inline float getval() {
		return g;
	}
	void backtrack() {
		//this->c = FG_GREEN;
		Spot* temp = prev;
		while (temp->prev) {
			temp->c = FG_CYAN;
			temp = temp->prev;
		}
	}

	~Spot() {
		if (Nabs)
			delete[] Nabs;
		nnabs = 0;
	}
};
template<typename T>
struct Node {
	T data;
	Node<T>* next;
	Node<T>(T data = 0, Node<T>* next = NULL) : data(data) {
		//this->data = data;
		this->next = next;
	}
	Node<T>& operator[](int x) {
		if (x == 0)
			return *this;
		Node<T>* temp = next;
		for (int i = 1; i < x && temp; i++) {
			temp = temp->next;
		}
		return *temp;
	}
	void add(T data) {
		if (next)
			next->add(data);
		else
			next = new Node<T>(data);
	}
	void print() {
		cout << data;
		if (next) {
			cout << "->";
			next->print();
		}
	}
	~Node() {
		if (next)
			delete next;
	}
};
template<typename T>
class LIST_2D {
	Node<T>** Head;
	int x;
	//int* y;
public:
	LIST_2D() {
		Head = NULL;
		x = 0;
		//y = NULL;
	}
	LIST_2D& add(T data, int y = 0) {
		if (x > y) {
			Head[y]->add(data);
		}
		else if (!Head || x == y) {
			Node<T>** temp = new Node<T> * [x + 1];
			for (int i = 0; Head && i < x; i++) {
				temp[i] = Head[i];
			}
			if (Head)
				delete[] Head;
			Head = temp;
			Head[x] = new Node<T>(data);
			x++;
		}
		return *this;
	}
	Node<T>& operator[](int x) {
		if (this->x > x) {
			return *(Head[x]);
		}
		else return *(Head[this->x - 1]);
	}
	void print() {
		for (int i = 0; i < x; i++) {
			Node<T>* temp = Head[i];
			temp->print();
			cout << endl;
		}
	}
	~LIST_2D() {
		for (int i = 0; i < x; i++) {
			delete Head[i];
		}
		if (Head)
			delete[] Head;
	}
};
template<typename T>
class SLL
{
protected:
	Node<T>* Head;
public:
	SLL() {
		Head = NULL;
	}
	SLL& addAtHead(T data) {
		Head = new Node<T>(data, Head);
		return *this;
	}
	SLL& addAtTail(T data) {
		if (Head) {
			Head->add(data);
		}
		else
			addAtHead(data);
		return *this;
	}
	Node<T>* getHead() {
		return Head;
	}
	void deleteHead() {
		if (Head) {
			Node<T>* temp = Head;
			Head = Head->next;
			temp->next = NULL;
			delete temp;
		}
	}
	void deleteEnd() {
		if (Head->next) {
			Node<T>* temp = Head;
			for (; temp->next; temp = temp->next);
			delete temp;
		}
		else {
			delete Head;
			Head = NULL;
		}
	}
	T operator[](int n) {
		return Head[n]->data;
	}
	int size() {
		Node<T>* temp = Head;
		int i = 0;
		for (; temp; i++, temp = temp->next);
		return i;
	}
	void print() {
		if (Head)
			Head->print();
		cout << "\n";
	}
	void sort() {
		int i, j;
		int n = this->size();
		for (i = 0; i < n - 1; i++) {
			Node<T>* re = NULL;
			Node<T>* temp = this->Head;
			for (j = 0; j < n - i - 1 && temp && temp->next; j++) {
				if (temp->data > temp->next->data) {
					if (re) {
						re->next = temp->next;
						temp->next = temp->next->next;
						re->next->next = temp;
						re = re->next;
					}
					else {
						Node<T>* temp = this->Head;
						this->Head = this->Head->next;
						temp->next = this->Head->next;
						this->Head->next = temp;
						re = this->Head;
					}
				}
				else {
					temp = temp->next;
					if (re)
						re = re->next;
				}
				//temp = temp->next;
			}
		}
		//this->print();
	}
	bool isIn(T d) {
		Node<T>* temp = Head;
		for (; temp; temp = temp->next) {
			if (temp->data == d)
				return true;
		}
		return false;
	}
	~SLL() {
		if (Head) {
			delete Head;
		}
	}
};
template<class T>
class SLLT :public SLL<T> {
public:
};
template<>
class SLLT<Spot*> :public SLL<Spot*> {
public:
	void sort() {
		Node<Spot*>* cur1 = Head;
		Node<Spot*>* cur2 = Head;
		int n = size();
		for (int i = 0; cur1; i++) {
			for (int j = i; cur2; j++) {
				if (*cur1->data > * cur2->data) {
					Spot* temp = cur1->data;
					cur1->data = cur2->data;
					cur2->data = temp;
				}
				cur2 = cur2->next;
			}
			cur1 = cur1->next;
			cur2 = cur1;
		}
	}
};
template<typename T>
class PQueue
{
	SLLT<T>* L;
public:
	PQueue() {
		L = new SLLT<T>;
	}
	void enqueue(T d, bool t = true) {
		L->addAtTail(d);
		if (t)
			L->sort();
	}
	void update() {
		L->sort();
	}
	T peek() {
		return L->getHead()->data;
	}
	int size() {
		return L->size();
	}
	void print() {
		int s = size();
		for (int i = 0; i < s; i++) {
			T temp = peek();
			cout << temp << endl;
			dequeue();
			enqueue(temp);
		}
	}
	bool isIn(T data) {
		return L->isIn(data);
	}
	void dequeue(bool t = true) {
		L->deleteHead();
		if (t)
			L->sort();
	}
	bool isEmpty() {
		return !L->getHead();
	}
	~PQueue() {
		delete L;
	}
};

template<typename T>
ostream& operator<<(ostream& o, Spot s) {
	o << s.g;
	return o;
}
void gotoxy(int x, int y)
{
	COORD coordinates;     // coordinates is declared as COORD
	coordinates.X = x;     // defining x-axis
	coordinates.Y = y;     //defining  y-axis
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}