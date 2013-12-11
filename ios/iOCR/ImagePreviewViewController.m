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

#import "iOCR/share/Common.h"

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

- (IBAction)set84:(id)sender {
    ImageView* imageView = (ImageView*)self.view;
    [imageView setMode: 84];
}

- (IBAction)set108:(id)sender {
    ImageView* imageView = (ImageView*)self.view;
    [imageView setMode: 108];
}

- (IBAction)processImage:(id)sender {
    ImageView* imageView = (ImageView*)self.view;
    IplImage* image = self.cvImage;
    
    //int imageWidth = image->width;
    int imageHeight = image->height;
    
    CGFloat rate = (CGFloat)imageHeight / 320.0f;

    // init the tesseract engine.
    tesseract::TessBaseAPI* tesseract = new tesseract::TessBaseAPI();
    if(tesseract->Init(NULL, "chi_sim+eng"))
    {
        NSLog(@"Failed");
        return;
    }
    else
    {
        NSLog(@"OK");
    }
    
    tesseract->SetVariable("chop_enable", "T");
    tesseract->SetVariable("use_new_state_cost", "F");
    tesseract->SetVariable("segment_segcost_rating", "F");
    tesseract->SetVariable("enable_new_segsearch", "0");
    tesseract->SetVariable("language_model_ngram_on", "0");
    tesseract->SetVariable("textord_force_make_prop_words", "F");
    tesseract->SetImage((uchar*)image->imageData, image->width, image->height, image->nChannels, image->widthStep);
    
    int gsbhX = imageView.gsbhX * rate;
    int gsbhY = imageView.gsbhY * rate;
    int gsbhW = imageView.gsbhW * rate;
    int gsbhH = imageView.gsbhH * rate;
    tesseract->SetRectangle(gsbhX, gsbhY, gsbhW, gsbhH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    //Boxa* boxes = tesseract->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
    char* gsbhUTF8 = tesseract->GetUTF8Text();
    NSString* gsbhStr = [NSString stringWithUTF8String: gsbhUTF8];
    NSLog(@"购货单位：%@", gsbhStr);
    
    int xsbhX = imageView.xsbhX * rate;
    int xsbhY = imageView.xsbhY * rate;
    int xsbhW = imageView.xsbhW * rate;
    int xsbhH = imageView.xsbhH * rate;
    tesseract->SetRectangle(xsbhX, xsbhY, xsbhW, xsbhH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    //Boxa* boxes = tesseract->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
    char* xsbhUTF8 = tesseract->GetUTF8Text();
    NSString* xsbhStr = [NSString stringWithUTF8String: xsbhUTF8];
    NSLog(@"销货单位：%@", xsbhStr);
    
    int fpdmX = imageView.fpdmX* rate;
    int fpdmY = imageView.fpdmY * rate;
    int fpdmW = imageView.fpdmW * rate;
    int fpdmH = imageView.fpdmH * rate;
    tesseract->SetRectangle(fpdmX, fpdmY, fpdmW, fpdmH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    //Boxa* boxes = tesseract->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
    char* fpdmUTF8 = tesseract->GetUTF8Text();
    NSString* fpdmStr = [NSString stringWithUTF8String: fpdmUTF8];
    NSLog(@"发票代码：%@", fpdmStr);
    
    int fphmX = imageView.fphmX * rate;
    int fphmY = imageView.fphmY * rate;
    int fphmW = imageView.fphmW * rate;
    int fphmH = imageView.fphmH * rate;
    tesseract->SetRectangle(fphmX, fphmY, fphmW, fphmH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    //Boxa* boxes = tesseract->GetComponentImages(tesseract::RIL_WORD, true, NULL, NULL);
    char* fphmUTF8 = tesseract->GetUTF8Text();
    NSString* fphmStr = [NSString stringWithUTF8String: fphmUTF8];
    NSLog(@"发票号码：%@", fphmStr);
    
    
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
