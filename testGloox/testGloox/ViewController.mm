//
//  ViewController.m
//  testGloox
//
//  Created by 신창헌 on 2016. 8. 27..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import "ViewController.h"

#include "gloox/util.h"

using namespace gloox;

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    util::escape("hello");
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
