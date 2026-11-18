#include <Povly.h>

class Sandbox: public Povly::Application {
    public:
        Sandbox(){
            
        }

        ~Sandbox(){
            
        }
};

Povly::Application* Povly::CreateApplication(){
    return new Sandbox();
}