#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <ctime>

int get_Number_Size(int number)
{
    int cnt = 0;
    if (number < 0) cnt += 1;
    while (number)
    {
        number /= 10;
        cnt += 1;
    }
    return cnt;
}

//Structure of binary tree's element (node)
struct Node
{
    Node(int value)
    {
        _left = NULL;
        _right = NULL;
        _number = value;
    }
    Node()
    {
        _left = NULL;
        _right = NULL;
        _number = NULL;
    }
    int _number;
    Node* _left;
    Node* _right;
};

//Queue for elements in breadth-first traverse
class Queue
{
private:
    Node** _arr;
    int _size;
public:
    Queue()
    {
        _arr = NULL;
        _size = 0;
    }
    ~Queue()
    {
        delete[] _arr;
        _size = 0;
    }

    void add_Element(Node* element);
    Node* get_Element();
    int get_Size();
};

void Queue::add_Element(Node* element)
{
    Node** temp = new Node*[_size + 1];
    for (int i = 0; i < _size; i++)
    {
        temp[i] = _arr[i];
    }
    temp[_size] = element;
    if (_arr) delete[] _arr;
    _arr = temp;
    _size++;
}

Node* Queue::get_Element()
{
    Node** temp = new Node*[_size - 1];
    for (int i = 0; i < _size - 1; i++)
    {
        temp[i] = _arr[i + 1];
    }
    Node* save = _arr[0];
    if (_arr) delete[] _arr;
    _arr = temp;
    _size--;
    return save;
}

int Queue::get_Size()
{
    return _size;
}

class Binary_Tree
{
private:
    //Element in the corner of binary tree
    Node* _root;
    int _size;
public:
    //Constructor
    Binary_Tree(int root)
    {
        _root = new Node(root);
        _size = 1;
    }
    Binary_Tree()
    {
        _root = NULL;
        _size = 0;
    }
    //Deconstructor
    ~Binary_Tree()
    {
        delete_Tree(_root);
    }

    void add_Element(int value);
    void add_Element(int value, std::ofstream& fout);
    bool search_Element(int value);
    void delete_Element(int value);
    void delete_Element(int value, std::ofstream& fout);
    void preorder_Traverse(Node* root);
    void postorder_Traverse(Node* root);
    void breadth_First_Traverse();
    void delete_Tree(Node* root);
    void show_Tree(Node* elem, int space, int sign);
    void show_Tree(Node* elem, int space, int sign, std::ofstream& fout);
    Node* get_Root();
    int get_Size();
};

void Binary_Tree::add_Element(int value)
{
    //Search of element's place become from root
    Node* temp = _root;
    //"While" runs before temp element exists and it's value isn't equal with value we want to add
    if (_root == NULL)
    {
        _root = new Node(value);
        return;
    }
    while (temp and temp->_number != value)
    {
        if (temp->_number > value and temp->_left == NULL)
        {
            temp->_left = new Node(value);
            _size++;
            return;
        }
        if (temp->_number < value and temp->_right == NULL)
        {
            temp->_right = new Node(value);
            _size++;
            return;
        }
        if (temp->_number > value) temp = temp->_left;
        else temp = temp->_right;
    }
    std::cout << "Element with value " << value << " already exists\n";
}

void Binary_Tree::add_Element(int value, std::ofstream& fout)
{
    Node* temp = _root;
    if (_root == NULL)
    {
        fout << "Element " << value << " becomes the root\n";
        _root = new Node(value);
        return;
    }
    while (temp and temp->_number != value)
    {
        if (temp->_number > value and temp->_left == NULL)
        {
            fout << "Element " << temp->_number << " becomes parent of " << value << ", which is added to its left side\n";
            temp->_left = new Node(value);
            _size++;
            return;
        }
        if (temp->_number < value and temp->_right == NULL)
        {
            fout << "Element " << temp->_number << " becomes parent of " << value << ", which is added to its right side\n";
            temp->_right = new Node(value);
            _size++;
            return;
        }
        if (temp->_number > value)
        {
            fout << "Element " << value << " more than " <<  temp->_number << ", so check left element\n";
            temp = temp->_left;
        }
        else
        {
            fout << "Element " << value << " less than or equal to " << temp->_number << ", so check right element\n";
            temp = temp->_right;
        }
    }
    fout << "Element with value " << value << " already exists. Don't add it to binary tree\n";
}

bool Binary_Tree::search_Element(int value)
{
    Node* temp = _root;
    while (temp and temp->_number != value)
    {
        if (temp->_number > value) temp = temp->_left;
        else temp = temp->_right;
    }
    if (temp and temp->_number == value) return 1;
    else return 0;
}

void Binary_Tree::delete_Element(int value)
{
    Node* temp = _root;
    //Node of tempopary node's parent
    Node* parent = NULL;
    while (temp and temp->_number != value)
    {
        parent = temp;
        if (temp->_number > value) temp = temp->_left;
        else temp = temp->_right;
    }
    if (!temp)
    {
        std::cout << "Element " << value << " wasn't found\n";
        return;
    }
    if (temp->_left == NULL)
    {
        if (parent and parent->_left == temp) parent->_left = temp->_right;
        if (parent and parent->_right == temp) parent->_right = temp->_right;
        _size--;
        delete temp;
        return;
    }
    if (temp->_right == NULL)
    {
        if (parent and parent->_left == temp) parent->_left = temp->_left;
        if (parent and parent->_right == temp) parent->_right = temp->_left;
        _size--;
        delete temp;
        return;
    }
    //If function still continues, tempopary node has two nodes
    Node* additional_node = temp->_right;
    while (additional_node->_left) additional_node = additional_node->_left;
    int save_number = additional_node->_number;
    delete_Element(save_number);
    temp->_number = save_number;
}

void Binary_Tree::delete_Element(int value, std::ofstream& fout)
{
    Node* temp = _root;
    Node* parent = NULL;
    while (temp and temp->_number != value)
    {
        parent = temp;
        if (temp->_number > value)
        {
            fout << "Element " << value << " more than " << temp->_number << ", so check left element\n";
            temp = temp->_left;
        }
        else
        {
            fout << "Element " << value << " less than or equal to " << temp->_number << ", so check right element\n";
            temp = temp->_right;
        }
    }
    if (!temp)
    {
        fout << "Element " << value << " wasn't in binary tree\n";
        return;
    }
    if (temp->_left == NULL)
    {
        fout << "Element " << temp->_number << " hasn't got left node ";
        if (parent and parent->_left == temp)
        {
            fout << " and it is it's parent left node, so give to last one value of temp's right element\n";
            parent->_left = temp->_right;
        }
        if (parent and parent->_right == temp)
        {
            fout << " and it is it's parent right node, so give to last one value of temp's right element\n";
            parent->_right = temp->_right;
        }
        _size--;
        delete temp;
        return;
    }
    if (temp->_right == NULL)
    {
        fout << "Element " << temp->_number << " hasn't got right node ";
        if (parent and parent->_left == temp)
        {
            fout << " and it is it's parent left node, so give to last one value of temp's left element\n";
            parent->_left = temp->_left;
        }
        if (parent and parent->_right == temp)
        {
            fout << " and it is it's parent right node, so give to last one value of temp's left element\n";
            parent->_right = temp->_left;
        }
        _size--;
        delete temp;
        return;
    }
    fout << "Element " << temp->_number << " has both nodes, so search the least right element\n";
    Node* additional_node = temp->_right;
    while (additional_node->_left) additional_node = additional_node->_left;
    int save_number = additional_node->_number;
    fout << "It is " << save_number << '\n';
    delete_Element(save_number);
    temp->_number = save_number;
}

void Binary_Tree::preorder_Traverse(Node* root)
{
    if (root)
    {
        std::cout << root->_number << ' ';
        preorder_Traverse(root->_left);
        preorder_Traverse(root->_right);
    }
}

void Binary_Tree::postorder_Traverse(Node* root)
{
    if (root)
    {
        postorder_Traverse(root->_left);
        postorder_Traverse(root->_right);
        std::cout << root->_number << ' ';
    }
}

void Binary_Tree::breadth_First_Traverse()
{
    Node* temp = _root;
    Queue elem_queue;
    elem_queue.add_Element(temp);
    while (elem_queue.get_Size() != 0)
    {
        temp = elem_queue.get_Element();
        std::cout << temp->_number << ' ';
        if (temp->_left) elem_queue.add_Element(temp->_left);
        if (temp->_right) elem_queue.add_Element(temp->_right);
    }
}

void Binary_Tree::delete_Tree(Node* root)
{
    if (root != NULL)
    {
        delete_Tree(root->_left);
        delete_Tree(root->_right);
        delete root;
    }
}

void Binary_Tree::show_Tree(Node* elem, int space, int sign)
{
    if (elem->_left != NULL) show_Tree(elem->_left, space + 4 + get_Number_Size(elem->_number), 1);
    int save = space;
    while (save)
    {
        std::cout << ' ';
        save -= 1;
    }
    if (sign == 1) std::cout << '.';
    else if (sign == 2) std::cout << "`";
    std::cout << "-->";
    std::cout << elem->_number << '\n';
    if (elem->_right != NULL) show_Tree(elem->_right, space + 4 + get_Number_Size(elem->_number), 2);
}

void Binary_Tree::show_Tree(Node* elem, int space, int sign, std::ofstream& fout)
{
    if (elem == NULL) return;
    if (elem->_left != NULL) show_Tree(elem->_left, space + 4 + get_Number_Size(elem->_number), 1, fout);
    int save = space;
    while (save)
    {
        fout << ' ';
        save -= 1;
    }
    if (sign == 1) fout << '.';
    else if (sign == 2) fout << "`";
    fout << "-->";
    fout << elem->_number << '\n';
    if (elem->_right != NULL) show_Tree(elem->_right, space + 4 + get_Number_Size(elem->_number), 2, fout);
}

Node* Binary_Tree::get_Root()
{
    return _root;
}

int Binary_Tree::get_Size()
{
    return _size;
}

void generate_Work(std::ofstream& task, std::ofstream& answer)
{
    Binary_Tree tree;
    int len = (rand() % 10 + 1);
    int* elements = new int[len + 1];
    int number;
    task << "1. Create binary tree with elements:\n";
    answer << "1. Forming binary tree:\n";
    for (int i = 0; i < len; i++)
    {
        number = (rand() % 200 - 100);
        elements[i] = number;
        answer << "Step " << i << ", element " << number << ":\n";
        task << number << ' ';
        tree.add_Element(number, answer);
        if (tree.get_Size()) tree.show_Tree(tree.get_Root(), 0, 0, answer);
    }
    number = (rand() % 200 - 100);
    task << "\n2. Add element " << number << " to binary tree\n";
    answer << "2. Adding element " << number << " to binary tree:\n";
    tree.add_Element(number, answer);
    tree.show_Tree(tree.get_Root(), 0, 0, answer);
    elements[len] = number;
    number = elements[rand() % (len + 1)];
    task << "3. Delete element " << number << " in binary tree\n";
    answer << "3. Deleting element " << number << " in binary tree:\n";
    tree.delete_Element(number, answer);
    tree.show_Tree(tree.get_Root(), 0, 0, answer);
}

int main()
{
    srand(time(0));
    Binary_Tree tree;
    int method_of_filling, task;
    unsigned int len = 0;
    int number = 0;
    try
    {

        std::ifstream fin("data.txt");
        std::ofstream fout_task("output_task.txt");
        std::ofstream fout_answer("output_key.txt");
        if (!fin.is_open()) throw "Check file: data.txt must exist";
        std::cout << "There are three methods of filling:\n";
        std::cout << "1. Filling by random numbers\n";
        std::cout << "2. Filling by console\n";
        std::cout << "3. Filling by file\n";
        std::cout << "Write number of method: ";
        std::cin >> method_of_filling;
        if (!std::cin) throw "Wrong method of filling (it must be an unsigned integer)";
        switch (method_of_filling)
        {
        case(1):
        {
            std::cout << "Write size of tree: ";
            std::cin >> len;
            if (!std::cin or len == 0) throw "Wrong length of tree (it must be an unsigned integer more then zero)";
            auto t_start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < len; i++)
            {
                tree.add_Element(rand() % 200 - 100);
            }
            auto t_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration< double > t_cnt = t_end - t_start;
            std::cout << "\nOn adding tree spent " << std::fixed << t_cnt.count() << "s.\n";
            break;
        }
        case(2):
        {
            std::cout << "Write value (when you write 12321, input ends): ";
            std::chrono::duration< double > t_cnt = std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now();
            while (number != 12321)
            {
                std::cout << "Write value (when you write 12321, input ends): ";
                std::cin >> number;
                if (!std::cin) throw ("Invalid value of array's element");
                auto t_start = std::chrono::high_resolution_clock::now();
                if (number != 12321) tree.add_Element(number);
                auto t_end = std::chrono::high_resolution_clock::now();
                t_cnt += t_end - t_start;
                len++;
            }
            std::cout << "\nOn adding tree spent " << std::fixed << t_cnt.count() << "s.\n";
            break;
        }
        case(3):
        {
            auto t_start = std::chrono::high_resolution_clock::now();
            while (fin)
            {
                fin >> number;
                tree.add_Element(number);
                len++;
            }
            auto t_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration< double > t_cnt = t_end - t_start;
            std::cout << "\nOn adding tree spent " << std::fixed << t_cnt.count() << "s.\n";
            break;
        }
        default:
        {
            std::cout << "There isn't operation with number " << method_of_filling << '\n';
            return 0;
        }
        }
        std::cout << '\n';
        tree.show_Tree(tree.get_Root(), 0, 0);
        std::cout << '\n';
        std::cout << "Task numbers: \n";
        std::cout << "1. Add new element to binary tree\n";
        std::cout << "2. Delete element of binary tree\n";
        std::cout << "3. Find element of binary tree\n";
        std::cout << "4. Show traverses\n";
        std::cout << "5. Generate work(-s)\n";
        std::cout << "0. Exit\n\n";
        std::cin >> task;
        if (!std::cin) throw ("Invalid method: it must be integer");
        while (task)
        {
            switch (task)
            {
            case(1):
            {
                std::cout << "Write number: ";
                std::cin >> number;
                if (!std::cin) throw ("Invalid element: it must be integer");
                auto t_start = std::chrono::high_resolution_clock::now();
                tree.add_Element(number);
                auto t_end = std::chrono::high_resolution_clock::now();
                std::chrono::duration< double > t_cnt = t_end - t_start;
                std::cout << "\nOn adding element spent " << std::fixed << t_cnt.count() << "s.\n\n";
                tree.show_Tree(tree.get_Root(), 0, 0);
                break;
            }
            case(2):
            {
                std::cout << "Write number: ";
                std::cin >> number;
                if (!std::cin) throw ("Invalid element: it must be integer");
                auto t_start = std::chrono::high_resolution_clock::now();
                tree.delete_Element(number);
                auto t_end = std::chrono::high_resolution_clock::now();
                std::chrono::duration< double > t_cnt = t_end - t_start;
                std::cout << "\nOn deleting element spent " << std::fixed << t_cnt.count() << "s.\n\n";
                tree.show_Tree(tree.get_Root(), 0, 0);
                break;
            }
            case(3):
            {
                auto t_start = std::chrono::high_resolution_clock::now();
                std::cout << "Write number: ";
                std::cin >> number;
                if (!std::cin) throw ("Invalid element: it must be integer");
                if (tree.search_Element(number)) std::cout << "\nElement " << number << " is in binary tree\n";
                else std::cout << "\nElement " << number << " is in binary tree\n";
                auto t_end = std::chrono::high_resolution_clock::now();
                std::chrono::duration< double > t_cnt = t_end - t_start;
                std::cout << "\nOn searching element spent " << std::fixed << t_cnt.count() << "s.\n";
                break;
            }
            case(4):
            {
                std::cout << "\nPreorder traverse:\n";
                tree.preorder_Traverse(tree.get_Root());
                std::cout << "\nPostorder traverse:\n";
                tree.postorder_Traverse(tree.get_Root());
                std::cout << "\nBreadth-first traverse:\n";
                tree.breadth_First_Traverse();
                std::cout << '\n';
                break;
            }
            case(5):
            {

                std::cout << "Write number: ";
                std::cin >> number;
                if (!std::cin or number < 0) throw ("Invalid number: it must be unsigned integer");
                for (int i = 0; i < number; i++)
                {
                    fout_task << "Work " << i + 1 << '\n';
                    fout_answer << "Work " << i + 1 << '\n';
                    generate_Work(fout_task, fout_answer);
                    fout_task << '\n';
                    fout_answer << '\n';
                }
                break;
            }
            }
            std::cout << "\nTask numbers: \n";
            std::cout << "1. Add new element to binary tree\n";
            std::cout << "2. Delete element of binary tree\n";
            std::cout << "3. Find element of binary tree\n";
            std::cout << "4. Show traverses\n";
            std::cout << "5. Generate work(-s)\n";
            std::cout << "0. Exit\n";
            std::cout << '\n';
            std::cin >> task;
            if (!std::cin) throw ("Invalid method: it must be integer");
        }
        fin.close();
        fout_task.close();
        fout_answer.close();
    }
    catch (const char* ex)
    {
        std::cout << ex;
    }
}