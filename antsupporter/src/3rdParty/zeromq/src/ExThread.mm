//
//  ExThread.mm
//
//  Created by d w 10/9/13.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "pthread.h"
#import "ExThread.h"

void setCurrentThreadName(char *threadname)
{
    @autoreleasepool {
        NSString *tn = [[NSString alloc] initWithUTF8String:threadname];
        [[NSThread currentThread] setName:tn];
        [tn release];
    }
}

