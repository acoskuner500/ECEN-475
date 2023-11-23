#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <map>
#include <limits>
using namespace std;

const int numNodes = 18, numPart = 3;

class Node {
    public:
        struct Edge {
            bool cut;
            int weight;
        };
        map<int,Edge> connected;
        int label = -1;         // Label of this node
        int partition = -1;     // Partition of this node
        bool swapped = false;
        Node () {}
        Node(int l, int p): label(l), partition(p) {}
};

void initNodeArr(Node nodeArr[numNodes]) {
    bool srcFound = false, destFound = false;
    string s, d;
    int i, sl, dl, sp, dp, si, di;
    Node src, dest, n = Node();
    for (i = 0; i < numNodes; i++) {
        nodeArr[i] = n;
    }

    ifstream is;
    is.open("Ahmet.Coskuner.input", ios::in);
    while (is.good()) {
        is >> s >> d;
        sl = stoi(s);
        dl = stoi(d);
        if (sl > dl) swap(sl, dl);
        sp = (sl - 1) / (numNodes / numPart);
        dp = (dl - 1) / (numNodes / numPart);
        for (i=0; i< numNodes; i++) {
            if (nodeArr[i].label == sl) {
                srcFound = true;
                if (sl == dl) destFound = true;
                (nodeArr[i].connected[dl].weight)++;
            } else if (nodeArr[i].label == dl) {
                destFound = true;
                (nodeArr[i].connected[sl].weight)++;
            }
        }
        if (!srcFound && !destFound) {
            if (sl == dl) {
                src = Node(sl,sp);
                nodeArr[sl-1] = src;
                (nodeArr[sl-1].connected[sl].weight)++;
            } else {
                src = Node(sl,sp);
                dest = Node(dl,dp);
                nodeArr[sl-1] = src;
                nodeArr[dl-1] = dest;
                (nodeArr[sl-1].connected[dl].weight)++;
                (nodeArr[dl-1].connected[sl].weight)++;
            }
        } else if (!srcFound) {
            src = Node(sl,sp);
            nodeArr[sl-1] = src;
            (nodeArr[sl-1].connected[dl].weight)++;
        } else if (!destFound) {
            dest = Node(dl,dp);
            nodeArr[dl-1] = dest;
            (nodeArr[dl-1].connected[sl].weight)++;
        }
        nodeArr[sl-1].connected[dl].cut = (sp != dp);
        if (sl != dl) nodeArr[dl-1].connected[sl].cut = (sp != dp);
        srcFound = false;
        destFound = false;
    }
    is.close();
}
