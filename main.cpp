///B-Tree///
//Insert, Search, split functions
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int MaxChild;

vector <int> CreateArray(){ //Creating a random array
    srand (static_cast <unsigned int>(time(0)));
    vector <int> array(20);
    for (int i = 0; i < array.size(); i++)
        array[i] = rand() % 100;
    return array;
}

struct BNode{
    bool is_leaf; // Is it a leaf?(list)
    vector<int>keys;
    vector<BNode*> children;

    //Constructor for each node
    BNode(bool n = false) { //n - the number which is need to be inserted
        is_leaf = n;
    }
};

void Split(BNode* parent, int child_ind, BNode* child) {
    BNode* new_child = new BNode(child->is_leaf); //New node for 2nd part of array
    int split_point = MaxChild / 2;
    //Transport the half of keys into the new array
    new_child->keys.assign(child->keys.begin() + split_point + 1, child->keys.end());
    child->keys.erase(child->keys.begin() + split_point + 1, child->keys.end());

    if (!child->is_leaf) {
        new_child->children.assign(child->children.begin() + split_point + 1, child->children.end());
        child->children.erase(child->children.begin() + split_point + 1, child->children.end());
    }
    //Insert the taken keys into the parent array
    parent->keys.insert(parent->keys.begin() + child_ind, child->keys[split_point]);
    child->keys.erase(child->keys.begin() + split_point);

    parent->children.insert(parent->children.begin() + child_ind + 1, new_child);

}

void Insert(BNode* &node, int key){
    if (!node) { //If node is empty
        node = new BNode(true);
        node->keys.push_back(key);
        return;
    }
    if (node->keys.size() >= MaxChild - 1){ //Node is overfull
        BNode* new_root = new BNode(false);
        new_root->children.push_back(node); //Current node is children for new root
        Split(new_root, 0, node);

        node = new_root;
    }

    int i = 0;
    while (i < node->keys.size() && key > node->keys[i])
        i++;
    if (node->is_leaf) {
        node->keys.insert(node->keys.begin() + i, key);
        return;
    }
    Insert(node->children[i], key);
}

BNode* Search(BNode* node, int num) {
    if (!node) { //if node is empty
        cout << "Keys not found.";
        return nullptr;
    }

    int i = 0;
    while (i < node->keys.size() && num > node->keys[i])
        i++;
    if (i < node->keys.size() && num == node->keys[i]) {
        cout << "Keys has found.";
        return node;
    }
    if (node->is_leaf) {
        cout << "End of tree, keys not found.";
        return nullptr;
    }
    return Search(node->children[i], num); //Recursive search in childNode
}


int main() {
    int to_find = 0;
    BNode* root = new BNode(true);
    vector<int>array = CreateArray();
    cout << "Our array:\n";
    for (int i = 0; i < array.size(); i++){
        cout << array[i] << " ";
    }
    cout << "\nWhat degree of B-tree do You want?\n";
    cin >> MaxChild;
    for (int i = 0; i < array.size(); i++){
        Insert(root,array[i]);
    }
    cout << "It's done.\n" << "Which element would You like to find?\n";
    cin >> to_find;
    Search(root, to_find);
    return 0;
}