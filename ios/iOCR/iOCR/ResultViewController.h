//
//  ResultViewController.h
//  iOCR
//
//  Created by liqilei on 12/12/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ResultViewController : UIViewController <UITextFieldDelegate, NSURLConnectionDelegate, NSURLConnectionDataDelegate>
@property (weak, nonatomic) IBOutlet UITextField *fpdm;
@property (weak, nonatomic) IBOutlet UITextField *fphm;
@property (weak, nonatomic) IBOutlet UITextField *gsbh;
@property (weak, nonatomic) IBOutlet UITextField *xsbh;
@property (weak, nonatomic) IBOutlet UITextField *year;
@property (weak, nonatomic) IBOutlet UITextField *month;
@property (weak, nonatomic) IBOutlet UITextField *day;
@property (weak, nonatomic) IBOutlet UITextField *jine;
@property (weak, nonatomic) IBOutlet UITextField *shuie;
@property (weak, nonatomic) IBOutlet UITextField *mm1;
@property (weak, nonatomic) IBOutlet UITextField *mm2;
@property (weak, nonatomic) IBOutlet UITextField *mm3;
@property (weak, nonatomic) IBOutlet UITextField *mm4;
- (IBAction)testOnline:(id)sender;
- (IBAction)goBack:(id)sender;
- (IBAction)doneEdit:(id)sender;

@end
