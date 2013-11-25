//
//  CameraViewController.m
//  iOCR
//
//  Created by liqilei on 11/22/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import "CameraViewController.h"
#import "CaptureView.h"

@interface CameraViewController ()
@property (strong,nonatomic) CaptureView* captureView;
@end

@implementation CameraViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.captureView = [[CaptureView alloc] init];
        self.captureView.frame = self.view.frame;
        self.captureView.backgroundColor = [UIColor clearColor];
        [self.view insertSubview:self.captureView atIndex:0];
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)takePhoto:(id)sender {
    [[NSNotificationCenter defaultCenter] postNotificationName: @"MyTakePhoto" object:nil];
}

@end
