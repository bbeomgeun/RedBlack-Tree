//12161104 �ڹ��� �˰��� 2������

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

class Graph { // ����׷���
	// ���� �Է¹޴� ���� ������ �����ص־��ϰ�, ���������� ������ ����� �������� ����
	// �� ���� dijkstra�� ������ �ȴ�(�ִܰŸ� �� �ִܰ��)
public:
	//map<AreaInfo * , vector<pair<int, AreaInfo * >>> adjacencyList;
	map<int, vector<pair<int, int>>> adjacencyList;
	AreaInfo* indexArray[MAX]; // ������ȣ 100000~999999
	pair<int, int> parent_id[MAX]; // �θ��ȣ�� �ִܰŸ�
	bool fringeCheck[MAX];
	int distanceArray[MAX];

	Graph() {
		memset(fringeCheck, false, sizeof(bool) * MAX);
		memset(distanceArray, -1, sizeof(int) * MAX);
	}

	void insertVertex(int _id, string _name, bool _flood) {
		AreaInfo* vertex = new AreaInfo(_id, _name, _flood); // �Է¹��� ������ vertex ����
		//this->adjacencyList[vertex]; // ��������Ʈ map�� ����
		indexArray[_id] = vertex; // id�� vertex ���� ����
	} // vertex ���� ���� �� index ���� ����

	void connectVertex(int _fromID, int _toID, int _weight) {
		/*AreaInfo *from_vertex = indexArray[_fromID];
		AreaInfo *to_vertex = indexArray[_toID];
		adjacencyList[from_vertex].push_back(make_pair(_weight, to_vertex));
		adjacencyList[to_vertex].push_back(make_pair(_weight, from_vertex));*/ // AreaInfo class point ���� ����
		adjacencyList[_fromID].push_back(make_pair(_weight, _toID));
		adjacencyList[_toID].push_back(make_pair(_weight, _fromID));
		// ����׷����̹Ƿ� ���� ��� ����
	}

	bool isFlooding(int _id) {
		return getVertex(_id)->isFlooding;
	}

	AreaInfo* getVertex(int _id) {
		return indexArray[_id];
	}

	pair<int, int>* dijkstra_A(int from_id, int to_id) { // �ִܰŸ�
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

				if (distanceArray[next] > distance + weight && !isFlooding(next)) { // �Ÿ��� update�Ǹ鼭 �������� ���� �����϶�
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
			/*�������: ��N W SN DN�� or ��None��
				N : ��������� ���������� Tree vertex���� set�� �߰��� �������� ��
				W : ��������� ������������ �ִܰŸ�
				SN : ��� �����̸�
				DN : ���� �����̸�*/
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
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> N >> M >> Q;
	Graph graph;

	for (int i = 0; i < N; i++) { // ���� ���� �Է¹ޱ�
		cin >> input_id >> input_name >> input_flooding;
		graph.insertVertex(input_id, input_name, input_flooding);
	}
	for (int i = 0; i < M; i++) { // ���� ���� ���� ���� �Է� �ޱ�
		cin >> input_fromID >> input_toID >> input_weight;
		graph.connectVertex(input_fromID, input_toID, input_weight);
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