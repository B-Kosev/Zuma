#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

vector<int>P, C;

struct Node
{
    int id, color;
    Node* next = nullptr;
    Node* prev = nullptr;

    Node(int id, int color, Node* next = nullptr, Node* prev = nullptr)
    {
        this->id = id;
        this->color = color;
        this->next = next;
        this->prev = prev;
    }
};

vector<Node*> nodes;

class List
{
private:
    Node* head, * tail;
    int lengthCount;
public:
    List()
    {
        head = nullptr;
        tail = nullptr;
        lengthCount = 0;
    }

    Node* getHead() { return head; }
    Node* getTail() { return tail; }
    int length() { return lengthCount; }
    void setHead(Node* head) { this->head = head; }
    void setTail(Node* tail) { this->tail = tail; }

    void push_back(int id, int color)
    {
        lengthCount++;
        if (head == nullptr)
        {
            head = new Node(id, color);
            tail = head;
            nodes.push_back(head);
        }
        else
        {
            Node* el = new Node(id, color);
            tail->next = el;
            el->prev = tail;
            tail = tail->next;
            nodes.push_back(el);
        }
    }

    void push_back(Node* el)
    {
        lengthCount++;
        if (head == nullptr)
        {
            head = el;
            tail = head;
        }
        else
        {
            tail->next = el;
            el->prev = tail;
            tail = tail->next;
        }
        nodes.push_back(el);
    }

    void insert(Node*target, int id, int color)
    {
        Node* el = new Node(id, color);
        Node* next = target->next;
        target->next = el;
        el->next = next;
        el -> prev = target;
        next->prev = el;
    }

    Node* insert(Node*target, Node* el)
    {    
        lengthCount++;
        if (target->next)
        {
            Node* next = target->next;
            target->next = el;
            el->next = next;
            el->prev = target;
            next->prev = el;
        }
        else
        {
            target->next = el;
            el->prev = target;
            el->next = nullptr;
        }

        return el;
    }

    void pop()
    {
        lengthCount--;

        Node* toPop = tail;

        if (head == tail)
        {
            head = nullptr;
            tail = nullptr;
        }

        if (tail != nullptr && tail != head)
        {
            tail = tail->prev;
            tail->next = nullptr;
        }

        delete toPop;
    }

    void remove(int pos)
    {
        lengthCount--;

        if (pos == lengthCount - 1)
        {
            pop();
            return;
        }

        Node* toRemove;

        if (pos == 0)
        {
            toRemove = head;
            head = head->next;
            head->prev = nullptr;
            delete toRemove;
            return;
        }

        toRemove = at(pos);
        Node* next = toRemove->next;
        Node* prev = toRemove->prev;

        next->prev = prev;
        prev->next = next;
        delete toRemove;
    }

    Node* at(int ind)
    {
        Node* current = head;
        while (current != nullptr)
        {
            if (current->id == ind) break;
            current = current->next;
        }
        return current;
    }

    void print()
    {
        if (lengthCount == 0)
            printf("%d", -1);
        Node* i = head;
        while (i)
        {
            printf("%d ", i->color);
            i = i->next;
        }
    }

    int destroy(Node* el, int deleteCount)
    {
        int color = el->color;

        //check if there are at least 3 element around el to be removed
        int minCounter = 1; //=el

        if (el->next && el->next->color == color) //if there is next element with same color
        {
            minCounter++;
            if (el->next->next && el->next->next->color == color) //if there are 2 elements after el with same color
                minCounter++;
        }
        if (el->prev && el->prev->color == color) //if there is previous el with same color
        {
            minCounter++;
            if (el->prev->prev && el->prev->prev->color == color) //if there are 2 elements before el with same color
                minCounter++;
        }

        if (minCounter < 3) //if there are not enough elements
        {
            lengthCount -= deleteCount;
            return deleteCount;
        }

        //if there are items to be destroyed
        //starting with el, make 2 pointers - one going forward and one going backwards

        Node* forward = nullptr;
        Node* backwards = nullptr;
        Node* connectL = nullptr, * connectR = nullptr;

        //loop until end of sequence
        bool flagL = true, flagR = true; //flags to check if next/prev element is valid

        forward = el;

        if (el->prev && el->prev->color == color) //if there is prev element with same color
            backwards = el->prev;
        else if (el->prev && el->prev->color != color) //if there is prev element with different color
        {
            connectL = el->prev;
            flagL = false;
        }
        else if(!el->prev) //if there is no prev element
            flagL = false;

        while (flagL || flagR) //loop unitl all elements with color are deleted
        {
            if (flagR)
            {
                if (forward->next && forward->next->color == color) //if there is next element with same color
                {
                    forward = forward->next;
                    delete forward->prev;
                    deleteCount++;
                }
                else flagR = false;
            }

            if (flagL)
            {
                if (backwards->prev && backwards->prev->color == color) //if there is previous element with same color
                {
                    backwards = backwards->prev;
                    delete backwards->next;
                    deleteCount++;
                }
                else flagL = false;
            }
        }

        //make connection points
        if (!connectL && backwards && backwards->prev)
            connectL = backwards->prev;

        if (forward && forward->next)
            connectR = forward->next;

        //delete last elements
        if (backwards)
        {
            delete backwards;
            deleteCount++;
        }
        if (forward)
        {
            delete forward;
            deleteCount++;
        }

        //now we have to connect the remaining list
        if (connectL && connectR) //if we have 2 valid elements to be connected on the left and right
        {
            connectL->next = connectR;
            connectR->prev = connectL;

            if (connectL->color == connectR->color) //if we have new sequence with same colors
                return destroy(connectL, deleteCount);
            else
            {
                lengthCount -= deleteCount;
                return deleteCount;
            }
        }
        else if (!connectL && connectR) //if we dont have left node this means that the right is the beggining of the new list
        {
            head = connectR;
            head->prev = nullptr;
            return deleteCount;
        }
        else if (connectL && !connectR) //if we dont have right node this means thaht the last is the tail of the new list
        {
            tail = connectL;
            tail->next = nullptr;
            return deleteCount;
        }
        else //if we dont have left and right - the list is empty
        {
            head = nullptr;
            tail = nullptr;
            lengthCount = 0;
            return deleteCount;
        }
    }
};




int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q, color, p, c;
    cin >> n;

    List list;

    bool gameOver = false;
    int destroyed = 0;

    for (int i = 0; i < n; i++) //make initial list
    {
        cin >> color;
        list.push_back(i, color);
    }

    cin >> q;
    for (int i = 0; i < q; i++)
    {
        cin >> p >> c;
        P.push_back(p);
        C.push_back(c);
    }


    for (int i = 0; i < q; i++)
    {
        Node* newBall = new Node(i + n, C[i]);

        if (!gameOver)
        {
            newBall = list.insert(nodes[P[i]], newBall);
            auto itPos = nodes.begin() + P[i] + 1;
            nodes.push_back(newBall);
        }

        if (gameOver)
            printf("Game Over\n");
        else
        {
            destroyed = list.destroy(newBall, 0);
            printf("%d\n", destroyed);
            if (list.length() == 0)
                gameOver = true;
        }
    }

    list.print();

    return 0;
}
