#include <fstream>
#include <bits/stdc++.h>

using namespace std;

/*
[1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
[3, 2, 2, 1, 6, 2, 4, 5, 2, 5, 2, 5, 2, 5, 2, 5, 5]
[0, 0, 1, 2, 4, 7, 9, 11, 13, 15, 16, 17]


0 0
1 0
2 1
4 2
7 4
9 7
11 9
13 11
15 13
16 15
17 16

[.033, .385, .344, .039, .08, .039, .016, .016, .016, .016, .016] are the probable desired values
*/


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
  printf("Starting uniform edge weight is %f for %d\n", uniform, n - 1);

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
  double sum = 0;
  for(int index = 1; index < n; ++index) {
	sum += prev_edges[index];
  }
  
  for(int index = 1; index < n; ++index) {
	prev_edges[index] = prev_edges[index] / sum;
  }
  print_vector(prev_edges);
  return prev_edges;
}
