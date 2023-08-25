#include "requests.hpp"

int main(){
    initCurl();
    cout<< "GET REQ: "<<httpsGet("https://flask-urona-n-linkup-server.onrender.com")<<endl;
    cout<< "POST REQ: "<< httpsPost("https://flask-urona-n-linkup-server.onrender.com/post", "data", "TEST::POST");
    curl_easy_cleanup(curl);
}