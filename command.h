//
//  command.h
//  task1
//
//  Created by Дмитрий Маслюков on 26/09/2019.
//  Copyright © 2019 Дмитрий Маслюков. All rights reserved.
//

#ifndef command_h
#define command_h

class genCluster: public Controller::command{
public:
    void processCommand(const char * args) override{
        
    }
    ~genCluster() override{}
};


#endif /* command_h */
