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
    
    CGContextSetLineWidth(context, 1.0);
    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);
    
    CGFloat leftMost = 10.0;
    CGFloat topMost = 100.0;
    
    CGFloat scale = 2.0;
    
    CGFloat width = 203.0 * scale;
    CGFloat height = 95.0 * scale;
    
    CGFloat rightMost = leftMost + width;
    CGFloat bottomMost = topMost + height;
    
    CGFloat hLine0 = topMost;
    CGFloat hLine1 = topMost + 22.0 * scale;
    CGFloat hLine2 = topMost + 65.0 * scale;
    CGFloat hLine3 = topMost + 75.0 * scale;
    CGFloat hLine4 = topMost + height;
    
    CGFloat redWidth = 30 * scale;
    CGFloat redHeight = 20 * scale;
    CGFloat redX = leftMost + 101 * scale - redWidth * 0.5;
    CGFloat redY = topMost - 2 * scale - redHeight;
    //Draw red stamp
    CGContextAddEllipseInRect(context, CGRectMake(redX, redY, redWidth, redHeight));
    
    //Draw 5 horizonal lines
    CGContextMoveToPoint(context, leftMost, hLine0);
    CGContextAddLineToPoint(context, rightMost, hLine0);
    
    CGContextMoveToPoint(context, leftMost, hLine1);
    CGContextAddLineToPoint(context, rightMost, hLine1);
    
    CGContextMoveToPoint(context, leftMost, hLine2);
    CGContextAddLineToPoint(context, rightMost, hLine2);
    
    CGContextMoveToPoint(context, leftMost, hLine3);
    CGContextAddLineToPoint(context, rightMost, hLine3);
    
    CGContextMoveToPoint(context, leftMost, hLine4);
    CGContextAddLineToPoint(context, rightMost, hLine4);
    
    //Draw vertical lines
    CGContextMoveToPoint(context, leftMost, topMost);
    CGContextAddLineToPoint(context, leftMost, bottomMost);
    
    CGContextMoveToPoint(context, rightMost, topMost);
    CGContextAddLineToPoint(context, rightMost, bottomMost);
    
    //1st row
    CGFloat vLine11 = leftMost + 8.0 * scale;
    CGFloat vLine12 = leftMost + 116.0 * scale;
    CGFloat vLine13 = leftMost + 121.0 * scale;
    
    CGContextMoveToPoint(context, vLine11, hLine0);
    CGContextAddLineToPoint(context, vLine11, hLine1);
    
    CGContextMoveToPoint(context, vLine12, hLine0);
    CGContextAddLineToPoint(context, vLine12, hLine1);
    
    CGContextMoveToPoint(context, vLine13, hLine0);
    CGContextAddLineToPoint(context, vLine13, hLine1);
    
    //2nd row
    CGFloat vLine21 = leftMost + 53.0 * scale;
    CGFloat vLine22 = leftMost + 78.0 * scale;
    CGFloat vLine23 = leftMost + 91.0 * scale;
    CGFloat vLine24 = leftMost + 111.0 * scale;
    CGFloat vLine25 = leftMost + 131.0 * scale;
    CGFloat vLine26 = leftMost + 162.0 * scale;
    CGFloat vLine27 = leftMost + 173.0 * scale;
    
    CGContextMoveToPoint(context, vLine21, hLine1);
    CGContextAddLineToPoint(context, vLine21, hLine3); //special
    
    CGContextMoveToPoint(context, vLine22, hLine1);
    CGContextAddLineToPoint(context, vLine22, hLine2);
    
    CGContextMoveToPoint(context, vLine23, hLine1);
    CGContextAddLineToPoint(context, vLine23, hLine2);
    
    CGContextMoveToPoint(context, vLine24, hLine1);
    CGContextAddLineToPoint(context, vLine24, hLine2);
    
    CGContextMoveToPoint(context, vLine25, hLine1);
    CGContextAddLineToPoint(context, vLine25, hLine2);
    
    CGContextMoveToPoint(context, vLine26, hLine1);
    CGContextAddLineToPoint(context, vLine26, hLine2);
    
    CGContextMoveToPoint(context, vLine27, hLine1);
    CGContextAddLineToPoint(context, vLine27, hLine2);

    
    //3rd row
    CGFloat vLine31 = leftMost + 8.0 * scale;
    CGFloat vLine32 = leftMost + 116.0 * scale;
    CGFloat vLine33 = leftMost + 121.0 * scale;
    
    CGContextMoveToPoint(context, vLine31, hLine3);
    CGContextAddLineToPoint(context, vLine31, hLine4);
    
    CGContextMoveToPoint(context, vLine32, hLine3);
    CGContextAddLineToPoint(context, vLine32, hLine4);
    
    CGContextMoveToPoint(context, vLine33, hLine3);
    CGContextAddLineToPoint(context, vLine33, hLine4);
    
    CGContextStrokePath(context);
    
}

@end
