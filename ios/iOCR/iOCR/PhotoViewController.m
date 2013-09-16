//
//  PhotoViewController.m
//  iOCR
//
//  Created by cstlab on 13-9-16.
//  Copyright (c) 2013年 liqilei. All rights reserved.
//

#import "PhotoViewController.h"
#import <opencv2/opencv.hpp>
#import "tesseract/baseapi.h"
#import "ImageAdapter.h"

@interface PhotoViewController ()

@end

@implementation PhotoViewController

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
	// Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)takePhoto:(id)sender
{
    UIImagePickerController* imagePicker = [[UIImagePickerController alloc] init];
    [imagePicker setSourceType: UIImagePickerControllerSourceTypeCamera];
    imagePicker.delegate = self;
    [self presentViewController:imagePicker animated:YES completion:nil];
}

- (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage* image = info[UIImagePickerControllerOriginalImage];
    if(image)
    {
        ImageAdapter* adapter = [ImageAdapter new];
        IplImage* cvImage = [adapter convertToIplImage:image];
        
        IplImage* grayImage = cvCreateImage( cvGetSize(cvImage), 8, 1);
        cvCvtColor(cvImage, grayImage, CV_BGR2GRAY);
        cvReleaseImage(&cvImage);
        cvImage = grayImage;
        
        image = [adapter convertToUIImage: cvImage];
        self.imageView.image = image;
    }
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
