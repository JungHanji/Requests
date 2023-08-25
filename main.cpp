#include <curl/curl.h>
#include <iostream>
#include <stdio.h>
using namespace std;

CURL *curl;

string getMessageFromCode(int status){
    switch (status) {
        case 100: return "Continue";
        case 101: return "Switching Protocol";
        case 102: return "Processing";
        case 103: return "Early Hints";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 207: return "Multi-Status";
        case 208: return "Already Reported";
        case 226: return "IM Used";
        case 300: return "Multiple Choice";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "unused";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 418: return "I'm a teapot";
        case 421: return "Misdirected Request";
        case 422: return "Unprocessable Entity";
        case 423: return "Locked";
        case 424: return "Failed Dependency";
        case 425: return "Too Early";
        case 426: return "Upgrade Required";
        case 428: return "Precondition Required";
        case 429: return "Too Many Requests";
        case 431: return "Request Header Fields Too Large";
        case 451: return "Unavailable For Legal Reasons";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        case 506: return "Variant Also Negotiates";
        case 507: return "Insufficient Storage";
        case 508: return "Loop Detected";
        case 510: return "Not Extended";
        case 511: return "Network Authentication Required";

        default:
        case 500: return "Internal Server Error";
    }
}

int initCurl(string certificates = "cacert-2023-08-22.pem"){
    curl = curl_easy_init();
    
    if(!curl){
        cout << "Error: curl not initialized" << endl;
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, 1);            
    curl_easy_setopt(curl, CURLOPT_CAINFO, certificates.data());
    curl_easy_setopt(curl, CURLOPT_CAPATH, certificates.data());

    return 0;
}

size_t getFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

string httpsGet(string url, string certificates = "cacert-2023-08-22.pem"){
    string response_string;

    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode response = curl_easy_perform (curl);
    if(response != CURLE_OK) fprintf(stderr, "Get problem: %s\n", curl_easy_strerror(response));

    return {response_string};
}

string httpsPost(string url, string key, string value, string certificates = "cacert-2023-08-22.pem"){
    
    string jsonStr = "{\"" + key + "\" : \"" + value +"\"}";
    string response_string;

    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getFunction); // call-back function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    CURLcode response = curl_easy_perform (curl);
    
    if(response != CURLE_OK) fprintf(stderr, "Post problem: %s\n", curl_easy_strerror(response));

    return {response_string};
}

int main(){
    initCurl();
    cout<< "GET REQ: "<<httpsGet("https://flask-urona-n-linkup-server.onrender.com")<<endl;
    cout<< "POST REQ: "<< httpsPost("https://flask-urona-n-linkup-server.onrender.com/post", "data", "TEST::POST");
    curl_easy_cleanup(curl);
}