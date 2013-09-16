//
//  ImageAdapter.h
//  iOCR
//
//  Created by cstlab on 13-9-16.
//  Copyright (c) 2013年 liqilei. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <opencv2/opencv.hpp>

@interface ImageAdapter : NSObject

- (IplImage *)convertToIplImage:(UIImage *)image;

- (UIImage *)convertToUIImage:(IplImage *)image;

@end
