class WebServer {
public:
    void stop(){
        this->~WebServer();
    }
};
