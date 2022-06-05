#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <stack>
#include <map>


struct Edge
{
    int weight;
  int indexTo;
};

struct Node
{
    int id;
    std::vector<Edge> edges;

    std::size_t prevIndex;
    int weight;
    bool visited;
};

    std::vector<Node> nodes;


bool read_nodes(std::istream& istream, std::size_t nodes_count, std::vector<Node>& graph)
{
    nodes.clear();

    for (std::size_t i = 0; i < nodes_count; i++)
    {
        int id;
        istream >> id;
        nodes.push_back({ id });
    }

    return true;
}

bool read_edges(std::istream& istream, std::size_t edges_count, std::vector<Node>& graph)
{
    for (auto& node : nodes)
    {
        node.edges.clear();
    }

    for (std::size_t i = 0; i < edges_count; i++)
    {
        int start_id, end_id;
        int weight;

        istream >> start_id >> end_id;
        istream >> weight;

        std::vector<Node> &nodes_ref = nodes;

        auto start_iter = std::find_if(nodes_ref.begin(), nodes_ref.end(), [start_id](const auto& node) { return node.id == start_id; });
        auto end_iter = std::find_if(nodes_ref.begin(), nodes_ref.end(), [end_id](const auto& node) { return node.id == end_id; });

        if (start_iter == nodes_ref.end() || end_iter == nodes_ref.end())
        {
            for (auto node : nodes)
            {
                node.edges.clear();
            }
            std::cout << "some mistake!";
            return false;
        }
        (*start_iter).edges.push_back(Edge{ weight,end_id-1});
    }

    return true;
}

std::vector<Node> read_graph(const std::string& file_path)
{
    std::vector<Node> graph;
    std::ifstream fin(file_path);
    if (fin)
    {
        std::size_t node_count;
        int edge_count;

        fin >> node_count >> edge_count;

        if (read_nodes(fin, node_count, graph))
        {
            read_edges(fin, edge_count, graph);
        }
    }
    return graph;
}

std::vector<std::size_t> convert_graph_to_path(std::vector<Node>& graph, int start_index, int end_index)
{
    std::vector<std::size_t> result;
    std::stack<std::size_t> tmp_path;
    std::size_t current_node = end_index;
    while (current_node != 1000000)
    {
        tmp_path.push(current_node);
        current_node = nodes[current_node].prevIndex;
    }
    while (!tmp_path.empty())
    {
        result.push_back(tmp_path.top());
        tmp_path.pop();
    }
    return result;
}

std::vector<std::size_t> find_path_Dijkstra(std::vector<Node>& graph, int start_index, int end_index)
{
    for (auto& node : nodes)                   
    {
        node.weight = 1000000;                     
        node.visited = false;
        node.prevIndex = 1000000;
    }                
    
    std::map<int,int> min_weigth_map;                
    nodes[start_index].weight = 0;             
    min_weigth_map.insert({ 0, start_index });       

    while (!min_weigth_map.empty())
    {
        auto [current_weight, current_index] = *(min_weigth_map.begin());
        min_weigth_map.erase(min_weigth_map.begin());
        if (nodes[current_index].visited)
        {
            continue;
        }

        nodes[current_index].visited = true;

        for (int i = 0; i < nodes[current_index].edges.size(); i++)     
        {
            int index_to = nodes[current_index].edges[i].indexTo;
            int edged_weight = nodes[current_index].edges[i].weight;
            if (!nodes[index_to].visited)
            {
                if (current_weight + edged_weight < nodes[index_to].weight)
                {
                    nodes[index_to].weight = current_weight + edged_weight;
                    nodes[index_to].prevIndex = current_index;
                    min_weigth_map.insert({ nodes[index_to].weight, index_to });
                }
            }
        }

    }

    return convert_graph_to_path(graph, start_index, end_index);
}

int main()
{
    std::vector<Node> graph = read_graph("input.txt");
    std::vector<std::size_t> path = find_path_Dijkstra(graph, 0, 1);

    for (auto path_node_index : path)
    {
        std::cout << nodes[path_node_index].id << " ";
    }

    std::cout << std::endl;
}
