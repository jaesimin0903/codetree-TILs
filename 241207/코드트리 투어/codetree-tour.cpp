#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include<cstring>
using namespace std;

struct Plan {
    int id, revenue, dest, cost;
};
struct Compare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        // 첫 번째 원소 기준 내림차순
        if (a.first != b.first) {
            return a.first < b.first; // 큰 값이 먼저 오도록
        }
        // 첫 번째 원소가 같다면 두 번째 원소 기준 오름차순
        return a.second > b.second; // 작은 값이 먼저 오도록
    }
};
int n, m;

int startCity = 0;

unordered_map<int, Plan> um;
vector<vector<pair<int, int>>> graph;
int cost[2010];





void createPlan(int id, int rev, int dest) {
    Plan p;
    p.id = id;
    p.revenue = rev;
    p.dest = dest;
    p.cost = cost[dest];
    um[id] = p;
}

void deletePlan(int id) {
    um.erase(id);
}

int sellPlan() {
    //djikstra

    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare>pq;

    for (auto i = um.begin(); i != um.end(); i++) {
        int dest = i->second.dest;
        int revenue = i->second.revenue;
        int id = i->second.id;
        int c = i->second.cost;
        if (revenue - c >= 0) {
            pq.push({ revenue-c, id });
        }
    }

    if (!pq.empty()) {
        deletePlan(pq.top().second);
        return pq.top().second;
    }
    else return -1;
}

void setAllStart(int start) {
    startCity = start;

    //memset(&cost, 1e9, sizeof(cost));

    for (int i = 0; i < n; i++) {
        cost[i] = 1e9;
    }
    cost[startCity] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0,startCity });

    while (!pq.empty()) {
        int cur = pq.top().second;
        int val = pq.top().first;

        pq.pop();

        if (cost[cur] < val)continue;

        for (int j = 0; j < graph[cur].size(); j++) {
            int next = graph[cur][j].second;
            int nextVal = graph[cur][j].first;

            if (cost[next] > val + nextVal) {
                cost[next] = val + nextVal;
                pq.push({ cost[next], next });
            }
        }
    }

    for (auto i = um.begin(); i != um.end(); i++) {
        Plan& p = i->second;
        p.cost = cost[p.dest];
    }
}

void debug() {
    cout << "\n----------------------------------\n";
    for (auto i = um.begin(); i != um.end(); i++) {
        Plan p = i->second;

        cout << p.id << " " << p.revenue << " " << p.dest << "start to dest " << p.cost << "\n";
    }
    cout << "\n----------------------------------\n";
}

void init() {
    cin >> n >> m;

    graph = vector<vector<pair<int, int>>>(n + 1, vector<pair<int, int>>());

    for (int i = 0; i < m; i++) {
        int v, u, w;
        cin >> v;
        cin >> u;
        cin >> w;
        graph[v].push_back({ w,u });
        graph[u].push_back({ w,v });
    }

    setAllStart(0);
}

void simiulation() {
    int T;
    cin >> T;
    while (T--) {
        int comm;
        cin >> comm;
        if (comm == 100) {
            init();
        }
        else if (comm == 200) {
            int id, rev, des;
            cin >> id >> rev >> des;
            createPlan(id, rev, des);
            //debug();
        }
        else if (comm == 300) {
            int id;
            cin >> id;
            deletePlan(id);
        }
        else if (comm == 400) {
            cout << sellPlan() << "\n";
            //debug();
        }
        else if (comm == 500) {
            int s;
            cin >> s;
            setAllStart(s);
        }
    }
}
int main() {
    // 여기에 코드를 작성해주세요.
    simiulation();
    return 0;
}