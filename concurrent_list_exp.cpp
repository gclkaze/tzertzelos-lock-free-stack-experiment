#include <iostream>
#include <memory>
#include <thread>
#include <functional>
#include <cassert>
#include <atomic>

class exp_stack{

    private:

        struct node{
    
            int v;
            node* next;
        
            node():next(nullptr),v(-1){}
            ~node(){}
        };

        std::atomic<node*> head{nullptr};

    public:

        exp_stack() : head(new node()){}
        ~exp_stack(){
            
            node* prev = head.load();
            node* current = head.load()->next;
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

            node* n  = new node();
            n->v = v;
            n->next = head.load();
            
            while(!head.compare_exchange_weak(n->next,n )){ //if head == n->next, then head = n, return true finally
                std::cout<<n->next<<"  "<<head<<std::endl;
            
            };
            //Outside of the cmp_exchng atomic loop, head == n->next for a slight moment, afterwards head gets value == n
            //std::cout<<head<<"=="<<n<<std::endl;
            
            if(head != n){
                //Head changed again by another thread that called compare_exchange_weak after the successful cmp_xch call of the current thread!!
                //Shit!!!
                std::cout<<head<<"   WHAT   "<<n<<std::endl;//exit(1);
            }
            
        }

        void dump(){
            
            node* current = head;
            
            if(!head)
                return;
            
            while(current){
                current = current->next;
            }
        }

        int length(){
            
            node* current = head;
            int length = 0;
            
            if(!head)
                return 0;
            
            while(current){
                
                current = current->next;
                length++;
            }
            
            std::cout << "List length " << std::dec << length << std::endl;
            assert(length == 30001);
            return length;
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
    
    s->length();

    delete s;
    return 1;
}
