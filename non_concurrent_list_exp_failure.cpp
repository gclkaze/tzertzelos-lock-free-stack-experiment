#include <iostream>
#include <memory>
#include <thread>
#include <functional>
#include <cassert>

class exp_stack{

    private:

        struct node{
    
            int v;
            node* next;
        
            node():next(nullptr),v(-1){}
            ~node(){}
        };

        node* head;

    public:
    
        exp_stack() : head(new node()){}
        ~exp_stack(){
    
            node* prev = head;
            node* current = head->next;
            int i = 0;
    
            while(current){
    
                i++;
                prev = current;
                current = current->next;
                prev->next = nullptr;
                delete prev;
            }
    
            std::cout << "Nodes freed " << i + 1 << std::endl;
            delete head;
            head = nullptr;
            dump();
        }

        void push(int v){
    
            auto n  = (new node());
            n->v = v;
            n->next = head;
            std::cout<<n->next<<"  "<<head<<std::endl;
            assert (n->next == head);
            head = n;
            assert(head == n);
    
        }

        void dump(){
    
            node* current = head;
            int length = 0;
    
            while(current){
                //std::cout << "v : " << current->v << std::endl;
                current = current->next;
                length++;
            }
    
            std::cout << "List length " << length << std::endl;
            assert(length == 30001);
        }
};

void fill(exp_stack& s){
    
    for(int i = 0;i<100;i++)
        s.push(1);
}


int main(){

    exp_stack* s = (new exp_stack());
    int i = 100;
    
    while(i){
    
        std::thread t(&::fill, std::ref(*s));
        std::thread t2(&::fill, std::ref(*s));
        std::thread t3(&::fill, std::ref(*s));
    
        t3.join();
        t.join();
        t2.join();

        i--;
    }
    
    s->dump();

    delete s;
    return 1;
}
