#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

class AreaInfo { // �������� -> vertex
public:
	int ID; // ������ȣ
	string areaName; // �����̸�
	bool isFlooding; // ħ������ true(1) ħ��, false(0) ħ�� x

	AreaInfo(int _id, string _areaName, bool _isFlooding) {
		this->ID = _id;
		this->areaName = _areaName;
		this->isFlooding = _isFlooding;
	} // ������
};

class RoadInfo { // �� ���� �� ���� ���� -> edge
public:
	int fromID; // ������ȣ 1
	int toID; // ������ȣ 2
	int weight; // ������ �Ÿ�

	RoadInfo(int _fromID, int _toID, int _weight) {
		this->fromID = _fromID;
		this->toID = _toID;
		this->weight = _weight;
	} // ������
};

class Graph {
	// ���� �Է¹޴� ���� ������ �����ص־��ϰ�, ���������� ������ ����� �������� ����
	// �� ���� dijkstra�� ������ �ȴ�(�ִܰŸ� �� �ִܰ��)
public:
	Graph() {
		map<AreaInfo*, vector<AreaInfo*>> adjacencyList;
	}

	int dijkstra_A() { // �ִܰŸ�
		
	}

	int dijkstra_B() { // �ִܰ��
		
	}

	void query_A(int _fromID, int _toID) {

	}

	void query_B(int _fromID, int _toID) {

	}
};

int N, M, Q; // ���������� ��, ���� ���� ���� ������ ��, ������ ��
int input_id, input_flooding, input_fromID, input_toID, input_weight;
string queryName, input_name;

int main() {
	cin >> N >> M >> Q;
	Graph graph;

	for (int i = 0; i < N; i++) { // ���� ���� �Է¹ޱ�
		cin >> input_id >> input_name >> input_flooding;
	}
	for (int i = 0; i < M; i++) { // ���� ���� ���� ���� �Է� �ޱ�
		cin >> input_fromID >> input_toID >> input_weight;
	}
	for (int i = 0; i < Q; i++) { // ���� �Է�
		cin >> queryName >> input_fromID >> input_toID;
		if (queryName == "A") { // �� ���� ���� �ִ� �Ÿ� ã��
			graph.query_A(input_fromID, input_toID);
		}
		else if (queryName == "B") { // �� ���� ���� �ִ� ��� ã��
			graph.query_B(input_fromID, input_toID);
		}
	}
	return 0;
}