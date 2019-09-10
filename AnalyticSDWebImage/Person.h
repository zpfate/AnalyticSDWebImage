//
//  Person.h
//  AnalyticSDWebImage
//
//  Created by Twisted Fate on 2019/8/9.
//  Copyright © 2019 Twisted Fate. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface Person : NSObject

@property (nonatomic, copy) void(^runBlock)(NSString *name);

@end

NS_ASSUME_NONNULL_END
