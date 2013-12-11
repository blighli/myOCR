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
#import "iOCR/share/AppInfo.h"

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
    gsbhStr = [gsbhStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    gsbhStr = [gsbhStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    NSLog(@"购货单位：%@", gsbhStr);
    
    int xsbhX = imageView.xsbhX * rate;
    int xsbhY = imageView.xsbhY * rate;
    int xsbhW = imageView.xsbhW * rate;
    int xsbhH = imageView.xsbhH * rate;
    tesseract->SetRectangle(xsbhX, xsbhY, xsbhW, xsbhH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    char* xsbhUTF8 = tesseract->GetUTF8Text();
    NSString* xsbhStr = [NSString stringWithUTF8String: xsbhUTF8];
    xsbhStr = [xsbhStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    xsbhStr = [xsbhStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    NSLog(@"销货单位：%@", xsbhStr);
    
    int fpdmX = imageView.fpdmX* rate;
    int fpdmY = imageView.fpdmY * rate;
    int fpdmW = imageView.fpdmW * rate;
    int fpdmH = imageView.fpdmH * rate;
    tesseract->SetRectangle(fpdmX, fpdmY, fpdmW, fpdmH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    char* fpdmUTF8 = tesseract->GetUTF8Text();
    NSString* fpdmStr = [NSString stringWithUTF8String: fpdmUTF8];
    fpdmStr = [fpdmStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    fpdmStr = [fpdmStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    NSLog(@"发票代码：%@", fpdmStr);
    
    int fphmX = imageView.fphmX * rate;
    int fphmY = imageView.fphmY * rate;
    int fphmW = imageView.fphmW * rate;
    int fphmH = imageView.fphmH * rate;
    tesseract->SetRectangle(fphmX, fphmY, fphmW, fphmH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789");
    char* fphmUTF8 = tesseract->GetUTF8Text();
    NSString* fphmStr = [NSString stringWithUTF8String: fphmUTF8];
    fphmStr = [fphmStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    fphmStr = [fphmStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    NSLog(@"发票号码：%@", fphmStr);
    
    int kprqX = imageView.kprqX * rate;
    int kprqY = imageView.kprqY * rate;
    int kprqW = imageView.kprqW * rate;
    int kprqH = imageView.kprqH * rate;
    tesseract->SetRectangle(kprqX, kprqY, kprqW, kprqH);
    NSString* whiteChars = @"年月日0123456789";
    tesseract->SetVariable("tessedit_char_whitelist", [whiteChars UTF8String]);
    char* kprqUTF8 = tesseract->GetUTF8Text();
    NSString* kprqStr = [NSString stringWithUTF8String: kprqUTF8];
    NSLog(@"开票日期：%@", kprqStr);
    
    int jeX = imageView.jeX * rate;
    int jeY = imageView.jeY * rate;
    int jeW = imageView.jeW * rate;
    int jeH = imageView.jeH * rate;
    tesseract->SetRectangle(jeX, jeY, jeW, jeH);
    whiteChars = @"￥0123456789.";
    tesseract->SetVariable("tessedit_char_whitelist", [whiteChars UTF8String]);
    char* jeUTF8 = tesseract->GetUTF8Text();
    NSString* jeStr = [NSString stringWithUTF8String: jeUTF8];
    jeStr = [jeStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    jeStr = [jeStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    jeStr = [jeStr stringByReplacingOccurrencesOfString:@"￥" withString:@""];
    NSLog(@"金额：%@", jeStr);
    
    int seX = imageView.seX * rate;
    int seY = imageView.seY * rate;
    int seW = imageView.seW * rate;
    int seH = imageView.seH * rate;
    tesseract->SetRectangle(seX, seY, seW, seH);
    whiteChars = @"¥0123456789.";
    tesseract->SetVariable("tessedit_char_whitelist", [whiteChars UTF8String]);
    char* seUTF8 = tesseract->GetUTF8Text();
    NSString* seStr = [NSString stringWithUTF8String: seUTF8];
    seStr = [seStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    seStr = [seStr stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    seStr = [seStr stringByReplacingOccurrencesOfString:@"￥" withString:@""];
    NSLog(@"税额：%@", seStr);
    
    int mmX = imageView.mmX * rate;
    int mmY = imageView.mmY * rate;
    int mmW = imageView.mmW * rate;
    int mmH = imageView.mmH * rate;
    tesseract->SetRectangle(mmX, mmY, mmW, mmH);
    tesseract->SetVariable("tessedit_char_whitelist", "0123456789+-*/<>");
    char* mmUTF8 = tesseract->GetUTF8Text();
    NSString* mmStr = [NSString stringWithUTF8String: mmUTF8];
    mmStr = [mmStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    NSLog(@"密码：%@", mmStr);
    
    
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
