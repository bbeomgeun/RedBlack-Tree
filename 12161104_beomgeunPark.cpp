//12161104 �ڹ��� �˰��� 1������

#include <iostream>
#include <vector>

using namespace std;

enum class Color { // ����� color�� red�� black���� �����ϱ� ���� enum Ŭ����
	COLOR_RED,
	COLOR_BLACK
};

// ���� pair<int, int>�� pair<string, int>�� ����Ϸ������� �������� ���������� ����ϱ� ���� Ŭ����ȭ (first, second ��� �� �̸�)
class Address_XY { // �ּ� ��ǥ�� ����ϱ� ���� Ŭ����
public:
	int x;
	int y;

	Address_XY(int _x, int _y) {
		this->x = _x;
		this->y = _y;
	}
};

class Record { // �������� �����ϱ� ���� Ŭ����
public:
	// ȯ�ڵ��� ���������� ���ҷ� �����ϱ� ���� vector ���
	vector<string> diseaseName; 
	vector<int> price;

	Record(string _diseaseName, int _price) { // vector�� ���� �߰����ش�.
		this->diseaseName.push_back(_diseaseName);
		this->price.push_back(_price);
	}

	void addDiseaseName(string _diseaseName) { 
		this->diseaseName.push_back(_diseaseName); // vector�� �߰����ش�.
	}

	void addPrice(int _price) {
		this->price.push_back(_price); // vector�� �߰����ش�.
	}
};

class Patient { // ȯ�� ������ ���� ȯ�� Ŭ����
public:
	int id; // ȯ�ڹ�ȣ (����Ű, ������): 1,000,000 ~ 1,999,999 ������ ����. 
	string name; // �̸� : ���� ���� 20 bytes �̳��� ���ڿ�
	string phoneNumber; // ����ó : ȸ���� ��ȭ��ȣ(���� ���� 11 bytes �̳��� ���ڿ�)
	Address_XY  * address; // �ּ� ��ǥ(x, y) : x�� y�� ���� 0 ~10, 000 ������ ������ ����.
	Record  * record; //������ : �Ʒ� �������� ����

	Patient(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		Address_XY * _address_xy = new Address_XY(_addressX, _addressY); // Ŭ���� ��ü ����
		Record  * _record = new Record(_diseaseName, _price);// Ŭ���� ��ü ����

		this->id = _id;
		this->name = _name;
		this->phoneNumber = _phoneNumber;
		this->address = _address_xy;
		this->record = _record;
	} // ȯ�� ������ �Է¹޾� ȯ�� ��ü�� �����ϴ� ������ �Լ�
};

class Node { // �����Ʈ������ �� ��带 �����ϱ� ���� ��� Ŭ����
public:
	Patient * patient; // ����� ȯ�� ����
	Node* parent; // �θ� ���
	Node* left; // ���� ���
	Node* right; // ������ ���
	int depth; // ����� depth
	Color color; // ����� color

	Node(Patient * _data) { 
		this->patient = _data;
		parent = left = right =NULL;
		depth = 0;
		// left right setcolor�ϸ� null�̶� ������ ���Ƿ� �� �Ǻ��� null(leaf) ���� black���� ������ش�.
		this->setColor(Color::COLOR_RED);
	} // �� ��� �߰� ��, �ش� ȯ������, depth = 0, red color, �ڽĳ�� (null)black���� ����

	void setColor(Color c) {
		if (this == NULL) { // Null ��带 ���� �� �������ش�.
			return;
		}
		else
			this->color = c;
	}

	int getID() {
		return this->patient->id;
	}

	Node* getParentNode() { // ���� ����� �θ� ��带 ����
		return this->parent;
	}

	Node* getUncleNode() { // ���� ��忡�� �θ��� ������带 ����
		Node* parentNode = this->parent;
		Node* grandParent = parentNode->parent;

		if (grandParent->left == parentNode) { // ���� grand�� left�� �θ�� �������� uncleNode�̴�.
			return grandParent->right;
		}
		else { // grand�� right�� �θ��̸� ������ uncleNode�̴�.
			return grandParent->left;
		}
	}

	Node* getGrandNode() { // ���� ��忡�� �θ��� �θ� ��带 ����
		return this->parent->parent;
	}
};

class Red_BlackTree { // ���� �� Ʈ���� �����ϱ� ���� Ŭ����
public:
	Node* root; // Ʈ�� ��� ����
	
	Red_BlackTree() { // �����ڷδ� NULL�� �ʱ�ȭ���ش�.
		this->root = NULL;
	}

	bool isDoubleRed(Node* curNode) { // doubleRed üũ (���� ���� �θ� ����� color�� ��� red�� true)
		if (curNode == root) // ���� root��带 üũ�Ϸ��� ������ ������ ���Ƿ� false return
			return false;

		Node* parent = curNode->getParentNode();
		if (curNode->color == Color::COLOR_RED && parent->color == Color::COLOR_RED)
			return true;
		else
			return false;
	}

	bool isBlack(Node* curNode) { // ���� node�� black üũ(���̰ų� �������(null)�̸� true ����)
		if (curNode == NULL)
			return true;
		else
			return curNode->color == Color::COLOR_BLACK;
	}

	Node* recoloring(Node* curNode) { // ���� ��� z�� parent, grand, uncle�� ���� �ٲ۴�. ������ �����̹Ƿ� ���� �����
		Node* parentNode = curNode->getParentNode();
		Node* grandNode = curNode->getGrandNode();
		Node* uncleNode = curNode->getUncleNode();

		if (grandNode != root) { // ��Ʈ�� �ƴ� ��� red�� �������ش�. (��Ʈ�� ������ ��)
			grandNode->setColor(Color::COLOR_RED);
		}
		uncleNode->setColor(Color::COLOR_BLACK);
		parentNode->setColor(Color::COLOR_BLACK);

		return grandNode; // grand�� red�� �ٲ�� ���̽��̹Ƿ� grand�� grand�� parent�� doubleRed�� ���ؾ���
	}

	Node* restructuring(Node* curNode) { // case 4���� ������ Ʈ���� �������ش�.
		bool isRoot = false;

		Node* grandNode = curNode->getGrandNode();
		Node* parentNode = curNode->getParentNode();
		Node* grandParentNode = NULL; // grand�� ��Ʈ�̸� ������ �Ұ����ϱ⿡ NULL�� �ʱ�ȭ

		int grandParentID = -1; // grand�� ��Ʈ�̸� ������ �Ұ����ϱ⿡ -1�� �ʱ�ȭ
		int grandID = grandNode->getID();
		int parentID = parentNode->getID();
		int curID = curNode->getID();

		if (grandNode == root) {
			isRoot = true;
		}
		else { // grandNode�� ��Ʈ�� �ƴϸ� ���� ����Ʈ���� ��Ʈ�� �ٲ�� ������ ����Ʈ���� �θ� �����ϱ� ����
			grandParentNode = grandNode->getParentNode(); 
			grandParentID =  grandParentNode->getID();
		}
		// grand, parent, curNode�� Ű������ ���̽��� ������ restructure�� �����Ѵ�.

		// case 1 curNode�� ��Ʈ�� �ȴ�. grandNode�� leftchild, parentNode�� rightChild
		if (grandID < curID && curID < parentID) { 
			if (!isRoot) { // restructure�� ����Ʈ���� ��������ش�.
				if (grandParentID > curID) {
					grandParentNode->left = curNode;
				}
				else {
					grandParentNode->right = curNode;
				}
				curNode->parent = grandParentNode; // curNode�� ����Ʈ���� ��Ʈ�̱⿡ ���� Ʈ���� �θ�� ��������ش�.
			}
			else {
				curNode->parent = NULL; // curNode�� ��Ʈ�� �ȴ�.
				root = curNode; // ��Ʈ�� �ٲ��ش�.
			} // ������� curNode�� grandParentNode�� ���� ����

			grandNode->right = curNode->left;
			if (curNode->left != NULL) // ���� left subtree�� ��� ������ �ִٸ� �� ���鵵 parent�� �ٲ���Ѵ�.
				curNode->left->parent = grandNode;
			curNode->left = grandNode;
			grandNode->parent = curNode;
			// ������� curNode�� grandNode�� ���� ����(curNode�� �ڽ� �ű��, �ڽ����� �����ϰ�, �θ�� �����ϰ�)

			parentNode->left = curNode->right;
			if (curNode->right != NULL) // ���� right subtree�� ��� ������ �ִٸ� �� ���鵵 parent�� �ٲ���Ѵ�.
				curNode->right->parent = parentNode;
			curNode->right = parentNode;
			parentNode->parent = curNode;
			// ������� curNode�� parentNode�� ���� ����(curNode�� �ڽ� �ű��, �ڽ����� �����ϰ�, �θ�� �����ϰ�)
			curNode->setColor(Color::COLOR_BLACK);
			parentNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);

			return curNode; // �� ����Ʈ�� ��Ʈ�� �������� depth�� update������ϱ� ������ �������ش�.
		}
		// case 2 curNode�� ��Ʈ�� �ȴ�. parentNode�� leftchild, grandNode�� rightChild
		else if (parentID < curID && curID < grandID) { 
			if (!isRoot) { // restructure�� ����Ʈ���� ��������ش�.
				if (grandParentID > curID) {
					grandParentNode->left = curNode;
				}
				else {
					grandParentNode->right = curNode;
				}
				curNode->parent = grandParentNode;
			}
			else { 
				curNode->parent = NULL; // curNode�� ��Ʈ�� �ȴ�.
				root = curNode; // ��Ʈ�� �ٲ��ش�.
			} // ������� curNode�� grandParentNode�� ���� ����

			parentNode->right = curNode->left;
			if(curNode->left != NULL)  // ���� left subtree�� ��� ������ �ִٸ� �� ���鵵 parent�� �ٲ���Ѵ�.
				curNode->left->parent = parentNode;
			curNode->left = parentNode;
			parentNode->parent = curNode;
			// ������� curNode�� grandNode�� ���� ����(curNode�� �ڽ� �ű��, �ڽ����� �����ϰ�, �θ�� �����ϰ�)

			grandNode->left = curNode->right;
			if (curNode->right != NULL)  // ���� right subtree�� ��� ������ �ִٸ� �� ���鵵 parent�� �ٲ���Ѵ�.
				curNode->right->parent = grandNode;
			curNode->right = grandNode;
			grandNode->parent = curNode;
			// ������� curNode�� parentNode�� ���� ����(curNode�� �ڽ� �ű��, �ڽ����� �����ϰ�, �θ�� �����ϰ�)
			curNode->setColor(Color::COLOR_BLACK);
			parentNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);
			
			return curNode; // �� ����Ʈ�� ��Ʈ�� �������� depth�� update������ϱ� ������ �������ش�.
		}
		// case 3 parentNode�� ��Ʈ�� �ȴ�. grandNode�� rightchild, curNode�� �״�� parent�� leftchild
		else if (curID < parentID && parentID < grandID) { 
			if (!isRoot) { // restructure�� ����Ʈ���� ��������ش�.
				if (grandParentID > parentID) {
					grandParentNode->left = parentNode;
				}
				else {
					grandParentNode->right = parentNode;
				}
				parentNode->parent = grandParentNode;
			}
			else {
				parentNode->parent = NULL; // parentNode�� ��Ʈ�� �ȴ�.
				root = parentNode; // ��Ʈ�� �ٲ��ش�.
			} // ������� parentNode�� grandParentNode�� ���� ����

			grandNode->left = parentNode->right;
			if (parentNode->right != NULL) // ���� right subtree�� ��� ������ �ִٸ� �� ���鵵 parent�� �ٲ���Ѵ�.
				parentNode->right->parent = grandNode;
			parentNode->right = grandNode;
			grandNode->parent = parentNode;
			// ������� curNode�� parentNode�� ���� ����(curNode�� �ڽ� �ű��, �ڽ����� �����ϰ�, �θ�� �����ϰ�)
			// �� ���̽��� left subtree�� �״�� �پ������� �ȴ�.
			parentNode->setColor(Color::COLOR_BLACK);
			curNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);

			return parentNode; // �� ����Ʈ�� ��Ʈ�� �������� depth�� update������ϱ� ������ �������ش�.
		}
		// case 4 parentNode�� ��Ʈ�� �ȴ�. grandNode�� leftchild, curNode�� �״�� parent�� rightchild
		else if (grandID < parentID && parentID < curID) {
			if (!isRoot) { // restructure�� ����Ʈ���� ��������ش�.
				if (grandParentID > parentID) {
					grandParentNode->left = parentNode;
				}
				else {
					grandParentNode->right = parentNode;
				}
				parentNode->parent = grandParentNode;
			}
			else {
				parentNode->parent = NULL; // parentNode�� ��Ʈ�� �ȴ�.
				root = parentNode; // ��Ʈ�� �ٲ��ش�.
			} // ������� parentNode�� grandParentNode�� ���� ����

			grandNode->right = parentNode->left;
			if (parentNode->left != NULL) // ���� left subtree�� ��� ������ �ִٸ� �� ���鵵 parent�� �ٲ���Ѵ�.
				parentNode->left->parent = grandNode;
			parentNode->left = grandNode;
			grandNode->parent = parentNode;
			// ������� curNode�� parentNode�� ���� ����(curNode�� �ڽ� �ű��, �ڽ����� �����ϰ�, �θ�� �����ϰ�)
			parentNode->setColor(Color::COLOR_BLACK);
			curNode->setColor(Color::COLOR_RED);
			grandNode->setColor(Color::COLOR_RED);

			return parentNode; // �� ����Ʈ�� ��Ʈ�� �������� depth�� update������ϱ� ������ �������ش�.
		}
	}

	// restructure�� ����Ǹ� depth�� �ٲ�� ������ �ش� ����Ʈ������ depth�� �ٽ� ������Ʈ���ش�.
	void depthTraversal(Node* newSubRoot, int _depth) {
		Node * curNode = newSubRoot;
		if (curNode != NULL) {
			curNode->depth = _depth;
			depthTraversal(curNode->left, _depth + 1);
			depthTraversal(curNode->right, _depth + 1);
		}
	}

	Node* insert(Node* insert_node) { // insertNode : �ش� ȯ������, depth = 0, �ڽ� : red color, �ڽ� : black���� ����
		if (root == NULL) { // �� Ʈ���� ��� root�� insert���ش�.
			root = insert_node;
			root->setColor(Color::COLOR_BLACK); // root�� ��� black���� �ٲ��ش�, root�� depth�� parent ���� �ٲ��� �ʿ� ����.
			return root; // ��Ʈ�� insert�ϰ� ����(���� �ٲ� �ʿ� ����)
		}
		else { // root�� �ִٸ� �ڽ��� �ڸ��� ã�Ƽ� insert �� recoloring �Ǵ� restructure ����
			Node* curNode = root;
			Node* upperNode = NULL;
			// ������ ��ġ ã��
			while (curNode != NULL) {
				upperNode = curNode;
				if (curNode->getID() == insert_node->getID()) { // ���� id�� �ߺ��ȴٸ� insert ����
					return curNode; // �ߺ��� ��带 ��ȯ�Ѵ� (query_I���� ���ؼ� ���ϰ��� insert�Ϸ��� �Ͱ� ������ ����, �ٸ� ������ ����
				}
				else if (curNode->getID() > insert_node->getID()) { // insert���� ���� ����� key�� �� ũ�� left�� ����
					curNode = curNode->left;
				}
				else if (curNode->getID() < insert_node->getID()) { // ���� ����� key�� �� ������ right���� ����.
					curNode = curNode->right;
				}
			} // while�� ����Ǹ� upperNode�� �ڽ� ��ġ�� _insertNode�� ���Եȴ�. left �Ǵ� right�� ��������, ���� curNode�� null

			if (upperNode->getID() > insert_node->getID()) {
				upperNode->left = insert_node;
			}
			else if (upperNode->getID() < insert_node-> getID()) {
				upperNode->right = insert_node;
			}
			insert_node->parent = upperNode;
			insert_node->depth = upperNode->depth + 1;
			// ���� insert_node �θ𿬰�, depth, patient, color - red, child - black ����

			Node * z = insert_node; // ���س�� z�� insert �� ��� ���谡 �ϼ��� insert_node�� ����

			while (isDoubleRed(z)) { // ��� z�� �������� �˻� ����
				// ���� z�� grand�� null�� ����� ����ó���� �Ϸ������� �� ���� z�� parent�� root(black)�� ���� isDoubleRed�� ������ false�� ���´�.
				if (isBlack((z->getUncleNode()))) { // uncle�� black�̸� restructuring
					Node * newSubRoot = restructuring(z); // ���� ��Ʈ�� �� ��带 ����
					// restructuring ���� �ش� subtree�� depth�� ������Ʈ���ش�.
					if (newSubRoot == root) // ��Ʈ�� depth 0���� ��ü ������Ʈ
						depthTraversal(newSubRoot, 0);
					else // �ƴϸ� ����Ʈ���� �θ��� depth+1�� �޾Ƽ� ������Ʈ(�θ��� depth�� �ȹٲ���� ������ �����´�)
						depthTraversal(newSubRoot, newSubRoot->getParentNode()->depth+1);
					break; // restructure�� �ѹ��̸� ������.
				}
				else  { // uncle red�̸� recoloring
					z = recoloring(z); 
					// ��� z�� parent, uncle, grand�� ���� �ٲ�, grand�� root�� ��� ���� �ٲ��� ����, ���ϰ��� z�� grand -> grand�� red�� �ǹǷ� grand�� ���ؾ���
					//���� z�� grand�� �Ǹ� grand�� root�� �ɼ��� ���� -> isDoubleRed�Լ��� root ����ó�� �߰�
				}
			}
			return insert_node; // �� �б���� ���� ���� insert�̹Ƿ� insert�� ��� ���� ����
		}
	}

	Node* find(int find_id) { // red-black tree�� find�� binary search tree�� find�� �����ϴ�.
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
		//while���� ������ ã�� ���� ���̴�.
		return NULL;
	}

	int diseaseTraversal(Node* node, string _diseaseName, int cnt) { 
		// �׷����� ������ȸ�ϸ� ������ ������ ���� ����� ���� ����.
		int tempCnt_left = cnt;
		int tempCnt_right = cnt;
		if (node != NULL) {
			if (node->patient->record->diseaseName.back() == _diseaseName) // ���� �������� ���� ������ ���ܸ�(�ֱ�)�� ��ġ�ϸ� cnt++
				cnt++;
			tempCnt_left = diseaseTraversal(node->left, _diseaseName, cnt); //������Ʈ�� cnt�� ���� left Ž��
			tempCnt_right = diseaseTraversal(node->right, _diseaseName, tempCnt_left); // left Ž���� ���� ����� ���� right Ž��
		}
		return tempCnt_right;
	}

	void query_I(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		// I ������ ���� �Լ�
		// ������ �޾� Ʈ���� ��带 �����ϰ�, ���� ��� ��尡 �Է��Ϸ��� ������ ��ġ�ϸ� ���� insert
		// �̹� �ߺ��� id�� ���� ��尡 ������ ���� ��带 �����ϹǷ� ��ġ���� �ʰ� ��������� depth�� ������ ����Ѵ�.
		Patient* newPatientInfo = new Patient (_id, _name, _phoneNumber, _addressX, _addressY, _diseaseName, _price);
		Node* newNode = new Node(newPatientInfo);

		Node* insertResultNode = insert(newNode);
		if (insertResultNode->patient == newNode->patient) { // ���� insert (�Է� == ����)
			cout << insertResultNode->depth << " " << 1 << "\n"; // �����ڵ� 1
		}
		else { // �̹� �ߺ��� insert(�Է� != ����)
			cout << insertResultNode->depth << " " << 0 << "\n"; // �����ڵ� 0
		}
	}

	void query_F(int _id) {
		// F ������ ���� �Լ�
		// find �Լ��� �̿��ؼ� �ش� id�� ���� ��带 ���� �� ���� ������ ���
		// ã�� ���ϸ� NULL�� ���ϵǰ� Not found�� ����Ѵ�.
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
		// A ������ ���� �Լ�
		//	�ϴ� �Է¹��� id�� �ش��ϴ� ��带 find�Ѵ�.
		Node* findNode = find(_id);
		if (findNode == NULL) { // ���ٸ� Not found ���
			cout << "Not found" << "\n";
		}
		else { // ��带 ã�Ҵٸ� ��������� �߰����ش�.
			Patient* p = findNode->patient;

			p->record->addDiseaseName(_diseaseName);
			p->record->addPrice(_price);
			cout << findNode->depth <<"\n";
		}
		return;
	}

	void query_E(string _diseaseName) {
		// E ������ ���� �Լ�
		//Ʈ���� ��ȸ�ϸ鼭 �������� ������� ������ �������� �����ְ� ����Ѵ�.
		cout << diseaseTraversal(root, _diseaseName, 0) << "\n"; // Ʈ���� ��ȸ�ϸ� ������ ���� ������ �� ����Ѵ�.
	}

};

int queryNum, input_id, input_addressX, input_addressY, input_price;
string queryName, input_name, input_phoneNumber, input_diseaseName;

int main() {
	cin >> queryNum;
	Red_BlackTree redblackTree; // �����Ʈ�� �ʱ�ȭ

	for (int t = 0; t < queryNum; t++) {
		cin >> queryName;
		if (queryName == "I") { // I = query_I ����
			cin >> input_id >> input_name >> input_phoneNumber >> input_addressX >> input_addressY >> input_diseaseName >> input_price;
			redblackTree.query_I(input_id, input_name, input_phoneNumber, input_addressX, input_addressY, input_diseaseName, input_price);
		}
		else if (queryName == "F") { // F = query_F ����
			cin >> input_id;
			redblackTree.query_F(input_id);
		}
		else if (queryName == "A") { // A = query_A ����
			cin >> input_id >> input_diseaseName >> input_price;
			redblackTree.query_A(input_id, input_diseaseName, input_price);
		}
		else if (queryName == "E") { // E = query_E ����
			cin >> input_diseaseName;
			redblackTree.query_E(input_diseaseName);
		}
	}
	return 0;
}