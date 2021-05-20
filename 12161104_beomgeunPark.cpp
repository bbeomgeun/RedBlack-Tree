//12161104 박범근 알고리즘 2차과제

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <stack>

#define MAX 1000000 // 10^6
#define INF 987654321

using namespace std;

enum class Status {
	UNSEEN,
	FRINGE,
	TREE
};

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

class Graph { // 무향그래프
	// 먼저 입력받는 지역 정보를 저장해둬야하고, 연결정보를 받으면 연결된 지역끼리 묶고
	// 그 이후 dijkstra를 돌리면 된다(최단거리 및 최단경로)
public:
	//map<AreaInfo * , vector<pair<int, AreaInfo * >>> adjacencyList;
	map<int, vector<pair<int, int>>> adjacencyList;
	AreaInfo* indexArray[MAX]; // 지역번호 100000~999999
	pair<int, int> parent_id[MAX]; // 부모번호와 최단거리
	bool fringeCheck[MAX];
	int distanceArray[MAX];

	Graph() {
		memset(fringeCheck, false, sizeof(bool) * MAX);
		memset(distanceArray, -1, sizeof(int) * MAX);
	}

	void insertVertex(int _id, string _name, bool _flood) {
		AreaInfo* vertex = new AreaInfo(_id, _name, _flood); // 입력받은 정보로 vertex 생성
		//this->adjacencyList[vertex]; // 인접리스트 map에 저장
		indexArray[_id] = vertex; // id와 vertex 정보 매핑
	} // vertex 정보 저장 및 index 정보 저장

	void connectVertex(int _fromID, int _toID, int _weight) {
		/*AreaInfo *from_vertex = indexArray[_fromID];
		AreaInfo *to_vertex = indexArray[_toID];
		adjacencyList[from_vertex].push_back(make_pair(_weight, to_vertex));
		adjacencyList[to_vertex].push_back(make_pair(_weight, from_vertex));*/ // AreaInfo class point 저장 버전
		adjacencyList[_fromID].push_back(make_pair(_weight, _toID));
		adjacencyList[_toID].push_back(make_pair(_weight, _fromID));
		// 무향그래프이므로 양쪽 모두 연결
	}

	bool isFlooding(int _id) {
		return getVertex(_id)->isFlooding;
	}

	AreaInfo* getVertex(int _id) {
		return indexArray[_id];
	}

	pair<int, int>* dijkstra_A(int from_id, int to_id) { // 최단거리
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //weight, id

		for (int i = 0; i < MAX; i++) {
			distanceArray[i] = INF;
		}

		pq.push(make_pair(0, from_id));
		distanceArray[from_id] = 0;
		parent_id[from_id] = make_pair(-1, 0);
		fringeCheck[from_id] = true;

		while (!pq.empty()) {
			int distance = pq.top().first;
			int here = pq.top().second;
			//AreaInfo* now_vertex = getVertex(here);
			pq.pop();

			if (here == to_id) {
				return parent_id;
			}

			for (int i = 0; i < adjacencyList[here].size(); i++) {
				int next = adjacencyList[here][i].second;
				int weight = adjacencyList[here][i].first;

				if (distanceArray[next] > distance + weight && !isFlooding(next)) { // 거리가 update되면서 범람되지 않은 구역일때
					distanceArray[next] = distance + weight;	  // decreaseKey
					pq.push(make_pair(distanceArray[next], next));
					parent_id[next] = make_pair(here, distanceArray[next]);
				}
			}
		}
		return parent_id;
	}


	void query_A(int _fromID, int _toID) {
		pair<int, int>* resultParentArray = dijkstra_A(_fromID, _toID);
		int minimalDistance = resultParentArray[_toID].second;
		if (minimalDistance > MAX || isFlooding(_fromID) || isFlooding(_toID)) {
			cout << "None" << "\n";
		}
		else {
			stack<int> st;

			for (int i = _toID; i != -1; i = resultParentArray[i].first) {
				st.push(i);
			}
			cout << st.size() << " " << minimalDistance << " " << getVertex(_fromID)->areaName << " " << getVertex(_toID)->areaName << "\n";
			/*출력형식: “N W SN DN” or “None”
				N : 출발지부터 목적지까지 Tree vertex들의 set에 추가된 지역들의 수
				W : 출발지부터 목적지까지의 최단거리
				SN : 출발 지역이름
				DN : 목적 지역이름*/
		}
	}

	void query_B(int _fromID, int _toID) {
		pair<int, int>* resultParentArray = dijkstra_A(_fromID, _toID);
		int minimalDistance = resultParentArray[_toID].second;
		if (minimalDistance > MAX || isFlooding(_fromID) || isFlooding(_toID)) {
			cout << "None" << "\n";
		}
		else {
			stack<int> st;

			for (int i = _toID; i != -1; i = resultParentArray[i].first) {
				st.push(i);
			}
			cout << st.size() << " ";
			while (!st.empty()) {
				cout << st.top() << " ";
				st.pop();
			}
			cout << "\n";
			/*출력형식: “N W SN DN” or “None”
							N : 출발지부터 목적지까지 Tree vertex들의 set에 추가된 지역들의 수
							 출발지부터 목적지까지의 최단경로 v0 ---- vn*/
		}
	}
};

int N, M, Q; // 지역정보의 수, 지역 간의 도로 정보의 수, 질의의 수
int input_id, input_flooding, input_fromID, input_toID, input_weight;
string queryName, input_name;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> N >> M >> Q;
	Graph graph;

	for (int i = 0; i < N; i++) { // 지역 정보 입력받기
		cin >> input_id >> input_name >> input_flooding;
		graph.insertVertex(input_id, input_name, input_flooding);
	}
	for (int i = 0; i < M; i++) { // 지역 간의 도로 정보 입력 받기
		cin >> input_fromID >> input_toID >> input_weight;
		graph.connectVertex(input_fromID, input_toID, input_weight);
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