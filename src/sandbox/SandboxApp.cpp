#include <Povly.h>

class Sandbox: public Povly::Application {
    public:
        Sandbox(){
            
        }

        ~Sandbox(){
            
        }
};

int main(){
    Sandbox* sandbox = new Sandbox();
    sandbox->Run();
    delete sandbox;
}