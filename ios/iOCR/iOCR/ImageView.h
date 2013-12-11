//
//  ImageView.h
//  iOCR
//
//  Created by liqilei on 12/2/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ImageView : UIView
{
    int _mode;
}

@property (strong, nonatomic) UIImage* image;
@property CGFloat offsetX;
@property CGFloat offsetY;

@property (nonatomic) CGPoint firstTouch;
@property (nonatomic) CGPoint lastTouch;

- (void) setMode: (int) mode;


@property (nonatomic) CGFloat gsbhX;
@property (nonatomic) CGFloat gsbhY;
@property (nonatomic) CGFloat gsbhW;
@property (nonatomic) CGFloat gsbhH;

@property (nonatomic) CGFloat xsbhX;
@property (nonatomic) CGFloat xsbhY;
@property (nonatomic) CGFloat xsbhW;
@property (nonatomic) CGFloat xsbhH;

@property (nonatomic) CGFloat kprqX;
@property (nonatomic) CGFloat kprqY;
@property (nonatomic) CGFloat kprqW;
@property (nonatomic) CGFloat kprqH;

@property (nonatomic) CGFloat jeX;
@property (nonatomic) CGFloat jeY;
@property (nonatomic) CGFloat jeW;
@property (nonatomic) CGFloat jeH;

@property (nonatomic) CGFloat seX;
@property (nonatomic) CGFloat seY;
@property (nonatomic) CGFloat seW;
@property (nonatomic) CGFloat seH;

@property (nonatomic) CGFloat mmX;
@property (nonatomic) CGFloat mmY;
@property (nonatomic) CGFloat mmW;
@property (nonatomic) CGFloat mmH;

@property (nonatomic) CGFloat fpdmX;
@property (nonatomic) CGFloat fpdmY;
@property (nonatomic) CGFloat fpdmW;
@property (nonatomic) CGFloat fpdmH;

@property (nonatomic) CGFloat fphmX;
@property (nonatomic) CGFloat fphmY;
@property (nonatomic) CGFloat fphmW;
@property (nonatomic) CGFloat fphmH;

@end
