//12161104 박범근 알고리즘 2차과제

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <algorithm>

#define MAX 1000000 // 10^6
#define INF 987654321 // 다익스트라 거리 초기화 값(무한대)

using namespace std;

class AreaInfo { // 지역정보 -> vertex
public:
	int ID; // 지역번호
	string areaName; // 지역이름
	bool isFlooding; // 침수여부 true(1) 침수 O, false(0) 침수 X

	AreaInfo(int _id, string _areaName, bool _isFlooding) {
		this->ID = _id;
		this->areaName = _areaName;
		this->isFlooding = _isFlooding;
	} // 생성자
};

class Graph { // 무향그래프
	 // 먼저 입력 받는 지역 정보를 저장해야 하고, 연결 정보를 받으면 연결된 지역끼리 묶고
	 // 그 이후 쿼리마다 dijkstra를 수행한다(최단거리 및 최단경로)
public:
	unordered_map<int, vector<pair<int, int>>> adjacencyList;
	AreaInfo* indexArray[MAX]; // 지역번호 100000~999999
	int distanceArray[MAX]; // 다익스트라를 진행하며 거리 저장용 배열
	int countTreeVertex; // 트리 vertex의 갯수
	int parent[MAX]; // 최단 경로를 출력하기 위한 parent 배열
	bool treeVertex[MAX]; // treeVertex의 중복을 막기 위한 체크 배열

	Graph() {
		countTreeVertex = 0;
		for (int i = 0; i < MAX; i++) {
			indexArray[i] = 0;
			parent[i] = -1;
			distanceArray[i] = INF;
			treeVertex[i] = false;
		}
	} // 그래프 생성자 (멤버 변수 초기화)

	void insertVertex(int _id, string _name, bool _flood) {
		AreaInfo* vertex = new AreaInfo(_id, _name, _flood); // 입력받은 정보로 vertex 생성
		indexArray[_id] = vertex; // id와 vertex 정보 매핑 (vertex를 id를 통해 O(1)에 접근하기 위함)
	} // vertex 정보 저장 및 index 정보 저장

	void connectVertex(int _fromID, int _toID, int _weight) {
		int flag = true;
		for (int i = 0; i < adjacencyList[_fromID].size(); i++) { // 이미 입력받은 간선이라면 업데이트
			if (_toID == adjacencyList[_fromID][i].second) {
				adjacencyList[_fromID][i].first = min(adjacencyList[_fromID][i].first, _weight);
				flag = false;
				break;
			}
		}
		if (flag) { // 입력받지 않은 간선이라면 인접리스트 양쪽에 push
			adjacencyList[_fromID].push_back(make_pair(_weight, _toID));
			adjacencyList[_toID].push_back(make_pair(_weight, _fromID));
		}
		else { // 나머지 간선 업데이트
			for (int i = 0; i < adjacencyList[_toID].size(); i++) {
				if (_fromID == adjacencyList[_toID][i].second) {
					adjacencyList[_toID][i].first = min(adjacencyList[_toID][i].first, _weight);
				}
			}
		}
		 // 양방향그래프이므로 양쪽 모두 연결
		// 만약 이미 입력받은 간선이면 최소로 연결해줘야하기 위해 해당 간선을 찾아서 최솟값으로 변경해주고
		// 만약 입력받은 간선이 아니라면 양쪽에 단순히 가중치와 id를 벡터에 넣어준다.
	}

	bool isFlooding(int _id) { // 범람 여부 체크 함수
		return getVertex(_id)->isFlooding;
	}

	AreaInfo* getVertex(int _id) { // 매핑한 array를 이용해서 Vertex를 반환, O(1)에 해당 vertex를 가져올 수 있다.
		return indexArray[_id];
	}

	void dijkstra_A(int from_id, int to_id) { // 최단거리와 최단경로를 찾는 다익스트라 함수
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // 우선순위를 weight, id
		countTreeVertex = 0; // tree Vertex 갯수 초기화
	
		for (int i = 0; i < MAX; i++) { // 다익스트라를 수행 할 때마다 distance와 부모 배열, 트리vertex 여부를 초기화해준다.
			distanceArray[i] = INF;
			parent[i] = -1;
			treeVertex[i] = false;
		}

		pq.push(make_pair(0, from_id)); // 다익스트라를 시작하며 시작점과 거리 0을 우선순위 큐에 넣는다. 
		distanceArray[from_id] = 0; // 시작점은 거리 0으로 초기화

		while (!pq.empty()) {
			int distance = pq.top().first; // 우선순위 큐에서 최단 distance와 vertex를 pop
			int here = pq.top().second;
			pq.pop();
			if (!treeVertex[here]) { // 거리 우선순위이기에 큐에서 중복된 vertex가 나올 수도 있다. 
				countTreeVertex++; // treeVertex가 아닌 경우는 treeVertex로 바꿔주며 Vertex 수를 세준다.
				treeVertex[here] = true;
			}
			if (here == to_id || distance > MAX) { // 만약 목적지 vertex에 도착하거나 누적 거리가 기준을 넘으면 종료
				return;
			}

			for (int i = 0; i < adjacencyList[here].size(); i++) { // 해당 vertex와 연결된 vertex를 찾기 위해 인접리스트를 탐색
				int weight = adjacencyList[here][i].first; // 이 연결된 vertex들은 방문하며 fringe vertex가 된다.
				int next = adjacencyList[here][i].second;

				if ( distanceArray[next] > (distance + weight) ) { // 거리가 update되면서 범람되지 않은 구역일때
					distanceArray[next] = distance + weight;	  // decreaseKey() 과정 수행
					pq.push(make_pair(distanceArray[next], next)); 
					parent[next] = here; // 경로 탐색을 위해 decreaseKey가 수행될 때 parent배열에 정보를 저장해준다.
				}
			}
		}
		return;
	}


	void query_A(int _fromID, int _toID) { // 쿼리 A에 관한 함수
		dijkstra_A(_fromID, _toID); 
		int minimalDistance = distanceArray[_toID]; // 다익스트라 수행 후 distance 배열에서 shortest distance를 가져온다.
		if (minimalDistance > MAX) { // 만약 기준치를 넘으면 None을 출력
			cout << "None" << "\n";
		}
		else { // 아닌 경우 tree Vertex의 갯수, 최단 거리, 출발 지역이름, 목적 지역이름을 출력
			cout << countTreeVertex << " " << minimalDistance << " " << getVertex(_fromID)->areaName << " " << getVertex(_toID)->areaName << "\n";
		}
	}

	void query_B(int _fromID, int _toID) { // 쿼리 B에 관한 함수
		dijkstra_A(_fromID, _toID); 
		int minimalDistance = distanceArray[_toID]; // 쿼리 A와 동일하게 거리 기준치를 비교해준다.
		if (minimalDistance > MAX) {
			cout << "None" << "\n";
		}
		else { // 아닌 경우 경로 탐색을 위해 stack을 이용
			stack<int> st;
			cout << countTreeVertex << " ";
			
			for (int i = _toID ; i != -1; i = parent[i]) { // 목적지 vertex부터 시작 vertex까지 parent배열을 타고가며 stack에 push한다.
				st.push(i);
			}

			while (!st.empty()) { // 최단 경로 출력
				cout << st.top() << " ";
				st.pop();
			}

			cout << "\n";
		}
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
		graph.insertVertex(input_id, input_name, input_flooding); // 정보를 vertex로 저장해준다.
	}
	for (int i = 0; i < M; i++) { // 지역 간의 도로 정보 입력 받기
		cin >> input_fromID >> input_toID >> input_weight;
		if (graph.getVertex(input_fromID)->isFlooding || graph.getVertex(input_toID)->isFlooding)
			continue; // 연산을 줄이기 위해 범람된 도로 정보는 연결해주지 않는다. (어차피 방문하지 못하므로)
		graph.connectVertex(input_fromID, input_toID, input_weight); // 아닌 경우 edge를 이어준다,
	}
	for (int i = 0; i < Q; i++) { // 질의 입력
		cin >> queryName >> input_fromID >> input_toID;
		if (graph.getVertex(input_fromID)->isFlooding || graph.getVertex(input_toID)->isFlooding){
			cout << "None" << "\n"; // 시작지 또는 목적지가 범람했다면 None을 출력
			continue;
		}
			
		if (queryName == "A") { // 두 지역 간의 최단 거리 찾기
			graph.query_A(input_fromID, input_toID);
		}
		else if (queryName == "B") { // 두 지역 간의 최단 경로 찾기
			graph.query_B(input_fromID, input_toID);
		}
	}
	return 0;
}