//211 - Kikteva Veronika
//Zadacha 1

#ifndef Controller_hpp
#define Controller_hpp

#include "Canvas.h"
#include "kmeans.hpp"
#include "ClusterBuilder.h"
#include <ctime>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
// u nas vsego odin object, poetomu mi sdelali constuctor privatnym
class Controller{
public: class command;
private:
    //sdelali constructor privatnym, chtoby ne sozdavat exemplyary vne klassa
    Controller();
    //exemplyar, ediniza
    static Controller * instance;
    void processCommand(const char *);
    std::map<const char *, command *> commands;
    Canvas canvas;
    int disp = 0;
public:
    //staticheskiy metod imeet dostup k privatnym polyam klassa i pozvolyaet rabotat' s exemplyarami, no v nego ne peredaets'a this
    static Controller & getInstance();
    //sadimsya na fail (stdin v osnovnom) i chitaem comandy
    void acquireListener(FILE*);
};

class Controller::command{
public:
    Controller * env;
    virtual void processCommand(const char * args) = 0;
    virtual ~command() = 0;
};

#endif /* Controller_hpp */
