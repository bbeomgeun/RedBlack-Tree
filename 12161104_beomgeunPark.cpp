//12161104 �ڹ��� �˰��� 2������

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <algorithm>


#define MAX 1000000 // 10^6
#define INF 987654321

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

class Graph { // ����׷���
	 //���� �Է¹޴� ���� ������ �����ص־��ϰ�, ���������� ������ ����� �������� ����
	 //�� ���� dijkstra�� ������ �ȴ�(�ִܰŸ� �� �ִܰ��)
public:
	unordered_map<int, vector<pair<int, int>>> adjacencyList;
	AreaInfo* indexArray[MAX]; // ������ȣ 100000~999999
	int distanceArray[MAX];
	int countTreeVertex;
	int parent[MAX];
	bool treeVertex[MAX];

	Graph() {
		countTreeVertex = 0;
		for (int i = 0; i < MAX; i++) {
			indexArray[i] = 0;
			parent[i] = -1;
		}
	}

	void insertVertex(int _id, string _name, bool _flood) {
		AreaInfo* vertex = new AreaInfo(_id, _name, _flood); // �Է¹��� ������ vertex ����
		indexArray[_id] = vertex; // id�� vertex ���� ����
	} // vertex ���� ���� �� index ���� ����

	void connectVertex(int _fromID, int _toID, int _weight) {
		int flag = true;
		for (int i = 0; i < adjacencyList[_fromID].size(); i++) {
			if (_toID == adjacencyList[_fromID][i].second) {
				adjacencyList[_fromID][i].first = min(adjacencyList[_fromID][i].first, _weight);
				flag = false;
				break;
			}
		}
		if (flag) {
			adjacencyList[_fromID].push_back(make_pair(_weight, _toID));
			adjacencyList[_toID].push_back(make_pair(_weight, _fromID));
		}
		else {
			for (int i = 0; i < adjacencyList[_toID].size(); i++) {
				if (_fromID == adjacencyList[_toID][i].second) {
					adjacencyList[_toID][i].first = min(adjacencyList[_toID][i].first, _weight);
				}
			}
		}
		 //����׷����̹Ƿ� ���� ��� ����
	}

	bool isFlooding(int _id) {
		return getVertex(_id)->isFlooding;
	}

	AreaInfo* getVertex(int _id) {
		return indexArray[_id];
	}

	void dijkstra_A(int from_id, int to_id) { // �ִܰŸ�
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //weight, id
		countTreeVertex = 0;
	
		for (int i = 0; i < MAX; i++) {
			distanceArray[i] = INF;
			parent[i] = -1;
			treeVertex[i] = false;
		}

		pq.push(make_pair(0, from_id));
		distanceArray[from_id] = 0;

		while (!pq.empty()) {
			int distance = pq.top().first;
			int here = pq.top().second;
			pq.pop();
			if (!treeVertex[here]) {
				countTreeVertex++; //todo : �ߺ��� �� ������ ���̽� üũ�غ���
				treeVertex[here] = true;
			}
			if (here == to_id || distance > MAX) {
				return;
			}

			for (int i = 0; i < adjacencyList[here].size(); i++) {
				int weight = adjacencyList[here][i].first;
				int next = adjacencyList[here][i].second;

				if ( distanceArray[next] > (distance + weight) ) { // �Ÿ��� update�Ǹ鼭 �������� ���� �����϶�
					distanceArray[next] = distance + weight;	  // decreaseKey
					pq.push(make_pair(distanceArray[next], next));
					parent[next] = here; // todo : parent �迭 �ʱ�ȭ�ϰ�, ���� -1�� �ƴ϶�� min �� ID �־���Ұ�
				}
			}
		}
		return;
	}


	void query_A(int _fromID, int _toID) {
		dijkstra_A(_fromID, _toID);
		int minimalDistance = distanceArray[_toID];
		if (minimalDistance > MAX) {
			cout << "None" << "\n";
		}
		else {
			cout << countTreeVertex << " " << minimalDistance << " " << getVertex(_fromID)->areaName << " " << getVertex(_toID)->areaName << "\n";
			/*�������: ��N W SN DN�� or ��None��
				N : ��������� ���������� Tree vertex���� set�� �߰��� �������� ��
				W : ��������� ������������ �ִܰŸ�
				SN : ��� �����̸�
				DN : ���� �����̸�*/
		}
	}

	void query_B(int _fromID, int _toID) {
		dijkstra_A(_fromID, _toID);
		int minimalDistance = distanceArray[_toID];
		if (minimalDistance > MAX) {
			cout << "None" << "\n";
		}
		else {
			stack<int> st;
			cout << countTreeVertex << " ";
			
			for (int i = _toID ; i != -1; i = parent[i]) {
				st.push(i);
			}

			while (!st.empty()) {
				cout << st.top() << " ";
				st.pop();
			}

			cout << "\n";
			/*�������: ��N W SN DN�� or ��None��
							N : ��������� ���������� Tree vertex���� set�� �߰��� �������� ��
							 ��������� ������������ �ִܰ�� v0 ---- vn*/
		}
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
		graph.insertVertex(input_id, input_name, input_flooding);
	}
	for (int i = 0; i < M; i++) { // ���� ���� ���� ���� �Է� �ޱ�
		cin >> input_fromID >> input_toID >> input_weight;
		if (graph.getVertex(input_fromID)->isFlooding || graph.getVertex(input_toID)->isFlooding)
			continue;
		graph.connectVertex(input_fromID, input_toID, input_weight);
	}
	for (int i = 0; i < Q; i++) { // ���� �Է�
		cin >> queryName >> input_fromID >> input_toID;
		if (graph.getVertex(input_fromID)->isFlooding || graph.getVertex(input_toID)->isFlooding){
			cout << "None" << "\n";
			continue;
		}
			
		if (queryName == "A") { // �� ���� ���� �ִ� �Ÿ� ã��
			graph.query_A(input_fromID, input_toID);
		}
		else if (queryName == "B") { // �� ���� ���� �ִ� ��� ã��
			graph.query_B(input_fromID, input_toID);
		}
	}
	return 0;
}