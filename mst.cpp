#include <QCoreApplication>
#include <vector>
#include <queue>
#include <algorithm>

class MinimumSpanningTreeFinder {
public:
    struct Edge {
        int from;
        int to;
        int weight;

        Edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}
    };

    static bool compareEdges(const Edge& a, const Edge& b) {
        return a.weight > b.weight;
    }

    std::vector<Edge> findMinimumSpanningTree(const std::vector<std::vector<int>>& edges) {
        std::vector<Edge> edgeList;
        for (const auto& edgeData : edges) {
            int from = edgeData[0];
            int to = edgeData[1];
            int weight = edgeData[2];
            edgeList.emplace_back(from, to, weight);
        }

        int numVertices = getNumVertices(edgeList);
        std::vector<bool> visited(numVertices, false);
        std::vector<Edge> minimumSpanningTree;

        std::priority_queue<Edge, std::vector<Edge>, decltype(&compareEdges)> pq(&compareEdges);

        visited[edgeList[0].from] = true;
        enqueueEdges(edgeList, edgeList[0].from, pq);

        while (!pq.empty()) {
            Edge edge = pq.top();
            pq.pop();

            int to = edge.to;
            if (visited[to])
                continue;

            visited[to] = true;
            minimumSpanningTree.push_back(edge);
            enqueueEdges(edgeList, to, pq);
        }

        return minimumSpanningTree;
    }

private:
    int getNumVertices(const std::vector<Edge>& edges) {
        int numVertices = 0;
        for (const auto& edge : edges) {
            numVertices = std::max(numVertices, std::max(edge.from, edge.to) + 1);
        }
        return numVertices;
    }

    void enqueueEdges(const std::vector<Edge>& edges, int vertex, std::priority_queue<Edge, std::vector<Edge>, decltype(&compareEdges)>& pq) {
        for (const auto& edge : edges) {
            if (edge.from == vertex)
                pq.emplace(edge.from, edge.to, edge.weight);
            else if (edge.to == vertex)
                pq.emplace(edge.to, edge.from, edge.weight);
        }
    }
};
