//
//  ImageAdapter.m
//  iOCR
//
//  Created by cstlab on 13-9-16.
//  Copyright (c) 2013年 liqilei. All rights reserved.
//

#import "ImageAdapter.h"

@implementation ImageAdapter
#pragma mark -
#pragma mark OpenCV Support Methods

// 把UIImage类型转换成IplImage类型.
// NOTE you SHOULD cvReleaseImage() for the return value when end of the code.
- (IplImage *)convertToIplImage:(UIImage *)image {
    CGImageRef imageRef = image.CGImage;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    IplImage *iplimage = cvCreateImage(cvSize(image.size.width, image.size.height),
                                       IPL_DEPTH_8U, 4);
    CGContextRef contextRef = CGBitmapContextCreate(iplimage->imageData,
                                                    iplimage->width,
                                                    iplimage->height,
                                                    iplimage->depth,
                                                    iplimage->widthStep,
                                                    colorSpace,
                                                    kCGImageAlphaPremultipliedLast |
                                                    kCGBitmapByteOrderDefault);
    CGContextDrawImage(contextRef,
                       CGRectMake(0, 0, image.size.width, image.size.height),
                       imageRef);
    CGContextRelease(contextRef);
    CGColorSpaceRelease(colorSpace);
    
    IplImage *ret = cvCreateImage(cvGetSize(iplimage), IPL_DEPTH_8U, 3);
    cvCvtColor(iplimage, ret, CV_RGB2BGR);
    cvReleaseImage(&iplimage);
    
    return ret;
}

// 把IplImage类型转换成UIImage类型.
// NOTE You should convert color mode as RGB before passing to this function.
- (UIImage *)convertToUIImage:(IplImage *)image {
    NSLog(@"IplImage (%d, %d) %d bits by %d channels, %d bytes/row %s",
          image->width,
          image->height,
          image->depth,
          image->nChannels,
          image->widthStep,
          image->channelSeq);
    
    
    IplImage* targetImage = cvCreateImage(cvGetSize(image), 8, 3);
    if(image->nChannels == 1)
    {
        cvCvtColor(image, targetImage, CV_GRAY2RGB);
    }
    else
    {
        cvCvtColor(image, targetImage, CV_BGR2RGB);
    }
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSData *data = [NSData dataWithBytes:targetImage->imageData length:targetImage->imageSize];
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    CGImageRef imageRef = CGImageCreate(targetImage->width,
                                        targetImage->height,
                                        targetImage->depth,
                                        targetImage->depth * targetImage->nChannels,
                                        targetImage->widthStep,
                                        colorSpace,
                                        kCGImageAlphaNone |
                                        kCGBitmapByteOrderDefault,
                                        provider,
                                        NULL,
                                        false,
                                        kCGRenderingIntentDefault);
    
    UIImage *ret = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    cvReleaseImage(&targetImage);
    
    return ret;
}
@end
