#include <fstream>
#include <bits/stdc++.h>


using namespace std;

vector<double> pagerank(vector<vector<int>> &csr) {
  int num_nodes = csr[2].size() - 1;
  double uniform_weight = 1.0 / num_nodes;
  cout << uniform_weight << endl;

  vector<double> node_weights(num_nodes + 1, uniform_weight);

  int edge_index = 0;


  for(int row_index = 1; row_index <= num_nodes; ++row_index) {
    int num_outgoing_edges = csr[2][row_index] - csr[2][row_index - 1];

    double distributed_weight = uniform_weight / num_outgoing_edges;

    int end_edge_index = edge_index + num_outgoing_edges;
    for(; edge_index < end_edge_index; ++edge_index) {
      node_weights[csr[1][edge_index]] += distributed_weight;
    }
  }

  for(double weight: node_weights) {
    printf("%f\n", weight);
  }
  return node_weights;
}
