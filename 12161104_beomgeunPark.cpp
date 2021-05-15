//12161104 박범근 알고리즘 1차과제

#include <iostream>
#include <vector>

using namespace std;

enum class Color { // 노드의 color를 red와 black으로 제한하기 위한 enum 클래스
	COLOR_RED,
	COLOR_BLACK
};

// 원래 pair<int, int>와 pair<string, int>를 사용하려했지만 변수명을 직관적으로 사용하기 위해 클래스화 (first, second 대신 각 이름)
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
	// 환자들의 진료정보를 원소로 저장하기 위해 vector 사용
	vector<string> diseaseName; 
	vector<int> price;

	Record(string _diseaseName, int _price) { // vector에 각각 추가해준다.
		this->diseaseName.push_back(_diseaseName);
		this->price.push_back(_price);
	}

	void addDiseaseName(string _diseaseName) { 
		this->diseaseName.push_back(_diseaseName); // vector에 추가해준다.
	}

	void addPrice(int _price) {
		this->price.push_back(_price); // vector에 추가해준다.
	}
};

class Patient { // 환자 정보를 담을 환자 클래스
public:
	int id; // 환자번호 (기준키, 유일함): 1,000,000 ~ 1,999,999 사이의 정수. 
	string name; // 이름 : 공백 없는 20 bytes 이내의 문자열
	string phoneNumber; // 연락처 : 회원의 전화번호(공백 없는 11 bytes 이내의 문자열)
	Address_XY  * address; // 주소 좌표(x, y) : x와 y는 각각 0 ~10, 000 사이의 임의의 정수.
	Record  * record; //진료기록 : 아래 정보들의 나열

	Patient(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		Address_XY * _address_xy = new Address_XY(_addressX, _addressY); // 클래스 객체 생성
		Record  * _record = new Record(_diseaseName, _price);// 클래스 객체 생성

		this->id = _id;
		this->name = _name;
		this->phoneNumber = _phoneNumber;
		this->address = _address_xy;
		this->record = _record;
	} // 환자 정보를 입력받아 환자 객체를 생성하는 생성자 함수
};

class Node { // 레드블랙트리에서 각 노드를 구성하기 위한 노드 클래스
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
		// left right setcolor하면 null이라 오류가 나므로 색 판별시 null(leaf) 또한 black으로 계산해준다.
		this->setColor(Color::COLOR_RED);
	} // 새 노드 추가 시, 해당 환자정보, depth = 0, red color, 자식노드 (null)black으로 생성

	void setColor(Color c) {
		if (this == NULL) { // Null 노드를 참조 시 종료해준다.
			return;
		}
		else
			this->color = c;
	}

	int getID() {
		return this->patient->id;
	}

	Node* getParentNode() { // 현재 노드의 부모 노드를 리턴
		return this->parent;
	}

	Node* getUncleNode() { // 현재 노드에서 부모의 형제노드를 리턴
		Node* parentNode = this->parent;
		Node* grandParent = parentNode->parent;

		if (grandParent->left == parentNode) { // 만약 grand의 left가 부모면 오른쪽이 uncleNode이다.
			return grandParent->right;
		}
		else { // grand의 right가 부모이면 왼쪽이 uncleNode이다.
			return grandParent->left;
		}
	}

	Node* getGrandNode() { // 현재 노드에서 부모의 부모 노드를 리턴
		return this->parent->parent;
	}
};

class Red_BlackTree { // 레드 블랙 트리를 구현하기 위한 클래스
public:
	Node* root; // 트리 멤버 변수
	
	Red_BlackTree() { // 생성자로는 NULL로 초기화해준다.
		this->root = NULL;
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

	bool isBlack(Node* curNode) { // 받은 node의 black 체크(블랙이거나 리프노드(null)이면 true 리턴)
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

		return grandNode; // grand가 red로 바뀌는 케이스이므로 grand와 grand의 parent의 doubleRed를 비교해야함
	}

	Node* restructuring(Node* curNode) { // case 4개로 나눠서 트리를 수정해준다.
		bool isRoot = false;

		Node* grandNode = curNode->getGrandNode();
		Node* parentNode = curNode->getParentNode();
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
		// grand, parent, curNode의 키값으로 케이스를 나눠서 restructure를 진행한다.

		// case 1 curNode가 루트가 된다. grandNode가 leftchild, parentNode가 rightChild
		if (grandID < curID && curID < parentID) { 
			if (!isRoot) { // restructure된 서브트리를 연결시켜준다.
				if (grandParentID > curID) {
					grandParentNode->left = curNode;
				}
				else {
					grandParentNode->right = curNode;
				}
				curNode->parent = grandParentNode; // curNode가 서브트리의 루트이기에 기존 트리의 부모와 연결시켜준다.
			}
			else {
				curNode->parent = NULL; // curNode가 루트가 된다.
				root = curNode; // 루트를 바꿔준다.
			} // 여기까지 curNode와 grandParentNode의 관계 정리

			grandNode->right = curNode->left;
			if (curNode->left != NULL) // 만약 left subtree에 노드 정보가 있다면 이 노드들도 parent를 바꿔야한다.
				curNode->left->parent = grandNode;
			curNode->left = grandNode;
			grandNode->parent = curNode;
			// 여기까지 curNode와 grandNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)

			parentNode->left = curNode->right;
			if (curNode->right != NULL) // 만약 right subtree에 노드 정보가 있다면 이 노드들도 parent를 바꿔야한다.
				curNode->right->parent = parentNode;
			curNode->right = parentNode;
			parentNode->parent = curNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
			curNode->setColor(Color::COLOR_BLACK);
			parentNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);

			return curNode; // 이 서브트리 루트를 기준으로 depth를 update해줘야하기 때문에 리턴해준다.
		}
		// case 2 curNode가 루트가 된다. parentNode가 leftchild, grandNode가 rightChild
		else if (parentID < curID && curID < grandID) { 
			if (!isRoot) { // restructure된 서브트리를 연결시켜준다.
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
				root = curNode; // 루트를 바꿔준다.
			} // 여기까지 curNode와 grandParentNode의 관계 정리

			parentNode->right = curNode->left;
			if(curNode->left != NULL)  // 만약 left subtree에 노드 정보가 있다면 이 노드들도 parent를 바꿔야한다.
				curNode->left->parent = parentNode;
			curNode->left = parentNode;
			parentNode->parent = curNode;
			// 여기까지 curNode와 grandNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)

			grandNode->left = curNode->right;
			if (curNode->right != NULL)  // 만약 right subtree에 노드 정보가 있다면 이 노드들도 parent를 바꿔야한다.
				curNode->right->parent = grandNode;
			curNode->right = grandNode;
			grandNode->parent = curNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
			curNode->setColor(Color::COLOR_BLACK);
			parentNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);
			
			return curNode; // 이 서브트리 루트를 기준으로 depth를 update해줘야하기 때문에 리턴해준다.
		}
		// case 3 parentNode가 루트가 된다. grandNode가 rightchild, curNode는 그대로 parent의 leftchild
		else if (curID < parentID && parentID < grandID) { 
			if (!isRoot) { // restructure된 서브트리를 연결시켜준다.
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
				root = parentNode; // 루트를 바꿔준다.
			} // 여기까지 parentNode와 grandParentNode의 관계 정리

			grandNode->left = parentNode->right;
			if (parentNode->right != NULL) // 만약 right subtree에 노드 정보가 있다면 이 노드들도 parent를 바꿔야한다.
				parentNode->right->parent = grandNode;
			parentNode->right = grandNode;
			grandNode->parent = parentNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
			// 이 케이스는 left subtree는 그대로 붙어있으면 된다.
			parentNode->setColor(Color::COLOR_BLACK);
			curNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);

			return parentNode; // 이 서브트리 루트를 기준으로 depth를 update해줘야하기 때문에 리턴해준다.
		}
		// case 4 parentNode가 루트가 된다. grandNode가 leftchild, curNode는 그대로 parent의 rightchild
		else if (grandID < parentID && parentID < curID) {
			if (!isRoot) { // restructure된 서브트리를 연결시켜준다.
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
				root = parentNode; // 루트를 바꿔준다.
			} // 여기까지 parentNode와 grandParentNode의 관계 정리

			grandNode->right = parentNode->left;
			if (parentNode->left != NULL) // 만약 left subtree에 노드 정보가 있다면 이 노드들도 parent를 바꿔야한다.
				parentNode->left->parent = grandNode;
			parentNode->left = grandNode;
			grandNode->parent = parentNode;
			// 여기까지 curNode와 parentNode의 관계 정리(curNode의 자식 옮기고, 자식으로 연결하고, 부모로 연결하고)
			parentNode->setColor(Color::COLOR_BLACK);
			curNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);

			return parentNode; // 이 서브트리 루트를 기준으로 depth를 update해줘야하기 때문에 리턴해준다.
		}
	}

	// restructure가 종료되면 depth가 바뀌기 때문에 해당 서브트리부터 depth를 다시 업데이트해준다.
	void depthTraversal(Node* newSubRoot, int _depth) {
		Node * curNode = newSubRoot;
		if (curNode != NULL) {
			curNode->depth = _depth;
			depthTraversal(curNode->left, _depth + 1);
			depthTraversal(curNode->right, _depth + 1);
		}
	}

	Node* insert(Node* insert_node) { // insertNode : 해당 환자정보, depth = 0, 자신 : red color, 자식 : black으로 생성
		if (root == NULL) { // 빈 트리일 경우 root에 insert해준다.
			root = insert_node;
			root->setColor(Color::COLOR_BLACK); // root의 경우 black으로 바꿔준다, root는 depth와 parent 정보 바꿔줄 필요 없다.
			return root; // 루트는 insert하고 종료(구조 바꿀 필요 없음)
		}
		else { // root가 있다면 자신의 자리를 찾아서 insert 후 recoloring 또는 restructure 진행
			Node* curNode = root;
			Node* upperNode = NULL;
			// 삽입할 위치 찾기
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

			while (isDoubleRed(z)) { // 노드 z를 기준으로 검사 시작
				// 만약 z의 grand가 null인 경우의 예외처리를 하려했지만 그 경우는 z의 parent가 root(black)인 경우로 isDoubleRed가 무조건 false가 나온다.
				if (isBlack((z->getUncleNode()))) { // uncle이 black이면 restructuring
					Node * newSubRoot = restructuring(z); // 새로 루트가 된 노드를 리턴
					// restructuring 이후 해당 subtree의 depth를 업데이트해준다.
					if (newSubRoot == root) // 루트면 depth 0부터 전체 업데이트
						depthTraversal(newSubRoot, 0);
					else // 아니면 서브트리의 부모의 depth+1를 받아서 업데이트(부모의 depth는 안바뀌었기 때문에 가져온다)
						depthTraversal(newSubRoot, newSubRoot->getParentNode()->depth+1);
					break; // restructure는 한번이면 끝난다.
				}
				else  { // uncle red이면 recoloring
					z = recoloring(z); 
					// 노드 z의 parent, uncle, grand의 색을 바꿈, grand가 root일 경우 색을 바꾸지 않음, 리턴값은 z의 grand -> grand가 red가 되므로 grand를 비교해야함
					//따라서 z는 grand가 되며 grand가 root가 될수도 있음 -> isDoubleRed함수에 root 예외처리 추가
				}
			}
			return insert_node; // 이 분기까지 오면 정상 insert이므로 insert한 노드 정보 리턴
		}
	}

	Node* find(int find_id) { // red-black tree의 find는 binary search tree의 find와 동일하다.
		Node* curNode = root;
		while (curNode != NULL) {
			if (curNode->getID() == find_id) {
				return curNode;
			}
			else if (curNode->getID() < find_id) {
				curNode = curNode->right;
			}
			else if (curNode->getID() > find_id) {
				curNode = curNode->left;
			}
		}
		//while문이 끝나면 찾지 못한 것이다.
		return NULL;
	}

	int diseaseTraversal(Node* node, string _diseaseName, int cnt) { 
		// 그래프를 전위순회하며 동일한 병명을 가진 노드의 수를 센다.
		int tempCnt_left = cnt;
		int tempCnt_right = cnt;
		if (node != NULL) {
			if (node->patient->record->diseaseName.back() == _diseaseName) // 병명 시퀀스의 가장 마지막 진단명(최근)가 일치하면 cnt++
				cnt++;
			tempCnt_left = diseaseTraversal(node->left, _diseaseName, cnt); //업데이트된 cnt를 갖고 left 탐색
			tempCnt_right = diseaseTraversal(node->right, _diseaseName, tempCnt_left); // left 탐색이 끝난 결과를 갖고 right 탐색
		}
		return tempCnt_right;
	}

	void query_I(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		// I 쿼리에 관한 함수
		// 정보를 받아 트리에 노드를 삽입하고, 삽입 결과 노드가 입력하려는 정보와 일치하면 정상 insert
		// 이미 중복된 id를 가진 노드가 있으면 기존 노드를 리턴하므로 일치하지 않고 기존노드의 depth와 거절을 출력한다.
		Patient* newPatientInfo = new Patient (_id, _name, _phoneNumber, _addressX, _addressY, _diseaseName, _price);
		Node* newNode = new Node(newPatientInfo);

		Node* insertResultNode = insert(newNode);
		if (insertResultNode->patient == newNode->patient) { // 정상 insert (입력 == 리턴)
			cout << insertResultNode->depth << " " << 1 << "\n"; // 승인코드 1
		}
		else { // 이미 중복된 insert(입력 != 리턴)
			cout << insertResultNode->depth << " " << 0 << "\n"; // 거절코드 0
		}
	}

	void query_F(int _id) {
		// F 쿼리에 관한 함수
		// find 함수를 이용해서 해당 id를 가진 노드를 리턴 후 관련 정보를 출력
		// 찾지 못하면 NULL이 리턴되고 Not found를 출력한다.
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
		// A 쿼리에 관한 함수
		//	일단 입력받은 id에 해당하는 노드를 find한다.
		Node* findNode = find(_id);
		if (findNode == NULL) { // 없다면 Not found 출력
			cout << "Not found" << "\n";
		}
		else { // 노드를 찾았다면 질병기록을 추가해준다.
			Patient* p = findNode->patient;

			p->record->addDiseaseName(_diseaseName);
			p->record->addPrice(_price);
			cout << findNode->depth <<"\n";
		}
		return;
	}

	void query_E(string _diseaseName) {
		// E 쿼리에 관한 함수
		//트리를 순회하면서 마지막에 진료받은 병명을 기준으로 더해주고 출력한다.
		cout << diseaseTraversal(root, _diseaseName, 0) << "\n"; // 트리를 순회하며 병명의 수를 집계한 후 출력한다.
	}

};

int queryNum, input_id, input_addressX, input_addressY, input_price;
string queryName, input_name, input_phoneNumber, input_diseaseName;

int main() {
	cin >> queryNum;
	Red_BlackTree redblackTree; // 레드블랙트리 초기화

	for (int t = 0; t < queryNum; t++) {
		cin >> queryName;
		if (queryName == "I") { // I = query_I 실행
			cin >> input_id >> input_name >> input_phoneNumber >> input_addressX >> input_addressY >> input_diseaseName >> input_price;
			redblackTree.query_I(input_id, input_name, input_phoneNumber, input_addressX, input_addressY, input_diseaseName, input_price);
		}
		else if (queryName == "F") { // F = query_F 실행
			cin >> input_id;
			redblackTree.query_F(input_id);
		}
		else if (queryName == "A") { // A = query_A 실행
			cin >> input_id >> input_diseaseName >> input_price;
			redblackTree.query_A(input_id, input_diseaseName, input_price);
		}
		else if (queryName == "E") { // E = query_E 실행
			cin >> input_diseaseName;
			redblackTree.query_E(input_diseaseName);
		}
	}
	return 0;
}