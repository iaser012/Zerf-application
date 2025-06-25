/* 
- Lenguaje C++.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <algorithm>

using namespace std;

class Node {
public:
    string name;
    bool isDirectory;
    map<string, shared_ptr<Node>> children;
    weak_ptr<Node> parent;

    Node(string name, bool isDirectory) : name(name), isDirectory(isDirectory) {}
};

class FileSystem {
private:
    shared_ptr<Node> root;
    shared_ptr<Node> current;

public:
    FileSystem() {
        root = make_shared<Node>("/", true);
        current = root;
    }

    void mkdir(const string& dirName) {
        if (current->children.count(dirName)) {
            cout << "Directory already exists.\n";
            return;
        }
        auto dir = make_shared<Node>(dirName, true);
        dir->parent = current;
        current->children[dirName] = dir;
    }

    void touch(const string& fileName) {
        if (current->children.count(fileName)) {
            cout << "File already exists.\n";
            return;
        }
        auto file = make_shared<Node>(fileName, false);
        file->parent = current;
        current->children[fileName] = file;
    }

    void ls() const {
        for (const auto& child : current->children) {
            cout << child.first;
            if (child.second->isDirectory)
                cout << "/";
            cout << "  ";
        }
        cout << "\n";
    }

    void cd(const string& dirName) {
        if (dirName == "..") {
            if (auto parent = current->parent.lock()) {
                current = parent;
            }
            return;
        }
        if (current->children.count(dirName) && current->children[dirName]->isDirectory) {
            current = current->children[dirName];
        } else {
            cout << "Directory not found.\n";
        }
    }

    void pwd() const {
        vector<string> path;
        auto node = current;
        while (node != root) {
            path.push_back(node->name);
            node = node->parent.lock();
        }
        reverse(path.begin(), path.end());
        cout << "/";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i != path.size() - 1) cout << "/";
        }
        cout << "\n";
    }
};

int main() {
    FileSystem fs;
    string cmd;
    while (true) {
        cout << "$ ";
        getline(cin, cmd);
        stringstream ss(cmd);
        string token;
        ss >> token;
        if (token == "exit") break;
        else if (token == "mkdir") {
            string name; ss >> name;
            fs.mkdir(name);
        } else if (token == "touch") {
            string name; ss >> name;
            fs.touch(name);
        } else if (token == "ls") {
            fs.ls();
        } else if (token == "cd") {
            string name; ss >> name;
            fs.cd(name);
        } else if (token == "pwd") {
            fs.pwd();
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
