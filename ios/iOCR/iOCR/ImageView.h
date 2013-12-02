//
//  ImageView.h
//  iOCR
//
//  Created by liqilei on 12/2/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ImageView : UIView
@property (strong, nonatomic) UIImage* image;
@property int offsetX;
@property int offsetY;
@end
