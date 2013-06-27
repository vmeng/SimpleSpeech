//  ATTSpeechKit.h
//  AT&T Speech SDK
//
// Licensed by AT&T under 'Software Development Kit Tools Agreement' 2012.
// TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION: http://developer.att.com/sdk_agreement/
// Copyright 2012 AT&T Intellectual Property. All rights reserved.
// For more information contact developer.support@att.com http://developer.att.com

#import <Foundation/NSObject.h>
@class NSString, NSDictionary, NSData;
#import <Foundation/NSDate.h>


#define ATTSKVersionString  @"2.0.1"
#define ATTSKBuildString    @"2A015"


/**
 The following constants represent the ATTSpeechService states that occur during 
 speech interaction.
 An application can test the current stage of a speech interaction by accessing
 the currentState property of the ATTSpeechService instance. The delegate can 
 also be notified automatically about the transitions among the stages by 
 implementing the speechService:willEnterState: method.
**/
typedef enum ATTSpeechServiceState {
    /** Nothing occurring. ATTSpeechService is in this state before beginning a 
     speech interaction and after delegate callbacks have returned. **/
    ATTSpeechServiceStateIdle = 0,
    /** Audio capture is taking place. **/
    ATTSpeechServiceStateRecording = 1,
    /** The server is processing audio data. **/
    ATTSpeechServiceStateProcessing = 2,
    /** Attempting to connect to the host before audio capture begins. **/
    ATTSpeechServiceStateConnecting = 3,
    /** Audio data are being sent to the server. **/
    ATTSpeechServiceStateSendingAudioData = 4,
    /** ATTSpeechService is handling an error condition. **/
    ATTSpeechServiceStateError = 6
} ATTSpeechServiceState;

/*
 The Speech SDK reports errors from a variety of sources, such as network errors, 
 HTTP error results, and errors reported by the operating system. It follows the 
 standard practices for NSError objects, so that each source of error 
 corresponds to a value of the NSError.domain property, and each distinct error 
 from a source corresponds to values of the NSError.code property.
*/

/** When a client-side error occurs, NSError.domain will match this value.  
 The NSError.code will be one of the ATTSpeechServiceErrorCode values.  **/
extern NSString* const ATTSpeechServiceErrorDomain;

/** These are NSError.code values for client-side errors when the NSError.domain 
 is ATTSpeechServiceErrorDomain. **/
typedef enum ATTSpeechServiceErrorCode {
    /** The application called [ATTSpeechService startListening] or 
     [ATTSpeechService startWithAudioData:] while a speech interaction was 
     already in process. **/
    ATTSpeechServiceErrorCodeAttemptAtReentrancy = 1,
    /** One of the properties on the ATTSpeechService object was invalid. **/
    ATTSpeechServiceErrorCodeInvalidParameter = 20,
    /** The URL that was passed to the recognitionURL property is invalid. **/
    ATTSpeechServiceErrorCodeInvalidURL = 30,
    /** The Speech SDK was unable to make a network connection to the Speech API 
     service. **/
    ATTSpeechServiceErrorCodeConnectionFailure = 50,
    /** The server did not send a full response because it timed out, or the 
     network connection was dropped. **/
    ATTSpeechServiceErrorCodeNoResponseFromServer = 51,
    /** [ATTSpeechService stopListening] was called before enough audio input 
     was collected.  Returned only when the minRecordingTime property is nonzero. **/
    ATTSpeechServiceErrorCodeAudioTooShort = 60,
    /** The user didn’t speak loud enough for the speech to be recognized.  
     Returned only when the cancelWhenSilent property is true. **/
    ATTSpeechServiceErrorCodeNoAudio = 61,
    /** The device doesn’t have a microphone. **/
    ATTSpeechServiceErrorCodeNoMicrophone = 70,
    /** The user canceled the speech interaction. This occurs, for instance, 
     when the user presses the Cancel button or puts the application in the 
     background.  **/
    ATTSpeechServiceErrorCodeCanceledByUser = 80
} ATTSpeechServiceErrorCode;


/** When a server-side error occurs, NSError.domain will match this value.  
 The NSError.code will be the HTTP status code (e.g. 404, 500, etc.).  **/
extern NSString* const ATTSpeechServiceHTTPErrorDomain;

/** Represents an infinite time duration for properties such as 
 ATTSpeechService.maxRecordingTime. **/
extern const float ATTSpeechServiceInfiniteInterval;


/** Value for ATTSpeechService.audioFormat to specify AMR narrowband 
 (8kHz sampling) format, which has a bitrate of 12.2kbps. **/
extern NSString* const ATTSKAudioFormatAMR_NB;

/** Value for ATTSpeechService.audioFormat to specify Speex wideband (16kHz 
 sampling) format, which has a variable bitrate averaging around 14kbps. **/
extern NSString* const ATTSKAudioFormatSpeex_WB;

/** Value for ATTSpeechService.audioFormat to specify uncompressed streaming
 WAV narrowband (8kHz sampling) format, which has a bitrate of 128kbps. **/
extern NSString* const ATTSKAudioFormatWAV_NB;
/** Value for ATTSpeechService.audioFormat to specify uncompressed streaming 
 WAV wideband (16kHz sampling) format, which has a bitrate of 256kbps. **/
extern NSString* const ATTSKAudioFormatWAV_WB;

#pragma mark



@protocol ATTSpeechServiceDelegate; // forward declaration


/**
 Singleton object that manages the device’s microphone input and network 
 connection to the Speech API on behalf of your application. It exposes a number 
 of properties that you can use to specify the parameters of a speech 
 recognition request.
**/
@interface ATTSpeechService : NSObject {
}

/** Returns the singleton speech service object. **/
+ (ATTSpeechService*) sharedSpeechService;

#pragma mark Interaction Status

/**
 An application-defined object that receives callbacks during speech recognition.
 Applications are required to set this property.
**/
@property (nonatomic, retain) id<ATTSpeechServiceDelegate> delegate;

/**
 Returns the current state of a speech interaction.
**/
@property (readonly, nonatomic) ATTSpeechServiceState currentState;

#pragma mark Request Properties

/**
 The URL of the Speech API service. 
 Applications are required to set this property.
**/
@property (nonatomic, retain) NSURL* recognitionURL;

/**
 Whether to wrap the audio data in a MIME multipart container.  
 Defaults to NO.
 When true, Speech SDK will post data in multipart format, and the delegate must 
 implement the speechServiceSendingPartsBeforeAudio: and/or 
 speechServiceSendingPartsAfterAudio: callbacks. 
**/
@property (assign, nonatomic) BOOL isMultipart;

/**
 MIME type of the audio data for the server.  
 It is used in the Content-Type HTTP header. This property is needed only when 
 uploading an audio file. When using the microphone, ATTSpeechService uses MIME 
 type for the chosen audio format.
**/
@property (retain, nonatomic) NSString* contentType;

/**
 The name of the Speech API speech context to use for recognition. 
 Applications are required to set this property when using the AT&T Speech API.
 It is used in the X-SpeechContext HTTP header.
**/
@property (nonatomic, retain) NSString* speechContext;

/**
 A collection of key-value argument pairs for a Speech API context.  
 It is used in the X-Args HTTP header.
**/
@property (copy, nonatomic) NSDictionary* xArgs;

/**
 Setting this property to true enables the Speech SDK to automatically generate 
 X-Arg values based on the device and application properties: 
 DeviceType, DeviceOS, DeviceTime, ClientSDK, CientApp, and ClientVersion.
**/
@property (assign, nonatomic) BOOL sendsDefaultXArgs;

/**
 Name of format in which to encode audio from the microphone.  
 Defaults to AMR narrowband format.  
 Use only the constants defined above.
**/
@property (retain, nonatomic) NSString* audioFormat;

/**
 An OAuth access token that validates speech requests from this application. 
 It is used in the Authentication: Bearer HTTP header.
**/
@property (nonatomic, copy) NSString* bearerAuthToken;

/**
 The user name for Basic Authentication.  
 Note that Speech API uses OAuth, not Basic Authentication.
**/
@property (nonatomic, copy) NSString* basicAuthUser;

/**
 The password for Basic Authentication.
 Note that Speech API uses OAuth, not Basic Authentication.
**/
@property (nonatomic, copy) NSString* basicAuthPassword;

/**
 A collection of HTTP headers to add to the request.
**/
@property (nonatomic, retain) NSDictionary* requestHeaders;

#pragma mark User Interface Properties

/**
 Setting this property to true enables the Speech SDK to display a progress
 meter and a button for canceling the interaction.
**/
@property (nonatomic, assign) BOOL showUI;

/**
 The text that the Speech SDK displays while capturing audio data from the
 microphone. Use this property only when the showUI property is true.
**/
@property (nonatomic, copy) NSString* recordingPromptMessage;

/**
 The button label that the Speech SDK displays while capturing audio data from
 the microphone. The user can press the button to manually endpoint audio input.
 Use this property only when the showUI property is true.
**/
@property (nonatomic, copy) NSString* recordingStopButtonTitle;

/**
 The text that the Speech SDK displays while waiting for the server to return a
 recognition result. Use this property only when the showUI property is true.
**/
@property (nonatomic, copy) NSString* processingPromptMessage;

/**
 The button label that the Speech SDK displays while waiting for the server to
 return a recognition result. The user can press the button to cancel the speech
 interaction. Use this property only when the showUI property is true.
**/
@property (nonatomic, copy) NSString* processingCancelButtonTitle;

#pragma mark Timing Properties

/**
 The maximum number of seconds that the Speech SDK waits for a connection to the
 server when initiating a recognition request.
**/
@property (nonatomic, assign) NSTimeInterval connectionTimeout;

/**
 Controls how the Speech SDK handles silent audio input. When the value is NO, 
 the Speech SDK will send silent audio to the server for processing. 
 When the value is YES, the Speech SDK will automatically return an error code 
 of ATTSpeechServiceErrorCodeNoAudio when it detects silent audio.
**/
@property (nonatomic, assign) BOOL cancelWhenSilent;

/**
 The maximum number of seconds that a user can remain silent before beginning to
 talk. To disable the timeout, set the property to
 ATTSpeechServiceInfiniteInterval.
**/
@property (nonatomic, assign) NSTimeInterval maxInitialSilence;

/**
 The number of seconds of silence after the user stops talking before the Speech
 SDK performs endpointing. To disable automatic endpointing, set the property to
 ATTSpeechServiceInfiniteInterval.
 */
@property (nonatomic, assign) NSTimeInterval endingSilence;

/**
 The minimum number of seconds of audio to capture from the microphone.
 This limit is mainly useful for manual endpointing via stopListening.
 If the user does not meet this limit, Speech SDK will return an error code of 
 ATTSpeechServiceErrorCodeAudioTooShort. 
 To disable this limit, set the property to zero.
**/
@property (nonatomic, assign) NSTimeInterval minRecordingTime;

/**
 The maximum number of seconds of audio to capture from the microphone.
 When this limit is exceeded, Speech SDK stops listening to the microphone and 
 waits for the Speech API to process the captured audio. To make this duration
 unlimited, set the property to ATTSpeechServiceInfiniteInterval.
**/
@property (nonatomic, assign) NSTimeInterval maxRecordingTime;

/**
 The maximum number of seconds that the Speech SDK waits for the server to
 complete a recognition response.
 */
@property (nonatomic, assign) NSTimeInterval serverResponseTimeout;

#pragma mark Response Properties

/**
 Contains the recognition response as an array of hypothesis strings. It is
 roughly equivalent to the JSON path recognition.nbest[*].hypothesis. Each string
 in the array is a possible way of interpreting the submitted speech. When the
 speech cannot be recognized, this property will be nil or have zero elements.
**/
@property (nonatomic, readonly) NSArray* responseStrings;

/**
 Contains the parsed JSON tree of the speech recognition response. 
 This property is nil if the speech service did not return JSON.
**/
@property (nonatomic, readonly) NSDictionary* responseDictionary;

/**
 Contains the raw bytes of the speech recognition response.
**/
@property (nonatomic, readonly) NSData* responseData;

/**
 Contains the HTTP status code of the speech recognition response.
**/
@property (nonatomic, readonly) NSUInteger statusCode;

/**
 Contains the HTTP headers that were included in the speech recognition response.
 Headers whose keys appear multiple times in a response (such as Set-Cookie)
 have their values concatenated together.
**/
@property (nonatomic, readonly) NSDictionary* responseHeaders;

#pragma mark Methods

/**
 Initializes the iOS audio subsystem. Calling this method is optional, but if it
 is called early during the application lifecycle, it can reduce the latency of
 the first speech interaction requested by the user.
**/
- (void) prepare;

/**
 Starts a speech interaction using the microphone. Before calling this method,
 set the request properties on the ATTSpeechService object to configure the
 interaction. After this method is called, the Speech SDK will invoke methods on
 the delegate object to report interaction status, errors, and the recognition
 result.
**/
- (BOOL) startListening;

/**
 Sends audio data to the Speech API for processing instead of using the
 microphone. Before calling this method, set the request properties on the
 ATTSpeechService object to configure the interaction. The method does no
 compression of the audio data, so it should already be in a format supported by
 the Speech API. After this method is called, the Speech SDK will invoke methods
 on the delegate object to report interaction status, errors, and the recognition
 result.
 @param audioData the encoded audio data to send for processing
**/
- (BOOL) startWithAudioData: (NSData*) audioData;

/**
 ￼Manually ends speech input. This method is optional because the Speech SDK will
 automatically end speech input when the user stops talking or a timeout expires.
 After this method is called, the Speech SDK will wait for the server to process
 the submitted speech, and then it will report the result or error to the
 delegate object.
**/
- (void) stopListening;

/**
 Keep acquiring data for the specified number of seconds.  This method is 
 optional in unimodal speech applications.  Call this in multimodal applications 
 to prevent Speech SDK from ending audio input while the user is supplying input 
 through other means.
 @param extraInterval The number of seconds to delay stopping the recording
**/
- (void) extendTimeout: (NSTimeInterval) extraInterval;

/**
 When the Speech SDK is posting multipart data to the Speech API, the application
 can call this method to add additional parts to the posted data.  It may only
 be called when the delegate is handling a speechServiceSendingPartsBeforeAudio:
 or speechServiceSendingPartsAfterAudio: callback.  It is an error to call this
 method in any other callbacks or outside of a speech interaction.
 @param partData The data for the part body.
 @param contentType The value for the Content-Type header of the part.
 @param contentDisposition The value for the Content-Disposition header of the part.
**/
- (void) addPart: (NSData*) partData
     contentType: (NSString*) contentType
     disposition: (NSString*) contentDisposition;
 
/**
 Cancels a speech interaction. After this method is called, the Speech SDK will
 not attempt to perform speech recognition on the submitted data, and it will
 not make further delegate callbacks related to the canceled interaction.
**/
- (void) cancel;


/**
 Call this method when you are done with all speech interactions to release all
 resources used by the Speech SDK. This method is intended for non-production
 builds of applications when investigating memory issues..
**/
+ (void) cleanupForTermination;
 
 
 
@end

#pragma mark -

/**
 While the ATTSpeechService object is performing a speech recognition 
 interaction, it calls methods on its delegate object. The application is 
 required to implement the two delegate methods that communicate the recognition 
 result to the application. The remaining methods are optional, giving the 
 application a chance to customize the behavior of the speech interaction.
**/
@protocol ATTSpeechServiceDelegate <NSObject>

#pragma mark Required Callbacks
@required

/**
 The Speech SDK calls this method when it returns a recognition result. The 
 ATTSpeechService object will contain properties that include the response data 
 and recognized text.
 @param speechService The service notifying the delegate of success.
**/
- (void) speechServiceSucceeded: (ATTSpeechService*) speechService;

/**
 The Speech SDK calls this method when speech recognition fails. The reasons for 
 failure can include the user canceling, network errors, or server errors.
 @param speechService The service notifying the delegate of failure.
 @param error The error that has occurred.
**/
- (void) speechService: (ATTSpeechService*) speechService
       failedWithError: (NSError*) error;


#pragma mark Optional Callbacks
@optional

/**
 The Speech SDK calls this method immediately before capturing audio to give the 
 application a chance to perform a task. For example, the application can 
 display a custom recording UI.
 @param speechService The service notifying the delegate that listening will start.
**/
- (void) speechServiceWillStartListening: (ATTSpeechService*) speechService;

/**
 The Speech SDK calls this method when it has begun capturing audio from the 
 microphone.
 @param speechService The service notifying the delegate that listening started.
**/
- (void) speechServiceIsListening: (ATTSpeechService*) speechService;

/**
 The Speech SDK calls this method when it is ready to end audio capture because 
 the user is silent. The delegate method should return NO if listening should 
 continue.
 @param speechService The service querying the delegate.
**/
- (BOOL) speechServiceShouldEndRecording: (ATTSpeechService*) speechService;
 
/**
 The Speech SDK calls this method when it has finished capturing audio from the 
 microphone.
 @param speechService The service notifying the delegate that listening ended.
**/
- (void) speechServiceHasStoppedListening: (ATTSpeechService*) speechService;

 
/**
 The Speech SDK calls this method repeatedly as it captures audio. The callback 
 rate is roughly 1/10 second. An application can use the audio level data to 
 update its UI.
 @param speechService The service notifying the delegate about the audio level.
 @param level The audio level.
**/
- (void) speechService: (ATTSpeechService*) speechService
            audioLevel: (float) level;

/**
 The Speech SDK calls this method when it transitions among states in a recording 
 interaction, for example, from capturing to processing.
 The method argument contains the state the service will enter, and 
 speechService.currentState contains the state the service is leaving.
 Upon exiting this delegate callback, the service will be in the new state.
 @param speechService The service notifying the delegate that the state is changing.
 @param newState The state the service is changing to.
**/
- (void) speechService: (ATTSpeechService*) speechService
        willEnterState: (ATTSpeechServiceState) newState;

/**
 When Speech SDK is posting data in multipart format, it calls this delegate 
 method so the application can add parts to the request.  The application should 
 call addPart:contentType:disposition to add a part.  Parts added during the 
 handling of this delegate callback will be received by the speech recognition 
 service before the audio from the microphone, so this is the appropriate place 
 to add inline grammars and hints to a request.
 @param speechService The service asking the delegate if it has any data to add.
**/
- (void) speechServiceSendingPartsBeforeAudio: (ATTSpeechService*) speechService;
 

/**
 When Speech SDK is posting data in multipart format, it calls this delegate 
 method so the application can add parts to the request.  The application should 
 call addPart:contentType:disposition to add a part.  Parts added during the 
 handling of this delegate callback will be received by the speech recognition 
 service after the audio from the microphone.
 @param speechService The service asking the delegate if it has any data to add.
**/
- (void) speechServiceSendingPartsAfterAudio: (ATTSpeechService*) speechService;

#pragma mark
#pragma mark Deprecated

/**
 This message is sent when the current state of the SpeechService changes.
 @param speechService The service notifying the delegate that the state has changed.
 @param newState The new state of the service
 @param oldState The old state of the service.
 @deprecated Replaced by speechService:willEnterState:.
**/
- (void) speechService: (ATTSpeechService*) speechService
currentStateChangedToState: (ATTSpeechServiceState) newState
             fromState: (ATTSpeechServiceState) oldState  __attribute__((deprecated));


@end

// Deprecated constants
enum {
    /** @deprecated Value is not returned by ATTSpeechService. **/
    ATTSpeechServiceErrorCodeRecordingFailure __attribute__((deprecated)) = 10,
    /** The application did not set the delegate of the ATTSpeechService object.
     @deprecated ATTSpeechService is unable to pass this code to a delegate callback, so it throws an exception instead. **/
    ATTSpeechServiceErrorCodeNoDelegate __attribute__((deprecated)) = 40,
    /** @deprecated Use ATTSpeechServiceErrorCodeAudioTooShort instead. **/
    ATTSpeechServiceErrorCodeAudioFailure __attribute__((deprecated)) = 60,
    /** @deprecated Values is not returned by ATTSpeechService. **/
    ATTSpeechServiceErrorUnintelligible __attribute__((deprecated)) = 62,
    
};

@interface ATTSpeechService (Deprecated)

/**
 This property has been renamed maxInitialSilence.  Use that property instead.
 @deprecated Use the property maxInitialSilence.
 */
@property (nonatomic, assign) float startRecordingTimeout __attribute__((deprecated));

/**
 This property has been renamed endingSilence.  Use that propery instead.
 @deprecated Use the property endingSilence.
 */
@property (nonatomic, assign) float stopRecordingTimeout __attribute__((deprecated));

/**
 Starts a speech interaction using the microphone.
 @deprecated Use the method startListening.
 **/
- (BOOL) startWithMicrophone __attribute__((deprecated));

/**
 Releases the response data from the last speech interaction. It is optional to
 call this method, because starting a new speech interaction automatically
 purges the old response data as well.
 @deprecated It is no longer necessary to call this method, because the response
 data are automatically cleared when either the success or failure delegate
 callback is completed.
 **/
- (void) clearResponse __attribute__((deprecated));


@end



