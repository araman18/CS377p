#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <pthread.h>
#include <algorithm>

using namespace std;

// index for the pagerank labels,
// note that every occurrence of "CURRENT" and "NEXT" will be replaced with 0 and 1
// you can disable this and simply use 0 and 1 if you find it easier to name your variable this way
#define CURRENT 0
#define NEXT 1

#define MAX_THREADS 16

int numThreads;

int threadArg[MAX_THREADS];
pthread_t handles[MAX_THREADS];

int perThread;

class CsrGraph {
// You shouldn't need to modify this class except for adding essential locks and relavent methods,
// but if you find it useful for your code you can add some minor helper functions.
// Make sure not to store too much additional information here.
// The whole point of using CSR is that it utilizes the space efficiently.

private:
    // use an array of struct for labels
    struct Node {
        // labels[0] is the current label, labels[1] is the next label
        double labels[2];
    };

    // helper struct for COO
    struct Edge {
        int src, dst, weight;

        Edge(int s, int d, int w) : src(s), dst(d), weight(w) {}
    };

    // number of nodes and edges in the graph
    int num_nodes;
    int num_edges;

    // csr representation of the graph (same as in the slides)
    int *rp;
    int *ci;
    int *ai;

    // pagerank labels
    Node *node;

    //Node locks

    bool is_allocated;

public:

    pthread_mutex_t *nodeLocks;

    CsrGraph() {
        // no dynamic allocated space used
        is_allocated = false;
    }

    CsrGraph(ifstream &f_dimacs) {
        // parse the input file to COO format and set number of nodes and edges
        vector <Edge> edges = parse_dimacs(f_dimacs);

        // convert COO to CSR and store the representation in the graph
        gen_csr(edges);

        // remind you to delete the space
        is_allocated = true;
    }

    ~CsrGraph() {
        if (!is_allocated) {
            return;
        }

        // clean up dynamic arrays
        delete[] node;
        delete[] nodeLocks;
        delete[] rp;
        delete[] ci;
        delete[] ai;
    }

    vector <Edge> parse_dimacs(ifstream &f_dimacs) {
        string line;
        vector <Edge> edges;

        // parse dimacs
        while (getline(f_dimacs, line)) {
            // skip comments
            if ('c' == line[0]) {
                continue;
            }

            // split the line into tokens
            vector <string> token;
            boost::split(token, line, [](char c) { return c == ' '; });

            // p <problem_type> <num_nodes> <num_edges>
            // get the number of nodes in the graph
            if ("p" == token[0]) {
                num_nodes = stoi(token[2]);
                num_edges = stoi(token[3]);
            }

                // a <src> <dst> <weight>
            else if ("a" == token[0]) {
                edges.push_back(Edge(stoi(token[1]), stoi(token[2]), stoi(token[3])));
            }
        }

        return edges;
    }

    void gen_csr(vector <Edge> &edges) {
        // sort the edges
        sort(edges.begin(), edges.end(),
             [](const Edge &e1, const Edge &e2) {
                 return (e1.src < e2.src) ? true :
                        (e1.src == e2.src) ? (e1.dst < e2.dst) : false;
             });

        // handle duplicate in the edges
        vector <Edge> good_edges;
        int edge_size = edges.size();
        int src = edges[0].src;
        int dst = edges[0].dst;
        int max_weight = edges[0].weight;
        for (int i = 1; i < edge_size; i++) {
            if (edges[i].src == edges[i - 1].src && edges[i].dst == edges[i - 1].dst) {
                // duplicate edge
                max_weight = edges[i].weight > max_weight ? edges[i].weight : max_weight;
            } else {
                // add the edge with max_weight to good_edges
                good_edges.push_back(Edge(edges[i - 1].src, edges[i - 1].dst, max_weight));

                // initialize max_weight
                max_weight = edges[i].weight;
            }
        }
        // deal with the last edge
        good_edges.push_back(Edge(edges[edge_size - 1].src, edges[edge_size - 1].dst, max_weight));

        // get the actual number of edges
        num_edges = good_edges.size();

        // allocate space for dynamic arrays
        node = new Node[num_nodes + 2];
        nodeLocks = new pthread_mutex_t[num_nodes + 2];
        for (int i = 0; i < num_nodes + 2; ++i) {
            int status = pthread_mutex_init(&nodeLocks[i], NULL);
        }
        rp = new int[num_nodes + 2];
        ci = new int[num_edges + 2];
        ai = new int[num_edges + 2];

        // rp[0] = 1 to be consistent with rp[1] = 1 (This affects the number of edges we get for node 1)
        rp[0] = 1;
        ci[0] = 0;
        ai[0] = 0;
        good_edges.insert(good_edges.begin(), Edge(rp[0], ci[0], ai[0]));

        // loop through each edge in COO and construct CSR representation
        int cur_node = 0;
        int cur_edge = 1;
        while (cur_edge <= num_edges) {
            const Edge &e = good_edges[cur_edge];
            // update cur_node and its coresponding rp when current source node is larger than current node
            if (e.src > cur_node) {
                cur_node++;
                rp[cur_node] = cur_edge;
            }
                // copy the destination and weight to ci and ai otherwise
            else {
                ci[cur_edge] = e.dst;
                ai[cur_edge] = e.weight;
                cur_edge++;
            }
        }

        // set the remaining nodes without edges
        for (int i = cur_node + 1; i <= num_nodes + 1; i++) {
            rp[i] = num_edges + 1;
        }
        ci[num_edges + 1] = num_nodes + 1;
        ai[num_edges + 1] = 0;
    }

    int node_size() {
        // return the number of nodes
        return num_nodes;
    }

    int size_edges() {
        // return the number of edges
        return num_edges;
    }

    int edge_begin(int n) {
        // return the starting index for out-going edges from node n
        return rp[n];
    }

    int edge_end(int n) {
        // return the ending index for out-going edges from node n
        return rp[n + 1];
    }

    double get_label(int n, int which) {
        // return the current or next label for node n
        return node[n].labels[which];
    }

    void set_label(int n, int which, double label) {
        // set the current or next label for node n
        node[n].labels[which] = label;
    }

    int get_out_degree(int n) {
        // return the number of out-degrees for node n
        return rp[n + 1] - rp[n];
    }

    int get_edge_dst(int e) {
        // return the destination node in edge e (index for ci)
        return ci[e];
    }
};

CsrGraph *graph;
double threshold;
double damping;

void reset_next_label(CsrGraph *g, const double damping, int threadNum) {
    // Modify this function in any way you want to make pagerank parallel
    int num_nodes = g->node_size();
    for (int n = threadNum; n <= num_nodes; n += perThread) {
        pthread_mutex_lock(&graph->nodeLocks[n]);
        g->set_label(n, NEXT, (1.0 - damping) / (double) num_nodes);
        pthread_mutex_unlock(&graph->nodeLocks[n]);
    }
}

bool is_converged(CsrGraph *g, const double threshold, int threadNum) {
    // Modify this function in any way you want to make pagerank parallel
    for (int n = threadNum; n <= g->node_size(); n+=perThread) {
        pthread_mutex_lock(&graph->nodeLocks[n]);
        const double cur_label = g->get_label(n, CURRENT);
        const double next_label = g->get_label(n, NEXT);
        pthread_mutex_unlock(&graph->nodeLocks[n]);
        if (fabs(next_label - cur_label) > threshold) {
            return false;
        }
    }
    return true;
}

void update_current_label(CsrGraph *g, int threadNum) {
    // Modify this function in any way you want to make pagerank parallel
    for (int n = threadNum; n <= g->node_size(); n += perThread) {
        pthread_mutex_lock(&graph->nodeLocks[n]);
        g->set_label(n, CURRENT, g->get_label(n, NEXT));
        pthread_mutex_unlock(&graph->nodeLocks[n]);
    }
}

void scale(CsrGraph *g, int threadNum) {
    // Modify this function in any way you want to make pagerank parallel

    double sum = 0.0;
    for (int n = 1; n <= g->node_size(); n++) {
        pthread_mutex_lock(&graph->nodeLocks[n]);
        sum += g->get_label(n, CURRENT);
        pthread_mutex_unlock(&graph->nodeLocks[n]);
    }
    for (int n = 1; n <= g->node_size(); n++) {
        pthread_mutex_lock(&graph->nodeLocks[n]);
        g->set_label(n, CURRENT, g->get_label(n, CURRENT) / sum);
        pthread_mutex_unlock(&graph->nodeLocks[n]);
    }
}

void *compute_pagerank(void *threadId) {
    // You have to divide the work and assign it to threads to make this function parallel
    int threadNum = *(int *) (threadId);
    int num_nodes = graph->node_size();

    for (int i = threadNum; i <= num_nodes; i += perThread) {
        pthread_mutex_lock(&graph->nodeLocks[i]);
        graph->set_label(i, CURRENT, 1.0 / num_nodes);
        pthread_mutex_unlock(&graph->nodeLocks[i]);
    }

    bool convergence = false;

    do {
        // reset next labels
        reset_next_label(graph, damping, threadNum);

        // apply current node contribution to others
        for (int n = threadNum; n <= num_nodes; n += perThread) {
            pthread_mutex_lock(&graph->nodeLocks[n]);
            double my_contribution = damping * graph->get_label(n, CURRENT) / (double) graph->get_out_degree(n);
            pthread_mutex_unlock(&graph->nodeLocks[n]);
            for (int e = graph->edge_begin(n); e < graph->edge_end(n); e++) {
                int dst = graph->get_edge_dst(e);
                pthread_mutex_lock(&graph->nodeLocks[dst]);
                graph->set_label(dst, NEXT, graph->get_label(dst, NEXT) + my_contribution);
                pthread_mutex_unlock(&graph->nodeLocks[dst]);
            }

        }

        // check the change across successive iterations to determine convergence
        convergence = is_converged(graph, threshold, threadNum);

        // update current labels
        update_current_label(graph, threadNum);
    } while (!convergence);

    scale(graph, threadNum);

}

void sort_and_print_label(CsrGraph *g, string out_file) {
    // You shouldn't need to change this.

    // prepare the label to be sorted
    vector <pair<int, double>> label;
    for (int n = 1; n <= g->node_size(); n++) {
        label.push_back(make_pair(n, g->get_label(n, CURRENT)));
    }

    // sort the labels in descending order then node number in ascending order
    sort(label.begin(), label.end(),
         [](const pair<int, double> &v1, const pair<int, double> &v2) {
             return (v1.second > v2.second) ? true :
                    (v1.second == v2.second) ? (v1.first < v2.first) : false;
         });

    // print the labels to the output file
    ofstream out_stream(out_file);
    for (const pair<int, double> &v: label) {
        out_stream << v.first << " " << fixed << setprecision(6) << v.second << endl;
    }
}

int main(int argc, char *argv[]) {
    // Ex: ./pagerank road-NY.dimacs road-NY.txt
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input.dimacs> <output_filename> <num threads>\n";
        return 0;
    }


    // make sure the input argument is valid
    ifstream f_dimacs(argv[1]);
    if (!f_dimacs) {
        cerr << "Failed to open " << argv[1] << endl;
        return 0;
    }

    numThreads = stoi(argv[3]);

    // construct the CSR graph
    graph = new CsrGraph(f_dimacs);

    perThread = (int) (ceil((1.0 * graph->node_size()) / (numThreads)));

    // define important constants for pagerank
    threshold = 1.0e-4;
    damping = 0.85;

    for (int t = 0; t < numThreads; t++) {
        threadArg[t] = t;
        pthread_create(&handles[t], NULL, compute_pagerank, &threadArg[t]);
    }
    // compute the pagerank using push-style method

    for (int i = 0; i < numThreads; i++) {
        pthread_join(handles[i], NULL);
    }

    double sum = 0;
    for (int i = 1; i <= graph->node_size(); ++i) {
        sum += graph->get_label(i, CURRENT);
    }
    printf("%f\n", sum);

    // sort and print the labels to the output file
    sort_and_print_label(graph, argv[2]);

    // delete the allocated space to the graph avoid memory leak
    delete graph;

    pthread_exit(NULL);
    return 0;
}
