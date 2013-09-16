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
#import "share/ImageProcess.h"

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
        
        
        ImageProcess* imageProcess = new ImageProcess();
        imageProcess->setImage(cvImage);
        
        
        ImageProcessParam param;
        param.debug = true;
        param.gray = true;
        
        param.threshold = true;
        param.thresholdBlockSize = 100;
        param.thresholdMeanBias = 1;
        
        param.hough = true;
        param.houghRho = 1;
        param.houghTheta = 1;
        param.houghThreshold = 50;
        param.houghParam1 = 50;
        param.houghParam2 = 1;
        
        param.corner = true;
        param.cornerDegree = 80;
        param.cornerGap = 50;
        
        param.background = false;
        
        param.normalize = false;
        param.normalizeTopMargin = 300;
        param.normalizeSideMargin = 100;
        param.normalizeWidth = 2000;
        param.normalizeHeight = 940;
        
        param.blueText =  false;
        param.blueTextMin = 100;
        param.blueTextMax = 1;
        
        //mImageProcess->setMasks(mOCRMasks);
        
        imageProcess->run(&param);

        
        image = [adapter convertToUIImage: imageProcess->getProcessedImage()];
        delete imageProcess;
        
        self.imageView.image = image;
    }
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
