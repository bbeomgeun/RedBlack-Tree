//12161104 박범근 알고리즘 1차과제

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#define NIL -1;

using namespace std;

enum class Color { // 노드의 color를 red와 black으로 제한하기 위한 enum class
	COLOR_RED,
	COLOR_BLACK
};

// 원래 pair<int, int>와 pair<string, int>를 사용했지만 변수명을 직관적으로 사용하기 위해 클래스화 (first, second -> 각 이름)
class Address_XY { // 주소 좌표를 기록하기 위한 클래스
public:
	int x;
	int y;

	Address_XY(int _x, int _y) {
		this->x = _x;
		this->y = _y;
	}
};

class Record { // 진료기록을 저장하기 위한 클래스
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
	int id; // 환자번호 (기준키, 유일함): 1,000,000 ~ 1,999,999 사이의 정수. 
	string name; // 이름 : 공백 없는 20 bytes 이내의 문자열.예) “Mary”
	string phoneNumber; // 연락처 : 회원의 전화번호(공백 없는 11 bytes 이내의 문자열).예) “01012345678”
	Address_XY  * address; // 주소 좌표(x, y) : x와 y는 각각 0 ~10, 000 사이의 임의의 정수.
	Record  * record; //진료기록 : 아래 정보들의 나열 (	-병명 : 공백 없는 20 bytes 이내의 문자열.예) “Headache”, 진료비: 10, 000 ~100, 000 사이의 정수)
	// 환자 정보를 담을 멤버 변수

	Patient(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		Address_XY * _address_xy = new Address_XY(_addressX, _addressY); // 클래스 객체 생성
		Record  * _record = new Record(_diseaseName, _price);// 클래스 객체 생성

		this->id = _id;
		this->name = _name;
		this->phoneNumber = _phoneNumber;
		this->address = _address_xy;
		this->record = _record;
	} // 환자 정보를 입력받아 환자 객체를 생성하는 생성자 멤버 함수

	void setDiseaseName(string _diseaseName) {
		this->record->diseaseName = _diseaseName;
	}

	void setPrice(int _price) {
		this->record->price = _price;
	}
};

class Node {
public:
	Patient * patient; // 노드의 환자 정보
	Node* parent; // 부모 노드
	Node* left; // 왼쪽 노드
	Node* right; // 오른쪽 노드
	int depth; // 노드의 depth
	Color color; // 노드의 color

	Node(Patient * _data) { 
		this->patient = _data;
		parent = left = right =NULL;
		depth = 0;
		// left right setcolor하면 null이라 오류남
		this->setColor(Color::COLOR_RED);
	} // 새 노드 추가 시, 해당 환자정보, depth = 0, red color, 자식노드 black으로 생성

	void setColor(Color c) { // 리프노드를 위해
		if (this == NULL) {
			return;
		}
		else
			this->color = c;
	}

	int getID() {
		return this->patient->id;
	}

	Node* getParentNode() {
		return this->parent;
	}

	Node* getUncleNode() { // 현재 노드에서 부모의 부모의 자식 노드를 리턴
		Node* parentNode = this->parent;
		Node* grandParent = parentNode->parent;

		if (grandParent->left == parentNode) { // 만약 grand의 left과 부모이면 오른쪽이 uncleNode
			return grandParent->right;
		}
		else { // grand의 right이 부모이면 왼쪽이 uncleNode
			return grandParent->left;
		}
	}

	Node* getGrandNode() {
		return this->parent->parent;
	}
};

class Red_BlackTree {
public:
	Node* root;
	
	Red_BlackTree() {
		this->root = NULL;
	}

	Node* getRoot() { // main에서 root를 받고 싶을때 사용
		return root;
	}

	bool isDoubleRed(Node* curNode) { // doubleRed 체크 (현재 노드와 부모 노드의 color가 모두 red면 true)
		if (curNode == root) // 만약 root노드를 체크하려고 들어오면 오류가 나므로 false return
			return false;

		Node* parent = curNode->getParentNode();
		if (curNode->color == Color::COLOR_RED && parent->color == Color::COLOR_RED)
			return true;
		else
			return false;
	}

	bool isBlack(Node* curNode) { // 받은 node의 black 체크(블랙이거나 리프노드(null)이면 true값)
		if (curNode == NULL)
			return true;
		else
			return curNode->color == Color::COLOR_BLACK;
	}

	Node* recoloring(Node* curNode) { // 기준 노드 z의 parent, grand, uncle의 색을 바꾼다. 포인터 연산이므로 값이 변경됨
		Node* parentNode = curNode->getParentNode();
		Node* grandNode = curNode->getGrandNode();
		Node* uncleNode = curNode->getUncleNode();

		if (grandNode != root) { // 루트가 아닐 경우 red로 변경해준다. (루트는 무조건 블랙)
			grandNode->setColor(Color::COLOR_RED);
		}
		uncleNode->setColor(Color::COLOR_BLACK);
		parentNode->setColor(Color::COLOR_BLACK);

		return grandNode; // grand가 red로 바뀌는 케이스이므로 grand와 grand의 parent의 red를 비교해야함
	}

	void restructuring(Node* curNode) { // case 4개로 나눠서 트리를 수정해준다.
		bool isRoot = false;

		Node* grandNode = curNode->getGrandNode();
		Node* parentNode = curNode->getParentNode();
		Node* uncleNode = curNode->getUncleNode();
		Node* grandParentNode = NULL; // grand가 루트이면 참조가 불가능하기에 NULL로 초기화

		int grandParentID = -1; // grand가 루트이면 참조가 불가능하기에 -1로 초기화
		int grandID = grandNode->getID();
		int parentID = parentNode->getID();
		int curID = curNode->getID();

		if (grandNode == root) {
			isRoot = true;
		}
		else { // grandNode가 루트가 아니면 현재 서브트리의 루트가 바뀌기 때문에 서브트리의 부모에 연결하기 위함
			grandParentNode = grandNode->getParentNode(); 
			grandParentID =  grandParentNode->getID();
		}

		if (grandID < curID && curID < parentID) { // case 1 curNode가 루트가 된다. grandNode가 leftchild, parentNode가 rightChild
			if (!isRoot) {
				if (grandParentID > curID) {
					grandParentNode->left = curNode;
				}
				else {
					grandParentNode->right = curNode;
				}
				curNode->parent = grandParentNode;
			}
			else {
				curNode->parent = NULL; // curNode가 루트가 된다.
			} // 여기까지 curNode와 grandParentNode의 관계 정리

			grandNode->right = curNode->left;
			curNode->left = grandNode;
			grandNode->parent = curNode;
			// 여기까지 curNode와 grandNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)

			parentNode->left = curNode->right;
			curNode->right = parentNode;
			parentNode->parent = curNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
		}
		else if (parentID < curID && curID < grandID) { // case 2 curNode가 루트가 된다. parentNode가 leftchild, grandNode가 rightChild
			if (!isRoot) {
				if (grandParentID > curID) {
					grandParentNode->left = curNode;
				}
				else {
					grandParentNode->right = curNode;
				}
				curNode->parent = grandParentNode;
			}
			else { 
				curNode->parent = NULL; // curNode가 루트가 된다.
			} // 여기까지 curNode와 grandParentNode의 관계 정리

			parentNode->right = curNode->left;
			curNode->left = parentNode;
			parentNode->parent = curNode;
			// 여기까지 curNode와 grandNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)

			grandNode->left = curNode->right;
			curNode->right = grandNode;
			grandNode->parent = curNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
		}
		else if (curID < parentID && parentID < grandID) { // case 3 parentNode가 루트가 된다. grandNode가 rightchild, curNode는 그대로 parent의 leftchild
			if (!isRoot) {
				if (grandParentID > parentID) {
					grandParentNode->left = parentNode;
				}
				else {
					grandParentNode->right = parentNode;
				}
				parentNode->parent = grandParentNode;
			}
			else {
				parentNode->parent = NULL; // parentNode가 루트가 된다.
			} // 여기까지 parentNode와 grandParentNode의 관계 정리

			grandNode->left = parentNode->right;
			parentNode->right = grandNode;
			grandNode->parent = parentNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
		}
		else if (grandID < parentID && parentID < curID) { // case 4 parentNode가 루트가 된다. grandNode가 leftchild, curNode는 그대로 parent의 rightchild
			if (!isRoot) {
				if (grandParentID > parentID) {
					grandParentNode->left = parentNode;
				}
				else {
					grandParentNode->right = parentNode;
				}
				parentNode->parent = grandParentNode;
			}
			else {
				parentNode->parent = NULL; // parentNode가 루트가 된다.
			} // 여기까지 parentNode와 grandParentNode의 관계 정리

			grandNode->right = parentNode->left;
			parentNode->left = grandNode;
			grandNode->parent = parentNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
		}
		return ;
	}

	Node* insert(Node* insert_node) { // insertNode : 해당 환자정보, depth = 0, 자신 : red color, 자식 : black으로 생성
		if (root == NULL) { // 빈 트리일 경우 root에 insert해준다.
			root = insert_node;
			root->setColor(Color::COLOR_BLACK); // root의 경우 black으로 바꿔준다, root는 depth와 parent 정보 바꿔줄 필요 없다.
			return root; // 루트는 insert하고 종료(recoloring 필요 없음)
		}
		else { // root가 있다면 자신의 자리를 찾아서 insert 후 recoloring 진행
			Node* curNode = root;
			Node* upperNode = NULL;
			while (curNode != NULL) {
				upperNode = curNode;
				if (curNode->getID() == insert_node->getID()) { // 만약 id가 중복된다면 insert 실패
					return curNode; // 중복된 노드를 반환한다 (query_I에서 비교해서 리턴값이 insert하려는 것과 같으면 승인, 다른 정보면 실패
				}
				else if (curNode->getID() > insert_node->getID()) { // insert보다 현재 노드의 key가 더 크면 left로 간다
					curNode = curNode->left;
				}
				else if (curNode->getID() < insert_node->getID()) { // 현재 노드의 key가 더 작으면 right으로 간다.
					curNode = curNode->right;
				}
			} // while이 종료되면 upperNode의 자식 위치에 _insertNode가 삽입된다. left 또는 right를 정해주자, 현재 curNode는 null

			if (upperNode->getID() > insert_node->getID()) {
				upperNode->left = insert_node;
			}
			else if (upperNode->getID() < insert_node-> getID()) {
				upperNode->right = insert_node;
			}
			insert_node->parent = upperNode;
			insert_node->depth = upperNode->depth + 1;
			// 현재 insert_node 부모연결, depth, patient, color - red, child - black 상태

			Node * z = insert_node; // 기준노드 z에 insert 후 노드 관계가 완성된 insert_node를 저장

			while (isDoubleRed(z)) { //복사해준 curNode를 기준으로 recoloring 시작
				// 만약 z의 grand가 null인 경우의 예외처리를 하려했지만 그 경우는 z의 parent가 root(black)인 경우로 isDoubleRed가 무조건 false가 나온다.
				if (isBlack((z->getUncleNode()))) { // uncle이 black이면 restructuring
					restructuring(z);
					break;
				}
				else  { // uncle red이면 recoloring
					z = recoloring(z); 
					// 노드 z의 parent, uncle, grand의 색을 바꿈, grand가 root일 경우 색을 바꾸지 않음, 리턴값은 z의 grand - grand가 red가 되므로 grand를 비교해야함
					//따라서 z는 grand가 되며 grand가 root가 될수도 있음 -> isDoubleRed함수에 root 예외처리 추가
				}
			}
			return insert_node; // 일단 구조를 정리와 별개로 여기까지 온거면 정상 insert이므로 insert한 노드 리턴
		}
	}

	Node* find(int find_id) { // red-black tree의 find는 binary search tree의 find와 동일하다.
		Node* curNode = root;
		while (curNode != NULL) {
			if (curNode->getID() == find_id) {
				return curNode;
			}
			else if (curNode->getID() < find_id) {
				curNode = curNode->left;
			}
			else if (curNode->getID() > find_id) {
				curNode = curNode->right;
			}
		}
		//while문이 끝나면 찾지 못한 것이다.
		return NULL;
	}

	int traversal(Node* node, string _diseaseName, int cnt) { 
		// 그래프를 전위순회하며 동일한 병명을 가진 노드의 수를 센다.
		// 루트부터 시작해서 cnt를 변수로 가지고 들어간다.
		// 만약 리프노드 이후 null노드이면 그대로 cnt를 반환
		// 아니면 왼쪽을 순회한 cnt를 오른쪽 순회가 가지고 간다.
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
		Patient* newPatientInfo = new Patient (_id, _name, _phoneNumber, _addressX, _addressY, _diseaseName, _price);
		Node* newNode = new Node(newPatientInfo);

		Node* insertResultNode = insert(newNode);
		if (insertResultNode->patient == newNode->patient) { // 정상 insert
			cout << insertResultNode->depth << " " << 1 << "\n"; // 승인코드 1
		}
		else { // 이미 중복된 insert
			cout << insertResultNode->depth << " " << 0 << "\n"; // 거절코드 0
		}
		// insert (K N H Ax Ay DI C)쿼리에 관한 내용
		// parameter로 patient 객체 생성하고, 트리에 삽입하는 과정(red-black tree)
		//print : 노드 depth와 신규가입처리여부 
		//-질의형식: “I K N H Ax Ay DI C”
		//	I : 환자를 신규 가입하는 질의를 나타내는 기호		//	K : 환자번호		//	N : 이름		//	H : 연락처		//	Ax : 주소의 x 좌표		//	Ay : 주소의 y 좌표
		//	DI : 병명		//	C : 진료비
		//	- 출력형식 : “D S”		//	D : 트리에서 환자 정보가 저장된 노드의 깊이(root의 깊이는 0)		//	S : 신규 가입 처리를 나타내는 정수(0 또는 1) (0: 거절, 1 : 승인)
		//- 설명 : 새로운 환자 정보를 입력받아 레드블랙트리에 노드를 삽입하고, 그 노드의 깊이를 출력한다.
		//	만약 동일한 환자번호가 이미 존재한다면, 그 노드의 깊이를 출력하고 신규 가입은 거절된다.
	}

	void query_F(int _id) {
		//- 질의형식: “F K”
	//F: 특정 환자를 검색하는 질의를 나타내는 기호
	//	K : 환자번호
	//	- 출력형식 : “D N H Ax Ay” 또는 “Not found”
	//	D : 트리에서 환자 정보가 저장된 노드의 깊이 	//	N : 이름 	//	H : 연락처 	//	Ax : 주소의 x 좌표 	//	Ay : 주소의 y 좌표
	//	- 설명 : 해당 환자를 탐색하여 존재하면 환자의 정보를 출력한다.
	//	만약 존재하지 않으면, “Not	found”(주의 : 대소문자, 띄어쓰기 등 정확히 일치시킬 것)를 출력한다.
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
		//-질의형식: “A K DI C”
		//	A : 특정 환자에 대해 새로운 진료 정보를 추가하는 질의를 나타내는 기호		//	K : 환자번호		//	DI : 병명		//	C : 진료비
		//	- 출력형식 : “D” 또는 “Not found”		//	D : 트리에서 환자 정보가 저장된 노드의 깊이
		//	- 설명 : 해당 환자를 탐색하여 입력받은 진료 내용을 진료기록에 추가하고, 환자에 대한 노드의
		//	깊이를 출력한다.만약 환자가 존재하지 않는 경우, “Not found”를 출력한다.
		Node* findNode = find(_id);
		if (findNode == NULL) {
			cout << "Not found" << "\n";
		}
		else {
			Patient* p = findNode->patient;
			//진료기록에 추가하라는게 기록 1, 2, 3을 다 갖고 있어야 하나? 아니면 덮어쓰기하라는건지?
			// 추가하는거면 class Record vector로 변경 후 push_back으로 진행하고, 검색할때 vector.back()으로 참조해야할듯
			// 일단은 overwrite 버전
			p->setDiseaseName(_diseaseName);
			p->setPrice(_price);
			cout << findNode->depth <<"\n";
		}
		return;
	}

	void query_E(string _diseaseName) {
		//-질의형식 : “E DI”
		//	E : 마지막에 진료받은 병명을 기준으로 특정 병명을 수를 집계하는 질의를 나타내는 기호
		//	DI : 병명
		//	- 출력형식 : “T”
		//	T : 모든 환자들의 진료기록에서 마지막으로 추가된 병명이 입력으로 주어진 병명과 같은
		//	환자의 수(T ≥ 0) - 설명 : 트리에 저장된 모든 환자들에 대해, 마지막으로 진단받은 병명이 입력으로 주어진 병명과
		//	동일하면 집계하고, 집계된 수를 출력하여 유행성의 정도를 파악한다.편의상, 골절 같은 전염성이
		//	없는 병명도 유행병으로 간주한다.
		cout << traversal(root, _diseaseName, 0) << "\n"; // 트리를 순회하며 병명의 수를 집계한 후 출력한다.
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