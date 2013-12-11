//
//  ResultViewController.m
//  iOCR
//
//  Created by liqilei on 12/12/13.
//  Copyright (c) 2013 liqilei. All rights reserved.
//

#import "ResultViewController.h"

@interface ResultViewController ()

@end

@implementation ResultViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.fpdm setDelegate: self];
    [self.fphm setDelegate: self];
    [self.gsbh setDelegate: self];
    [self.xsbh setDelegate: self];
    [self.year setDelegate: self];
    [self.month setDelegate: self];
    [self.day setDelegate: self];
    [self.jine setDelegate: self];
    [self.shuie setDelegate: self];
    [self.mm1 setDelegate: self];
    [self.mm2 setDelegate: self];
    [self.mm3 setDelegate: self];
    [self.mm4 setDelegate: self];
    
    NSLog(@"setDelegate selft");

    // Do any additional setup after loading the view from its nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)connection:(NSURLConnection *)connection canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace {
    return [protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust];
}

- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge {
    if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust])
        [challenge.sender useCredential:[NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust] forAuthenticationChallenge:challenge];
    
    [challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge];
}

- (IBAction)testOnline:(id)sender {
    
    NSString *urlString = @"https://web1.nb-n-tax.gov.cn:7006/wlrz/fpwlrz.do";
    
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
    [request setURL:[NSURL URLWithString:urlString]];
    [request setHTTPMethod:@"POST"];
    
    NSString *contentType = [NSString stringWithFormat:@"text/xml"];
    [request addValue:contentType forHTTPHeaderField: @"Content-Type"];
    
    NSMutableData *postBody = [NSMutableData data];
    [postBody appendData:[[NSString stringWithFormat:
                           @"nsrsbh=%@&czydm=%@&passwd=%@"
                           "&fpdm=%@&fphm=%@&kpyear=%@&kpmonth=%@&kpday=%@"
                           "&gsbh=%@&miwen1=%@&miwen2=%@&miwen3=%@&miwen4=%@"
                           "&jine=%@&shuie=%@&xsbh=%@&fpid=%@&action=%@",
                           self.gsbh.text,@"001",@"001",
                           self.fpdm.text,self.fphm.text,self.year.text,self.month.text,self.day.text,
                           self.gsbh.text,self.mm1,self.mm2,self.mm3,self.mm4,
                           self.jine.text,self.shuie.text,self.xsbh,@"001",@"wlrz"]
                          dataUsingEncoding: NSUTF8StringEncoding]];
    
    [request setHTTPBody:postBody];
    
    NSURLConnection* conn = [[NSURLConnection alloc] initWithRequest: request delegate:self];
    [conn start];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSStringEncoding gbkEncoding = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
    NSString *result = [[NSString alloc] initWithData:data encoding: gbkEncoding];
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"认证结果" message:result delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
}

- (IBAction)goBack:(id)sender {
    [self dismissViewControllerAnimated:NO completion:nil];
}
- (IBAction)backgroundTap:(id)sender {
    [self.fpdm resignFirstResponder];
    [self.fphm resignFirstResponder];
    [self.gsbh resignFirstResponder];
    [self.xsbh resignFirstResponder];
    [self.year resignFirstResponder];
    [self.month resignFirstResponder];
    [self.day resignFirstResponder];
    [self.jine resignFirstResponder];
    [self.shuie resignFirstResponder];
    [self.mm1 resignFirstResponder];
    [self.mm2 resignFirstResponder];
    [self.mm3 resignFirstResponder];
    [self.mm4 resignFirstResponder];
    
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}

//开始编辑输入框的时候，软键盘出现，执行此事件
-(void)textFieldDidBeginEditing:(UITextField *)textField
{
    NSLog(@"textFieldDidBeginEditing");
    
    CGRect frame = textField.frame;
    int offset = frame.origin.y + 32 - (self.view.frame.size.height - 216.0);//键盘高度216
    
    NSTimeInterval animationDuration = 0.30f;
    [UIView beginAnimations:@"ResizeForKeyboard" context:nil];
    [UIView setAnimationDuration:animationDuration];
    
    //将视图的Y坐标向上移动offset个单位，以使下面腾出地方用于软键盘的显示
    if(offset > 0)
        self.view.frame = CGRectMake(0.0f, -offset, self.view.frame.size.width, self.view.frame.size.height);
    
    [UIView commitAnimations];
}

//当用户按下return键或者按回车键，keyboard消失
-(BOOL)textFieldShouldReturn:(UITextField *)textField
{
    NSLog(@"textFieldShouldReturn");
    
    [textField resignFirstResponder];
    return YES;
}

//输入框编辑完成以后，将视图恢复到原始状态
-(void)textFieldDidEndEditing:(UITextField *)textField
{
    
    NSLog(@"textFieldDidEndEditing");

    self.view.frame =CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
}

- (IBAction)doneEdit:(id)sender {
    [sender resignFirstResponder];
}
@end
