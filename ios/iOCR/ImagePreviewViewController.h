//
//  ImagePreviewViewController.h
//  iOCR
//
//  Created by liqilei on 11/18/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <opencv2/opencv.hpp>
#import "ResultViewController.h"

@interface ImagePreviewViewController : UIViewController
@property IplImage* cvImage;
@property (nonatomic) ResultViewController* resultViewController;
@end
