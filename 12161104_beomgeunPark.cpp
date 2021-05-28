//12161104 �ڹ��� �˰��� 2������

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <algorithm>

#define MAX 1000000 // 10^6
#define INF 987654321 // ���ͽ�Ʈ�� �Ÿ� �ʱ�ȭ ��(���Ѵ�)

using namespace std;

class AreaInfo { // �������� -> vertex
public:
	int ID; // ������ȣ
	string areaName; // �����̸�
	bool isFlooding; // ħ������ true(1) ħ�� O, false(0) ħ�� X

	AreaInfo(int _id, string _areaName, bool _isFlooding) {
		this->ID = _id;
		this->areaName = _areaName;
		this->isFlooding = _isFlooding;
	} // ������
};

class Graph { // ����׷���
	 // ���� �Է� �޴� ���� ������ �����ؾ� �ϰ�, ���� ������ ������ ����� �������� ����
	 // �� ���� �������� dijkstra�� �����Ѵ�(�ִܰŸ� �� �ִܰ��)
public:
	unordered_map<int, vector<pair<int, int>>> adjacencyList;
	AreaInfo* indexArray[MAX]; // ������ȣ 100000~999999
	int distanceArray[MAX]; // ���ͽ�Ʈ�� �����ϸ� �Ÿ� ����� �迭
	int countTreeVertex; // Ʈ�� vertex�� ����
	int parent[MAX]; // �ִ� ��θ� ����ϱ� ���� parent �迭
	bool treeVertex[MAX]; // treeVertex�� �ߺ��� ���� ���� üũ �迭

	Graph() {
		countTreeVertex = 0;
		for (int i = 0; i < MAX; i++) {
			indexArray[i] = 0;
			parent[i] = -1;
			distanceArray[i] = INF;
			treeVertex[i] = false;
		}
	} // �׷��� ������ (��� ���� �ʱ�ȭ)

	void insertVertex(int _id, string _name, bool _flood) {
		AreaInfo* vertex = new AreaInfo(_id, _name, _flood); // �Է¹��� ������ vertex ����
		indexArray[_id] = vertex; // id�� vertex ���� ���� (vertex�� id�� ���� O(1)�� �����ϱ� ����)
	} // vertex ���� ���� �� index ���� ����

	void connectVertex(int _fromID, int _toID, int _weight) {
		int flag = true;
		for (int i = 0; i < adjacencyList[_fromID].size(); i++) { // �̹� �Է¹��� �����̶�� ������Ʈ
			if (_toID == adjacencyList[_fromID][i].second) {
				adjacencyList[_fromID][i].first = min(adjacencyList[_fromID][i].first, _weight);
				flag = false;
				break;
			}
		}
		if (flag) { // �Է¹��� ���� �����̶�� ��������Ʈ ���ʿ� push
			adjacencyList[_fromID].push_back(make_pair(_weight, _toID));
			adjacencyList[_toID].push_back(make_pair(_weight, _fromID));
		}
		else { // ������ ���� ������Ʈ
			for (int i = 0; i < adjacencyList[_toID].size(); i++) {
				if (_fromID == adjacencyList[_toID][i].second) {
					adjacencyList[_toID][i].first = min(adjacencyList[_toID][i].first, _weight);
				}
			}
		}
		 // �����׷����̹Ƿ� ���� ��� ����
		// ���� �̹� �Է¹��� �����̸� �ּҷ� ����������ϱ� ���� �ش� ������ ã�Ƽ� �ּڰ����� �������ְ�
		// ���� �Է¹��� ������ �ƴ϶�� ���ʿ� �ܼ��� ����ġ�� id�� ���Ϳ� �־��ش�.
	}

	bool isFlooding(int _id) { // ���� ���� üũ �Լ�
		return getVertex(_id)->isFlooding;
	}

	AreaInfo* getVertex(int _id) { // ������ array�� �̿��ؼ� Vertex�� ��ȯ, O(1)�� �ش� vertex�� ������ �� �ִ�.
		return indexArray[_id];
	}

	void dijkstra_A(int from_id, int to_id) { // �ִܰŸ��� �ִܰ�θ� ã�� ���ͽ�Ʈ�� �Լ�
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // �켱������ weight, id
		countTreeVertex = 0; // tree Vertex ���� �ʱ�ȭ
	
		for (int i = 0; i < MAX; i++) { // ���ͽ�Ʈ�� ���� �� ������ distance�� �θ� �迭, Ʈ��vertex ���θ� �ʱ�ȭ���ش�.
			distanceArray[i] = INF;
			parent[i] = -1;
			treeVertex[i] = false;
		}

		pq.push(make_pair(0, from_id)); // ���ͽ�Ʈ�� �����ϸ� �������� �Ÿ� 0�� �켱���� ť�� �ִ´�. 
		distanceArray[from_id] = 0; // �������� �Ÿ� 0���� �ʱ�ȭ

		while (!pq.empty()) {
			int distance = pq.top().first; // �켱���� ť���� �ִ� distance�� vertex�� pop
			int here = pq.top().second;
			pq.pop();
			if (!treeVertex[here]) { // �Ÿ� �켱�����̱⿡ ť���� �ߺ��� vertex�� ���� ���� �ִ�. 
				countTreeVertex++; // treeVertex�� �ƴ� ���� treeVertex�� �ٲ��ָ� Vertex ���� ���ش�.
				treeVertex[here] = true;
			}
			if (here == to_id || distance > MAX) { // ���� ������ vertex�� �����ϰų� ���� �Ÿ��� ������ ������ ����
				return;
			}

			for (int i = 0; i < adjacencyList[here].size(); i++) { // �ش� vertex�� ����� vertex�� ã�� ���� ��������Ʈ�� Ž��
				int weight = adjacencyList[here][i].first; // �� ����� vertex���� �湮�ϸ� fringe vertex�� �ȴ�.
				int next = adjacencyList[here][i].second;

				if ( distanceArray[next] > (distance + weight) ) { // �Ÿ��� update�Ǹ鼭 �������� ���� �����϶�
					distanceArray[next] = distance + weight;	  // decreaseKey() ���� ����
					pq.push(make_pair(distanceArray[next], next)); 
					parent[next] = here; // ��� Ž���� ���� decreaseKey�� ����� �� parent�迭�� ������ �������ش�.
				}
			}
		}
		return;
	}


	void query_A(int _fromID, int _toID) { // ���� A�� ���� �Լ�
		dijkstra_A(_fromID, _toID); 
		int minimalDistance = distanceArray[_toID]; // ���ͽ�Ʈ�� ���� �� distance �迭���� shortest distance�� �����´�.
		if (minimalDistance > MAX) { // ���� ����ġ�� ������ None�� ���
			cout << "None" << "\n";
		}
		else { // �ƴ� ��� tree Vertex�� ����, �ִ� �Ÿ�, ��� �����̸�, ���� �����̸��� ���
			cout << countTreeVertex << " " << minimalDistance << " " << getVertex(_fromID)->areaName << " " << getVertex(_toID)->areaName << "\n";
		}
	}

	void query_B(int _fromID, int _toID) { // ���� B�� ���� �Լ�
		dijkstra_A(_fromID, _toID); 
		int minimalDistance = distanceArray[_toID]; // ���� A�� �����ϰ� �Ÿ� ����ġ�� �����ش�.
		if (minimalDistance > MAX) {
			cout << "None" << "\n";
		}
		else { // �ƴ� ��� ��� Ž���� ���� stack�� �̿�
			stack<int> st;
			cout << countTreeVertex << " ";
			
			for (int i = _toID ; i != -1; i = parent[i]) { // ������ vertex���� ���� vertex���� parent�迭�� Ÿ���� stack�� push�Ѵ�.
				st.push(i);
			}

			while (!st.empty()) { // �ִ� ��� ���
				cout << st.top() << " ";
				st.pop();
			}

			cout << "\n";
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
		graph.insertVertex(input_id, input_name, input_flooding); // ������ vertex�� �������ش�.
	}
	for (int i = 0; i < M; i++) { // ���� ���� ���� ���� �Է� �ޱ�
		cin >> input_fromID >> input_toID >> input_weight;
		if (graph.getVertex(input_fromID)->isFlooding || graph.getVertex(input_toID)->isFlooding)
			continue; // ������ ���̱� ���� ������ ���� ������ ���������� �ʴ´�. (������ �湮���� ���ϹǷ�)
		graph.connectVertex(input_fromID, input_toID, input_weight); // �ƴ� ��� edge�� �̾��ش�,
	}
	for (int i = 0; i < Q; i++) { // ���� �Է�
		cin >> queryName >> input_fromID >> input_toID;
		if (graph.getVertex(input_fromID)->isFlooding || graph.getVertex(input_toID)->isFlooding){
			cout << "None" << "\n"; // ������ �Ǵ� �������� �����ߴٸ� None�� ���
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