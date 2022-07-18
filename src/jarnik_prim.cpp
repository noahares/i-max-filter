#include "jarnik_prim.h"

#include <algorithm>
#include <iostream>
#include <utility>

void jp_from_node(algen::VertexId root,
                  const JarnikPrim::GraphRepresentation &graph,
                  std::vector<algen::Weight> &best_weights,
                  std::vector<algen::VertexId> &prev,
                  std::vector<std::size_t> &component_ids,
                  std::vector<std::size_t> &node_to_msf_array,
                  algen::WEdgeList &msf) {

    JarnikPrim::PriorityQueue pq;

    // prepare algorithm by "visiting" root
    best_weights[root] = 0;
    component_ids[root] = root;
    node_to_msf_array[root] = msf.size();
    for (auto it = graph.beginEdges(root); it != graph.endEdges(root); ++it) {
        best_weights[it->second] = it->first;
        prev[it->second] = root;
        pq.push(*it);
    }

    while (!pq.empty()) {
        JarnikPrim::GraphRepresentation::Edge current = pq.top(); // weight, vertexid
        pq.pop();

        if (current.first > best_weights[current.second])
            continue;

        // edge selected
        node_to_msf_array[current.second] = msf.size();
        msf.push_back({prev[current.second], current.second, current.first});
        component_ids[current.second] = root;

        // check out neighbors of node at other end
        for (auto it = graph.beginEdges(current.second); it != graph.endEdges(current.second); ++it) {
            if (it->first < best_weights[it->second]) {
                best_weights[it->second] = it->first;
                prev[it->second] = current.second;
                pq.push(*it);
            }
        }
    }
}

void JarnikPrim::operator()(const algen::WEdgeList &edge_list,
                            algen::WEdgeList &msf,
                            const algen::VertexId num_vertices,
                            std::vector<std::size_t> &component_ids,
                            std::vector<std::size_t> &node_to_msf_array) {
    assert(component_ids.size() == num_vertices);
    assert(node_to_msf_array.size() == num_vertices);

    const GraphRepresentation graph(edge_list, num_vertices);
    std::vector<algen::Weight> best_weights(num_vertices, W_INF);
    std::vector<algen::VertexId> prev(num_vertices, -1);

    msf.reserve(num_vertices);

    for (algen::VertexId root = 0; root < num_vertices; ++root) {
        if (best_weights[root] != W_INF)
            continue;
        jp_from_node(root, graph, best_weights, prev, component_ids, node_to_msf_array, msf);
    }
}
