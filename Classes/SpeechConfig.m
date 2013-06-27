//  SpeechConfig.m
//
// Implements customization parameters for this application's use of
// AT&T Speech SDK.
//
// Customize the functions declared here with the parameters of your application.

#import "SpeechConfig.h"


/** The URL of AT&T Speech API. **/
NSURL* SpeechServiceUrl(void)
{
    return [NSURL URLWithString: @"https://api.att.com/speech/v3/speechToText"];
}

/** The URL of AT&T Speech API OAuth service. **/
NSURL* SpeechOAuthUrl(void)
{
    return [NSURL URLWithString: @"https://api.att.com/oauth/token"];
}

/** Unobfuscates the OAuth client_id credential for the application. **/
NSString* SpeechOAuthKey(void)
{
//    #error Add code to unobfuscate your Speech API credentials, then delete this line.
    return @"6f25e6fc79c417aaaaddc94752cda689";
//    return MY_UNOBFUSCATE(my_obfuscated_client_id);
}

/** Unobfuscates the OAuth client_secret credential for the application. **/
NSString* SpeechOAuthSecret(void)
{
//    #error Add code to unobfuscate your Speech API credentials, then delete this line.
    return @"6fc8c2566d57c01b";
//    return MY_UNOBFUSCATE(my_obfuscated_client_secret);
}

/** The OAuth scope for the Speech API requests. **/
NSString* SpeechOAuthScope(void)
{
    return @"SPEECH";
}
