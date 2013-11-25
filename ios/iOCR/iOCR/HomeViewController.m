//
//  HomeViewController.m
//  iOCR
//
//  Created by liqilei on 11/18/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import "HomeViewController.h"
#import "CameraViewController.h"
#import "ImagePreviewViewController.h"
#import "ImageAdapter.h"

@interface HomeViewController ()
@property (strong, nonatomic) CameraViewController* cameraView;
@property (strong, nonatomic) UIImagePickerController* imagePicker;
@property (strong, nonatomic) ImagePreviewViewController* imagePreviewController;
@end

@implementation HomeViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.imagePreviewController = [[ImagePreviewViewController alloc] init];
        
        self.cameraView = [[CameraViewController alloc] init];
        
        self.imagePicker = [[UIImagePickerController alloc] init];
        self.imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
        self.imagePicker.delegate = self;
        self.imagePicker.cameraOverlayView = self.cameraView.view;
        self.imagePicker.cameraOverlayView.layer.position = CGPointMake(160,240);
        self.imagePicker.showsCameraControls = NO;
        
        CGAffineTransform transform = self.imagePicker.cameraOverlayView.transform;
        transform = CGAffineTransformRotate(transform, (M_PI/2.0));
        self.imagePicker.cameraOverlayView.transform = transform;
        
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(takePhotoDone) name:@"MyTakePhoto" object:nil];

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

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}

- (IBAction)takePhoto:(id)sender {
    [self presentViewController:self.imagePicker animated:NO completion:nil];
}

- (void) takePhotoDone
{
    [self.imagePicker takePicture];
}

- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage* image = info[UIImagePickerControllerOriginalImage];
    NSLog(@"width = %f",image.size.width);
    NSLog(@"height = %f",image.size.height);
    
    [self dismissViewControllerAnimated:YES completion:^{
        ImageAdapter *adapter = [[ImageAdapter alloc] init];
        self.imagePreviewController.cvImage = [adapter normalToIplImage: image];
        [self presentViewController:self.imagePreviewController animated:NO completion:nil];
    }];
    
}

@end
