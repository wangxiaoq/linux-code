#include <iostream>
#include <fstream>
#include <map>
#include <memory>

/*
 * input format: start_vertex end_vertex weight
 */

struct vertex {
    vertex(int i)
        : id(i) {}
    int id;
    /* adj vertex and weight */
    std::map<int, int> adjs;
};

std::map<int, std::shared_ptr<vertex>> graph;

int load_data_from_file(char *filename)
{
    int start, end, weight;
    std::ifstream inf(filename);
    if (!inf.is_open()) {
        return -1;
    }

    while (1) {
        inf >> start >> end >> weight;
        if (!inf) {
            break;
        }
        if (graph.find(start) == graph.end()) {
            graph[start] = std::make_shared<vertex>(start);
        }
        graph[start]->adjs.insert(std::make_pair(end, weight));
        if (graph.find(end) == graph.end()) {
            graph[end] = std::make_shared<vertex>(end);
        }
        graph[end]->adjs.insert(std::make_pair(start, weight));
    }

    return 0;
}

void dijkstra(void)
{
    int start;
    std::cout << "start point:";
    std::cin >> start;
    /* id, last vertex, weight */
    std::map<int, std::pair<int, int>> known;
    /* id, last vertex, weight */
    std::map<int, std::pair<int, int>> to_be_known;

    known.insert(std::make_pair(start, std::make_pair(start, 0)));
    for (const auto &p : graph[start]->adjs) {
        to_be_known.insert(std::make_pair(p.first, std::make_pair(start, p.second + known[start].second)));
    }
    
    while (known.size() != graph.size()) {
        std::map<int, std::pair<int, int>>::iterator min_it = to_be_known.begin();
        for (auto it = to_be_known.begin(); it != to_be_known.end(); ++it) {
            if (it->second.second < min_it->second.second) {
                min_it = it;
            }
        }
        known.insert(*min_it);
        int last = min_it->first;
        to_be_known.erase(min_it);

        for (const auto &p : graph[last]->adjs) {
            if (known.find(p.first) != known.end()) {
                continue;
            }
            if (to_be_known.find(p.first) == to_be_known.end()) {
                to_be_known.insert({p.first, {last, p.second + known[last].second}});
            } else {
                if (p.second + known[last].second < to_be_known[p.first].second) {
                    to_be_known[p.first] = {last, p.second + known[last].second};
                }
            }
        }
    }

    for (const auto &p : known) {
        std::cout << p.first << " " << p.second.first << " " << p.second.second << std::endl;
    }
}

int main(int argc ,char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ./a.out filename" << std::endl;
        return 0;
    }

    load_data_from_file(argv[1]);

    for (const std::pair<int, std::shared_ptr<vertex>> &p : graph) {
        std::cout << p.first << " ";
        for (auto v : p.second->adjs) {
            std::cout << v.first << "(" << v.second << ") ";
        }
        std::cout << std::endl;
    }

    dijkstra();

    return 0;
}
