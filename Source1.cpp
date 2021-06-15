#include <iostream>
#include <vector>
#include <cmath>
#include<fstream>
#include<cstring>
#include<string>
#include <sstream>
#include <list>
#include <iterator>

using namespace std;

class info {
	friend ostream& operator << (ostream& out, info obj);

public:
	int id;						//user ID
	char access;
	int key;					//Priority
	info() {
		id = 0;
		key = 0;
		access = '\0';
	}
	info(int i, char a, int p) {
		id = i;
		access = a;
		key = p;
	}
	info(const info& obj) {
		id = obj.id;
		access = obj.access;
		key = obj.key;
	}
	info&  operator = (const info& obj) {
		id = obj.id;
		access = obj.access;
		key = obj.key;
		return *this;
	}
	bool operator > (const info& obj) {
		if (key > obj.key)
			return true;
		return false;
	}
	bool operator == (const info& obj) {
		if (key == obj.key) {
			if (id == obj.id) {
				if (access == obj.access)
					return true;
			}
		}
		return false;
	}

	bool operator < (const info& obj) {
		if (key < obj.key)
			return true;
		return false;
	}
	~info() {}
};

ostream& operator << (ostream& out, info obj) {

	//cout << "KEY  " << obj.key << "		";
	cout << "User " << obj.id << " ";
	cout << " , " << obj.access << endl;

	return out;
}

template <class T>
class Queue {
	friend int Load(ifstream& file, info& obj, vector<info>& v);
	friend class ListData;
	friend ostream& operator << (ostream& out, ListData obj);

private:
	vector<T> items;
	T min;

public:
	Queue()
	{}
	Queue(const Queue<T>& q) {
		items = q.items;
		min = q.min;
	}
	Queue<T>& operator = (const Queue<T>& q) {
		items = q.items;
		min = q.min;
		return *this;
	}
	bool operator == (const Queue<T>& q) {
		if (items.size() != q.items.size())
			return false;
		for (int i = 0; i < items.size(); i++) {
			if (items[i] != q.items[i])
				return false;
		}
		return true;
	}
	void insert(vector<T> arr) {
		for (int i = 0; i < arr.size(); i++) {
			items.push_back(arr[i]);
		}
		BuildQueue(items);
	}
	void BuildQueue(vector<T> arr) {
		/*for (int i = 0; i < arr.size(); i++)
		for (int j = 0; j < arr.size() - i - 1; j++)
		if (arr[j] > arr[j + 1])
		swap(arr[j], arr[j + 1]);
		for (int i = 0; i < arr.size(); i++) {
		items.push_back(arr[i]);
		}*/
		for (int i = 0; i < arr.size(); i++)
			items.push_back(arr[i]);
		for (int i = (items.size() / 2) - 1; i >= 0; i--)
			ReheapDown(i);
		min = items[0];
		for (int i = items.size() / 2; i < items.size(); i++)
		{
			if (items[i] < min)
				min = items[i];
		}
	}
	/*T& operator [](int x) {
		return items[x];
	}*/
	void ReheapDown(int cnode)
	{
		while (cnode < items.size())
		{
			int leftchild = 2 * cnode + 1;
			int rightchild = 2 * cnode + 2;
			int maxchild = leftchild;

			if (leftchild < items.size() && rightchild <= items.size()) {
				if (items[leftchild] < items[rightchild])
					maxchild = rightchild;
				if (items[maxchild] > items[cnode])
					swap(items[maxchild], items[cnode]);
			}
			cnode = maxchild;
		}
	}
	T ExtractMax() {

		T temp = items[0];
		items[0] = items[items.size() - 1];
		items.pop_back();
		ReheapDown(0);
		return temp;
	}
	T FindMax() {
		if (items.empty()) {
			cout << "Queue is empty" << endl;
			return nullptr;
		}
		else {
			return items[0];
		}
	}
	T FindMin() {
		return min;
	}
	bool isEmpty() {
		if (items.empty())
			return true;
		return false;
	}
	void display()
	{
		if (isEmpty())
			cout << endl << "Empty Queue" << endl;
		else {
			for (int i = 0; i < items.size(); i++)
				cout << items[i] << "		";
		}
	}
};

class ListData {

	friend ostream& operator << (ostream& out, ListData obj);
public:
	Queue<info>* waitingQueue;
	vector<info> currentusers;
	int FIleID;

	ListData() {
		FIleID = 0;
		waitingQueue = NULL;
	}
	ListData(Queue<info>* q, int id) {							//creating currentusers array from coming queue

		waitingQueue = q;
		if (!waitingQueue->items.empty()) {
			currentusers.push_back(waitingQueue->items[0]);
			info first = waitingQueue->items[0];
			waitingQueue->items.erase(waitingQueue->items.begin());
			if (waitingQueue->items.size() >= 1) {
				int i = 0;
				while (i < waitingQueue->items.size()) {
					if (first.access == 'r' || first.access == 'R') {
						info user = waitingQueue->items[i];
						if (user.access == 'w' || user.access == 'W') {
							break;
						}
						if (user.access == 'r' || user.access == 'R') {
							currentusers.push_back(waitingQueue->items[i]);
							waitingQueue->items.erase(waitingQueue->items.begin());
						}
					}
					i++;
				}
			}
		}
		FIleID = id;
	}
	ListData(const ListData& D) {
		waitingQueue = D.waitingQueue;
		FIleID = D.FIleID;
		currentusers = D.currentusers;
	}
	ListData& operator = (const ListData& D) {
		waitingQueue = D.waitingQueue;
		FIleID = D.FIleID;
		currentusers = D.currentusers;
		return *this;
	}
	void RequestAccess(info user) {

		int prior = 0;
		if (waitingQueue->items.empty()) {
			currentusers.push_back(user);
			return;
		}
		else {
			vector<info>::iterator ite = currentusers.begin();
			for (int i = 0; i < currentusers.size(); i++, ite++) {
				info current;
				current = currentusers[i];
				if (current.key < user.key) {
					prior = 1;
					if (user.access == 'r') {
						if (current.access == 'r') {
							currentusers.insert(ite, user);
							return;
						}
						else if (current.access == 'w') {
							info temp = currentusers[0];
							currentusers.pop_back();
							vector<info> x;
							x.push_back(temp);
							waitingQueue->insert(x);
							currentusers.push_back(user);
							return;
						}
					}
				}
			}
			if (prior == 0) {
				vector<info> v;
				v.push_back(user);
				waitingQueue->insert(v);					//inserts with relative priority
			}
		}
		return;
	}
	//bool operator == (const ListData& D) {			//F
	//	if (waitingQueue == D.waitingQueue) {
	//		if (FIleID == D.FIleID) {
	//			if (currentusers == D.currentusers)
	//				return true;
	//		}
	//	}
	//	return false;
	//}
	bool operator != (int id) {						//in subsctipt operator(hash class)
		if (FIleID == id)
			return false;
		return true;
	}
	int operator* (int x) {
		return FIleID * x;
	}
	~ListData() {
		/*if (waitingQueue)
			delete waitingQueue;*/
	}
};

ostream& operator << (ostream& out, ListData obj) {

	cout << "File " << obj.FIleID << " .. Access granted to:  ";
	if (obj.waitingQueue->items.size() == 0)
	{
		cout << "None" << endl;
		cout << "Next:		None" << endl;
		cout << "Waiting:	None" << endl;
		return out;
	}
	cout << obj.waitingQueue->items[0];
	if (obj.waitingQueue->items.size() == 1) {
		cout << "Next:	None" << endl;
		cout << "Waiting Users:	 None" << endl;
		return out;
	}
	cout << "Next:	" << obj.waitingQueue->items[1];
	cout << "waiting Users:	 " << obj.waitingQueue->items.size() - 2;
	cout << endl;

	return out;
}

template <typename T>
class HashTable {

private:
	list<T>* hash;
	int size = 10007;
	int a = 26;
	int b = 15;
	int p = 100003;
public:
	HashTable() {
		hash = new list<T>[10007];
	}
	HashTable(list<T>* h, int s) {
		hash = h;
		size = s;
	}
	HashTable(const HashTable<T>& h) {
		hash = h.hash;
		size = h.size;
		a = h.a;
		b = h.b;
		p = h.p;
	}
	bool present(int id) {				//main

		auto ite = hash[id].begin();
		while ((*ite) != id) {
			if (ite == hash[id].end())
				return false;
			ite++;
		}
		if (hash[id].empty())
			return true;
		return true;
	}
	T& operator [] (int FileID) {

		int index = (a*FileID + b) % p;
		index = index % size;
		auto ite = hash[index].begin();
		while (*(ite) != FileID)
			ite++;
		return *ite;
	}
	HashTable<T>& operator = (HashTable<T>& h) {
		hash = h.hash;
		size = h.size;
		a = h.a;
		b = h.b;
		p = h.p;
		return *this;
	}
	void Insert(T obj) {

		int index = (obj*a + b) % p;
		index = index % size;
		//list<T>::iterator ite;
		//ite = hash[index].begin();

		//bool flag = true;
		//while (!(ite == hash[index].end())) {
		//	if (*ite == obj)
		//		flag = false;
		//}
		//if (flag)
		hash[index].push_back(obj);
	}
	void Print() {
		for (int i = 0; i < size; i++) {
			if (!hash[i].empty()) {
				cout << "H-> " << i + 1 << "  ";
				auto ite = hash[i].begin();
				if (ite == hash[i].end()) {
					cout << *ite;
					cout << endl;
					break;
				}
				while (!(ite == hash[i].end())) {
					cout << *ite;
					cout << endl;
					ite++;
				}
			}
		}
	}
};
int Load(ifstream& file, info& obj, vector<info>& v) {                         //while loading from file

	string str;
	getline(file, str);
	stringstream temp(str);
	string str2;
	string in[4];													//array of 4 objects to input a line of file 

	for (int i = 0; getline(temp, str2, '\t'); i++) {              //the members are separated by tabs
		in[i] = str2;
	}
	int ID = (atoi(in[0].c_str()));
	obj.id = (atoi(in[1].c_str()));
	obj.key = (atoi(in[2].c_str()));

	v.push_back(obj);

	return ID;
}

int  main() {

	cout << "Enter 1 to Load data" << endl;
	cout << "Enter 2 to insert file in HashTable" << endl;
	cout << "Enter 3 to delete file from HashTable" << endl;
	cout << "Enter 4 to Request an access to a file by a user" << endl;
	cout << "Enter 5 to Release the file by a user" << endl;
	cout << "Enter 6 to print HashTable" << endl;
	int id = 0;
	char ch = '\0';
	int choice;

	HashTable<ListData> Table;
	while (true) {
		cin >> choice;
		if (choice == 1) {
			char ar[100];
			ar[0] = 0;
			cout << "Enter the file name to load into the list: " << endl;
			cin >> ar;

			ifstream input;
			input.open(ar);
			if (!input)
				cout << "Error in opening file." << endl;
			else {
				string f;
				getline(input, f);
				int i = 0;
				int p = 0;
				while (!input.eof()) {

					vector<info> v;
					info obj(id, ch, p);
					int id = Load(input, obj, v);
					Queue<info>* q = new Queue<info>();
					q->BuildQueue(v);
					ListData L(q, id);
					Table.Insert(L);
				}
				cout << "File loaded successfully" << endl << endl;
				Table.Print();
				//ProductMap.printTree();
			}
		}
		else if (choice == 4) {
			int id, user, p;
			char ch;
			cout << "Enter File ID" << endl;
			cin >> id;
			cout << "Enter User ID" << endl;
			cin >> user;
			cout << "Enter priority" << endl;
			cin >> p;
			cout << "Enter access type" << endl;
			cin >> ch;

			info usr(id, ch, user);

			int index = (26 * id + 15) % 100003;
			index = index % 10007;
			if (Table.present(index)) {
				ListData File = Table[index];
				File.RequestAccess(usr);
			}
			else
				cout << "File ID not present!" << endl;

		}
		cout << "Enter 1 to Load data" << endl;
		cout << "Enter 2 to insert file in HashTable" << endl;
		cout << "Enter 3 to delete file from HashTable" << endl;
		cout << "Enter 4 to Request an access to a file by a user" << endl;
		cout << "Enter 5 to Release the file by a user" << endl;
		cout << "Enter 6 to print HashTable" << endl;
	}

	return 0;
}