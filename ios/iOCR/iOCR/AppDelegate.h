//
//  AppDelegate.h
//  iOCR
//
//  Created by cstlab on 13-9-16.
//  Copyright (c) 2013年 liqilei. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PhotoViewController.h"
#import "HomeViewController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) PhotoViewController* photoViewController;
@property (strong, nonatomic) HomeViewController* homeViewController;

@end
