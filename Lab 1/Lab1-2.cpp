#include "graph.h"
#define PRINT

int main() {
    int stopIterations = 50;

    int i,j;
    Node nodeArr[numNodes];             // Node array
    vector<Node> nodes;                 // Vector version of node array
    vector<Node>::iterator it1, it2;    // nodes vector iterator
    map<int,Node::Edge>::iterator connIt; // Edges iterator

    initNodeArr(nodeArr);
    for (i=0; i<numNodes; i++) {
        nodes.insert(nodes.begin() + i,nodeArr[i]);
    }

    int max;            // Max improvement
    int imp;            // Improvement value
    int totalCuts = 0;  // Total number of cut edges
    int stopCnt = -1;   // Cut count after tentative swap
    bool firstPass = true;

    map<pair<Node*,Node*>,int> improvement;             // Table of swap pairs and their improvement scores
    map<pair<Node*,Node*>,int>::iterator iit;           // Improvement table iterator
    vector<pair<pair<Node*,Node*>,int>> tentative;      // Tentative swaps list with cut count
    vector<pair<pair<Node*,Node*>,int>>::iterator tit;  // Tentative swaps list iterator
    map<int,Node::Edge>::iterator n1it, n2it;           // Connected list iterator
    pair<Node*,Node*> swapPair;                         // Pair of nodes to potentially swap
    pair<Node*,Node*> swapMax;                          // Pair for tentative swap

    cout << "Starting improvement list:\n";
    do {
        // Find pair with the highest improvement
        max = numeric_limits<int>::min();
        for (it1 = nodes.begin(); it1 != nodes.end(); it1++) {
            for (it2 = nodes.begin(); it2 != nodes.end(); it2++) {
                imp = 0;
                if (it1->partition < it2->partition && !it1->swapped && !it2->swapped) {
                    for (n1it = it1->connected.begin(); n1it != it1->connected.end(); n1it++) {
                        for (Node node : nodes) {
                            if (node.label == n1it->first) {
                                // if first node edge's pairs are in first node's partition, imp++
                                // if first node edge's pairs are in second node's partition, imp--
                                if (node.partition == it2->partition) imp++;
                                if (node.partition == it1->partition && node.label != it1->label) imp--;
                            }
                        }
                    }
                    for (n2it = it2->connected.begin(); n2it != it2->connected.end(); n2it++) {
                        for (Node node : nodes) {
                            if (node.label == n2it->first) {
                                // if second node edge's pairs are in first node's partition, imp++
                                // if second node edge's pairs are in second node's partition, imp--
                                if (node.partition == it1->partition) imp++;
                                if (node.partition == it2->partition && node.label != it2->label) imp--;
                                break;
                            }
                        }   
                    }
                    if (it1->connected.find(it2->label) != it1->connected.end()) {
                        imp -= 2;                   // decrease by 2 if nodes are connected
                        if(firstPass) totalCuts++;  // set total cut count in first pass
                    }
                    swapPair = make_pair(&(*it1), &(*it2)); // create swap pair
                    improvement[swapPair] = imp;            // insert it into improvement table
                    
                    #ifdef PRINT
                    cout << "\t[" << swapPair.first->label << ",";
                    cout << swapPair.second->label << "]: " << imp;
                    #endif

                    if (imp > max) {
                        max = imp;
                        swapMax = swapPair;
                        #ifdef PRINT
                        cout << " swapMax";
                        #endif
                    }
                    #ifdef PRINT
                    cout << endl;
                    #endif
                }
            }
        }
        cout << "Improvemenet list size: " << improvement.size() << endl;
        
        // Tentative swap; add to tentative swap list with cut count
        int l1 = swapMax.first->label, l2 = swapMax.second->label;
        int oldp1 = swapMax.first->partition, oldp2 = swapMax.second->partition;
        swap(swapMax.first->partition, swapMax.second->partition);
        swapMax.first->swapped = true;
        swapMax.second->swapped = true;

        if (firstPass) stopCnt = totalCuts;
        firstPass = false;
        #ifdef PRINT
        cout << "Cuts before: " << stopCnt << endl;
        #endif
        stopCnt -= max;
        tentative.push_back(make_pair(swapMax,stopCnt));

        #ifdef PRINT
        cout << "\tswap: " << swapMax.first->label << "," << swapMax.second->label;
        cout << "\n\timprovement: " << improvement[swapMax];
        cout << "\n\tcuts after:" << tentative.back().second << endl;
        #endif
        
        // Find nodes connected to swap pair nodes and cut or uncut their edges
        for (n1it = swapMax.first->connected.begin(); n1it != swapMax.first->connected.end(); n1it++) {
            for (it1 = nodes.begin(); it1 != nodes.end(); it1++) {
                if (it1->label == n1it->first) {
                    if (it1->partition == oldp1) n1it->second.cut = true;
                    if (it1->partition == oldp2) n1it->second.cut = false;
                }
            }
        }
        for (n2it = swapMax.second->connected.begin(); n2it != swapMax.second->connected.end(); n2it++) {
            for (it1 = nodes.begin(); it1 == nodes.end(); it1++) {
                if (it1->label == n2it->first) {
                    if (it1->partition == oldp2) n2it->second.cut = true;
                    if (it1->partition == oldp1) n2it->second.cut = true;
                }
            }
        }

        // Remove swapped pair nodes from improvement table
        while (true) {
            for (iit = improvement.begin(); iit != improvement.end(); iit++) {
                if (iit->first.first->label == l1 ||
                    iit->first.second->label == l1 ||
                    iit->first.first->label == l2 || 
                    iit->first.second->label == l2) {
                    #ifdef PRINT
                    cout << "Removing [" << iit->first.first->label << "," << iit->first.second->label << "]: " << iit->second << endl;
                    #endif
                    improvement.erase(iit);
                    break;
                }
            }
            if (iit == improvement.end()) break;
        }
        cout << "\nRemaining improvement table:\n";
    } while (stopCnt != totalCuts && improvement.size() != 0);
    cout << "No more swaps possible\n\n";

    int initial = totalCuts;
    int mincut = totalCuts;                     // Min cuts in tentative swap list
    pair<pair<Node*,Node*>,int> stop;           // Pair for min cut
    int iterations = 1;
    for (tit = tentative.begin(); tit != tentative.end(); tit++) {
        cout << "Cuts after " << iterations++ << " iterations: " << tit->second;
        if (tit->second < mincut) {
            mincut = tit->second;
            stop = make_pair(tit->first, tit->second);
        }
        cout << endl;
    }
    
    iterations = 0;
    // If mincut < totalCuts, complete swaps up to stop
    if (mincut < totalCuts) {
        tit = tentative.begin();
        while (true) {
            totalCuts = tit->second;
            Node *n1 = tit->first.first;
            Node *n2 = tit->first.second;
            for (n1it = n1->connected.begin(); n1it != n1->connected.end(); n1it++) {
                for (it1 = nodes.begin(); it1 != nodes.end(); it1++) {
                    if (it1->label == n1it->first) {
                        if (it1->partition == n1->partition) n1it->second.cut = true;
                        if (it1->partition == n2->partition) n1it->second.cut = false;
                    }
                }
            }

            for (n2it = n2->connected.begin(); n2it != n2->connected.end(); n2it++) {
                for (it1 = nodes.begin(); it1 != nodes.end(); it1++) {
                    if (it1->label == n2it->first) {
                        if (it1->partition == n2->partition) n2it->second.cut = true;
                        if (it1->partition == n1->partition) n2it->second.cut = false;
                    }
                }
            }
            swap(n1->partition, n2->partition);
            iterations++;
            if (iterations == stopIterations) {
                cout <<"Stopped at " << stopIterations << " iterations.\n";
                break;
            }
            if (tit->first == stop.first) {
                cout << "Stopped at mincut swap.\nLast swap ";
                cout << stop.first.first->label << "," << stop.first.second->label;
                cout << " with " << stop.second << " cuts remaining\n";
                break;
            }
            tit++;
            if (tit == tentative.end()) {
                cout << "No more swaps possible.\n";
                break;
            }
        }
    }
    cout << "\nInitial cutset:" << initial << "\nMin cutset value after ";
    cout << iterations << " iterations: " << totalCuts << endl;
    return 0;
}