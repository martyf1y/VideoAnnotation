//
//  ofxAVFVideoRenderer.m
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//

#import "ofxAVFVideoRenderer.h"

@implementation AVFVideoRenderer

@synthesize player, playerItem, playerLayer, layerRenderer;
@synthesize loop, movieIsDone, myRate;
@synthesize frameRate = _frameRate;
@synthesize bPaused = _bPaused;
@synthesize playbackRate = _playbackRate;


- (void) loadFile:(NSString *)filename {
    loading = YES;
    ready = NO;
    deallocWhenReady = NO;
    _bPaused = NO;
    _playbackRate = 1.0;

    loop = NO;
    movieIsDone = NO;
    myRate = 0.0;
    _frameRate = 0.0;
    //NSURL *fileURL = [NSURL URLWithString:filename];
    NSURL *fileURL = [NSURL fileURLWithPath:[filename stringByStandardizingPath]];
    
    //NSLog(@"Trying to load %@", filename);

    AVURLAsset *asset = [AVURLAsset URLAssetWithURL:fileURL options:nil];
    NSString *tracksKey = @"tracks";
    
    [asset loadValuesAsynchronouslyForKeys:@[tracksKey] completionHandler: ^{
        static const NSString *ItemStatusContext;
        // Perform the following back on the main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
            // Check to see if the file loaded
            NSError *error;
            AVKeyValueStatus status = [asset statusOfValueForKey:tracksKey error:&error];
            
            
            if(status == AVKeyValueStatusLoaded) {
                // Asset metadata has been loaded. Set up the player
                
                // Extract the video track to get the video size
                AVAssetTrack *videoTrack = [[asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0];
                videoSize = [videoTrack naturalSize];
                videoDuration = asset.duration;
                _currentTime = kCMTimeZero;
                _duration = asset.duration;
                _frameRate = [videoTrack nominalFrameRate];

                self.playerItem = [AVPlayerItem playerItemWithAsset:asset];
                [self.playerItem addObserver:self forKeyPath:@"status" options:0 context:&ItemStatusContext];
                
                // Notify this object when the player reaches the end
                // This allows us to loop the video
                [[NSNotificationCenter defaultCenter] addObserver:self
                                                         selector:@selector(playerItemDidReachEnd:)
                                                             name:AVPlayerItemDidPlayToEndTimeNotification
                                                           object:self.playerItem];
                
                self.player = [AVPlayer playerWithPlayerItem:self.playerItem];
                
                self.playerLayer = [AVPlayerLayer playerLayerWithPlayer:player];
                
                self.layerRenderer = [CARenderer rendererWithCGLContext:CGLGetCurrentContext() options:nil];
                self.layerRenderer.layer = playerLayer;
                
                // this should be the right position 10.7 - 10.9
                self.layerRenderer.bounds = CGRectMake(0, 0, videoSize.width, videoSize.height);
                [self.layerRenderer.layer setAnchorPoint:CGPointMake(0.0, 0.0)];
                
                self.playerLayer.bounds = self.layerRenderer.bounds;
            
                ready = YES;
                loading = NO;
            }
            else {
                loading = NO;
                ready = NO;
                NSLog(@"There was an error loading the file:\n%@", error);
            }
            
            // If dealloc is called immediately after loadFile, we have to defer releasing properties
            if(deallocWhenReady) [self dealloc];
            [pool release];
        });
    }];
}

- (void) dealloc {
    if(loading) {
        deallocWhenReady = YES;
    }
    else {
        [self stop];

        // SK: Releasing the CARenderer is slow for some reason
        //     It will freeze the main thread for a few dozen mS.
        //     If you're swapping in and out videos a lot, the loadFile:
        //     method should be re-written to just re-use and re-size
        //     these layers/objects rather than releasing and reallocating
        //     them every time a new file is needed.
        
        if(self.layerRenderer) [self.layerRenderer release];
    
        [[NSNotificationCenter defaultCenter] removeObserver:self];
        if(self.playerItem) [self.playerItem removeObserver:self forKeyPath:@"status"];
        
        if(self.player) [self.player release];
        if(self.playerItem) [self.playerItem release];
        if(self.playerLayer) [self.playerLayer release];
        if(!deallocWhenReady) [super dealloc];
    }
}

- (BOOL) isLoading { return loading; }
- (BOOL) isReady { return ready; }

- (CGSize) getVideoSize {
    return videoSize;
}

- (CMTime) getVideoDuration {
    return videoDuration;
}

- (void) play {
    [self.player play];
    self.player.rate = _playbackRate;

}

- (void) stop {
    [self.player pause];
    [self.player seekToTime:kCMTimeZero];
}

//--------------------------------------------------------------
- (void)setPaused:(BOOL)bPaused
{
    _bPaused = bPaused;
    if (_bPaused) {
        [self.player pause];
    }
    else {
        [self.player play];
        self.player.rate = _playbackRate;
    }
}

- (void) observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context {
    
}

- (void) playerItemDidReachEnd:(NSNotification *) notification {
    
    if(loop) {
        if(myRate < 0) [self.player seekToTime:videoDuration];
        else [self.player seekToTime:kCMTimeZero];
        if(myRate!=0) [self.player setRate:myRate];
        else [self.player play];
    } else {
        
        movieIsDone = YES;
    }
}

- (void) render {
    // From https://qt.gitorious.org/qt/qtmultimedia/blobs/700b4cdf42335ad02ff308cddbfc37b8d49a1e71/src/plugins/avfoundation/mediaplayer/avfvideoframerenderer.mm
    
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, videoSize.width, videoSize.height);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0f, videoSize.width, videoSize.height, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // old 10.8 hack
    //glTranslatef(videoSize.width/2,videoSize.height/2,0);
    //glTranslatef(videoSize.width,videoSize.height,0);
    
    [layerRenderer beginFrameAtTime:CACurrentMediaTime() timeStamp:NULL];
    [layerRenderer addUpdateRect:layerRenderer.layer.bounds];
    [layerRenderer render];
    [layerRenderer endFrame];

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glPopAttrib();
    
    glFinish(); //Rendering needs to be done before passing texture to video frame
}

//--------------------------------------------------------------
- (double)duration
{
    return CMTimeGetSeconds(_duration);
}

//--------------------------------------------------------------
- (int)totalFrames
{
    return self.duration * self.frameRate;
}

//--------------------------------------------------------------
- (double)currentTime
{
  //  if (self.theFutureIsNow) {
    //    return CMTimeGetSeconds(_currentTime);
    //}
    
    return CMTimeGetSeconds(self.player.currentTime);
}

//--------------------------------------------------------------
- (void)setCurrentTime:(double)currentTime
{
    [player seekToTime:CMTimeMakeWithSeconds(currentTime, _duration.timescale)];
}

//--------------------------------------------------------------
- (int)currentFrame
{
    return self.currentTime * self.frameRate;
}

//--------------------------------------------------------------
- (void)setCurrentFrame:(int)currentFrame
{
    float position = currentFrame / (float)self.totalFrames;
    [self setPosition:position];
}

//--------------------------------------------------------------
- (void)setPosition:(double)position
{
    double time = self.duration * position;
    //    [self.player seekToTime:CMTimeMakeWithSeconds(time, NSEC_PER_SEC)];
    [player seekToTime:CMTimeMakeWithSeconds(time, _duration.timescale)];
}

//--------------------------------------------------------------
- (void)setPlaybackRate:(double)playbackRate
{
    _playbackRate = playbackRate;
    [player setRate:_playbackRate];
}

//--------------------------------------------------------------
- (float)volume
{
    return self.player.volume;
}

//--------------------------------------------------------------
- (void)setVolume:(float)volume
{
    self.player.volume = volume;
}
@end
