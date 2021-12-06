#include <string>

using namespace std;

#ifndef __ERROR__
#define __ERROR__

enum ErrorType {Bad_Request, Not_Found, Permission_Denied, Empty};

class Error : public exception {
    public:
    Error(ErrorType errorType) : type(errorType) {}
    string whatType() {
        switch (type) {
        case Bad_Request:
            return "Bad Request";
            break;
        case Not_Found:
            return "Not Found";
            break;
        case Permission_Denied:
            return "Permission Denied";
            break;
        case Empty:
            return "Empty";
            break;
        default:
            return "";
            break;
        }
    }
    string whatCode() {
        switch (type) {
        case Bad_Request:
            return "400";
            break;
        case Not_Found:
            return "404";
            break;
        case Permission_Denied:
            return "403";
            break;
        case Empty:
            return "204";
            break;
        default:
            return "";
            break;
        }
    }
    private:
    ErrorType type;
};

#endif