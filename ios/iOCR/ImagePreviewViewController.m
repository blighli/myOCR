//
//  ImagePreviewViewController.m
//  iOCR
//
//  Created by liqilei on 11/18/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import "ImagePreviewViewController.h"
#import "ImageView.h"
#import "ImageAdapter.h"
#import "ImageProcess.h"

@interface ImagePreviewViewController ()

@end

@implementation ImagePreviewViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
}

- (void) viewWillAppear:(BOOL)animated
{
    ImageView* imageView = (ImageView*)self.view;
    ImageAdapter *adapter = [[ImageAdapter alloc] init];
    imageView.image = [adapter convertToUIImage: self.cvImage];
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskLandscapeRight;
}
- (IBAction)close:(id)sender {
    [self dismissViewControllerAnimated:NO completion:nil];
}

@end
