//12161104 �ڹ��� �˰��� 1������

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;
// ���� pair<int, int>�� pair<string, int>�� ��������� �������� ���������� ����ϱ� ���� Ŭ����ȭ (first, second -> �� �̸�)
class Adress_XY { // �ּ� ��ǥ�� ����ϱ� ���� Ŭ����
public:
	int x;
	int y;

	Adress_XY(int _x, int _y) {
		this->x = _x;
		this->y = _y;
	}
};

class Record { // �������� �����ϱ� ���� Ŭ����
public:
	string diseaseName;
	int price;

	Record(string _diseaseName, int _price) {
		this->diseaseName = _diseaseName;
		this->price = _price;
	}
};

class Patient {
public:
	int id; // ȯ�ڹ�ȣ (����Ű, ������): 1,000,000 ~ 1,999,999 ������ ����. 
	string name; // �̸� : ���� ���� 20 bytes �̳��� ���ڿ�.��) ��Mary��
	string phoneNumber; // ����ó : ȸ���� ��ȭ��ȣ(���� ���� 11 bytes �̳��� ���ڿ�).��) ��01012345678��
	Adress_XY  * address; // �ּ� ��ǥ(x, y) : x�� y�� ���� 0 ~10, 000 ������ ������ ����.
	Record  * record; //������ : �Ʒ� �������� ���� (	-���� : ���� ���� 20 bytes �̳��� ���ڿ�.��) ��Headache��, �����: 10, 000 ~100, 000 ������ ����)
	// ȯ�� ������ ���� ��� ����

	Patient(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		Adress_XY * _address_xy = new Adress_XY(_addressX, _addressY); // Ŭ���� ��ü ����
		Record  * _record = new Record(_diseaseName, _price);// Ŭ���� ��ü ����

		this->id = _id;
		this->name = _name;
		this->phoneNumber = _phoneNumber;
		this->address = _address_xy;
		this->record = _record;
	} // ȯ�� ������ �Է¹޾� ȯ�� ��ü�� �����ϴ� ������ ��� �Լ�
};

class Node {
public:
	Patient * patient;
	Node* parent;
	Node* left;
	Node* right;
	int depth;

	Node(Patient * _data) {
		this->patient = _data;
		parent = left = right = NULL;
		depth = 0;
	}
};

class Red_BlackTree {
public:
	Node* root;
	
	Red_BlackTree() {
		this->root = NULL;
	}

	Node* getRoot() { // main���� root�� �ް� ������ ���
		return root;
	}

	void insert(Node* insert_node) {
		if (root == NULL) { // �� Ʈ���� ��� root�� insert���ش�.
			root = insert_node;
		}
		else { // root�� �ִٸ� �ڽ��� �ڸ��� ã�Ƽ� insert
			Node* curNode = root;
			Node* upperNode = NULL;
			bool findFlag = false;
			while (curNode != NULL) {
				upperNode = curNode;
				if (curNode->patient->id == insert_node->patient->id) {
					findFlag = true;
					int depth = curNode->depth;
					break;
				}
				else if (curNode->patient->id < insert_node->patient->id) {
					curNode = curNode->left;
				}
				else if (curNode->patient->id > insert_node->patient->id) {
					curNode = curNode->right;
				}
			}
		}
	}

	Node* find(int find_id) { // red-black tree�� find�� binary search tree�� find�� �����ϴ�.
		Node* curNode = root;
		while (curNode != NULL) {
			if (curNode->patient->id == find_id) {
				return curNode;
			}
			else if (curNode->patient->id < find_id) {
				curNode = curNode->left;
			}
			else if (curNode->patient->id > find_id) {
				curNode = curNode->right;
			}
		}
		//while���� ������ ã�� ���� ���̴�.
		return NULL;
	}

	int traversal(Node* node, string _diseaseName, int cnt) { 
		// �׷����� ������ȸ�ϸ� ������ ������ ���� ����� ���� ����.
		// ��Ʈ���� �����ؼ� cnt�� ������ ������ ����.
		// ���� ������� ���� null����̸� �״�� cnt�� ��ȯ
		// �ƴϸ� ������ ��ȸ�� cnt�� ������ ��ȸ�� ������ ����.
		int tempCnt_left = cnt;
		int tempCnt_right = cnt;
		if (node != NULL) {
			if (node->patient->record->diseaseName == _diseaseName)
				cnt++;
			tempCnt_left = traversal(node->left, _diseaseName, cnt);
			tempCnt_right = traversal(node->right, _diseaseName, tempCnt_left);
		}
		return tempCnt_right;
	}
	void query_I(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		// insert (K N H Ax Ay DI C)������ ���� ����
		// parameter�� patient ��ü �����ϰ�, Ʈ���� �����ϴ� ����(red-black tree)
		//print : ��� depth�� �ű԰���ó������ 
		//-��������: ��I K N H Ax Ay DI C��
		//	I : ȯ�ڸ� �ű� �����ϴ� ���Ǹ� ��Ÿ���� ��ȣ
		//	K : ȯ�ڹ�ȣ
		//	N : �̸�
		//	H : ����ó
		//	Ax : �ּ��� x ��ǥ
		//	Ay : �ּ��� y ��ǥ
		//	DI : ����
		//	C : �����
		//	- ������� : ��D S��
		//	D : Ʈ������ ȯ�� ������ ����� ����� ����(root�� ���̴� 0)
		//	S : �ű� ���� ó���� ��Ÿ���� ����(0 �Ǵ� 1) (0: ����, 1 : ����)
		//- ���� : ���ο� ȯ�� ������ �Է¹޾� �����Ʈ���� ��带 �����ϰ�, �� ����� ���̸� ����Ѵ�.
		//	���� ������ ȯ�ڹ�ȣ�� �̹� �����Ѵٸ�, �� ����� ���̸� ����ϰ� �ű� ������ �����ȴ�.
	}

	void query_F(int _id) {
		//- ��������: ��F K��
	//F: Ư�� ȯ�ڸ� �˻��ϴ� ���Ǹ� ��Ÿ���� ��ȣ
	//	K : ȯ�ڹ�ȣ
	//	- ������� : ��D N H Ax Ay�� �Ǵ� ��Not found��
	//	D : Ʈ������ ȯ�� ������ ����� ����� ���� 	//	N : �̸� 	//	H : ����ó 	//	Ax : �ּ��� x ��ǥ 	//	Ay : �ּ��� y ��ǥ
	//	- ���� : �ش� ȯ�ڸ� Ž���Ͽ� �����ϸ� ȯ���� ������ ����Ѵ�.
	//	���� �������� ������, ��Not	found��(���� : ��ҹ���, ���� �� ��Ȯ�� ��ġ��ų ��)�� ����Ѵ�.
		Node* findNode = find(_id);
		if (findNode == NULL) {
			cout << "Not found" << "\n";
		}
		else {
			Patient* p = findNode->patient;
			cout << findNode->depth << " " << p->name << " " << p->phoneNumber << " " << p->address->x << " " << p->address->y << " " << "\n";
		}
		return;
	}

	void query_A(int _id, string _diseaseName, int _price) {
		//-��������: ��A K DI C��
		//	A : Ư�� ȯ�ڿ� ���� ���ο� ���� ������ �߰��ϴ� ���Ǹ� ��Ÿ���� ��ȣ
		//	K : ȯ�ڹ�ȣ
		//	DI : ����
		//	C : �����
		//	- ������� : ��D�� �Ǵ� ��Not found��
		//	D : Ʈ������ ȯ�� ������ ����� ����� ����
		//	- ���� : �ش� ȯ�ڸ� Ž���Ͽ� �Է¹��� ���� ������ �����Ͽ� �߰��ϰ�, ȯ�ڿ� ���� �����
		//	���̸� ����Ѵ�.���� ȯ�ڰ� �������� �ʴ� ���, ��Not found���� ����Ѵ�.

	}

	void query_E(string _diseaseName) {
		//-�������� : ��E DI��
		//	E : �������� ������� ������ �������� Ư�� ������ ���� �����ϴ� ���Ǹ� ��Ÿ���� ��ȣ
		//	DI : ����
		//	- ������� : ��T��
		//	T : ��� ȯ�ڵ��� �����Ͽ��� ���������� �߰��� ������ �Է����� �־��� ����� ����
		//	ȯ���� ��(T �� 0) - ���� : Ʈ���� ����� ��� ȯ�ڵ鿡 ����, ���������� ���ܹ��� ������ �Է����� �־��� �����
		//	�����ϸ� �����ϰ�, ����� ���� ����Ͽ� ���༺�� ������ �ľ��Ѵ�.���ǻ�, ���� ���� ��������
		//	���� ���� ���ິ���� �����Ѵ�.
		cout << traversal(root, _diseaseName, 0) << "\n"; // Ʈ���� ��ȸ�ϸ� ������ ���� ������ �� ����Ѵ�.
	}

};

int queryNum, input_id, input_addressX, input_addressY, input_price;
string queryName, input_name, input_phoneNumber, input_diseaseName;

int main() {
	cin >> queryNum;
	Red_BlackTree redblackTree;

	for (int t = 0; t < queryNum; t++) {
		cin >> queryName;

		if (queryName == "I") {
			cin >> input_id >> input_name >> input_phoneNumber >> input_addressX >> input_addressY >> input_diseaseName >> input_price;
			redblackTree.query_I(input_id, input_name, input_phoneNumber, input_addressX, input_addressY, input_diseaseName, input_price);
		}
		else if (queryName == "F") {
			cin >> input_id;
			redblackTree.query_F(input_id);
		}
		else if (queryName == "A") {
			cin >> input_id >> input_diseaseName >> input_price;
			redblackTree.query_A(input_id, input_diseaseName, input_price);
		}
		else if (queryName == "E") {
			cin >> input_diseaseName;
			redblackTree.query_E(input_diseaseName);
		}
	}
	return 0;
}