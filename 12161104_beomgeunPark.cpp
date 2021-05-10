//12161104 박범근 알고리즘 1차과제

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;
// 원래 pair<int, int>와 pair<string, int>를 사용했지만 변수명을 직관적으로 사용하기 위해 클래스화 (first, second -> 각 이름)
class Adress_XY { // 주소 좌표를 기록하기 위한 클래스
public:
	int x;
	int y;

	Adress_XY(int _x, int _y) {
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
	Adress_XY address; // 주소 좌표(x, y) : x와 y는 각각 0 ~10, 000 사이의 임의의 정수.
	Record record; //진료기록 : 아래 정보들의 나열 (	-병명 : 공백 없는 20 bytes 이내의 문자열.예) “Headache”, 진료비: 10, 000 ~100, 000 사이의 정수)
	// 환자 정보를 담을 멤버 변수

	Patient(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		Adress_XY _address_xy(_addressX, _addressY); // 클래스 객체 생성
		Record _record(_diseaseName, _price);// 클래스 객체 생성

		this->id = _id;
		this->name = _name;
		this->phoneNumber = _phoneNumber;
		this->address = _address_xy;
		this->record = _record;
	} // 환자 정보를 입력받아 환자 객체를 생성하는 생성자 멤버 함수
};

class Node {
public:
	Patient patient;
	Node* parent;
	Node* left;
	Node* right;
	int depth;

	Node(Patient _data) {
		this->patient = _data;
		parent = left = right = NULL;
	}
};

class Red_BlackTree {
public:
	Patient* root;

	void query_I(int _id, string _name, string _phoneNumber, int _addressX, int _addressY, string _diseaseName, int _price) {
		// insert (K N H Ax Ay DI C)쿼리에 관한 내용
		// parameter로 patient 객체 생성하고, 트리에 삽입하는 과정(red-black tree)
		//print : 노드 depth와 신규가입처리여부 
		//-질의형식: “I K N H Ax Ay DI C”
		//	I : 환자를 신규 가입하는 질의를 나타내는 기호
		//	K : 환자번호
		//	N : 이름
		//	H : 연락처
		//	Ax : 주소의 x 좌표
		//	Ay : 주소의 y 좌표
		//	DI : 병명
		//	C : 진료비
		//	- 출력형식 : “D S”
		//	D : 트리에서 환자 정보가 저장된 노드의 깊이(root의 깊이는 0)
		//	S : 신규 가입 처리를 나타내는 정수(0 또는 1) (0: 거절, 1 : 승인)
		//- 설명 : 새로운 환자 정보를 입력받아 레드블랙트리에 노드를 삽입하고, 그 노드의 깊이를 출력한다.
		//	만약 동일한 환자번호가 이미 존재한다면, 그 노드의 깊이를 출력하고 신규 가입은 거절된다.
	}

	void query_F(int _id) {
		//- 질의형식: “F K”
	//F: 특정 환자를 검색하는 질의를 나타내는 기호
	//	K : 환자번호
	//	- 출력형식 : “D N H Ax Ay” 또는 “Not found”
	//	D : 트리에서 환자 정보가 저장된 노드의 깊이
	//	N : 이름
	//	H : 연락처
	//	Ax : 주소의 x 좌표
	//	Ay : 주소의 y 좌표
	//	- 설명 : 해당 환자를 탐색하여 존재하면 환자의 정보를 출력한다.
	//	만약 존재하지 않으면, “Not	found”(주의 : 대소문자, 띄어쓰기 등 정확히 일치시킬 것)를 출력한다.
	}

	void query_A(int _id, string _diseaseName, int _price) {
		//-질의형식: “A K DI C”
		//	A : 특정 환자에 대해 새로운 진료 정보를 추가하는 질의를 나타내는 기호
		//	K : 환자번호
		//	DI : 병명
		//	C : 진료비
		//	- 출력형식 : “D” 또는 “Not found”
		//	D : 트리에서 환자 정보가 저장된 노드의 깊이
		//	- 설명 : 해당 환자를 탐색하여 입력받은 진료 내용을 진료기록에 추가하고, 환자에 대한 노드의
		//	깊이를 출력한다.만약 환자가 존재하지 않는 경우, “Not found”를 출력한다.

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
	}

};


int main() {

}