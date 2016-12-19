//
// Created by Marek on 2016-12-19.
//

#include <iostream>
#include <vector>

#define MAX_NUMBER_OF_LEAS 500006
#define MAX_NUMBER_OF_QUERIES 500006
#define NO_EDGE -1

using namespace std;

/**
 * TODO
 * dla danego wierzchołka potrzebujemy:
 *  ? tablica ojców w odległości 2^i
 *  + odległości najdalej oddalonego wierzchołka w górę i wskaźnieka do niego
 *  + odległości najdalej oddalonego wierzchołka w dół i wskaźnieka do niego
 *  + jego wysokości
 *  + brat
 *  + drzewo
 *  - test
 *  - the rest
 */

int visited[MAX_NUMBER_OF_LEAS], height[MAX_NUMBER_OF_LEAS],
        furthest_down[MAX_NUMBER_OF_LEAS], furthest_up[MAX_NUMBER_OF_LEAS],
        left_son[MAX_NUMBER_OF_LEAS], right_son[MAX_NUMBER_OF_LEAS],
        brother[MAX_NUMBER_OF_LEAS];
int N, M;
vector<int> neighbours[MAX_NUMBER_OF_LEAS], binary_tree[MAX_NUMBER_OF_LEAS];
bool DEBUG = true;

void push_edge(int i, int second) {
    if (second != NO_EDGE) {
        if (DEBUG) {
            cout << "Pushing edge " << i << ", " << second << endl;
        }
        neighbours[i].push_back(second);
        neighbours[second].push_back(i);
    }
}

void build_edges() {
    scanf("%d", &N);
    int l, r;
    for (int i = 1; i <= N; i++) {
        scanf("%d%d", &l, &r);
        push_edge(i, l);
        push_edge(i, r);
    }
}

void build_tree(int current_lea, int current_height) {
    if (DEBUG) {
        cout << "I'm in " << current_lea << " and height is " <<
                current_height << endl;
    }
    visited[current_lea] = 1;
    height[current_lea] = current_height;
    bool was_left = false;
    for (int lea: neighbours[current_lea]) {
        if (visited[lea] == 0) {
            if (!was_left) {
                if (DEBUG) {
                    cout << lea << " <- " << current_lea << endl;
                }
                was_left = true;
                left_son[current_lea] = lea;
            } else {
                if (DEBUG) {
                    cout << current_lea << " -> " << lea << endl;
                }
                right_son[current_lea] = lea;
            }
            build_tree(lea, current_height + 1);
        }
    }
    if (DEBUG) {
        cout << "Ended " << current_lea << endl;
    }
}

void build_height(int current_lea, int current_height) {
    if (DEBUG) {
        cout << "Height of " << current_lea << " is " << current_height << endl;
    }
    height[current_lea] = current_height;
    visited[current_lea] = 0;
    for (int lea: binary_tree[current_lea]) {
        if (visited[lea] == 1) {
            build_height(lea, current_height + 1);
        }
    }
}

void update_furthest_down (int current_lea) {
    if (current_lea != 0) {
        update_furthest_down(left_son[current_lea]);
        update_furthest_down(right_son[current_lea]);
        int lfd = furthest_down[left_son[current_lea]], rfd =
                furthest_down[right_son[current_lea]];
        if (lfd == rfd) {
            furthest_down[current_lea] = current_lea;
        } else {
            furthest_down[current_lea] = height[lfd] >= height[rfd] ? lfd : rfd;
        }
        if (DEBUG) {
            cout << "Furthest down for " << current_lea << " si " <<
                    furthest_down[current_lea] << endl;
        }
    }
}

void pair_brothers(int left, int right) {
    if (left != 0 && right != 0) {
        if (DEBUG) {
            cout << left << " and " << right << " are brothers" << endl;
        }
        brother[left] = right;
        brother[right] = left;
    }
}

void find_brothers (int current_lea) {
    if (current_lea != 0) {
        pair_brothers(left_son[current_lea], right_son[current_lea]);
        find_brothers(left_son[current_lea]);
        find_brothers(right_son[current_lea]);
    }
}

void update_furthest_up (int current_lea, int my_father) {
    if (current_lea != 0) {
        if (current_lea == my_father) {
            furthest_up[current_lea] = current_lea;
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);

    build_edges();
    build_tree(1, 0);
    update_furthest_down(1);
    find_brothers(1);
    return 0;
}

