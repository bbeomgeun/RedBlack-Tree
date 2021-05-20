#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

class AreaInfo { // 지역정보 -> vertex
public:
	int ID; // 지역번호
	string areaName; // 지역이름
	bool isFlooding; // 침수여부 true(1) 침수, false(0) 침수 x

	AreaInfo(int _id, string _areaName, bool _isFlooding) {
		this->ID = _id;
		this->areaName = _areaName;
		this->isFlooding = _isFlooding;
	} // 생성자
};

class RoadInfo { // 두 지역 간 도로 정보 -> edge
public:
	int fromID; // 지역번호 1
	int toID; // 지역번호 2
	int weight; // 도로의 거리

	RoadInfo(int _fromID, int _toID, int _weight) {
		this->fromID = _fromID;
		this->toID = _toID;
		this->weight = _weight;
	} // 생성자
};

class Graph {
	// 먼저 입력받는 지역 정보를 저장해둬야하고, 연결정보를 받으면 연결된 지역끼리 묶고
	// 그 이후 dijkstra를 돌리면 된다(최단거리 및 최단경로)
public:
	Graph() {
		map<AreaInfo*, vector<AreaInfo*>> adjacencyList;
	}

	int dijkstra_A() { // 최단거리
		
	}

	int dijkstra_B() { // 최단경로
		
	}

	void query_A(int _fromID, int _toID) {

	}

	void query_B(int _fromID, int _toID) {

	}
};

int N, M, Q; // 지역정보의 수, 지역 간의 도로 정보의 수, 질의의 수
int input_id, input_flooding, input_fromID, input_toID, input_weight;
string queryName, input_name;

int main() {
	cin >> N >> M >> Q;
	Graph graph;

	for (int i = 0; i < N; i++) { // 지역 정보 입력받기
		cin >> input_id >> input_name >> input_flooding;
	}
	for (int i = 0; i < M; i++) { // 지역 간의 도로 정보 입력 받기
		cin >> input_fromID >> input_toID >> input_weight;
	}
	for (int i = 0; i < Q; i++) { // 질의 입력
		cin >> queryName >> input_fromID >> input_toID;
		if (queryName == "A") { // 두 지역 간의 최단 거리 찾기
			graph.query_A(input_fromID, input_toID);
		}
		else if (queryName == "B") { // 두 지역 간의 최단 경로 찾기
			graph.query_B(input_fromID, input_toID);
		}
	}
	return 0;
}