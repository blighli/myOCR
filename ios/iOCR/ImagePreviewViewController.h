//
//  ImagePreviewViewController.h
//  iOCR
//
//  Created by liqilei on 11/18/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <opencv2/opencv.hpp>

@interface ImagePreviewViewController : UIViewController
@property IplImage* cvImage;
@end
