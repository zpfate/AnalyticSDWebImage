//
//  AppDelegate.m
//  AnalyticSDWebImage
//
//  Created by Twisted Fate on 2019/7/31.
//  Copyright © 2019 Twisted Fate. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@property (nonatomic, assign) UIBackgroundTaskIdentifier backTaskId;

@property (nonatomic, strong) NSTimer *timer;

@end

@implementation AppDelegate

static int count = 0;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    

    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
    
    NSLog(@"%s", __func__);
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    NSLog(@"%s", __func__);
    self.timer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(backAction) userInfo:nil repeats:YES];
    
    
    // 申请后台权限
    __block UIBackgroundTaskIdentifier backTaskId;
    backTaskId = [application beginBackgroundTaskWithExpirationHandler:^{
        NSLog(@"backTask reaches 0");
        [application endBackgroundTask:backTaskId];
        backTaskId = UIBackgroundTaskInvalid;
    }];
}

- (void)backAction {
    
    count++;
    
    NSLog(@"count ====== %d", count);
    
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
    NSLog(@"%s", __func__);

//    [application endBackgroundTask:self.backTaskId];
    
    [self.timer invalidate];
    count = 0;
    
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    NSLog(@"%s", __func__);
 
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    NSLog(@"%s", __func__);

}


@end
