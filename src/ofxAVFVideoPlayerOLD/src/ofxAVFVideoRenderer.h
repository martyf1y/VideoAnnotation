

//
//  ofxAVFVideoRenderer.h
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//


#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import <AVFoundation/AVFoundation.h>
#import <OpenGL/OpenGL.h>

@interface AVFVideoRenderer : NSObject
{
    AVPlayer *player;
    AVPlayerItem *playerItem;
    AVPlayerLayer *playerLayer;
    
    CARenderer *layerRenderer;
    
    CGSize videoSize;
    CMTime videoDuration;
    CMTime _currentTime;
    CMTime _duration;

    double _frameRate;
    BOOL loading;
    BOOL ready;
    BOOL deallocWhenReady;
    
    BOOL loop;
    BOOL movieIsDone;
    BOOL _bPaused;
    double _playbackRate;


    float myRate;
}
    @property (nonatomic, retain) AVPlayer *player;
    @property (nonatomic, retain) AVPlayerItem *playerItem;
    @property (nonatomic, retain) AVPlayerLayer *playerLayer;
    @property (nonatomic, retain) CARenderer *layerRenderer;
    @property (nonatomic, assign) BOOL loop;
    @property (nonatomic, assign, getter = isPaused, setter = setPaused:) BOOL bPaused;
    @property (nonatomic, assign) BOOL movieIsDone;
    @property (nonatomic, assign) float myRate;
    @property (nonatomic, assign, readonly) int totalFrames;
    @property (nonatomic, assign) double currentTime;
    @property (nonatomic, assign, readonly) double frameRate;
    @property (nonatomic, assign, readonly) double duration;
    @property (nonatomic, assign) int currentFrame;
    @property (nonatomic, assign) double playbackRate;
    @property (nonatomic, assign) float volume;


- (void) loadFile:(NSString *)filename;
- (void) play;
- (void) stop;
- (void) playerItemDidReachEnd:(NSNotification *) notification;
//- (void) update;
- (BOOL) isReady;
- (BOOL) isLoading;
- (void) render;

- (CGSize) getVideoSize;
- (CMTime) getVideoDuration;

@end
