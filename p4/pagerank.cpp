#include <fstream>
#include <bits/stdc++.h>

using namespace std;


void print_vector(vector<double> &v) {
  cout << "[" ;
  for(double &a: v) {
    cout << a  << ", ";
  }
  cout << "]" << endl;
}

vector<double> pagerank(vector<vector<int>> &csr) {
  vector<int> starting_edge_diffs = csr[2];
  vector<int> end_edges = csr[1];

  int n = starting_edge_diffs.size();
  double uniform = 1.0 / (n - 1);
  //printf("Starting uniform edge weight is %f for %d\n", uniform, n - 1);

  vector<double> prev_edges(n, uniform);
  vector<double> curr_edges(n, 0);

  double change_threshold = 10e-4;
  bool converge = false;
  while (!converge) {
    double max_change = 0;

    int e_index = 0;
    for(int index = 1; index < n; ++index) {
       int num_outgoing = starting_edge_diffs[index] - starting_edge_diffs[index - 1];
       //printf("Node number %d has %d outgoing edges\n", index, num_outgoing);

       int end_index = e_index + num_outgoing;
       double to_distribute = prev_edges[index] / num_outgoing;
       while(e_index < end_index) {
         curr_edges[end_edges[e_index]] += to_distribute;
         ++e_index;
       }
    }

    for(int index = 1; index < n; ++index) {
      double new_val = (0.15/(n-1.0)) + (0.85 * curr_edges[index]);
      double change = abs(prev_edges[index] - new_val);
      max_change = max(max_change, change);

      prev_edges[index] = new_val;

    }
    converge = max_change < change_threshold;
    fill(curr_edges.begin(), curr_edges.end(), 0);
  }

  double sum = accumulate(prev_edges.begin() + 1, prev_edges.end(), 0.0);
  for_each(prev_edges.begin() + 1, prev_edges.end(), [&sum](double &n){n /= sum;});

  return prev_edges;
}
