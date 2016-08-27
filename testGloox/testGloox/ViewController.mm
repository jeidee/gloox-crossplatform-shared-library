//
//  ViewController.m
//  testGloox
//
//  Created by 신창헌 on 2016. 8. 27..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import "ViewController.h"

#include "gloox-wrapper/MsgClient.h"

using namespace jd;
using namespace std;

MsgClient* gMc = NULL;

@interface ViewController ()

@end

@implementation ViewController

void onLog(const char* tag, const char* log) {
    printf("%s :: %s\n", tag, log);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    if (gMc != NULL) {
        gMc->disConnect();
        delete gMc;
    }
    gMc = new MsgClient();
    
    gMc->setLoginInfo("test1@bypass", "1234", "localhost", 5223);
    
    gMc->registerOnLog(&onLog);
    
    if (!gMc->connect()) {
        printf("Connect failed\n");
        return;
    }
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewDidDisappear:(BOOL)animated {
    gMc->disConnect();
    delete gMc;
    gMc = NULL;
}

@end
