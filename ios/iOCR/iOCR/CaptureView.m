//
//  CaptureView.m
//  iOCR
//
//  Created by liqilei on 11/22/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import "CaptureView.h"

@implementation CaptureView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    
    NSLog(@"drawRect");
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetLineWidth(context, 2.0);
    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);
    
    CGRect theRect = CGRectMake(15,60,400,200);
    CGContextAddRect(context, theRect);
    CGContextDrawPath(context, kCGPathStroke);
    
}

@end
